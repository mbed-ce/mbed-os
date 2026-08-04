/*
 * Copyright (c) 2026 MbedCE Community Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

#include "cmsis.h"
#include "platform/mbed_mpu_mgmt.h"
#include "platform/mbed_toolchain.h"
#include "platform/mbed_wait_api.h"

/*
 * Execute the delay loop from SRAM. At 64 MHz, the STM32WB0 flash wait state
 * makes the generic flash-resident Cortex-M0+ loop significantly too slow.
 */

/* Cortex-M0+ takes five cycles per iteration. */
#define LOOP_SCALER 5000

MBED_ALIGN(16)
static uint16_t delay_loop_code[] = {
    0x1E40, /* SUBS R0,R0,#1 */
    0xBF00, /* NOP */
    0xBF00, /* NOP */
    0xD2FB, /* BCS .-3 */
    0x4770  /* BX LR */
};

/* Set the low address bit to select Thumb execution. */
#define delay_loop ((void (*)())((uintptr_t)delay_loop_code + 1U))

void wait_ns(unsigned int ns)
{
    const uint32_t cycles_per_us = SystemCoreClock / 1000000U;
    const uint32_t count = (cycles_per_us * ns) / LOOP_SCALER;

    mbed_mpu_manager_lock_ram_execution();
    delay_loop(count);
    mbed_mpu_manager_unlock_ram_execution();
}
