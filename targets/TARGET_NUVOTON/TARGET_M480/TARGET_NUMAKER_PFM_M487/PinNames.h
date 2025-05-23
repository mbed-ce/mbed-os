/*
 * Copyright (c) 2020, Nuvoton Technology Corporation
 *
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

/* MBED TARGET LIST: NUMAKER_PFM_M487 */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    // Not connected
    NC = (int)0xFFFFFFFF,

    // Generic naming
    PA_0    = NU_PINNAME(0, 0), PA_1, PA_2, PA_3, PA_4, PA_5, PA_6, PA_7, PA_8, PA_9, PA_10, PA_11, PA_12, PA_13, PA_14, PA_15,
    PB_0    = NU_PINNAME(1, 0), PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7, PB_8, PB_9, PB_10, PB_11, PB_12, PB_13, PB_14, PB_15,
    PC_0    = NU_PINNAME(2, 0), PC_1, PC_2, PC_3, PC_4, PC_5, PC_6, PC_7, PC_8, PC_9, PC_10, PC_11, PC_12, PC_13, PC_14,
    PD_0    = NU_PINNAME(3, 0), PD_1, PD_2, PD_3, PD_4, PD_5, PD_6, PD_7, PD_8, PD_9, PD_10, PD_11, PD_12, PD_13, PD_14,
    PE_0    = NU_PINNAME(4, 0), PE_1, PE_2, PE_3, PE_4, PE_5, PE_6, PE_7, PE_8, PE_9, PE_10, PE_11, PE_12, PE_13, PE_14, PE_15,
    PF_0    = NU_PINNAME(5, 0), PF_1, PF_2, PF_3, PF_4, PF_5, PF_6, PF_7, PF_8, PF_9, PF_10, PF_11,
    PG_0    = NU_PINNAME(6, 0), PG_1, PG_2, PG_3, PG_4, PG_5, PG_6, PG_7, PG_8, PG_9, PG_10, PG_11, PG_12, PG_13, PG_14, PG_15,
    PH_0    = NU_PINNAME(7, 0), PH_1, PH_2, PH_3, PH_4, PH_5, PH_6, PH_7, PH_8, PH_9, PH_10, PH_11,

    // Arduino UNO naming
    ARDUINO_UNO_A0 = PB_6,
    ARDUINO_UNO_A1 = PB_7,
    ARDUINO_UNO_A2 = PB_8,
    ARDUINO_UNO_A3 = PB_9,
    ARDUINO_UNO_A4 = PB_0,
    ARDUINO_UNO_A5 = PB_1,

    ARDUINO_UNO_D0 = PB_2,
    ARDUINO_UNO_D1 = PB_3,
    ARDUINO_UNO_D2 = PC_9,
    ARDUINO_UNO_D3 = PC_10,
    ARDUINO_UNO_D4 = PC_11,
    ARDUINO_UNO_D5 = PC_12,
    ARDUINO_UNO_D6 = PE_4,
    ARDUINO_UNO_D7 = PE_5,
    ARDUINO_UNO_D8 = PA_5,
    ARDUINO_UNO_D9 = PA_4,
    ARDUINO_UNO_D10 = PA_3,
    ARDUINO_UNO_D11 = PA_0,
    ARDUINO_UNO_D12 = PA_1,
    ARDUINO_UNO_D13 = PA_2,
    ARDUINO_UNO_D14 = PG_1,
    ARDUINO_UNO_D15 = PG_0,

    // Note: board-specific
    // UART naming
    CONSOLE_TX           = MBED_CONF_TARGET_USB_UART_TX,
    CONSOLE_RX           = MBED_CONF_TARGET_USB_UART_RX,
#if defined(MBED_CONF_TARGET_STDIO_UART_TX)
    STDIO_UART_TX   = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    STDIO_UART_TX   = CONSOLE_TX,
#endif
#if defined(MBED_CONF_TARGET_STDIO_UART_RX)
    STDIO_UART_RX   = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    STDIO_UART_RX   = CONSOLE_RX,
#endif

    // Force PinName to 32-bit required by NU_PINNAME_BIND(...)
    FORCE_ENUM_PINNAME_32BIT = 0x7FFFFFFF,

} PinName;

// LEDs
#define LED1 PH_0 // Red LED
#define LED2 PH_1 // Yellow LED
#define LED3 PH_2 // Green LED

// Buttons
#define BUTTON1 PG_15
#define BUTTON2 PF_11

#ifdef __cplusplus
}
#endif

#endif // MBED_PINNAMES_H
