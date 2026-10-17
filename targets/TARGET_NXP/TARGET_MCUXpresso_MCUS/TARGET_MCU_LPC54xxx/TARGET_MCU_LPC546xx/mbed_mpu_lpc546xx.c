/* mbed Microcontroller Library
 * Copyright (c) 2025 Jamie Smith
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

#include "hal/mpu_api.h"
#include "platform/mbed_assert.h"
#include "cmsis.h"

enum {
    REGION_CODE,
    REGION_BOOTROM,
    REGION_SRAMX,
    REGION_SPIF,
    REGION_SRAM,
    REGION_PERIPHERAL,
    REGION_USB_SRAM, // Note: this needs to be higher numbered than PERIPHERAL because it overlaps the peripheral mem space and needs to take priority
    REGION_EXT_RAM
};

// Custom MPU init function for LPC546xx MCUs.
// This is needed because it has a memory bank (SRAMX) at an address lower than 0x2000 0000, which the default
// Mbed MPU configuration cannot handle.
void mbed_mpu_init()
{
    // Disable the MPU
    ARM_MPU_Disable();

    /*
     * MPU region map:
     *
     * Start         End             Name            Mbed MPU protection
     * 0x0000 0000 - 0x0007 FFFF     Code            Write disabled
     * 0x0300 0000 - 0x0300 FFFF     Boot ROM        Write disabled
     * 0x0400 0000 - 0x0400 7FFF     SRAMX           Execute disabled
     * 0x1000 0000 - 0x17FF FFFF     SPIF Mem Mapped Write disabled
     * 0x2000 0000 - 0x23FF FFFF     SRAM            Execute disabled
     * 0x4000 0000 - 0x7FFF FFFF     Peripheral
     * 0x4010 0000 - 0x4010 1FFF     USB SRAM        Execute disabled
     * 0x8000 0000 - 0xDFFF FFFF     External RAM    Execute disabled
     */

    // Code regions
    ARM_MPU_SetRegion(
        ARM_MPU_RBAR(
            REGION_CODE,                // Region
            0x00000000),                // Base
        ARM_MPU_RASR(
            0,                          // DisableExec
            ARM_MPU_AP_RO,              // AccessPermission
            0,                          // TypeExtField
            0,                          // IsShareable
            1,                          // IsCacheable
            0,                          // IsBufferable
            0,                          // SubRegionDisable
            ARM_MPU_REGION_SIZE_512KB)  // Size
    );
    ARM_MPU_SetRegion(
        ARM_MPU_RBAR(
            REGION_BOOTROM,             // Region
            0x03000000),                // Base
        ARM_MPU_RASR(
            0,                          // DisableExec
            ARM_MPU_AP_RO,              // AccessPermission
            0,                          // TypeExtField
            0,                          // IsShareable
            1,                          // IsCacheable
            0,                          // IsBufferable
            0,                          // SubRegionDisable
            ARM_MPU_REGION_SIZE_64KB)   // Size
    );
    ARM_MPU_SetRegion(
        ARM_MPU_RBAR(
            REGION_SPIF,                // Region
            0x10000000),                // Base
        ARM_MPU_RASR(
            0,                          // DisableExec
            ARM_MPU_AP_RO,              // AccessPermission
            0,                          // TypeExtField
            0,                          // IsShareable
            1,                          // IsCacheable
            0,                          // IsBufferable
            0,                          // SubRegionDisable
            ARM_MPU_REGION_SIZE_128MB)  // Size
    );

    // Peripheral region
    ARM_MPU_SetRegion(
        ARM_MPU_RBAR(
            REGION_PERIPHERAL,          // Region
            0x40000000),                // Base
        ARM_MPU_RASR(
            0,                          // DisableExec
            ARM_MPU_AP_FULL,            // AccessPermission
            2,                          // TypeExtField
            0,                          // IsShareable
            0,                          // IsCacheable
            0,                          // IsBufferable
            0,                          // SubRegionDisable
            ARM_MPU_REGION_SIZE_1GB)    // Size
    );

    // SRAMs
    ARM_MPU_SetRegion(
        ARM_MPU_RBAR(
            REGION_SRAMX,               // Region
            0x04000000),                // Base
        ARM_MPU_RASR(
            1,                          // DisableExec
            ARM_MPU_AP_FULL,            // AccessPermission
            0,                          // TypeExtField
            0,                          // IsShareable
            1,                          // IsCacheable
            1,                          // IsBufferable
            0,                          // SubRegionDisable
            ARM_MPU_REGION_SIZE_32KB)  // Size
    );
    ARM_MPU_SetRegion(
        ARM_MPU_RBAR(
            REGION_SRAM,                // Region
            0x20000000),                // Base
        ARM_MPU_RASR(
            1,                          // DisableExec
            ARM_MPU_AP_FULL,            // AccessPermission
            0,                          // TypeExtField
            0,                          // IsShareable
            1,                          // IsCacheable
            1,                          // IsBufferable
            0,                          // SubRegionDisable
            ARM_MPU_REGION_SIZE_64MB)   // Size
    );
    ARM_MPU_SetRegion(
        ARM_MPU_RBAR(
            REGION_USB_SRAM,            // Region
            0x40100000),                // Base
        ARM_MPU_RASR(
            1,                          // DisableExec
            ARM_MPU_AP_FULL,            // AccessPermission
            0,                          // TypeExtField
            0,                          // IsShareable
            1,                          // IsCacheable
            1,                          // IsBufferable
            0,                          // SubRegionDisable
            ARM_MPU_REGION_SIZE_8KB)    // Size
    );
    ARM_MPU_SetRegion(
        ARM_MPU_RBAR(
            REGION_EXT_RAM,             // Region
            0x80000000),                // Base
        ARM_MPU_RASR(
            1,                          // DisableExec
            ARM_MPU_AP_FULL,            // AccessPermission
            0,                          // TypeExtField
            0,                          // IsShareable
            1,                          // IsCacheable
            1,                          // IsBufferable
            (1 << 7) | (1 << 6),        // SubRegionDisable
            ARM_MPU_REGION_SIZE_2GB)    // Size
    );

    // Enable the MPU
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Pos);
}

void mbed_mpu_free()
{
    // Flush memory writes before configuring the MPU.
    __DMB();

    // Disable the MPU
    MPU->CTRL = 0;

    // Ensure changes take effect
    __DSB();
    __ISB();
}

static void enable_region(bool enable, uint32_t region)
{
    MPU->RNR = region;
    MPU->RASR = (MPU->RASR & ~MPU_RASR_ENABLE_Msk) | (enable << MPU_RASR_ENABLE_Pos);
}

void mbed_mpu_enable_rom_wn(bool enable)
{
    // Flush memory writes before configuring the MPU.
    __DMB();

    enable_region(enable, REGION_CODE);
    enable_region(enable, REGION_BOOTROM);
    enable_region(enable, REGION_SPIF);

    // Ensure changes take effect
    __DSB();
    __ISB();
}

void mbed_mpu_enable_ram_xn(bool enable)
{
    // Flush memory writes before configuring the MPU.
    __DMB();

    enable_region(enable, REGION_SRAMX);
    enable_region(enable, REGION_SRAM);
    enable_region(enable, REGION_USB_SRAM);
    enable_region(enable, REGION_EXT_RAM);

    // Ensure changes take effect
    __DSB();
    __ISB();
}
