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

/* MBED TARGET LIST: RZ_A1H */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

#define PORT_SHIFT  4

typedef enum {
    P0_0 = 0x0000, P0_1, P0_2, P0_3, P0_4, P0_5, P0_6, P0_7, P0_8, P0_9, P0_10, P0_11, P0_12, P0_13, P0_14, P0_15,
    P1_0 = 0x0100, P1_1, P1_2, P1_3, P1_4, P1_5, P1_6, P1_7, P1_8, P1_9, P1_10, P1_11, P1_12, P1_13, P1_14, P1_15,
    P2_0 = 0x0200, P2_1, P2_2, P2_3, P2_4, P2_5, P2_6, P2_7, P2_8, P2_9, P2_10, P2_11, P2_12, P2_13, P2_14, P2_15,
    P3_0 = 0x0300, P3_1, P3_2, P3_3, P3_4, P3_5, P3_6, P3_7, P3_8, P3_9, P3_10, P3_11, P3_12, P3_13, P3_14, P3_15,
    P4_0 = 0x0400, P4_1, P4_2, P4_3, P4_4, P4_5, P4_6, P4_7, P4_8, P4_9, P4_10, P4_11, P4_12, P4_13, P4_14, P4_15,
    P5_0 = 0x0500, P5_1, P5_2, P5_3, P5_4, P5_5, P5_6, P5_7, P5_8, P5_9, P5_10, P5_11, P5_12, P5_13, P5_14, P5_15,

    // mbed Pin Names
#define LED1 P4_8
#define LED2 P4_7
#define BUTTON0 P2_5

    CONSOLE_TX = P1_9,
    CONSOLE_RX = P1_10,

#ifdef TARGET_FF_ARDUINO_UNO
    // Arduino Pin Names
    ARDUINO_UNO_D0 = P1_10,
    ARDUINO_UNO_D1 = P1_9,
    ARDUINO_UNO_D2 = P4_9,
    ARDUINO_UNO_D3 = P1_4,
    ARDUINO_UNO_D4 = P1_7,
    ARDUINO_UNO_D5 = P1_5,
    ARDUINO_UNO_D6 = P3_3,
    ARDUINO_UNO_D7 = P1_13,
    ARDUINO_UNO_D8 = P4_2,
    ARDUINO_UNO_D9 = P3_2,
    ARDUINO_UNO_D10 = P1_3,
    ARDUINO_UNO_D11 = P1_1,
    ARDUINO_UNO_D12 = P1_0,
    ARDUINO_UNO_D13 = P1_2,
    ARDUINO_UNO_D14 = P4_1,
    ARDUINO_UNO_D15 = P4_0,

    ARDUINO_UNO_A0 = P0_0,
    ARDUINO_UNO_A1 = P0_1,
    ARDUINO_UNO_A2 = P0_2,
    ARDUINO_UNO_A3 = P0_3,
    ARDUINO_UNO_A4 = P0_4,
    ARDUINO_UNO_A5 = P0_13,
#endif

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

typedef enum {
    PullUp = 0,
    PullDown = 3,
    PullNone = 2,
    OpenDrain = 4,
    PullDefault = PullNone
} PinMode;

#define PINGROUP(pin) (((pin)>>PORT_SHIFT)&0x0f)
#define PINNO(pin) ((pin)&0x0f)

#ifdef __cplusplus
}
#endif

#endif
