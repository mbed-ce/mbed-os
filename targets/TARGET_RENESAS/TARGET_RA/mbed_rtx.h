/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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
#ifndef MBED_MBED_RTX_H
#define MBED_MBED_RTX_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Symbols exported by the RA FSP linker script */
extern uint32_t __StackTop;
extern uint32_t __StackLimit;
extern uint32_t __HeapBase;
extern uint32_t __HeapLimit;

/* ISR stack region (Mbed/RTX use this for isr stack boundaries) */
#define ISR_STACK_START   ((uint32_t)&__StackLimit)
#define ISR_STACK_SIZE    ((uint32_t)&__StackTop - (uint32_t)&__StackLimit)

/* Main heap region used by Mbed allocator / RTX */
#define HEAP_START        ((uint32_t)&__HeapBase)
#define HEAP_SIZE         ((uint32_t)&__HeapLimit - (uint32_t)&__HeapBase)

#ifdef __cplusplus
}
#endif

#endif // MBED_MBED_RTX_H
