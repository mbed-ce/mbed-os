/* mbed Microcontroller Library
 * Copyright (c) 2024, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#if DEVICE_PWMOUT

#include "hal/pwmout_api.h"
#include "PeripheralPins.h"
#include "pinmap.h"
#include "objects.h"
#include "mbed_assert.h"

#include <math.h>

// Change to 1 to enable debug prints of what's being calculated.
// Must comment out the critical section calls in PwmOut to use.
#define APOLLO3_PWMOUT_DEBUG 1

#if APOLLO3_PWMOUT_DEBUG
#include <stdio.h>
#include <inttypes.h>
#endif

struct pwm_clock_freq {
    uint32_t clockSetting;
    uint32_t frequency;
};

// Table of options for PWM clock source vs clock frequency, in decreasing order of clock frequency
// Note that the Apollo3 uses a fixed external oscillator frequency, so this is possible to define statically.
// There are three oscillators available, each of which can be used for PWM:
// - HFRC - internal high freq RC oscillator, 48MHz +-3.5%
// - XT - external crystal, 32.768kHz, likely 50ppm or better tolerance
// - LFRC - internal low freq RC oscillator, 1.024kHz +-32% (no that's not a typo!)
// This means we have quite a wide range of base clock frequencies available, though period accuracy will be pretty
// poor if the LFRC gets selected.
#define NUM_CLOCK_OPTIONS 16
static const struct pwm_clock_freq pwm_clocks[NUM_CLOCK_OPTIONS] = {
    {AM_HAL_CTIMER_HFRC_12MHZ, 12000000},
    {AM_HAL_CTIMER_HFRC_3MHZ, 3000000},
    {AM_HAL_CTIMER_HFRC_187_5KHZ, 187500},
    {AM_HAL_CTIMER_HFRC_47KHZ, 46875},
    {AM_HAL_CTIMER_XT_32_768KHZ, 32768},
    {AM_HAL_CTIMER_XT_16_384KHZ, 16384},
    {AM_HAL_CTIMER_HFRC_12KHZ, 11719},
    {AM_HAL_CTIMER_XT_DIV4,8192},
    {AM_HAL_CTIMER_XT_DIV8,4096},
    {AM_HAL_CTIMER_XT_2_048KHZ,2048},
    {AM_HAL_CTIMER_XT_DIV32,1024},

    // Note: NOT adding this one because the accuracy is bad
    // {AM_HAL_CTIMER_LFRC_512HZ, 512},

    {AM_HAL_CTIMER_XT_256HZ, 256},
    {AM_HAL_CTIMER_LFRC_32HZ, 32},
    {AM_HAL_CTIMER_LFRC_1HZ, 1},

    // Note: there's also a 1/16Hz option but not adding it b/c the struct is using an integer for frequency
};

/// Largest top count value supported by hardware.  Using this value will provide the highest duty cycle resolution.
const uint16_t MAX_TOP_COUNT = 65534;



/// Calculate the effective PWM period (in floating point seconds) based on a divider and top_count value
static float calc_effective_pwm_period(float divider, uint16_t top_count)
{
    // Note: The hardware counts to top_count *inclusively*, so we have to add 1
    // to get the number of clock cycles that a given top_count value will produce
    return 1.0f / ((clock_get_hz(clk_sys) / divider) / (top_count + 1));
}

/// Calculate the best possible top_count value (rounding up) for a divider and a desired pwm period
static uint16_t calc_top_count_for_period(float divider, float desired_pwm_period)
{
    // Derivation:
    // desired_pwm_period = 1.0f / ((clock_get_hz(clk_sys) / divider) / (top_count + 1))
    // desired_pwm_period = (top_count + 1) / (clock_get_hz(clk_sys) / divider)
    // desired_pwm_period * (clock_get_hz(clk_sys) / divider) - 1 = top_count

    long top_count_float = lroundf(desired_pwm_period * (clock_get_hz(clk_sys) / divider) - 1);
    MBED_ASSERT(top_count_float <= MAX_TOP_COUNT);
    return (uint16_t)top_count_float;
}

/// Calculate the best possible floating point divider value for a desired pwm period.
/// This function assumes that top_count is set to MAX_TOP_COUNT.
static float calc_divider_for_period(float desired_pwm_period)
{
    // Derivation:
    // (desired_pwm_period * clock_get_hz(clk_sys)) / divider - 1 = top_count
    // (desired_pwm_period * clock_get_hz(clk_sys)) / divider = top_count + 1
    // divider = (desired_pwm_period * clock_get_hz(clk_sys)) / (top_count + 1)

    return (desired_pwm_period * clock_get_hz(clk_sys)) / (MAX_TOP_COUNT + 1);
}

/// Convert PWM divider from floating point to a fixed point number (rounding up).
/// The divider is returned as an 8.4 bit fixed point number, which is what the Pico registers use.
static uint16_t pwm_divider_float_to_fixed(float divider_float)
{
    // To convert to a fixed point number, multiply by 16 and then round up
    uint16_t divider_exact = ceil(divider_float * 16);

    // Largest supported divider is 255 and 15/16
    if(divider_exact > 0xFFF)
    {
        divider_exact = 0xFFF;
    }
    return divider_exact;
}

/// Convert PWM divider from the fixed point hardware value (8.4 bits) to a float.
static float pwm_divider_fixed_to_float(uint16_t divider_fixed)
{
    return divider_fixed / 16.0f;
}

void pwmout_init(pwmout_t *obj, PinName pin)
{
    MBED_ASSERT(obj != NULL);

    // Find PWM module from pinmap
    const PWMName pwmName = pinmap_peripheral(pin, PinMap_PWM_OUT);

    /* Populate PWM object with values. */
    obj->pwmName = pwmName;

    // Configure the pin
    am_hal_ctimer_output_config(APOLLO3_PWMNAME_GET_CTIMER(pwmName),
                                APOLLO3_PWMNAME_GET_SEGMENT(pwmName),
                                pin,
                                APOLLO3_PWMNAME_GET_OUTPUT(pwmName),
                                AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA);
}

