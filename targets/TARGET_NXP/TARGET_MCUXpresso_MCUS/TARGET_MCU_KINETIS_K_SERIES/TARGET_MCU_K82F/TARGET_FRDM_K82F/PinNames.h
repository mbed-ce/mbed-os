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

/* MBED TARGET LIST: FRDM_K82F */

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
    PTA0  = (0 << GPIO_PORT_SHIFT | 0 ),
    PTA1  = (0 << GPIO_PORT_SHIFT | 1 ),
    PTA2  = (0 << GPIO_PORT_SHIFT | 2 ),
    PTA3  = (0 << GPIO_PORT_SHIFT | 3 ),
    PTA4  = (0 << GPIO_PORT_SHIFT | 4 ),
    PTA5  = (0 << GPIO_PORT_SHIFT | 5 ),
    PTA6  = (0 << GPIO_PORT_SHIFT | 6 ),
    PTA7  = (0 << GPIO_PORT_SHIFT | 7 ),
    PTA8  = (0 << GPIO_PORT_SHIFT | 8 ),
    PTA9  = (0 << GPIO_PORT_SHIFT | 9 ),
    PTA10 = (0 << GPIO_PORT_SHIFT | 10),
    PTA11 = (0 << GPIO_PORT_SHIFT | 11),
    PTA12 = (0 << GPIO_PORT_SHIFT | 12),
    PTA13 = (0 << GPIO_PORT_SHIFT | 13),
    PTA14 = (0 << GPIO_PORT_SHIFT | 14),
    PTA15 = (0 << GPIO_PORT_SHIFT | 15),
    PTA16 = (0 << GPIO_PORT_SHIFT | 16),
    PTA17 = (0 << GPIO_PORT_SHIFT | 17),
    PTA18 = (0 << GPIO_PORT_SHIFT | 18),
    PTA19 = (0 << GPIO_PORT_SHIFT | 19),
    PTA20 = (0 << GPIO_PORT_SHIFT | 20),
    PTA21 = (0 << GPIO_PORT_SHIFT | 21),
    PTB0  = (1 << GPIO_PORT_SHIFT | 0 ),
    PTB1  = (1 << GPIO_PORT_SHIFT | 1 ),
    PTB2  = (1 << GPIO_PORT_SHIFT | 2 ),
    PTB3  = (1 << GPIO_PORT_SHIFT | 3 ),
    PTB8  = (1 << GPIO_PORT_SHIFT | 8 ),
    PTB9  = (1 << GPIO_PORT_SHIFT | 9 ),
    PTB10 = (1 << GPIO_PORT_SHIFT | 10),
    PTB11 = (1 << GPIO_PORT_SHIFT | 11),
    PTB16 = (1 << GPIO_PORT_SHIFT | 16),
    PTB17 = (1 << GPIO_PORT_SHIFT | 17),
    PTB18 = (1 << GPIO_PORT_SHIFT | 18),
    PTB19 = (1 << GPIO_PORT_SHIFT | 19),
    PTB20 = (1 << GPIO_PORT_SHIFT | 20),
    PTB21 = (1 << GPIO_PORT_SHIFT | 21),
    PTB22 = (1 << GPIO_PORT_SHIFT | 22),
    PTB23 = (1 << GPIO_PORT_SHIFT | 23),
    PTC0  = (2 << GPIO_PORT_SHIFT | 0 ),
    PTC1  = (2 << GPIO_PORT_SHIFT | 1 ),
    PTC2  = (2 << GPIO_PORT_SHIFT | 2 ),
    PTC3  = (2 << GPIO_PORT_SHIFT | 3 ),
    PTC4  = (2 << GPIO_PORT_SHIFT | 4 ),
    PTC5  = (2 << GPIO_PORT_SHIFT | 5 ),
    PTC6  = (2 << GPIO_PORT_SHIFT | 6 ),
    PTC7  = (2 << GPIO_PORT_SHIFT | 7 ),
    PTC8  = (2 << GPIO_PORT_SHIFT | 8 ),
    PTC9  = (2 << GPIO_PORT_SHIFT | 9 ),
    PTC10 = (2 << GPIO_PORT_SHIFT | 10),
    PTC11 = (2 << GPIO_PORT_SHIFT | 11),
    PTC12 = (2 << GPIO_PORT_SHIFT | 12),
    PTC13 = (2 << GPIO_PORT_SHIFT | 13),
    PTC14 = (2 << GPIO_PORT_SHIFT | 14),
    PTC15 = (2 << GPIO_PORT_SHIFT | 15),
    PTC16 = (2 << GPIO_PORT_SHIFT | 16),
    PTC17 = (2 << GPIO_PORT_SHIFT | 17),
    PTC18 = (2 << GPIO_PORT_SHIFT | 18),
    PTC19 = (2 << GPIO_PORT_SHIFT | 19),
    PTD0  = (3 << GPIO_PORT_SHIFT | 0 ),
    PTD1  = (3 << GPIO_PORT_SHIFT | 1 ),
    PTD2  = (3 << GPIO_PORT_SHIFT | 2 ),
    PTD3  = (3 << GPIO_PORT_SHIFT | 3 ),
    PTD4  = (3 << GPIO_PORT_SHIFT | 4 ),
    PTD5  = (3 << GPIO_PORT_SHIFT | 5 ),
    PTD6  = (3 << GPIO_PORT_SHIFT | 6 ),
    PTD7  = (3 << GPIO_PORT_SHIFT | 7 ),
    PTD8  = (3 << GPIO_PORT_SHIFT | 8 ),
    PTD9  = (3 << GPIO_PORT_SHIFT | 9 ),
    PTD11 = (3 << GPIO_PORT_SHIFT | 11),
    PTD12 = (3 << GPIO_PORT_SHIFT | 12),
    PTD13 = (3 << GPIO_PORT_SHIFT | 13),
    PTD14 = (3 << GPIO_PORT_SHIFT | 14),
    PTE0  = (4 << GPIO_PORT_SHIFT | 0 ),
    PTE1  = (4 << GPIO_PORT_SHIFT | 1 ),
    PTE2  = (4 << GPIO_PORT_SHIFT | 2 ),
    PTE3  = (4 << GPIO_PORT_SHIFT | 3 ),
    PTE4  = (4 << GPIO_PORT_SHIFT | 4 ),
    PTE5  = (4 << GPIO_PORT_SHIFT | 5 ),
    PTE6  = (4 << GPIO_PORT_SHIFT | 6 ),
    PTE7  = (4 << GPIO_PORT_SHIFT | 7 ),
    PTE8  = (4 << GPIO_PORT_SHIFT | 8 ),
    PTE9  = (4 << GPIO_PORT_SHIFT | 9 ),
    PTE10 = (4 << GPIO_PORT_SHIFT | 10),
    PTE11 = (4 << GPIO_PORT_SHIFT | 11),
    PTE12 = (4 << GPIO_PORT_SHIFT | 12),
    PTE13 = (4 << GPIO_PORT_SHIFT | 13),
    PTE16 = (4 << GPIO_PORT_SHIFT | 16),
    PTE17 = (4 << GPIO_PORT_SHIFT | 17),
    PTE18 = (4 << GPIO_PORT_SHIFT | 18),
    PTE19 = (4 << GPIO_PORT_SHIFT | 19),

    DAC0_OUT = 0xFEFE, /* DAC does not have Pin Name in RM */

    // USB Pins
    CONSOLE_TX = PTC15,
    CONSOLE_RX = PTC14,

    // Arduino Headers
    ARDUINO_UNO_D0 = PTB16,
    ARDUINO_UNO_D1 = PTB17,
    ARDUINO_UNO_D2 = PTC12,
    ARDUINO_UNO_D3 = PTD0,
    ARDUINO_UNO_D4 = PTC11,
    ARDUINO_UNO_D5 = PTC10,
    ARDUINO_UNO_D6 = PTC8,
    ARDUINO_UNO_D7 = PTC9,
    ARDUINO_UNO_D8 = PTC3,
    ARDUINO_UNO_D9 = PTC5,
    ARDUINO_UNO_D10 = PTD4,
    ARDUINO_UNO_D11 = PTD2,
    ARDUINO_UNO_D12 = PTD3,
    ARDUINO_UNO_D13 = PTD1,
    ARDUINO_UNO_D14 = PTA1,
    ARDUINO_UNO_D15 = PTA2,

    ARDUINO_UNO_A0 = PTB0,
    ARDUINO_UNO_A1 = PTB1,
    ARDUINO_UNO_A2 = PTC1,
    ARDUINO_UNO_A3 = PTC2,
    ARDUINO_UNO_A4 = PTB3,
    ARDUINO_UNO_A5 = PTB2,

    // QSPI flash pins
    QSPI_FLASH1_IO0 = PTE2,
    QSPI_FLASH1_IO1 = PTE4,
    QSPI_FLASH1_IO2 = PTE3,
    QSPI_FLASH1_IO3 = PTE0,
    QSPI_FLASH1_SCK = PTE1,
    QSPI_FLASH1_CSN = PTE5,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

// LEDs
#define LED_RED   PTC8
#define LED_GREEN PTC9
#define LED_BLUE  PTC10

// Standardized LED names
#define LED1 LED_RED
#define LED2 LED_GREEN
#define LED3 LED_BLUE

// Standardized button names
#define BUTTON1 PTA4
#define BUTTON2 PTC6

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
