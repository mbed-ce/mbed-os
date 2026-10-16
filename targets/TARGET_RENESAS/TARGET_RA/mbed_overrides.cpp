/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "cmsis.h"
#include "objects.h"
#include "platform/mbed_error.h"

int mbed_sdk_inited = 0;

// This function is called after RAM initialization and before main.
extern "C" void mbed_sdk_init()
{
    /* Enable Pin access globally */
    R_BSP_PinAccessEnable();

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
