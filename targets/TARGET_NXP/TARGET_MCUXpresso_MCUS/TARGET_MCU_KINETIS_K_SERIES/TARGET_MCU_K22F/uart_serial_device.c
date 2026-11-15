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

#include "uart_serial_api.h"
#include "cmsis.h"

bool serial_is_enabled(uint32_t uart_index)
{
    bool clock_enabled = false;
    switch (uart_index) {
        case 0:
            clock_enabled = (SIM->SCGC4 & SIM_SCGC4_UART0_MASK) >> SIM_SCGC4_UART0_SHIFT;
            break;
        case 1:
            clock_enabled = (SIM->SCGC4 & SIM_SCGC4_UART1_MASK) >> SIM_SCGC4_UART1_SHIFT;
            break;
        case 2:
            clock_enabled = (SIM->SCGC4 & SIM_SCGC4_UART2_MASK) >> SIM_SCGC4_UART2_SHIFT;
            break;
        default:
            break;
    }

    return clock_enabled;
}
