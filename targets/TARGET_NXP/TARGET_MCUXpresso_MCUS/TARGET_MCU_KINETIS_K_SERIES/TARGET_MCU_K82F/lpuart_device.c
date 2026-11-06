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

#include "lpuart_device.h"
#include "cmsis.h"
#include "fsl_lpuart.h"
#include "lpuart_serial_api.h"

bool serial_is_enabled(uint32_t uart_index)
{
    bool clock_enabled = false;
    switch (uart_index) {
        case 0:
            clock_enabled = (SIM->SCGC2 & SIM_SCGC2_LPUART0_MASK) >> SIM_SCGC2_LPUART0_SHIFT;
            break;
        case 1:
            clock_enabled = (SIM->SCGC2 & SIM_SCGC2_LPUART1_MASK) >> SIM_SCGC2_LPUART1_SHIFT;
            break;
        case 2:
            clock_enabled = (SIM->SCGC2 & SIM_SCGC2_LPUART2_MASK) >> SIM_SCGC2_LPUART2_SHIFT;
            break;
        case 3:
            clock_enabled = (SIM->SCGC2 & SIM_SCGC2_LPUART3_MASK) >> SIM_SCGC2_LPUART3_SHIFT;
            break;
        case 4:
            clock_enabled = (SIM->SCGC2 & SIM_SCGC2_LPUART4_MASK) >> SIM_SCGC2_LPUART4_SHIFT;
            break;
        default:
            break;
    }

    return clock_enabled;
}

uint32_t serial_get_clock(size_t index) {
    switch (SIM->SOPT2 & SIM_SOPT2_LPUARTSRC_MASK) {
        case SIM_SOPT2_LPUARTSRC(3U): {
            return CLOCK_GetInternalRefClkFreq();
            break;
        }
        case SIM_SOPT2_LPUARTSRC(2U): {
            return CLOCK_GetOsc0ErClkFreq();
            break;
        }
        case SIM_SOPT2_LPUARTSRC(1U): {
            return CLOCK_GetPllFllSelClkFreq();
            break;
        }
        default: {
            return 0; // Clock disabled
        }
    }
}

void LPUART0_IRQHandler() {
    uint32_t status_flags = LPUART0->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 0);
}

void LPUART1_IRQHandler() {
    uint32_t status_flags = LPUART1->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 0);
}

void LPUART2_IRQHandler() {
    uint32_t status_flags = LPUART2->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 0);
}

void LPUART3_IRQHandler() {
    uint32_t status_flags = LPUART3->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 0);
}

void LPUART4_IRQHandler() {
    uint32_t status_flags = LPUART4->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 0);
}