/** Deinitialize the pwmout object
 *
 * Parameter obj The pwmout object
 */
void pwmout_free(pwmout_t *obj)
{
    MBED_ASSERT(obj != NULL);
    pwm_set_enabled(obj->slice, false);
}

/** Set the output duty-cycle in range <0.0f, 1.0f>
 *
 * pulse 0.0f represents 0 percentage, 1.0f represents 100 percent.
 * Parameter obj     The pwmout object
 * Parameter percent The floating-point percentage number
 */
void pwmout_write(pwmout_t *obj, float percent)
{
    obj->percent = percent;

    // Per datasheet section 4.5.2.2, a period value of top_count + 1 produces 100% duty cycle
    int32_t new_reset_counts = lroundf((obj->top_count + 1) * percent);

    // Clamp to valid values
    if(new_reset_counts > obj->top_count + 1)
    {
        new_reset_counts = obj->top_count + 1;
    }
    else if(new_reset_counts < 0)
    {
        new_reset_counts = 0;
    }

#if APOLLO3_PWMOUT_DEBUG
    printf("new_reset_counts: %" PRIu32 "\n", new_reset_counts);
#endif

    pwm_set_chan_level(obj->slice, obj->channel, new_reset_counts);
    pwm_set_enabled(obj->slice, true);
}

/** Read the current float-point output duty-cycle
 *
 * Parameter obj The pwmout object
 * Return A floating-point output duty-cycle
 */
float pwmout_read(pwmout_t *obj)
{
    /* Return percentage stored in object instead of calculating the value.
     * This prevents floating point rounding errors.
     */
    return obj->percent;
}

