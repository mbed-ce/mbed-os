/* mbed Microcontroller Library
 * Copyright (c) 2026, Arm Limited and affiliates.
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

/* MBED TARGET LIST: RASPBERRY_PI_PICO */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "PeripheralNames.h"
#include "pico.h"
#include "boards/pico.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    p0  = 0,
    p1  = 1,
    p2  = 2,
    p3  = 3,
    p4  = 4,
    p5  = 5,
    p6  = 6,
    p7  = 7,
    p8  = 8,
    p9  = 9,
    p10 = 10,
    p11 = 11,
    p12 = 12,
    p13 = 13,
    p14 = 14,
    p15 = 15,
    p16 = 16,
    p17 = 17,
    p18 = 18,
    p19 = 19,
    p20 = 20,
    p21 = 21,
    p22 = 22,
    p23 = 23,
    p24 = 24,
    p25 = 25,
    p26 = 26,
    p27 = 27,
    p28 = 28,
    p29 = 29,

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,

#ifndef ARDUINO_ARCH_MBED
    A0          = 26,
    A1          = 27,
    A2          = 28,
    A3          = 29,
#endif

    CONSOLE_TX = p0,
    CONSOLE_RX = p1,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

#define LED1 p25

#ifdef __cplusplus
}
#endif

#endif
