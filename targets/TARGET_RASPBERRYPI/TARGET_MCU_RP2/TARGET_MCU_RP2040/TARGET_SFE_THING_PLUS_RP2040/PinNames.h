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

/* MBED TARGET LIST: SFE_THING_PLUS_RP2040 */

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
    ADC_VREF = 0xF1,

    // Analog pins
#ifndef ARDUINO_ARCH_MBED
    A0          = IO_26,
    A1          = IO_27,
    A2          = IO_28,
    A3          = IO_29,
#endif

    CONSOLE_TX = IO_0,
    CONSOLE_RX = IO_1,

    // Header pin numbering.
    // This should match the labels on the board.
    HDR_P26_ADC0 = IO_26,
    HDR_P27_ADC1 = IO_27,
    HDR_P28_ADC2 = IO_28,
    HDR_P29_ADC3 = IO_29,
    HDR_P2_SCK = IO_2,
    HDR_P3_COPI = IO_3,
    HDR_P4_CIPO = IO_4,
    HDR_P1_RX = IO_1,
    HDR_P0_TX = IO_0,
    HDR_P16 = IO_16,
    HDR_P17 = IO_17,
    HDR_P18 = IO_18,
    HDR_P19 = IO_19,
    HDR_P20 = IO_20,
    HDR_P21 = IO_21,
    HDR_P22 = IO_22,

    // Pins 7 and 23 shorted together on this header pin
    HDR_SCL_P7 = IO_7,
    HDR_SCL_P23 = IO_23,

    HDR_SDA_P6 = IO_6,

    // Internal multicolor LED
    ONBOARD_WS2812_DI = IO_8,

    // Connected to MAX17048 fuel gauge ~ALRT pin
    BATT_ALERT = IO_24,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

// Onboard LED
#define LED1 IO_25

// Default communication pins
#define I2C_SCL IO_7
#define I2C_SDA IO_6

#define SPI_SCLK IO_2
#define SPI_MOSI IO_3
#define SPI_MISO IO_4

// MicroSD connector
#define SD_CMD_MOSI IO_18
#define SD_CLK IO_17
#define SD_DAT0_MISO IO_12
#define SD_DAT1 IO_11
#define SD_DAT2 IO_10
#define SD_DAT3_CS IO_9

#ifdef __cplusplus
}
#endif

#endif
