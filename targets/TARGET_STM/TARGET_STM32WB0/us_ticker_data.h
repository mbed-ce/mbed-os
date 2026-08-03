/* SPDX-License-Identifier: Apache-2.0 */
#ifndef MBED_US_TICKER_DATA_H
#define MBED_US_TICKER_DATA_H

#include "stm32wb0x.h"
#include "stm32wb0x_ll_tim.h"

#define TIM_MST TIM16
#define TIM_MST_IRQ TIM16_IRQn
#define TIM_MST_RCC __HAL_RCC_TIM16_CLK_ENABLE()
#define TIM_MST_RESET_ON __HAL_RCC_TIM16_FORCE_RESET()
#define TIM_MST_RESET_OFF __HAL_RCC_TIM16_RELEASE_RESET()
#define TIM_MST_BIT_WIDTH 16
#define TIM_MST_PCLK 1

#endif
