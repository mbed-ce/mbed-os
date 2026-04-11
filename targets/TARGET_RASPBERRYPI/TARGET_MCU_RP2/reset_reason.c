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
    if (watchdog_caused_reboot()) {
        return RESET_REASON_WATCHDOG;
    }
    else if(vreg_and_chip_reset_hw->chip_reset & VREG_AND_CHIP_RESET_CHIP_RESET_HAD_RUN_BITS) {
        return RESET_REASON_PIN_RESET;
    }
    else if(vreg_and_chip_reset_hw->chip_reset & VREG_AND_CHIP_RESET_CHIP_RESET_HAD_POR_BITS) {
        return RESET_REASON_POWER_ON;
    }

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

}

void hal_reset_reason_get_capabilities(reset_reason_capabilities_t *cap)
{
    cap->reasons = (1 << RESET_REASON_PIN_RESET) |
            (1 << RESET_REASON_POWER_ON) |
            (1 << RESET_REASON_WATCHDOG);
}

#endif // DEVICE_RESET_REASON
