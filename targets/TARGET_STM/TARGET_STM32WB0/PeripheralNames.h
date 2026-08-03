/* SPDX-License-Identifier: Apache-2.0 */
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"

typedef enum { ADC_1 = (int) ADC1_BASE } ADCName;
typedef enum {
    UART_1 = (int) USART1_BASE,
    LPUART_1 = (int) LPUART1_BASE,
} UARTName;
typedef enum { SPI_3 = (int) SPI3_BASE } SPIName;
typedef enum { I2C_1 = (int) I2C1_BASE } I2CName;
typedef enum {
    PWM_2 = (int) TIM2_BASE,
    PWM_16 = (int) TIM16_BASE,
    PWM_17 = (int) TIM17_BASE,
} PWMName;

#endif
