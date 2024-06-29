/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2023 STMicroelectronics.
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
#if ADC2_BASE
    ADC_2 = (int)ADC2_BASE,
#endif
#if ADC3_BASE
    ADC_3 = (int)ADC3_BASE
#endif
} ADCName;

typedef enum {
    DAC_1 = DAC1_BASE,
#if DAC2_BASE
    DAC_2 = DAC2_BASE,
#endif
} DACName;

typedef enum {
    UART_1 = (int)USART1_BASE,
    UART_2 = (int)USART2_BASE,
    UART_3 = (int)USART3_BASE,
#if UART4_BASE
    UART_4 = (int)UART4_BASE,
#endif
#if UART5_BASE
    UART_5 = (int)UART5_BASE,
#endif
#if USART6_BASE
    UART_6 = (int)USART6_BASE,
#endif
#if UART7_BASE
    UART_7 = (int)UART7_BASE,
#endif
#if UART8_BASE
    UART_8 = (int)UART8_BASE,
#endif
#if UART9_BASE
    UART_9 = (int)UART9_BASE,
#endif
#if USART10_BASE
    UART_10 = (int)USART10_BASE,
#endif
#if USART11_BASE
    UART_11 = (int)USART11_BASE,
#endif
#if UART12_BASE
    UART_12 = (int)UART12_BASE,
#endif
    LPUART_1 = (int)LPUART1_BASE
} UARTName;

#define DEVICE_SPI_COUNT 6
typedef enum {
    SPI_1 = (int)SPI1_BASE,
    SPI_2 = (int)SPI2_BASE,
    SPI_3 = (int)SPI3_BASE,
#if SPI4_BASE
    SPI_4 = (int)SPI4_BASE,
#endif
#if SPI5_BASE
    SPI_5 = (int)SPI5_BASE,
#endif
#if SPI6_BASE
    SPI_6 = (int)SPI6_BASE
#endif
} SPIName;

typedef enum {
    I2C_1 = (int)I2C1_BASE,
    I2C_2 = (int)I2C2_BASE,
#if I2C3_BASE
    I2C_3 = (int)I2C3_BASE,
#endif
#if I2C4_BASE
    I2C_4 = (int)I2C4_BASE,
#endif
} I2CName;

typedef enum {
#if HRTIM1_BASE
    PWM_I  = (int)HRTIM1_BASE,
#endif
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
#if TIM17_BASE
    PWM_17 = (int)TIM17_BASE,
#endif
#if TIM23_BASE
    PWM_23 = (int)TIM23_BASE,
#endif
#if TIM24_BASE
    PWM_24 = (int)TIM24_BASE,
#endif
} PWMName;

typedef enum {
    CAN_1 = (int)FDCAN1_BASE,
#if FDCAN2_BASE
    CAN_2 = (int)FDCAN2_BASE,
#endif
#if FDCAN3_BASE
    CAN_3 = (int)FDCAN3_BASE,
#endif
} CANName;

#if defined OCTOSPI1_R_BASE
typedef enum {
    OSPI_1 = (int)OCTOSPI1_R_BASE,
} OSPIName;
#endif

typedef enum {
#if defined USB_DRD_FS_BASE 
    USB_FS = (int)USB_DRD_FS_BASE
#else
    USB_FS = (int)USB_DRD_BASE
#endif
} USBName;


#ifdef __cplusplus
}
#endif

#endif
