/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
#include "stm32h5xx.h"
#include "mbed_error.h"

/**
  * @brief  Enable ICACHE and define a MPU region to avoid HardFaults when accessing OTP and RO regions
  * @param  None
  * @retval None
  */

void Cache_Init()
{
    MPU_Attributes_InitTypeDef   attr;
    MPU_Region_InitTypeDef       region;

    /* Disable MPU before perloading and config update */
    HAL_MPU_Disable();

    /* Configure 0x00000000-0x08FFF7FF as Read Only, Executable and Cacheable */
    region.Enable           = MPU_REGION_ENABLE;
    region.Number           = MPU_REGION_NUMBER0;
    region.AttributesIndex  = MPU_ATTRIBUTES_NUMBER0;
    region.BaseAddress      = 0x00000000;
    region.LimitAddress     = 0x08FFF7FF;
    region.AccessPermission = MPU_REGION_ALL_RO;
    region.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
    region.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
    HAL_MPU_ConfigRegion(&region);

    /* Define cacheable memory via MPU */
    attr.Number             = MPU_ATTRIBUTES_NUMBER5;
    attr.Attributes         = INNER_OUTER(MPU_NOT_CACHEABLE);
    HAL_MPU_ConfigMemoryAttributes(&attr);

    /* Configure 0x08FFF800-0X0FFFFFFF as Read Only, Not Executable and Non-cacheable */
    region.Enable           = MPU_REGION_ENABLE;
    region.Number           = MPU_REGION_NUMBER5;
    region.AttributesIndex  = MPU_ATTRIBUTES_NUMBER5;
    region.BaseAddress      = 0x08FFF800;
    region.LimitAddress     = MBED_CONF_TARGET_MPU_ROM_END;
    region.AccessPermission = MPU_REGION_ALL_RO;
    region.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
    region.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
    HAL_MPU_ConfigRegion(&region);

    /* Enable the MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

    /* Enable ICACHE */
    HAL_ICACHE_Enable();
}
