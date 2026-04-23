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
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"
#include "PinNames.h"

typedef enum {
    UART_0 = 0,
    UART_1,
} UARTName;

typedef enum {
    ADC0 = 0,
} ADCName;

typedef enum {
    SPI_0 = 0,
    SPI_1
} SPIName;

typedef enum {
    I2C_0 = 0,
    I2C_1
} I2CName;

typedef enum {
    PWM_0 = 0,
    PWM_1,
    PWM_2,
    PWM_3,
    PWM_4,
    PWM_5,
    PWM_6,
    PWM_7,
#if PICO_RP2350 && !PICO_RP2350A
    PWM_8,
    PWM_9,
    PWM_10,
    PWM_11
#endif
} PWMName;

/*  Defines to be used by application */
typedef enum {
    PIN_INPUT = 0,
    PIN_OUTPUT
} PinDirection;

typedef enum {
    PullNone          = 0,
    PullUp            = 1, // Resistance 32-86k @ 3.3V
    PullDown          = 2, // Resistance 36-113k @ 3.3V
    OpenDrainNoPull   = 4,
    OpenDrainPullUp   = OpenDrainNoPull & PullUp,
    OpenDrainPullDown = OpenDrainNoPull & PullDown,
    PushPullNoPull    = PullNone,
    PushPullPullUp    = PullUp,
    PushPullPullDown  = PullDown,
    OpenDrain         = OpenDrainPullUp,
    PullDefault       = PullNone
} PinMode;

#define STDIO_UART_TX     CONSOLE_TX
#define STDIO_UART_RX     CONSOLE_RX
#define STDIO_UART        uart0

#endif
