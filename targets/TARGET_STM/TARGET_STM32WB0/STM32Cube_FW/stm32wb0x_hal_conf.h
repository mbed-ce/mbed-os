/* Copyright (c) 2024 STMicroelectronics.
 * Copyright (c) 2026 MbedCE Community Contributors (Jan Kamidra)
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef STM32WB0X_HAL_CONF_H
#define STM32WB0X_HAL_CONF_H

#define HAL_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_IWDG_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_RTC_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED

#define USE_HAL_TIM_REGISTER_CALLBACKS 0U
#define USE_HAL_RTC_REGISTER_CALLBACKS 0U
#define USE_HAL_UART_REGISTER_CALLBACKS 0U

#ifndef HSE_VALUE
#define HSE_VALUE 32000000UL
#endif
#ifndef HSE_STARTUP_TIMEOUT
#define HSE_STARTUP_TIMEOUT 100UL
#endif
#ifndef HSI_VALUE
#define HSI_VALUE 64000000UL
#endif
#ifndef LSI_VALUE
#define LSI_VALUE 32000UL
#endif
#ifndef LSE_VALUE
#define LSE_VALUE 32768UL
#endif
#ifndef RC64MPLL_VALUE
#define RC64MPLL_VALUE 64000000UL
#endif
#ifndef LSE_STARTUP_TIMEOUT
#define LSE_STARTUP_TIMEOUT 5000UL
#endif
#ifndef LSE_DRIVE_LEVEL
#define LSE_DRIVE_LEVEL RCC_LSEDRIVE_MEDIUMLOW
#endif
#ifndef CFG_HW_RCC_HSE_CAPACITOR_TUNE
#define CFG_HW_RCC_HSE_CAPACITOR_TUNE 32
#endif

#define VDD_VALUE 3300UL
#define TICK_INT_PRIORITY ((1UL << __NVIC_PRIO_BITS) - 1UL)
#define USE_RTOS 0U

#include "stm32wb0x_hal_rcc.h"
#include "stm32wb0x_hal_gpio.h"
#include "stm32wb0x_hal_dma.h"
#include "stm32wb0x_hal_cortex.h"
#include "stm32wb0x_hal_flash.h"
#include "stm32wb0x_hal_iwdg.h"
#include "stm32wb0x_hal_pwr.h"
#include "stm32wb0x_hal_rtc.h"
#include "stm32wb0x_hal_tim.h"
#include "stm32wb0x_hal_uart.h"

#ifdef USE_FULL_ASSERT
#include "stm32_assert.h"
#else
#define assert_param(expr) ((void)0U)
#endif

#endif
