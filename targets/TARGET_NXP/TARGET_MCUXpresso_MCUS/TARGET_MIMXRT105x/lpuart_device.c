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

bool serial_is_enabled(uint32_t uart_index)
{
    bool clock_enabled = false;
    switch (uart_index) {
        case 1:
            clock_enabled = (CCM->CCGR5 & CCM_CCGR5_CG12_MASK) >> CCM_CCGR5_CG12_SHIFT;
            break;
        case 2:
            clock_enabled = (CCM->CCGR0 & CCM_CCGR0_CG14_MASK) >> CCM_CCGR0_CG14_SHIFT;
            break;
        case 3:
            clock_enabled = (CCM->CCGR0 & CCM_CCGR0_CG6_MASK) >> CCM_CCGR0_CG6_SHIFT;
            break;
        case 4:
            clock_enabled = (CCM->CCGR1 & CCM_CCGR1_CG12_MASK) >> CCM_CCGR1_CG12_SHIFT;
            break;
        case 5:
            clock_enabled = (CCM->CCGR3 & CCM_CCGR3_CG1_MASK) >> CCM_CCGR3_CG1_SHIFT;
            break;
        case 6:
            clock_enabled = (CCM->CCGR3 & CCM_CCGR3_CG3_MASK) >> CCM_CCGR3_CG3_SHIFT;
            break;
        case 7:
            clock_enabled = (CCM->CCGR5 & CCM_CCGR5_CG13_MASK) >> CCM_CCGR5_CG13_SHIFT;
            break;
        case 8:
            clock_enabled = (CCM->CCGR6 & CCM_CCGR6_CG7_MASK) >> CCM_CCGR6_CG7_SHIFT;
            break;
        default:
            break;
    }

    return clock_enabled;
}

uint32_t serial_get_clock(size_t index)
{
    (void)index;
    return BOARD_CLOCKFULLSPEED_UART_CLK_ROOT;
}

void LPUART1_IRQHandler()
{
    uint32_t status_flags = LPUART1->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 1);
}

void LPUART2_IRQHandler()
{
    uint32_t status_flags = LPUART2->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 2);
}

void LPUART3_IRQHandler()
{
    uint32_t status_flags = LPUART3->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 3);
}

void LPUART4_IRQHandler()
{
    uint32_t status_flags = LPUART4->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 4);
}

void LPUART5_IRQHandler()
{
    uint32_t status_flags = LPUART5->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 5);
}

void LPUART6_IRQHandler()
{
    uint32_t status_flags = LPUART6->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 6);
}

void LPUART7_IRQHandler()
{
    uint32_t status_flags = LPUART7->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 7);
}

void LPUART8_IRQHandler()
{
    uint32_t status_flags = LPUART8->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 8);
}