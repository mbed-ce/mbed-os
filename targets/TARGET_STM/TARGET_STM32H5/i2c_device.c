/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include "i2c_device.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "stm32h5xx_ll_rcc.h"

/*  Define I2C Device */
#if DEVICE_I2C

/**
  * @brief  Get I2C clock source frequency according I2C instance used.
  * @param  i2c I2C instance name.
  * @retval I2C clock source frequency in Hz.
  */
uint32_t i2c_get_pclk(I2CName i2c)
{
    uint32_t clocksource;
    uint32_t pclk = 0;
#if !defined(RCC_CR_PLL3ON)
    PLL2_ClocksTypeDef pll2clock; //STM32H503
#else
    PLL3_ClocksTypeDef pll3clock; //STM32H563
#endif

    if (i2c == I2C_1) {
        clocksource = __HAL_RCC_GET_I2C1_SOURCE();
        switch (clocksource) {
            case RCC_I2C1CLKSOURCE_PCLK1:
                pclk = HAL_RCC_GetPCLK1Freq();
                break;
            case RCC_I2C1CLKSOURCE_CSI:
                pclk = CSI_VALUE;
                break;
            case RCC_I2C1CLKSOURCE_HSI:
                pclk = HSI_VALUE;
                break;
#if !defined(RCC_CR_PLL3ON)
            case RCC_I2C1CLKSOURCE_PLL2R:
                HAL_RCCEx_GetPLL2ClockFreq(&pll2clock);
                pclk = pll2clock.PLL2_R_Frequency;
                break;
#else
            case RCC_I2C1CLKSOURCE_PLL3R:
                HAL_RCCEx_GetPLL3ClockFreq(&pll3clock);
                pclk = pll3clock.PLL3_R_Frequency;
                break;
#endif /* RCC_CR_PLL3ON */
        }
    }
    else if (i2c == I2C_2) {
        clocksource = __HAL_RCC_GET_I2C2_SOURCE();
        switch (clocksource) {
            case RCC_I2C2CLKSOURCE_PCLK1:
                pclk = HAL_RCC_GetPCLK1Freq();
                break;
            case RCC_I2C2CLKSOURCE_CSI:
                pclk = CSI_VALUE;
                break;
            case RCC_I2C2CLKSOURCE_HSI:
                pclk = HSI_VALUE;
                break;
#if !defined(RCC_CR_PLL3ON)
            case RCC_I2C2CLKSOURCE_PLL2R:
                HAL_RCCEx_GetPLL2ClockFreq(&pll2clock);
                pclk = pll2clock.PLL2_R_Frequency;
                break;
#else
            case RCC_I2C2CLKSOURCE_PLL3R:
                HAL_RCCEx_GetPLL3ClockFreq(&pll3clock);
                pclk = pll3clock.PLL3_R_Frequency;
                break;
#endif /* RCC_CR_PLL3ON */
        }
    }
#if defined I2C3_BASE
    else if (i2c == I2C_3) {
        clocksource = __HAL_RCC_GET_I2C3_SOURCE();
        switch (clocksource) {
            case RCC_I2C3CLKSOURCE_PCLK3:
                pclk = HAL_RCC_GetPCLK3Freq();
                break;
            case RCC_I2C3CLKSOURCE_CSI:
                pclk = CSI_VALUE;
                break;
            case RCC_I2C3CLKSOURCE_HSI:
                pclk = HSI_VALUE;
                break;
#if defined(RCC_CR_PLL3ON)
            case RCC_I2C3CLKSOURCE_PLL3R:
                HAL_RCCEx_GetPLL3ClockFreq(&pll3clock);
                pclk = pll3clock.PLL3_R_Frequency;
                break;
#endif
        }
    }
#endif
#if defined I2C4_BASE
    else if (i2c == I2C_4) {
        clocksource = __HAL_RCC_GET_I2C4_SOURCE();
        switch (clocksource) {
            case RCC_I2C4CLKSOURCE_PCLK3:
                pclk = HAL_RCC_GetPCLK3Freq();
                break;
            case RCC_I2C4CLKSOURCE_CSI:
                pclk = CSI_VALUE;
                break;
            case RCC_I2C4CLKSOURCE_HSI:
                pclk = HSI_VALUE;
                break;
#if defined(RCC_CR_PLL3ON)
            case RCC_I2C4CLKSOURCE_PLL3R:
                HAL_RCCEx_GetPLL3ClockFreq(&pll3clock);
                pclk = pll3clock.PLL3_R_Frequency;
                break;
#endif
        }
    }
#endif
    else {
        // should not happend
        error("I2C: unknown instance");
    }
    return pclk;
}
#endif // DEVICE_I2C
