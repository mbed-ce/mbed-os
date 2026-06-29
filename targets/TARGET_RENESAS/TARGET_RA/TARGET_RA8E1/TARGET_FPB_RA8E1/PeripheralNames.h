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

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART_0 = 0,
    UART_1,
    UART_2,
    UART_3,
    UART_4,
    UART_9 = 9
} UARTName;

typedef enum {
    PWM_GPT0 = 0x00,
    PWM_GPT1,
    PWM_GPT2,
    PWM_GPT3,
    PWM_GPT4,
    PWM_GPT5,
    PWM_GPT10,
    PWM_GPT11,
    PWM_GPT12,
    PWM_GPT13,
    PWM_AGT0,
    PWM_AGT1,
} PWMName;

typedef enum {
    PORT_0= BSP_IO_PORT_00,
    PORT_1= BSP_IO_PORT_01,
    PORT_2= BSP_IO_PORT_02,
    PORT_3= BSP_IO_PORT_03,
    PORT_4= BSP_IO_PORT_04,
    PORT_5= BSP_IO_PORT_05,
    PORT_6= BSP_IO_PORT_06,
    PORT_7= BSP_IO_PORT_07,
    PORT_8= BSP_IO_PORT_08,
    PORT_9= BSP_IO_PORT_09,
} PortName;

typedef enum {
    ADC_0= 0,
    ADC_1,
} ADCName;

typedef enum {
    DAC_0= 0,
} DACName;

typedef enum {
    IRQ_0= 0,
    IRQ_1,
    IRQ_2,
    IRQ_3,
    IRQ_4,
    IRQ_5,
    IRQ_6,
    IRQ_7,
    IRQ_8,
    IRQ_9,
    IRQ_10,
    IRQ_11,
    IRQ_12,
    IRQ_13,
    IRQ_14,
    IRQ_15,
} IRQName;

typedef enum {
    SPI_0 = 0,
    SPI_1,
} SPIName;

typedef enum {
    I2C_0 = 0,
    I2C_1,
} I2CName;

typedef enum {
    CAN_0 = 0,
    CAN_1,
} CANName;


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
#define STDIO_UART UART_9

#define IRQ_CHANNELS_COUNT (16)
#define UART_COUNT (6)
#define CAN_COUNT  (2)

#ifdef __cplusplus
}
#endif

#endif
