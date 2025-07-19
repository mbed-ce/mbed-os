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
#include <mbed_error.h>

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
#define NUM_CLOCK_SOURCE_OPTIONS 16
static const struct pwm_clock_freq pwm_clock_sources[NUM_CLOCK_SOURCE_OPTIONS] = {
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
/// The hardware performs (CMPR register value + 1) counts and it's a 16-bit register, so the actual max top count is 2^16.
const uint32_t MAX_TOP_COUNT = 65536;

void pwmout_init(pwmout_t *obj, PinName pin)
{
    MBED_ASSERT(obj != NULL);

    // Find PWM module from pinmap
    const PWMName pwmName = pinmap_peripheral(pin, PinMap_PWM_OUT);

    /* Populate PWM object with values. */
    obj->pwm_name = pwmName;

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
    // - Determine the fastest clock frequency that we can use while still hitting the needed period
    // - Calculate the correct top_count value that will produce as close to the desired period as possible
    // - Write the new top_count value into the hardware

    size_t clk_source_idx;
    bool found_clk_source = false;
    for(clk_source_idx = 0; clk_source_idx < NUM_CLOCK_SOURCE_OPTIONS; ++clk_source_idx) {
        const float divider_max_period = MAX_TOP_COUNT / (float)pwm_clock_sources[clk_source_idx].frequency;
        if(divider_max_period >= desired_period) {
            found_clk_source = true;
            break;
        }
    }
    if(!found_clk_source) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_PWM, MBED_ERROR_CODE_INVALID_ARGUMENT), "Clock frequency too slow!");
    }

    // Now that we have found the best clock source, calculate top_count to hit the desired period
    obj->clock_period = 1.0f / (float)pwm_clock_sources[clk_source_idx].frequency;
    obj->top_count = lroundf(desired_period / obj->clock_period);

    // The hardware cannot support a top_count of less than 2. If that happened than it means the
    // frequency is too fast.
    if(obj->top_count < 2) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_PWM, MBED_ERROR_CODE_INVALID_ARGUMENT), "Clock frequency too fast!");
    }

#if APOLLO3_PWMOUT_DEBUG
    printf("clk_source_idx = %zu, obj->clock_period = %f, obj->top_count = %" PRIu32 "\n",
           clk_source_idx,
           obj->clock_period,
           obj->top_count);
#endif

    // Set new clock source. This stops the timer.
    am_hal_ctimer_config_single(APOLLO3_PWMNAME_GET_CTIMER(obj->pwm_name),
        APOLLO3_PWMNAME_GET_SEGMENT(obj->pwm_name),
        AM_HAL_CTIMER_FN_PWM_REPEAT | clk_source_idx);

    // Set new period value. Note that:
    // - We need to program a value of (top_count - 1) into the HW register
    // - As part of setting the period we also need to program the duty cycle. For now we program it to zero,
    //   in anticipation of a future pwmout_write() call
    if(APOLLO3_PWMNAME_GET_OUTPUT(obj->pwm_name) == AM_HAL_CTIMER_OUTPUT_NORMAL) {
        am_hal_ctimer_period_set(APOLLO3_PWMNAME_GET_CTIMER(obj->pwm_name),
            APOLLO3_PWMNAME_GET_SEGMENT(obj->pwm_name),
            obj->top_count - 1,
            0);
    }
    else {
        am_hal_ctimer_aux_period_set(APOLLO3_PWMNAME_GET_CTIMER(obj->pwm_name),
            APOLLO3_PWMNAME_GET_SEGMENT(obj->pwm_name),
            obj->top_count - 1,
            0);
    }

    am_hal_ctimer_start(APOLLO3_PWMNAME_GET_CTIMER(obj->pwm_name), APOLLO3_PWMNAME_GET_SEGMENT(obj->pwm_name));
}

void pwmout_period_ms(pwmout_t *obj, int period)
{
    /* Set new period. */
    pwmout_period(obj, period / 1000.0f);
}

void pwmout_period_us(pwmout_t *obj, int period)
{
    /* Set new period. */
    pwmout_period(obj, period / 1000000.0f);
}

int pwmout_read_period_us(pwmout_t *obj)
{
    return lroundf(1000000 * obj->top_count * obj->clock_period);
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
