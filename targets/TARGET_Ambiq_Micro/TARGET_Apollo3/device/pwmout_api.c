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

#include <string.h>
#include <math.h>

// Change to 1 to enable debug prints of what's being calculated.
// Must comment out the critical section calls in PwmOut to use.
#define APOLLO3_PWMOUT_DEBUG 0

#if APOLLO3_PWMOUT_DEBUG
#include <stdio.h>
#include <inttypes.h>
#endif

struct pwm_clock_freq {
    uint32_t clock_setting;
    float frequency;
};

// Table of options for PWM clock source vs clock frequency, in decreasing order of clock frequency
// Note that the Apollo3 uses a fixed external oscillator frequency, so this is possible to define statically.
// There are three oscillators available, each of which can be used for PWM:
// - HFRC - internal high freq RC oscillator, 48MHz +-3.5% uncalibrated, but better with auto-calibration.
// - XT - external crystal, 32.768kHz, likely 50ppm or better tolerance
// - LFRC - internal low freq RC oscillator, 1.024kHz +-32% (no that's not a typo!)
// This means we have quite a wide range of base clock frequencies available, though period accuracy will be pretty
// poor if the LFRC gets selected.
#define NUM_CLOCK_SOURCE_OPTIONS 16
#define HFRC_FREQ 48000000
#define XT_FREQ 32768
#define LFRC_FREQ 1024
static const struct pwm_clock_freq pwm_clock_sources[NUM_CLOCK_SOURCE_OPTIONS] = {
    {AM_HAL_CTIMER_HFRC_12MHZ, HFRC_FREQ/4.0f},
    {AM_HAL_CTIMER_HFRC_3MHZ, HFRC_FREQ/16.0f},
    {AM_HAL_CTIMER_HFRC_187_5KHZ, HFRC_FREQ/256.0f},
    {AM_HAL_CTIMER_HFRC_47KHZ, HFRC_FREQ/1024.0f},
    {AM_HAL_CTIMER_XT_32_768KHZ, XT_FREQ},
    {AM_HAL_CTIMER_XT_16_384KHZ, XT_FREQ/2.0f},
    {AM_HAL_CTIMER_HFRC_12KHZ, HFRC_FREQ/4096.0f},
    {AM_HAL_CTIMER_XT_DIV4,XT_FREQ/4.0f},
    {AM_HAL_CTIMER_XT_DIV8,XT_FREQ/8.0f},
    {AM_HAL_CTIMER_XT_2_048KHZ, XT_FREQ/16.0f},
    {AM_HAL_CTIMER_XT_DIV32,XT_FREQ/32.0f},

    // Note: NOT adding this one because the accuracy is bad
    // {AM_HAL_CTIMER_LFRC_512HZ, LFRC_FREQ/2.0f},

    {AM_HAL_CTIMER_XT_256HZ, XT_FREQ/128.0f},
    {AM_HAL_CTIMER_LFRC_32HZ, LFRC_FREQ/32.0f},
    {AM_HAL_CTIMER_LFRC_1HZ, LFRC_FREQ/1024.0f},

    // Note: there may also be a 1/16Hz clock source option, but the SDK and datasheet seem
    // to disagree about it.
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
    memset(obj, 0, sizeof(pwmout_t));
    obj->pwm_name = pwmName;
    obj->pin = pin;
}

void pwmout_free(pwmout_t *obj)
{
    MBED_ASSERT(obj != NULL);
    am_hal_ctimer_stop(APOLLO3_PWMNAME_GET_CTIMER(obj->pwm_name),
        APOLLO3_PWMNAME_GET_SEGMENT(obj->pwm_name));
}

void pwmout_write(pwmout_t *obj, float percent)
{
    MBED_ASSERT(obj != NULL);
    if (percent < 0 || percent > 1) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_PWM, MBED_ERROR_CODE_INVALID_ARGUMENT), "Invalid PWM duty cycle!");
    }

    // Calculate how many counts out of top_count we should be on
    obj->on_counts = lroundf(percent * obj->top_count);

#if APOLLO3_PWMOUT_DEBUG
    printf("obj->on_counts: %" PRIu32 "\n", obj->on_counts);
