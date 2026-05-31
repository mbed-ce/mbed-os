/* mbed Microcontroller Library
 * Copyright (c) 2026, Arm Limited and affiliates.
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

#include "cmsis.h"
#include "objects.h"
#include "platform/mbed_error.h"
#include "mbed_interface.h"
#include "mbed_boot.h"
#include "MbedCRC.h"

#include "hardware/resets.h"
#include "hardware/clocks.h"
#include "hardware/sync.h"
#include "pico/binary_info.h"
#include "pico/runtime_init.h"

#include <stdarg.h>
#include <hardware/flash.h>

int mbed_sdk_inited = 0;

void mbed_sdk_init()
{
    // Reset bootrom state
#if !PICO_RUNTIME_NO_INIT_BOOTROM_RESET
    runtime_init_bootrom_reset();
#endif
#if !PICO_RUNTIME_NO_INIT_PER_CORE_BOOTROM_RESET
    runtime_init_per_core_bootrom_reset();
#endif

    // Reset all peripherals to put system into a known state
    runtime_init_early_resets();

    // Release all spin locks
    spin_locks_reset();

    // Ensure USB PHY is in low-power state -- must be cleared before beginning USB operations.
    runtime_init_usb_power_down();

    // Enable coprocessors
#if !PICO_RUNTIME_NO_INIT_PER_CORE_ENABLE_COPROCESSORS
    runtime_init_per_core_enable_coprocessors();
#endif

    // Set up clock tree
    runtime_init_clocks();
    runtime_init_post_clock_resets();
    SystemCoreClockUpdate();

#if !PICO_RUNTIME_NO_INIT_RP2040_GPIO_IE_DISABLE
    // After resetting BANK0 we should disable IE on 26-29 as these may have mid-rail voltages when
    // ADC is in use
    runtime_init_rp2040_gpio_ie_disable();
#endif

	mbed_sdk_inited = 1;
}

// Add a basic "binary info" note which says that this is an Mbed OS program
#define reset_section_attr __attribute__((section(".reset")))
bi_decl_with_attr(bi_program_name("Mbed OS CE Program"), reset_section_attr)

// Pico SDK panic handlers
void __attribute__((noreturn)) panic_unsupported() {
    panic("not supported");
}

#ifdef NDEBUG
// Simple assert failure handler for release builds, to save space
void hard_assertion_failure(void) {
    panic("Hard assert");
}
#endif

void __attribute__((noreturn)) __printflike(1, 0) panic(const char *fmt, ...) {
    mbed_error_printf("\n*** PANIC ***\n");

    // Try and format the message to the stack
    const size_t MSG_BUF_LEN = 256;
    char msgBuf[MSG_BUF_LEN];
    va_list args;
    va_start(args, fmt);
    snprintf(msgBuf, MSG_BUF_LEN, fmt, args);
    va_end(args);

    MBED_ERROR(
            MBED_MAKE_ERROR(
                MBED_MODULE_HAL,
                MBED_ERROR_CODE_UNKNOWN
            ),
            msgBuf
        );
}

#if !PICO_NO_FLASH
void mbed_mac_address(char *mac) {
    // Generate a random MAC address using the flash device's unique ID.
    // This is not suitable for production use, but will ensure there are
    // no MAC conflicts.

    uint8_t flashUID[FLASH_UNIQUE_ID_SIZE_BYTES];
    flash_get_unique_id(flashUID);

    // The flash unique ID is 64 bits, but we need a unique ID no more
    // than 48 bits long. So, use a CRC.
    mbed::MbedCRC<POLY_32BIT_ANSI, 32, mbed::CrcMode::BITWISE> crcCalc;
    uint32_t crc;
    crcCalc.compute(flashUID, FLASH_UNIQUE_ID_SIZE_BYTES, &crc);

    // Build the MAC address
    mac[0] = 2; // Locally administered, unicast
    mac[1] = 0;
    mac[2] = crc >> 24;
    mac[3] = (crc >> 16) & 0xFF;
    mac[4] = (crc >> 8) & 0xFF;
    mac[5] = crc & 0xFF;
}
#endif