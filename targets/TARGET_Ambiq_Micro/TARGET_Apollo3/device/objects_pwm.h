/* mbed Microcontroller Library
 * Copyright (c) 2025 Jamie Smith
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

#ifndef OBJECTS_PWM_H
#define OBJECTS_PWM_H

#include "am_hal_ctimer.h"

#ifdef __cplusplus
extern "C" {
#endif

// This MCU has 8 CTIMER modules, each of which contains two segments. The two segments are functionally independent
// except that the B segment can optionally be combined with the A segment for a 32-bit counter (which we don't use).
// Each segment has two independent compare outputs, meaning that we have a total of 32 possible PWM outputs.

// PWM name is composed of the CTIMER number, whether it's an A or B segment, and whether this is the first or second
// output from it.
#define APOLLO3_PWM_NAME(ctimerNum, aOrB, outputNum) ((ctimerNum << 2) | (aOrB << 1) | outputNum)

typedef enum {
    CTIMER_A0_OUT1 = APOLLO3_PWM_NAME(0, 0, 0),
    CTIMER_A0_OUT2 = APOLLO3_PWM_NAME(0, 0, 1),

    CTIMER_A1_OUT1 = APOLLO3_PWM_NAME(1, 0, 0),
    CTIMER_A1_OUT2 = APOLLO3_PWM_NAME(1, 0, 1),

    CTIMER_A2_OUT1 = APOLLO3_PWM_NAME(2, 0, 0),
    CTIMER_A2_OUT2 = APOLLO3_PWM_NAME(2, 0, 1),

    CTIMER_A3_OUT1 = APOLLO3_PWM_NAME(3, 0, 0),
    CTIMER_A3_OUT2 = APOLLO3_PWM_NAME(3, 0, 1),

    CTIMER_A4_OUT1 = APOLLO3_PWM_NAME(4, 0, 0),
    CTIMER_A4_OUT2 = APOLLO3_PWM_NAME(4, 0, 1),

    CTIMER_A5_OUT1 = APOLLO3_PWM_NAME(5, 0, 0),
    CTIMER_A5_OUT2 = APOLLO3_PWM_NAME(5, 0, 1),

    CTIMER_A6_OUT1 = APOLLO3_PWM_NAME(6, 0, 0),
    CTIMER_A6_OUT2 = APOLLO3_PWM_NAME(6, 0, 1),

    CTIMER_A7_OUT1 = APOLLO3_PWM_NAME(7, 0, 0),
    CTIMER_A7_OUT2 = APOLLO3_PWM_NAME(7, 0, 1),

    CTIMER_B0_OUT1 = APOLLO3_PWM_NAME(0, 1, 0),
    CTIMER_B0_OUT2 = APOLLO3_PWM_NAME(0, 1, 1),

    CTIMER_B1_OUT1 = APOLLO3_PWM_NAME(1, 1, 0),
    CTIMER_B1_OUT2 = APOLLO3_PWM_NAME(1, 1, 1),

    CTIMER_B2_OUT1 = APOLLO3_PWM_NAME(2, 1, 0),
    CTIMER_B2_OUT2 = APOLLO3_PWM_NAME(2, 1, 1),

    CTIMER_B3_OUT1 = APOLLO3_PWM_NAME(3, 1, 0),
    CTIMER_B3_OUT2 = APOLLO3_PWM_NAME(3, 1, 1),

    CTIMER_B4_OUT1 = APOLLO3_PWM_NAME(4, 1, 0),
    CTIMER_B4_OUT2 = APOLLO3_PWM_NAME(4, 1, 1),

    CTIMER_B5_OUT1 = APOLLO3_PWM_NAME(5, 1, 0),
    CTIMER_B5_OUT2 = APOLLO3_PWM_NAME(5, 1, 1),

    CTIMER_B6_OUT1 = APOLLO3_PWM_NAME(6, 1, 0),
    CTIMER_B6_OUT2 = APOLLO3_PWM_NAME(6, 1, 1),

    CTIMER_B7_OUT1 = APOLLO3_PWM_NAME(7, 1, 0),
    CTIMER_B7_OUT2 = APOLLO3_PWM_NAME(7, 1, 1),
} PWMName;

// Get the CTIMER number of a PWM
#define APOLLO3_PWMNAME_GET_CTIMER(pwm_name) (pwm_name >> 2)

// Convert from PWM name to AM_HAL_CTIMER_TIMERA/AM_HAL_CTIMER_TIMERB macro
#define APOLLO3_PWMNAME_GET_SEGMENT(pwm_name) (pwm_name & 0b10 ? AM_HAL_CTIMER_TIMERB : AM_HAL_CTIMER_TIMERA)

// Convert from PWM name to AM_HAL_CTIMER_OUTPUT_NORMAL/AM_HAL_CTIMER_OUTPUT_SECONDARY enum value
#define APOLLO3_PWMNAME_GET_OUTPUT(pwm_name) (pwm_name & 0b1 ? AM_HAL_CTIMER_OUTPUT_SECONDARY : AM_HAL_CTIMER_OUTPUT_NORMAL)

struct pwmout_s
{
    // Pin that the channel is on
    PinName pin;

    // PWM name that this channel is using
    PWMName pwm_name;

    // True iff the pin is connected to the PWM timer
    bool pin_is_connected_to_pwm;

    // Clock period configured on this PWM, in floating point seconds
    float clock_period;

    // Number of counts that the PWM output will make before a new PWM cycle starts
    uint32_t top_count;

    // Number of counts that the PWM output will stay on for.
    // Zero = full off, top_count = full on
    uint32_t on_counts;
};

#ifdef __cplusplus
}
#endif

#endif