#endif

    // If we want 0% or 100% duty cycle, we need to do that by connecting the pin to forceed 0 or forced 1
    if (obj->on_counts == 0 || obj->on_counts == obj->top_count) {
        am_hal_ctimer_stop(APOLLO3_PWMNAME_GET_CTIMER(obj->pwm_name),
            APOLLO3_PWMNAME_GET_SEGMENT(obj->pwm_name));
        am_hal_ctimer_output_config(APOLLO3_PWMNAME_GET_CTIMER(obj->pwm_name),
                                APOLLO3_PWMNAME_GET_SEGMENT(obj->pwm_name),
                                obj->pin,
                                obj->on_counts == 0 ? AM_HAL_CTIMER_OUTPUT_FORCE0 : AM_HAL_CTIMER_OUTPUT_FORCE1,
                                AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA);
        obj->pin_is_connected_to_pwm = false;
    }
    else {

        // If the pin is not connected to the PWM timer, set that up
        if (!obj->pin_is_connected_to_pwm) {
            am_hal_ctimer_output_config(APOLLO3_PWMNAME_GET_CTIMER(obj->pwm_name),
                APOLLO3_PWMNAME_GET_SEGMENT(obj->pwm_name),
                obj->pin,
                APOLLO3_PWMNAME_GET_OUTPUT(obj->pwm_name),
                AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA);
            obj->pin_is_connected_to_pwm = true;
        }

        // Set new period value. Note that:
        // - We have to set the top count and the on count at the same time
        // - The HW adds 1 to the programmed values, so we have to subtract 1 when passing them in
        if(APOLLO3_PWMNAME_GET_OUTPUT(obj->pwm_name) == AM_HAL_CTIMER_OUTPUT_NORMAL) {
            am_hal_ctimer_period_set(APOLLO3_PWMNAME_GET_CTIMER(obj->pwm_name),
                APOLLO3_PWMNAME_GET_SEGMENT(obj->pwm_name),
                obj->top_count - 1,
                obj->on_counts - 1);
        }
        else {
            am_hal_ctimer_aux_period_set(APOLLO3_PWMNAME_GET_CTIMER(obj->pwm_name),
                APOLLO3_PWMNAME_GET_SEGMENT(obj->pwm_name),
                obj->top_count - 1,
                obj->on_counts - 1);
        }

        // Start timer if not running
        am_hal_ctimer_start(APOLLO3_PWMNAME_GET_CTIMER(obj->pwm_name),
            APOLLO3_PWMNAME_GET_SEGMENT(obj->pwm_name));
    }
}

float pwmout_read(pwmout_t *obj)
{
    return ((float)obj->on_counts) / obj->top_count;
}

void pwmout_period(pwmout_t *obj, const float desired_period)
{
    MBED_ASSERT(obj != NULL);
    const float old_duty_cycle = pwmout_read(obj);

    // To find the period, we perform the following steps:
    // - Determine the fastest clock frequency that we can use while still hitting the needed period
    // - Calculate the correct top_count value that will produce as close to the desired period as possible
    // - Write the new top_count value into the hardware

    size_t clk_source_idx;
    bool found_clk_source = false;
    for(clk_source_idx = 0; clk_source_idx < NUM_CLOCK_SOURCE_OPTIONS; ++clk_source_idx) {
        const float divider_max_period = MAX_TOP_COUNT / pwm_clock_sources[clk_source_idx].frequency;
        if(divider_max_period >= desired_period) {
            found_clk_source = true;
            break;
        }
    }
    if(!found_clk_source) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_PWM, MBED_ERROR_CODE_INVALID_ARGUMENT), "Clock frequency too slow!");
    }

    // Now that we have found the best clock source, calculate top_count to hit the desired period
    obj->clock_period = 1.0f / pwm_clock_sources[clk_source_idx].frequency;
    obj->top_count = lroundf(desired_period / obj->clock_period);

    // The hardware cannot support a top_count of less than 2. If that happened than it means the
    // frequency is too fast.
    if(obj->top_count < 2) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_PWM, MBED_ERROR_CODE_INVALID_ARGUMENT), "Clock frequency too fast!");
    }

#if APOLLO3_PWMOUT_DEBUG
    printf("clk_source_idx = %zu, obj->clock_period = %f ms, obj->top_count = %" PRIu32 "\n",
           clk_source_idx,
           obj->clock_period*1e3f,
           obj->top_count);
#endif

    // Set new clock source. This stops the timer.
    am_hal_ctimer_config_single(APOLLO3_PWMNAME_GET_CTIMER(obj->pwm_name),
        APOLLO3_PWMNAME_GET_SEGMENT(obj->pwm_name),
        AM_HAL_CTIMER_FN_PWM_REPEAT | pwm_clock_sources[clk_source_idx].clock_setting);

    // Set the old duty cycle, which also writes the period and starts the timer
    pwmout_write(obj, old_duty_cycle);
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
    MBED_ASSERT(obj != NULL);
    return lroundf(1e6f * obj->top_count * obj->clock_period);
}

void pwmout_pulsewidth(pwmout_t *obj, float pulse)
{
    pwmout_write(obj, pulse / (obj->top_count * obj->clock_period));
}

void pwmout_pulsewidth_ms(pwmout_t *obj, int pulse)
{
    pwmout_write(obj, (pulse * .001f) / (obj->top_count * obj->clock_period));
}

void pwmout_pulsewidth_us(pwmout_t *obj, int pulse)
{
    pwmout_write(obj, (pulse * .000001f) / (obj->top_count * obj->clock_period));
}

int pwmout_read_pulsewidth_us(pwmout_t *obj) {
    return lroundf(obj->on_counts * obj->clock_period * 1e6f);
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM_OUT;
}

#endif // DEVICE_PWMOUT
