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
#include "clock_config.h"

bool serial_is_enabled(uint32_t uart_index)
{
    int clock_enabled = 0;
    switch (uart_index) {
        case 1:
            clock_enabled = CCM->LPCG[kCLOCK_Lpuart1].DIRECT & CCM_LPCG_DIRECT_ON_MASK;
            break;
        case 2:
            clock_enabled = CCM->LPCG[kCLOCK_Lpuart2].DIRECT & CCM_LPCG_DIRECT_ON_MASK;
            break;
        case 3:
            clock_enabled = CCM->LPCG[kCLOCK_Lpuart3].DIRECT & CCM_LPCG_DIRECT_ON_MASK;
            break;
        case 4:
            clock_enabled = CCM->LPCG[kCLOCK_Lpuart4].DIRECT & CCM_LPCG_DIRECT_ON_MASK;
            break;
        case 5:
            clock_enabled = CCM->LPCG[kCLOCK_Lpuart5].DIRECT & CCM_LPCG_DIRECT_ON_MASK;
            break;
        case 6:
            clock_enabled = CCM->LPCG[kCLOCK_Lpuart6].DIRECT & CCM_LPCG_DIRECT_ON_MASK;
            break;
        case 7:
            clock_enabled = CCM->LPCG[kCLOCK_Lpuart7].DIRECT & CCM_LPCG_DIRECT_ON_MASK;
            break;
        case 8:
            clock_enabled = CCM->LPCG[kCLOCK_Lpuart8].DIRECT & CCM_LPCG_DIRECT_ON_MASK;
            break;
        case 9:
            clock_enabled = CCM->LPCG[kCLOCK_Lpuart9].DIRECT & CCM_LPCG_DIRECT_ON_MASK;
            break;
        case 10:
            clock_enabled = CCM->LPCG[kCLOCK_Lpuart10].DIRECT & CCM_LPCG_DIRECT_ON_MASK;
            break;
        case 11:
            clock_enabled = CCM->LPCG[kCLOCK_Lpuart11].DIRECT & CCM_LPCG_DIRECT_ON_MASK;
            break;
        case 12:
            clock_enabled = CCM->LPCG[kCLOCK_Lpuart12].DIRECT & CCM_LPCG_DIRECT_ON_MASK;
            break;
        default:
            break;
    }

    return clock_enabled;
}
uint32_t serial_get_clock(size_t index)
{
    return CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart1 + index);
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

void LPUART9_IRQHandler()
{
    uint32_t status_flags = LPUART9->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 9);
}

void LPUART10_IRQHandler()
{
    uint32_t status_flags = LPUART10->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 10);
}

void LPUART11_IRQHandler()
{
    uint32_t status_flags = LPUART11->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 11);
}

void LPUART12_IRQHandler()
{
    uint32_t status_flags = LPUART12->STAT;
    mbed_lpuart_irq((status_flags & kLPUART_TxDataRegEmptyFlag), (status_flags & kLPUART_RxDataRegFullFlag), 12);
}