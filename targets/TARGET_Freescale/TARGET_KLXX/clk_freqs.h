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
#ifndef MBED_CLK_FREQS_H
#define MBED_CLK_FREQS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PeripheralPins.h"

//Get the peripheral bus clock frequency
static inline uint32_t bus_frequency(void) {
    return (SystemCoreClock / (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> SIM_CLKDIV1_OUTDIV4_SHIFT) + 1));
}

//Get external oscillator (crystal) frequency
uint32_t extosc_frequency(void);

//Get MCG PLL/2 or FLL frequency, depending on which one is active, sets PLLFLLSEL bit
uint32_t mcgpllfll_frequency(void);

#ifdef __cplusplus
}
#endif

#endif
