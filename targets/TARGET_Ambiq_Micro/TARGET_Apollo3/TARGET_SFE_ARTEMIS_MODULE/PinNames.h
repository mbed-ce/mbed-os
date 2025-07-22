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

/* MBED TARGET LIST: SFE_ARTEMIS_MODULE */

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
    // Apollo3 I/O pins - CSP and BGA packages
    IO_0 = 0,
    IO_1 = 1,
    IO_2 = 2,
    IO_3 = 3,
    IO_4 = 4,
    IO_5 = 5,
    IO_6 = 6,
    IO_7 = 7,
    IO_8 = 8,
    IO_9 = 9,
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
    IO_39 = 39,
    IO_40 = 40,
    IO_41 = 41,
    IO_44 = 44,
    IO_47 = 47,
    IO_48 = 48,
    IO_49 = 49,

    // Apollo3 I/O pins - BGA package only
    IO_30 = 30,
    IO_31 = 31,
    IO_32 = 32,
    IO_33 = 33,
    IO_34 = 34,
    IO_35 = 35,
    IO_36 = 36,
    IO_37 = 37,
    IO_38 = 38,
    IO_42 = 42,
    IO_43 = 43,
    IO_45 = 45,
    IO_46 = 46,

    SERIAL_TX = IO_48,
    SERIAL_RX = IO_49,
    CONSOLE_TX = SERIAL_TX,
    CONSOLE_RX = SERIAL_RX,

    // Not a real pin on the device, but can be passed to AnalogIn to read the internal temperature sensor
    INT_TEMP_SENSOR = 0x10000,

    // Not connected
    NC = NC_VAL
} PinName;

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
