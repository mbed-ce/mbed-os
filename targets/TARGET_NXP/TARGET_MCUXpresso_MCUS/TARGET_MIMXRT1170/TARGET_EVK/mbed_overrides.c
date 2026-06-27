/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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
#include "pinmap.h"
#include "clock_config.h"
#include "fsl_clock.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_xbara.h"

#include <mbed_assert.h>
#include <mbed_math_helpers.h>

// Symbols defined in linker script for noncache region
extern uint8_t __noncachedata_start__[];
extern uint8_t __noncachedata_end__[];

void BOARD_ConfigMPU(void)
{
   
    /* Disable I cache and D cache */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
    }

    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
    }

    ARM_MPU_Disable();

    /*
     * Add default region to deny access to whole address space to workaround speculative prefetch.
     * Refer to Arm errata 1013783-B for more details.
     */
    /* Region 0 setting: No data access permission. */
    MPU->RBAR = ARM_MPU_RBAR(0, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_NONE, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_4GB);

    /* Region 1 setting: Memory with Device type, not shareable, non-cacheable (SEMC0-SEMC1) */
    MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 2 setting: Memory with Device type, not shareable, non-cacheable. (CAAM Secure RAM, FlexSPI1 control registers) */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 3 setting: Memory with Normal type, not shareable, outer/inner write back (ITCM) */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

    /* Region 4 setting: Memory with Normal type, not shareable, outer/inner write back (DTCM) */
    MPU->RBAR = ARM_MPU_RBAR(4, 0x20000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

    /* Region 5 setting: Memory with Normal type, not shareable, outer/inner write back (CM4 TCM, OCRAM1, OCRAM2) */
    MPU->RBAR = ARM_MPU_RBAR(5, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_1MB);

    /* Region 6 setting: Memory with Normal type, not shareable, outer/inner write back (ORCAM1 ECC, OCRAM2 ECC, OCRAM M7) */
    MPU->RBAR = ARM_MPU_RBAR(6, 0x20300000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512KB);

    /* Region 7: Noncached memory. */
    ptrdiff_t noncached_region_size = __noncachedata_end__ - __noncachedata_start__;
    if(noncached_region_size > 0) {
        // Check configuration from linker script
        MBED_ASSERT(mbed_is_power_of_two(noncached_region_size));
        MBED_ASSERT(((uintptr_t)__noncachedata_start__) % noncached_region_size == 0);

        // Region size constant is the log2 of the region size, offset by 1
        const uint32_t region_size = mbed_integer_log_2(noncached_region_size) - 1;

        MPU->RBAR = ARM_MPU_RBAR(7, ((uintptr_t)__noncachedata_start__));
        MPU->RASR =
            ARM_MPU_RASR_EX(
                1,                          // DisableExec
                ARM_MPU_AP_FULL,            // AccessPermission
                ARM_MPU_ACCESS_NORMAL(ARM_MPU_CACHEP_NOCACHE, ARM_MPU_CACHEP_NOCACHE, true), // Access and cache policy
                0U,                         // SubRegionDisable
                region_size);               // Size
    }

    /* Region 8 setting: Memory with Normal type, not shareable, outer/inner write back. (FlexSPI1) */
    MPU->RBAR = ARM_MPU_RBAR(8, 0x30000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_RO, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_16MB);

    /* Region 9 setting: Memory with Normal type, not shareable, outer/inner write back (SEMC0) */
    MPU->RBAR = ARM_MPU_RBAR(9, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_64MB);
   
    /* Region 11 setting: Memory with Device type, not shareable, non-cacheable (AIPS peripherals) */
    MPU->RBAR = ARM_MPU_RBAR(11, 0x40000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_16MB);

    /* Region 12 setting: Memory with Device type, not shareable, non-cacheable (SIM_M/SIM_DISP) */
    MPU->RBAR = ARM_MPU_RBAR(12, 0x41000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_2MB);

    /* Region 13 setting: Memory with Device type, not shareable, non-cacheable (SIM_M7) */
    MPU->RBAR = ARM_MPU_RBAR(13, 0x41400000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1MB);

    /* Region 14 setting: Memory with Device type, not shareable, non-cacheable (GPU2D) */
    MPU->RBAR = ARM_MPU_RBAR(14, 0x41800000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1MB);

    /* Region 15 setting: Memory with Device type, not shareable, non-cacheable (AIPS M7) */
    MPU->RBAR = ARM_MPU_RBAR(15, 0x42000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1MB);

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enable I cache and D cache */
    SCB_EnableDCache();
    SCB_EnableICache();

}


// called before main
void mbed_sdk_init()
{
    BOARD_ConfigMPU();
    BOARD_BootClockRUN();

}

void spi_setup_clock()
{
   
}

uint32_t spi_get_clock(void)
{
    return 1;
}

void us_ticker_setup_clock()
{
 //  CLOCK_EnableClock(s_pitClocks[PIT_GetInstance(base)]);
}

uint32_t us_ticker_get_clock()
{
return CLOCK_GetRootClockFreq(kCLOCK_Root_Bus);
}

void serial_setup_clock(void)
{
      BOARD_ConfigMPU();
      BOARD_BootClockRUN();

}

uint32_t serial_get_clock(void)
{
    return CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart1);;
}

void i2c_setup_clock()
{
  
}

uint32_t i2c_get_clock()
{
    return 1;
}

uint32_t pwm_get_clock()
{
    return CLOCK_GetFreq(kCLOCK_Root_Bus);
}

void pwm_setup(uint32_t instance)
{
    /* Use default clock settings */
    /* Set the PWM Fault inputs to a low value */
    XBARA_Init(XBARA1);

    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1234Fault2);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1234Fault3);

    switch (instance) {
        case 1:
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault0);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault1);
            break;
        case 2:
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm2Fault0);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm2Fault1);
            break;
        case 3:
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm3Fault0);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm3Fault1);
            break;
        case 4:
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm4Fault0);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm4Fault1);
            break;
        default:
            break;
    }
}



