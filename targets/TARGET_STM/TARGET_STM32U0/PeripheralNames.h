/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ADC_1 = (int)ADC1_BASE,
} ADCName;

typedef enum {
    DAC_1 = (int)DAC1_BASE,
} DACName;

typedef enum {
    UART_1 = (int)USART1_BASE,
    UART_2 = (int)USART2_BASE,
#if USART3_BASE
    UART_3 = (int)USART3_BASE,
#endif
#if USART4_BASE
    UART_4 = (int)USART4_BASE,
#endif
    LPUART_1 = (int)LPUART1_BASE,
#if LPUART2_BASE
	LPUART_2 = (int)LPUART2_BASE,
#endif
#if LPUART3_BASE
	LPUART_3 = (int)LPUART3_BASE,
#endif
} UARTName;

#define DEVICE_SPI_COUNT 3
typedef enum {
    SPI_1 = (int)SPI1_BASE,
#if SPI2_BASE
    SPI_2 = (int)SPI2_BASE,
#endif
#if SPI3_BASE
    SPI_3 = (int)SPI3_BASE,
#endif
} SPIName;

typedef enum {
    I2C_1 = (int)I2C1_BASE,
#if I2C3_BASE
    I2C_2 = (int)I2C2_BASE,
#endif
#if I2C3_BASE
    I2C_3 = (int)I2C3_BASE,
#endif
#if I2C4_BASE
    I2C_4 = (int)I2C4_BASE,
#endif
} I2CName;

typedef enum {
#if TIM1_BASE
    PWM_1  = (int)TIM1_BASE,
#endif
#if TIM2_BASE
    PWM_2  = (int)TIM2_BASE,
#endif
#if TIM3_BASE
    PWM_3  = (int)TIM3_BASE,
#endif
#if TIM4_BASE
    PWM_4  = (int)TIM4_BASE,
#endif
#if TIM5_BASE
    PWM_5  = (int)TIM5_BASE,
#endif
#if TIM6_BASE
    PWM_6  = (int)TIM6_BASE,
#endif
#if TIM7_BASE
    PWM_7  = (int)TIM7_BASE,
#endif
#if TIM8_BASE
    PWM_8  = (int)TIM8_BASE,
#endif
#if TIM9_BASE
    PWM_9  = (int)TIM9_BASE,
#endif
#if TIM10_BASE
    PWM_10 = (int)TIM10_BASE,
#endif
#if TIM11_BASE
    PWM_11 = (int)TIM11_BASE,
#endif
#if TIM12_BASE
    PWM_12 = (int)TIM12_BASE,
#endif
#if TIM13_BASE
    PWM_13 = (int)TIM13_BASE,
#endif
#if TIM14_BASE
    PWM_14 = (int)TIM14_BASE,
#endif
#if TIM15_BASE
    PWM_15 = (int)TIM15_BASE,
#endif
#if TIM16_BASE
    PWM_16 = (int)TIM16_BASE,
#endif
} PWMName;

typedef enum {
    USB_FS = (int)USB_DRD_BASE
} USBName;


#ifdef __cplusplus
}
#endif

#endif
