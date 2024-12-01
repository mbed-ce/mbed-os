/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#ifndef MBED_PINNAMESTYPES_H
#define MBED_PINNAMESTYPES_H

#include "cyhal_gpio.h"
#include "cybsp_types.h"

// Pin Modes
#define PullNone CYHAL_GPIO_DRIVE_PULL_NONE
#define PullDefault CYHAL_GPIO_DRIVE_NONE
#define PullDown CYHAL_GPIO_DRIVE_PULLDOWN
#define PullUp CYHAL_GPIO_DRIVE_PULLUP

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT = 0,
    PIN_OUTPUT
} PinDirection;

typedef cyhal_gpio_drive_mode_t PinMode;
typedef cyhal_gpio_t PinName;
static inline PinName cyhal_gpio_to_rtos(cyhal_gpio_t pin)
{
    return pin;
}

#ifdef __cplusplus
}
#endif

#endif
