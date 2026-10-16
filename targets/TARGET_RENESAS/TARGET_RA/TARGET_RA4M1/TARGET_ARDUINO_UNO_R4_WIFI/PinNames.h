/* mbed Microcontroller Library
 * Copyright (c) 2026 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

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

typedef enum {
    P0_0 = 0x0000, P0_1, P0_2, P0_3, P0_4, P0_5, P0_6, P0_7, P0_8, P0_9, P0_10, P0_11, P0_12, P0_13, P0_14, P0_15,
    P1_0 = 0x0100, P1_1, P1_2, P1_3, P1_4, P1_5, P1_6, P1_7, P1_8, P1_9, P1_10, P1_11, P1_12, P1_13, P1_14, P1_15,
    P2_0 = 0x0200, P2_1, P2_2, P2_3, P2_4, P2_5, P2_6, P2_7, P2_8, P2_9, P2_10, P2_11, P2_12, P2_13, P2_14, P2_15,
    P3_0 = 0x0300, P3_1, P3_2, P3_3, P3_4, P3_5, P3_6, P3_7, P3_8, P3_9, P3_10, P3_11, P3_12, P3_13, P3_14, P3_15,
    P4_0 = 0x0400, P4_1, P4_2, P4_3, P4_4, P4_5, P4_6, P4_7, P4_8, P4_9, P4_10, P4_11, P4_12, P4_13, P4_14, P4_15,
    P5_0 = 0x0500, P5_1, P5_2, P5_3, P5_4, P5_5, P5_6, P5_7, P5_8, P5_9, P5_10, P5_11, P5_12, P5_13, P5_14, P5_15,

    /* mbed Pin Names (board LED) */
    /* LED on D13 */
#define LED1 P1_2

    /* Console UART (SCI9) to ESP32-S3 USB bridge; also D22/D23 */
    CONSOLE_TX = P1_9,
    CONSOLE_RX = P1_10,

#ifdef TARGET_FF_ARDUINO_UNO
    /* Arduino Header Pin Names (UNO R4 WiFi pinout) */
    ARDUINO_UNO_D0 = P3_1,
    ARDUINO_UNO_D1 = P3_2,
    ARDUINO_UNO_D2 = P1_4,
    ARDUINO_UNO_D3 = P1_5,
    ARDUINO_UNO_D4 = P1_6,
    ARDUINO_UNO_D5 = P1_7,
    ARDUINO_UNO_D6 = P1_11,
    ARDUINO_UNO_D7 = P1_12,
    ARDUINO_UNO_D8 = P3_4,
    ARDUINO_UNO_D9 = P3_3,
    ARDUINO_UNO_D10 = P1_3,
    ARDUINO_UNO_D11 = P4_11,
    ARDUINO_UNO_D12 = P4_10,
    ARDUINO_UNO_D13 = P1_2,
    ARDUINO_UNO_D14 = P1_1,
    ARDUINO_UNO_D15 = P1_0,

    ARDUINO_UNO_A0 = P0_14,
    ARDUINO_UNO_A1 = P0_0,
    ARDUINO_UNO_A2 = P0_1,
    ARDUINO_UNO_A3 = P0_2,
    ARDUINO_UNO_A4 = P1_1,
    ARDUINO_UNO_A5 = P1_0,

    /* WiFi board extras */
    ARDUINO_UNO_D22 = P1_9,     /* TX (ESP32-S3 bridge UART) */
    ARDUINO_UNO_D23 = P1_10,    /* RX (ESP32-S3 bridge UART) */
    ARDUINO_UNO_D24 = P5_1,     /* TX WIFI */
    ARDUINO_UNO_D25 = P5_2,     /* RX WIFI */
    ARDUINO_UNO_D26 = P4_0,     /* QWIC SCL */
    ARDUINO_UNO_D27 = P4_1,     /* QWIC SDA */
    ARDUINO_UNO_D28 = P0_3,
    ARDUINO_UNO_D29 = P0_4,
#endif

    /* USB switch: drive high to route USB-C to RA4M1 (default: ESP32-S3) */
    USB_SWITCH = P4_8,

    /* Not connected */
    NC = (int)0xFFFFFFFF
} PinName;

typedef enum {
    PullUp = 0,
    PullDown = 3,
    PullNone = 2,
    OpenDrain = 4,
    PullDefault = PullNone
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
