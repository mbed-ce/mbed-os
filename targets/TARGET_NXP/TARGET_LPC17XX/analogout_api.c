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
#include "analogout_api.h"

#include "cmsis.h"
#include "PeripheralPinMaps.h"

void analogout_init_direct(dac_t *obj, const PinMap *pinmap) {
    obj->dac = (DACName)pinmap->peripheral;
    
    // power is on by default, set DAC clk divider is /4
    LPC_SC->PCLKSEL0 &= ~(0x3 << 22);
    
    // map out (must be done before accessing registers)
    pin_function(pinmap->pin, pinmap->function);
    pin_mode(pinmap->pin, PullNone);
    
    analogout_write_u16(obj, 0);
}

void analogout_init(dac_t *obj, PinName pin) {
    PinMap pinmap;
    pinmap.pin = pin;
    pinmap.function = pinmap_find_function(pin, PinMap_DAC);
    pinmap.peripheral = pinmap_peripheral(pin, PinMap_DAC);
    MBED_ASSERT(pinmap.peripheral != NC);

    analogout_init_direct(obj, &pinmap);
}

void analogout_free(dac_t *obj) {}

static inline void dac_write(int value) {
    value &= 0x3FF; // 10-bit
    
    // Set the DAC output
    LPC_DAC->DACR = (0 << 16)       // bias = 0
                  | (value << 6);
}

static inline int dac_read() {
    return (LPC_DAC->DACR >> 6) & 0x3FF;
}

void analogout_write(dac_t *obj, float value) {
    if (value < 0.0f) {
        dac_write(0);
    } else if (value > 1.0f) {
        dac_write(0x3FF);
    } else {
        dac_write(value * (float)0x3FF);
    }
}

void analogout_write_u16(dac_t *obj, uint16_t value) {
    dac_write(value >> 6); // 10-bit
}

float analogout_read(dac_t *obj) {
    uint32_t value = dac_read();
    return (float)value * (1.0f / (float)0x3FF);
}

uint16_t analogout_read_u16(dac_t *obj) {
    uint32_t value = dac_read(); // 10-bit
    return (value << 6) | ((value >> 4) & 0x003F);
}

const PinMap *analogout_pinmap()
{
    return PinMap_DAC;
}