void pwmout_period(pwmout_t *obj, const float desired_period)
{
    // To find the period, we perform the following steps:
    // - Determine the slowest clock frequency that we can use while still hitting the needed period
    // - Calculate the correct top_count value that will produce as close to the desired period as possible


    if(period <= calc_effective_pwm_period(1, MAX_TOP_COUNT))
    {
        // Short period.  Leave divider at 1 and reduce top_count to match the expected period
        obj->clock_divider = 1.0f;
        obj->cfg.div = PWM_CHn_DIV_1;
        obj->top_count = calc_top_count_for_period(obj->clock_divider, period);
    }
    else
    {
        // Long period, need to use divider.

        // Step 1: Calculate exact desired divider such that top_count would equal MAX_TOP_COUNT
        float desired_divider = calc_divider_for_period(period);

        // Step 2: Round desired divider upwards to the next value the hardware can do.
        // We go upwards so that the top_count value can be trimmed downwards for the best period accuracy.
        uint16_t divider_fixed_point = pwm_divider_float_to_fixed(desired_divider);
        obj->cfg.div = divider_fixed_point;

        // Step 3: Get the divider we'll actually be using as a float
        obj->clock_divider = pwm_divider_fixed_to_float(divider_fixed_point);

        // Step 4: For best accuracy, recalculate the top_count value using the divider.
        obj->top_count = calc_top_count_for_period(obj->clock_divider, period);

#if APOLLO3_PWMOUT_DEBUG
        printf("period = %f, desired_divider = %f\n",
               period,
               desired_divider);
#endif
    }

    // Save period for later
    obj->period = period;

#if APOLLO3_PWMOUT_DEBUG
    printf("obj->clock_divider = %f, obj->cfg.div = %" PRIu32 ", obj->top_count = %" PRIu16 "\n",
           obj->clock_divider,
           obj->cfg.div,
           obj->top_count);
#endif

    // Set the new divider and top_count values.
    pwm_config_set_wrap(&(obj->cfg), obj->top_count);
    pwm_init(obj->slice, &(obj->cfg), false);
}

/** Set the PWM period specified in miliseconds, keeping the duty cycle the same
 *
 * Parameter obj The pwmout object
 * Parameter ms  The milisecond period
 */
void pwmout_period_ms(pwmout_t *obj, int period)
{
    /* Set new period. */
    pwmout_period(obj, period / 1000.0f);
}

/** Set the PWM period specified in microseconds, keeping the duty cycle the same
 *
 * Parameter obj The pwmout object
 * Parameter us  The microsecond period
 */
void pwmout_period_us(pwmout_t *obj, int period)
{
    /* Set new period. */
    pwmout_period(obj, period / 1000000.0f);
}

/** Read the PWM period specified in microseconds
 *
 * @param obj The pwmout object
 * @return A int output period
 */
int pwmout_read_period_us(pwmout_t *obj)
{
    return lroundf(1000000 * calc_effective_pwm_period(obj->clock_divider, obj->top_count));
}

/** Set the PWM pulsewidth specified in seconds, keeping the period the same.
 *
 * Parameter obj     The pwmout object
 * Parameter seconds The floating-point pulsewidth in seconds
 */
void pwmout_pulsewidth(pwmout_t *obj, float pulse)
{
    pwmout_write(obj, pulse / obj->period);
}

/** Set the PWM pulsewidth specified in miliseconds, keeping the period the same.
 *
 * Parameter obj The pwmout object
 * Parameter ms  The floating-point pulsewidth in miliseconds
 */
void pwmout_pulsewidth_ms(pwmout_t *obj, int pulse)
{
    pwmout_write(obj, (pulse * .001f) / obj->period);
}

/** Set the PWM pulsewidth specified in microseconds, keeping the period the same.
 *
 * Parameter obj The pwmout object
 * Parameter us  The floating-point pulsewidth in microseconds
 */
void pwmout_pulsewidth_us(pwmout_t *obj, int pulse)
{
    pwmout_write(obj, (pulse * .000001f) / obj->period);
}

int pwmout_read_pulsewidth_us(pwmout_t *obj) {
    return lroundf(obj->period * obj->percent * 1000000);
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM_OUT;
}

#endif // DEVICE_PWMOUT
