/*
 * Copyright (c) 2019-2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* MBED TARGET LIST: SFE_ARTEMIS */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "am_bsp.h"
#include "objects_gpio.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define NC_VAL (int)0xFFFFFFFF

typedef enum
{
    // Digital naming
    D0 = 25,
    D1 = 24,
    D2 = 35,
    D3 = 4,
    D4 = 22,
    D5 = 23,
    D6 = 27,
    D7 = 28,
    D8 = 32,
    D9 = 12,
    D10 = 13,
    D11 = 7,
    D12 = 6,
    D13 = 5,
    D14 = 40,
    D15 = 39,
    D16 = 29,
    D17 = 11,
    D18 = 34,
    D19 = 33,
    D20 = 16,
    D21 = 31,

    // Analog naming
    A0 = D16,
    A1 = D17,
    A2 = D18,
    A3 = D19,
    A4 = D20,
    A5 = D21,
    A6 = D2,
    // A7 = ??
    A8 = D8,
    A9 = D9,
    A10 = D10,

    // UART
    SERIAL_TX = AM_BSP_PRIM_UART_TX_PIN,
    SERIAL_RX = AM_BSP_PRIM_UART_RX_PIN,
    CONSOLE_TX = SERIAL_TX,
    CONSOLE_RX = SERIAL_RX,

    SERIAL1_TX = D1,
    SERIAL1_RX = D0,

    // Not connected
    NC = NC_VAL
} PinName;

// LEDs
#define LED1 D13 // Blue LED

// I2C bus
#define I2C_SCL D15
#define I2C_SDA D14
#define QWIIC_SCL I2C_SCL
#define QWIIC_SDA I2C_SDA

// SPI bus
#define SPI_CLK D13
#define SPI_SDO D11
#define SPI_SDI D12

#if defined(MBED_CONF_TARGET_STDIO_UART_TX)
#define STDIO_UART_TX MBED_CONF_TARGET_STDIO_UART_TX
#else
#define STDIO_UART_TX CONSOLE_TX
#endif
#if defined(MBED_CONF_TARGET_STDIO_UART_RX)
#define STDIO_UART_RX MBED_CONF_TARGET_STDIO_UART_RX
#else
#define STDIO_UART_RX CONSOLE_RX
#endif

#ifdef __cplusplus
}
#endif

#endif
