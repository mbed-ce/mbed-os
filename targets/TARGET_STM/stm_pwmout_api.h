/* mbed Microcontroller Library
 * Copyright (c) 2024 STMicroelectronics
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

#ifndef MBED_OS_STM_PWMOUT_API_H
#define MBED_OS_STM_PWMOUT_API_H

#include "PeripheralNames.h"
#include "PinNames.h"

struct pwmout_s {

    // PWM (timer) peripheral that this pwmout is created with
    PWMName pwm;

    // Pin that this pwmout is using.  This is used to reset the pin function in pwmout_free()
    PinName pin;

    // Current period of the pwmout, in microseconds
    uint32_t period;

    // Current compare value.  Once the PWM counter becomes >= this value,
    // the PWM turns off.
    uint32_t compare_value;
    
    // How many counts the PWM timer makes before it resets.
    // Example: if top_count = 3, the timer will count 0, 1, 2, 0, 1, 2, etc.
    uint16_t top_count;

    // Channel number on the timer that this pin is connected to
    uint8_t channel;

    // Whether this hardware channel is an inverted output (1) or not (0)
    uint8_t inverted;
};

#endif //MBED_OS_STM_PWMOUT_API_H