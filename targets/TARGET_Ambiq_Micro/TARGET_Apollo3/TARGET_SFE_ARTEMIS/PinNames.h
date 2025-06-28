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
    p0 = 25,
    p1 = 24,
    p2 = 35,
    p3 = 4,
    p4 = 22,
    p5 = 23,
    p6 = 27,
    p7 = 28,
    p8 = 32,
    p9 = 12,
    p10 = 13,
    p11 = 7,
    p12 = 6,
    p13 = 5,
    p14 = 40,
    p15 = 39,
    p16 = 29,
    p17 = 11,
    p18 = 34,
    p19 = 33,
    p20 = 16,
    p21 = 31,

#ifdef TARGET_FF_ARDUINO_UNO
    // Arduino form factor pins
    ARDUINO_UNO_D0 = p0,
    ARDUINO_UNO_D1 = p1,
    ARDUINO_UNO_D2 = p2,
    ARDUINO_UNO_D3 = p3,
    ARDUINO_UNO_D4 = p4,
    ARDUINO_UNO_D5 = p5,
    ARDUINO_UNO_D6 = p6,
    ARDUINO_UNO_D7 = p7,
    ARDUINO_UNO_D8 = p8,
    ARDUINO_UNO_D9 = p9,
    ARDUINO_UNO_D10 = p10,
    ARDUINO_UNO_D11 = p11,
    ARDUINO_UNO_D12 = p12,
    ARDUINO_UNO_D13 = p13,
    ARDUINO_UNO_D14 = p14,
    ARDUINO_UNO_D15 = p15,

    ARDUINO_UNO_A0 = p16,
    ARDUINO_UNO_A1 = p17,
    ARDUINO_UNO_A2 = p18,
    ARDUINO_UNO_A3 = p19,
    ARDUINO_UNO_A4 = p20,
    ARDUINO_UNO_A5 = p21,
    ARDUINO_UNO_A6 = p2,
#endif

    // UART
    SERIAL_TX = AM_BSP_PRIM_UART_TX_PIN,
    SERIAL_RX = AM_BSP_PRIM_UART_RX_PIN,
    CONSOLE_TX = SERIAL_TX,
    CONSOLE_RX = SERIAL_RX,

    SERIAL1_TX = p1,
    SERIAL1_RX = p0,

    // Not connected
    NC = NC_VAL
} PinName;

// LEDs
#define LED1 p13 // Blue LED

// I2C bus
// note: I2C_SCL and I2C_SDA defines are provided by the FF_ARDUINO_UNO header
#define QWIIC_SCL I2C_SCL
#define QWIIC_SDA I2C_SDA

// SPI bus
#define SPI_CLK p13
#define SPI_SDO p11
#define SPI_SDI p12

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
