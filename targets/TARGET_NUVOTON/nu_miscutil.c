/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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

#include "mbed_assert.h"
#include "PinNames.h"
#include "nu_miscutil.h"

void nu_nop(uint32_t n)
{
    uint32_t times = n / 10;
    uint32_t rmn = n % 10;
    
    while (times --) {
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
    }
    
    switch (rmn) {
        case 9:
            __NOP();
            // fall through
        case 8:
            __NOP();
            // fall through
        case 7:
            __NOP();
            // fall through
        case 6:
            __NOP();
            // fall through
        case 5:
            __NOP();
            // fall through
        case 4:
            __NOP();
            // fall through
        case 3:
            __NOP();
            // fall through
        case 2:
            __NOP();
            // fall through
        case 1:
            __NOP();
            // fall through
        default:
            break;
    }
}
