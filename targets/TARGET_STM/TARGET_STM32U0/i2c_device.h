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

#ifndef MBED_I2C_DEVICE_H
#define MBED_I2C_DEVICE_H

#include "PeripheralNames.h"

#ifdef __cplusplus
extern "C" {
#endif

/*  Define I2C Device */
#if DEVICE_I2C

/*  Define IP version */
#define I2C_IP_VERSION_V2

#define I2C1_EV_IRQn I2C1_IRQn
#define I2C1_ER_IRQn I2C1_IRQn
#define I2C2_EV_IRQn I2C2_3_4_IRQn 
#define I2C2_ER_IRQn I2C2_3_4_IRQn 
#define I2C3_EV_IRQn I2C2_3_4_IRQn 
#define I2C3_ER_IRQn I2C2_3_4_IRQn 
#define I2C4_EV_IRQn I2C2_3_4_IRQn 
#define I2C4_ER_IRQn I2C2_3_4_IRQn 

// Common settings: I2C clock = 56 MHz, Analog filter = ON, Digital filter coefficient = 0
#define TIMING_VAL_56M_CLK_100KHZ 0x20C04963   // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_56M_CLK_400KHZ 0x2060091A   // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_56M_CLK_1MHZ   0x00600A19   // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_56M              56000000     // 56 MHz

#define I2C_IT_ALL (I2C_IT_ERRI|I2C_IT_TCI|I2C_IT_STOPI|I2C_IT_NACKI|I2C_IT_ADDRI|I2C_IT_RXI|I2C_IT_TXI)

/*  Family specifc settings for clock source */
#define I2CAPI_I2C1_CLKSRC RCC_I2C1CLKSOURCE_PCLK1 
#define I2CAPI_I2C3_CLKSRC RCC_I2C3CLKSOURCE_PCLK1 

uint32_t i2c_get_pclk(I2CName i2c);
uint32_t i2c_get_timing(I2CName i2c, uint32_t current_timing, int current_hz, int hz);

#endif // DEVICE_I2C

#ifdef __cplusplus
}
#endif

#endif
