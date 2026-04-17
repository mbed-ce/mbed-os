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
    // RP2040 I/O pins
    IO_0  = 0,
    IO_1  = 1,
    IO_2  = 2,
    IO_3  = 3,
    IO_4  = 4,
    IO_5  = 5,
    IO_6  = 6,
    IO_7  = 7,
    IO_8  = 8,
    IO_9  = 9,
    IO_10 = 10,
    IO_11 = 11,
    IO_12 = 12,
    IO_13 = 13,
    IO_14 = 14,
    IO_15 = 15,
    IO_16 = 16,
    IO_17 = 17,
    IO_18 = 18,
    IO_19 = 19,
    IO_20 = 20,
    IO_21 = 21,
    IO_22 = 22,
    IO_23 = 23,
    IO_24 = 24,
    IO_25 = 25,
    IO_26 = 26,
    IO_27 = 27,
    IO_28 = 28,
    IO_29 = 29,

    // ADC internal channels
    ADC_TEMP = 0xF0,

    // Analog pins
#ifndef ARDUINO_ARCH_MBED
    A0          = IO_26,
    A1          = IO_27,
    A2          = IO_28,
    A3          = IO_29,
#endif

    CONSOLE_TX = IO_0,
    CONSOLE_RX = IO_1,

    // Raspberry Pi Pico PCB header pin numbering.
    // See diagram here: https://www.raspberrypi.com/documentation/microcontrollers/pico-series.html#layout_non-wireless
    // This follows the innermost (grey) numbers.
    PICO_P0 = IO_0,
    PICO_P1 = IO_1,
    PICO_P4 = IO_2,
    PICO_P5 = IO_3,
    PICO_P6 = IO_4,
    PICO_P7 = IO_5,
    PICO_P9 = IO_6,
    PICO_P10 = IO_7,
    PICO_P11 = IO_8,
    PICO_P12 = IO_9,
    PICO_P14 = IO_10,
    PICO_P15 = IO_11,
    PICO_P16 = IO_12,
    PICO_P17 = IO_13,
    PICO_P19 = IO_14,
    PICO_P20 = IO_15,
    PICO_P21 = IO_16,
    PICO_P22 = IO_17,
    PICO_P24 = IO_18,
    PICO_P25 = IO_19,
    PICO_P26 = IO_20,
    PICO_P27 = IO_21,
    PICO_P29 = IO_22,
    PICO_P31 = IO_26,
    PICO_P32 = IO_27,
    PICO_P34 = IO_28,

    // These three pins (plus the LED defined below) are connected internally on the Raspberry Pi Pico
    // rather than being pinned out
    BUCK_REG_PWM_MODE = IO_23,
    USB_VBUS_PRESENT = IO_24,
    VSYS_MEAS = IO_29,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

// Onboard LED
#define LED1 IO_25

// Default communication pins
#define I2C_SCL IO_5
#define I2C_SDA IO_4

#define SPI_SCLK IO_18
#define SPI_MOSI IO_19
#define SPI_MISO IO_16
#define SPI_CS IO_17

#ifdef __cplusplus
}
#endif

#endif
