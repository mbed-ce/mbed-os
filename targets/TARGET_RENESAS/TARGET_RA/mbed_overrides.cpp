/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "cmsis.h"
#include "objects.h"
#include "platform/mbed_error.h"
#include "platform/mbed_toolchain.h"

int mbed_sdk_inited = 0;

#if defined(BSP_MCU_GROUP_RA8P1) && (BSP_CFG_CPU_CORE == 0) && !defined(RA8P1_DISABLE_SECONDARY_CORE_BOOT)

/* The RA8P1 boots from the Cortex-M85 (CPU0) only. When a Cortex-M33 (CPU1)
 * image is present in its flash partition (see bsp_dualcore_partition.h),
 * release it from reset so both cores run. The vector table of the CPU1
 * image starts at BSP_PARTITION_FLASH_CPU1_S_START (0x020C0000), which is
 * also the value written into CPSCU.CPU1INITVTOR.
 *
 * Before activating CPU1, perform a cheap sanity check of its vector table
 * (initial SP must point into SRAM, Reset_Handler must point into the CPU1
 * flash window) so that an erased or single-core device does not launch the
 * M33 into garbage (which would end in lockup).
 *
 * Applications that need full control over CPU1 start-up can define
 * RA8P1_DISABLE_SECONDARY_CORE_BOOT and call R_BSP_SecondaryCoreStart()
 * (bsp_common.h) themselves at a time of their choosing. */
static void ra8p1_boot_secondary_core_if_present()
{
    const volatile uint32_t *cpu1_vectors = (const volatile uint32_t *) BSP_PARTITION_FLASH_CPU1_S_START;
    uint32_t initial_sp = cpu1_vectors[0];
    uint32_t reset_handler = cpu1_vectors[1];

    bool sp_ok = (initial_sp >= 0x20000000UL) && (initial_sp <= 0x24000000UL);
    bool entry_ok = (reset_handler >= (uint32_t) BSP_PARTITION_FLASH_CPU1_S_START) &&
                    (reset_handler < (uint32_t) BSP_PARTITION_FLASH_CPU1_S_START + BSP_PARTITION_FLASH_CPU1_S_SIZE) &&
                    ((reset_handler & 1UL) != 0UL);

    if (sp_ok && entry_ok) {
        R_BSP_SecondaryCoreStart();
    }
    /* No valid CPU1 image: stay single-core. */
}

#endif /* BSP_MCU_GROUP_RA8P1 && CPU0 && !RA8P1_DISABLE_SECONDARY_CORE_BOOT */

// This function is called after RAM initialization and before main.
extern "C" void mbed_sdk_init()
{
    /* Enable Pin access globally */
    R_BSP_PinAccessEnable();

#if defined(BSP_MCU_GROUP_RA8P1) && (BSP_CFG_CPU_CORE == 0) && !defined(RA8P1_DISABLE_SECONDARY_CORE_BOOT)
    /* Release the Cortex-M33 (CPU1) from reset if a CPU1 image is flashed
     * (RA8P1 only, primary core only). */
    ra8p1_boot_secondary_core_if_present();
#endif

#if defined(TARGET_ARDUINO_UNO_R4_MINIMA) || defined(TARGET_ARDUINO_UNO_R4_WIFI)
    /* The Arduino UNO R4 bootloader (DFU on Minima, UART/bossac via the
     * ESP32-S3 bridge on WiFi) disables global interrupts (PRIMASK = 1)
     * before jumping to the application. Without re-enabling them, every
     * exception (peripheral IRQs, SVC, PendSV, SysTick) is silently
     * blocked, which breaks the RTOS scheduler and all interrupt-driven
     * peripherals.
     *
     * Before re-enabling, clear all stale pending interrupts left by
     * the bootloader (e.g. RTC carry, USB events from DFU) so they
     * don't fire against uninitialized drivers. */

    /* Clear all NVIC pending bits for IRQ 0..31 */
    NVIC->ICPR[0] = 0xFFFFFFFFu;

    /* Clear all IELSR IR (Interrupt Request) flags: IR is bit 16 in
     * each IELSR register on this MCU, write 0 to clear */
    for (uint32_t i = 0; i < 32u; i++) {
        if (R_ICU->IELSR[i] & (1UL << 16)) {
            R_ICU->IELSR[i] = R_ICU->IELSR[i] & ~(1UL << 16);
        }
    }

    __enable_irq();
#endif

    mbed_sdk_inited = 1;
}
