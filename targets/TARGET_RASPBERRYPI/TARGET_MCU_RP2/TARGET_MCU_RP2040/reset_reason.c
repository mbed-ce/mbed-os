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

#include "reset_reason_api.h"
#include "hardware/watchdog.h"

#ifdef DEVICE_RESET_REASON

#include "hardware/structs/resets.h"

#include "hardware/structs/vreg_and_chip_reset.h"
#include "hardware/regs/vreg_and_chip_reset.h"
#include "hardware/structs/watchdog.h"

reset_reason_t hal_reset_reason_get(void)
{
    // Use this function instead of watchdog_caused_reboot() because it uses additional,
    // scratch-register-based heuristics to determine if the watchdog reboot was "real",
    // or if it was caused by a non-actual-WD-timeout reason like loading firmware through the bootrom
    if (watchdog_enable_caused_reboot()) {
        return RESET_REASON_WATCHDOG;
    }
    else if(vreg_and_chip_reset_hw->chip_reset & VREG_AND_CHIP_RESET_CHIP_RESET_HAD_RUN_BITS) {
        return RESET_REASON_PIN_RESET;
    }
    else if(vreg_and_chip_reset_hw->chip_reset & VREG_AND_CHIP_RESET_CHIP_RESET_HAD_PSM_RESTART_BITS) {
        // Note: This will only be reached with the debugger resets the core from a lockup,
        // but may as well add it.
        return RESET_REASON_LOCKUP;
    }

    // NOTE: The "power on reset" flag (VREG_AND_CHIP_RESET_CHIP_RESET_HAD_POR_BITS) is also seen for most types of software
    // reset, e.g. NVIC_SystemReset() and a reset through OpenOCD. So, we report a so-called "power-on reset"
    // as UNKNOWN since we can't really determine what actually caused it.
    // See forum thread I started here:
    // https://forums.raspberrypi.com/viewtopic.php?p=2378084
    return RESET_REASON_UNKNOWN;
}


uint32_t hal_reset_reason_get_raw(void)
{
    // Return the watchdog reset reason register concatenated with bytes 1 and 2
    // of the vreg_and_chip_reset reset reason register
    return (vreg_and_chip_reset_hw->chip_reset & 0xFFFF00) | (watchdog_hw->reason & 0xFF);
}


void hal_reset_reason_clear(void)
{
    // Reset flag register used for watchdog_enable_caused_reboot()
    watchdog_hw->scratch[4] = 0;
}

void hal_reset_reason_get_capabilities(reset_reason_capabilities_t *cap)
{
    cap->reasons = (1 << RESET_REASON_PIN_RESET) |
            (1 << RESET_REASON_WATCHDOG) |
            (1 << RESET_REASON_LOCKUP);
}

#endif // DEVICE_RESET_REASON
