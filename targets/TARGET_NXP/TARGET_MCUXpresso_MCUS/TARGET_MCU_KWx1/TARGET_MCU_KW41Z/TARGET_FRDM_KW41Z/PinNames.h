/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

/* MBED TARGET LIST: KW41Z */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

#define GPIO_PORT_SHIFT 12

typedef enum {
    PTA0  = (0 << GPIO_PORT_SHIFT | 0),
    PTA1  = (0 << GPIO_PORT_SHIFT | 1),
    PTA2  = (0 << GPIO_PORT_SHIFT | 2),
    PTA16 = (0 << GPIO_PORT_SHIFT | 16),
    PTA17 = (0 << GPIO_PORT_SHIFT | 17),
    PTA18 = (0 << GPIO_PORT_SHIFT | 18),
    PTA19 = (0 << GPIO_PORT_SHIFT | 19),
    PTB0  = (1 << GPIO_PORT_SHIFT | 0),
    PTB1  = (1 << GPIO_PORT_SHIFT | 1),
    PTB2  = (1 << GPIO_PORT_SHIFT | 2),
    PTB3  = (1 << GPIO_PORT_SHIFT | 3),
    PTB16 = (1 << GPIO_PORT_SHIFT | 16),
    PTB17 = (1 << GPIO_PORT_SHIFT | 17),
    PTB18 = (1 << GPIO_PORT_SHIFT | 18),
    PTC1  = (2 << GPIO_PORT_SHIFT | 1),
    PTC2  = (2 << GPIO_PORT_SHIFT | 2),
    PTC3  = (2 << GPIO_PORT_SHIFT | 3),
    PTC4  = (2 << GPIO_PORT_SHIFT | 4),
    PTC5  = (2 << GPIO_PORT_SHIFT | 5),
    PTC6  = (2 << GPIO_PORT_SHIFT | 6),
    PTC7  = (2 << GPIO_PORT_SHIFT | 7),
    PTC16 = (2 << GPIO_PORT_SHIFT | 16),
    PTC17 = (2 << GPIO_PORT_SHIFT | 17),
    PTC18 = (2 << GPIO_PORT_SHIFT | 18),
    PTC19 = (2 << GPIO_PORT_SHIFT | 19),

    LED_RED   = PTC1,
    LED_GREEN = PTA19,
    LED_BLUE  = PTA18,

    // LEDs and buttons
#define LED1 PTC1
#define LED2 PTA19
#define LED3 PTA18

#define BUTTON1 PTC4
#define BUTTON2 PTC5

    // USB Pins
    CONSOLE_TX = PTC7,
    CONSOLE_RX = PTC6,

    // Not connected
    NC = (int)0xFFFFFFFF,

#ifdef TARGET_FF_ARDUINO_UNO
    // Arduino Headers
    ARDUINO_UNO_D0 = PTC6,
    ARDUINO_UNO_D1 = PTC7,
    ARDUINO_UNO_D2 = PTC19,
    ARDUINO_UNO_D3 = PTC16,
    ARDUINO_UNO_D4 = PTC4,
    ARDUINO_UNO_D5 = PTC17,
    ARDUINO_UNO_D6 = PTC18,
    ARDUINO_UNO_D7 = PTA1,
    ARDUINO_UNO_D8 = PTA0,
    ARDUINO_UNO_D9 = PTC1,
    ARDUINO_UNO_D10 = PTA19,
    ARDUINO_UNO_D11 = PTA16,
    ARDUINO_UNO_D12 = PTA17,
    ARDUINO_UNO_D13 = PTA18,
    ARDUINO_UNO_D14 = PTC3,
    ARDUINO_UNO_D15 = PTC2,

    ARDUINO_UNO_A0 = NC,
    ARDUINO_UNO_A1 = PTB18,
    ARDUINO_UNO_A2 = PTB2,
    ARDUINO_UNO_A3 = PTB3,
    ARDUINO_UNO_A4 = PTB1,
    ARDUINO_UNO_A5 = PTB0,
#endif

    DAC0_OUT = PTB18
} PinName;


typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp   = 2,
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
