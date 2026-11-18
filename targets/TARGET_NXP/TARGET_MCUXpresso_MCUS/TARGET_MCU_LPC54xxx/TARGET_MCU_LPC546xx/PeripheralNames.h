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
#include "PortNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OSC32KCLK = 0,
} RTCName;

typedef FlexcommName UARTName;

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
#define STDIO_UART        Flexcomm0

typedef FlexcommName I2CName;

#define ADC_INSTANCE_SHIFT           8
#define ADC_B_CHANNEL_SHIFT        5

typedef enum {
    ADC0_SE0  = 0,
    ADC0_SE1  = 1,
    ADC0_SE2  = 2,
    ADC0_SE3  = 3,
    ADC0_SE4  = 4,
    ADC0_SE5  = 5,
    ADC0_SE6  = 6,
    ADC0_SE7  = 7,
    ADC0_SE8  = 8,
    ADC0_SE9  = 9,
    ADC0_SE10 = 10,
    ADC0_SE11 = 11,
} ADCName;

typedef enum {
    CAN_0 = 0,
    CAN_1 = 1
} CANName;

// Pin function bitfields
#define MBED_LPC_PIN_FUNCTION_FUNC_Msk 0xF
#define MBED_LPC_PIN_FUNCTION_FUNC_Pos 0
#define MBED_LPC_PIN_FUNCTION_SSELNUM_Msk (0xF << 16)
#define MBED_LPC_PIN_FUNCTION_SSELNUM_Pos 16

typedef FlexcommName SPIName;

typedef enum {
    QSPI_0 = 0
} QSPIName;

#ifdef __cplusplus
}
#endif

#endif
