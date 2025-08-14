/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "pinmap.h"
#include "mbed_error.h"

void pin_function(PinName pin, int function) {
    MBED_ASSERT(pin != (PinName)NC);

    uint32_t pin_number = (uint32_t)pin - (uint32_t)P0_0;
    int index = pin_number >> 4;
    int offset = (pin_number & 0xF) << 1;
    
    PINCONARRAY->PINSEL[index] &= ~(0x3 << offset);
    PINCONARRAY->PINSEL[index] |= function << offset;
}

void pin_mode(PinName pin, PinMode mode) {
    MBED_ASSERT(pin != (PinName)NC);
    
    uint32_t pin_number = (uint32_t)pin - (uint32_t)P0_0;
    int index = pin_number >> 5;
    int offset = pin_number & 0x1F;
    bool open_drain = (uint32_t) mode & (uint32_t) OpenDrain;
    
    if(open_drain) {
        PINCONARRAY->PINMODE_OD[index] |= 1 << offset;
    }
    else {
        PINCONARRAY->PINMODE_OD[index] &= ~(1 << offset);
    }
    
    // Even if open drain mode is active, the normal pinmode is still used when the open drain pin is outputting a 1.
    // So, always set the PINMODE register.
    const uint8_t pinmode_sel_msk = 0x3;
    uint8_t mode_no_drain = mode & pinmode_sel_msk;
    
    index = pin_number >> 4;
    offset = (pin_number & 0xF) << 1;
    
    PINCONARRAY->PINMODE[index] &= ~(pinmode_sel_msk << offset);
    PINCONARRAY->PINMODE[index] |= mode_no_drain << offset;
}
