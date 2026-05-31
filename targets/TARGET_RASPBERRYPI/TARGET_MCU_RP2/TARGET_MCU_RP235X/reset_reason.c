/* mbed Microcontroller Library
 * Copyright (c) 2026 Jamie Smith
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

#include <pico/bootrom.h>

#include "reset_reason_api.h"
#include "hardware/watchdog.h"

#ifdef DEVICE_RESET_REASON

#include "hardware/structs/resets.h"
#include "hardware/powman.h"
#include "hardware/structs/watchdog.h"

reset_reason_t hal_reset_reason_get(void)
{
    // Use this function instead of checking the watchdog bits because it uses additional,
    // scratch-register-based heuristics to determine if the watchdog reboot was "real",
    // or if it was caused by a non-actual-WD-timeout reason like loading firmware through the bootrom
    if(watchdog_enable_caused_reboot()) {
        return RESET_REASON_WATCHDOG;
    }

    const uint32_t powman_chip_reset = powman_hw->chip_reset;
    if(powman_chip_reset & (POWMAN_CHIP_RESET_HAD_GLITCH_DETECT_BITS | POWMAN_CHIP_RESET_HAD_BOR_BITS)) {
        return RESET_REASON_BROWN_OUT;
    }
    else if(powman_chip_reset & POWMAN_CHIP_RESET_HAD_SWCORE_PD_BITS) {
        // I THINK this is set only when waking from dormant (core completely off) mode,
        // which Mbed does not currently support
        return RESET_REASON_WAKE_LOW_POWER;
    }
    else if(powman_chip_reset & POWMAN_CHIP_RESET_HAD_RESCUE_BITS) {
        // Note: This will only be reached with the debugger resets the core from a lockup,
        // but may as well add it.
        return RESET_REASON_LOCKUP;
    }
    else if(powman_chip_reset & POWMAN_CHIP_RESET_HAD_DP_RESET_REQ_BITS) {
        return RESET_REASON_DEBUGGER;
    }
    else if(powman_chip_reset & POWMAN_CHIP_RESET_HAD_RUN_LOW_BITS) {
        return RESET_REASON_PIN_RESET;
    }
    else if(powman_chip_reset & POWMAN_CHIP_RESET_HAD_POR_BITS) {
        return RESET_REASON_POWER_ON;
    }

    return RESET_REASON_UNKNOWN;
}


uint32_t hal_reset_reason_get_raw(void)
{
    // Return the watchdog reset reason register concatenated with the upper
    // 16 bits of the POWMAN CHIP_RESET register and the bootrom reset type register.
    return (powman_hw->chip_reset & 0xFFFF0000) | (rom_get_last_boot_type() << 8) | (watchdog_hw->reason & 0xFF);
}


void hal_reset_reason_clear(void)
{
    // Reset flag register used for watchdog_enable_caused_reboot()
    watchdog_hw->scratch[4] = 0;
}

void hal_reset_reason_get_capabilities(reset_reason_capabilities_t *cap)
{
    cap->reasons = (1 << RESET_REASON_WATCHDOG) |
        (1 << RESET_REASON_BROWN_OUT) |
        (1 << RESET_REASON_WAKE_LOW_POWER) |
        (1 << RESET_REASON_LOCKUP) |
        (1 << RESET_REASON_DEBUGGER) |
        (1 << RESET_REASON_PIN_RESET) |
        (1 << RESET_REASON_SOFTWARE) |
        (1 << RESET_REASON_POWER_ON);
}

#endif // DEVICE_RESET_REASON
