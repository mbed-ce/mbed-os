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

#ifndef __RP2040_CMSIS_H__
#define __RP2040_CMSIS_H__

#include "RP2040.h"
#include "system_RP2040.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t ram_vector_table[]; // defined in runtime_init.c
#define NVIC_RAM_VECTOR_ADDRESS ram_vector_table

#if PICO_RP2040
#define NVIC_NUM_VECTORS 42
#else
#error "Unknown number of vectors!"
#endif

#ifdef __cplusplus
}
#endif

#endif
