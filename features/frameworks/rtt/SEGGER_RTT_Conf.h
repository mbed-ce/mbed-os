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

#pragma once

// This file gets included from ASM so we need to not include C code if that is happening
#ifndef __ASSEMBLER__
#include "device.h" // for __DCACHE_PRESENT
#include "mbed_critical.h"
#endif

// Buffer counts and sizes
#define SEGGER_RTT_MAX_NUM_UP_BUFFERS MBED_CONF_RTT_MAX_CHANNELS
#define SEGGER_RTT_MAX_NUM_DOWN_BUFFERS MBED_CONF_RTT_MAX_CHANNELS
#define BUFFER_SIZE_UP MBED_CONF_RTT_CH0_UP_BUFFER_SIZE
#define BUFFER_SIZE_DOWN MBED_CONF_RTT_CH0_DOWN_BUFFER_SIZE

// Define RTT locks in terms of Mbed critical sections
#ifndef __ASSEMBLER__
#define SEGGER_RTT_LOCK() core_util_critical_section_enter()
#define SEGGER_RTT_UNLOCK() core_util_critical_section_exit()
#endif

// Cache setup
#if __DCACHE_PRESENT
// NOTE: On targets with a data cache, the RTT control block and buffers must be put in an uncached section of memory.
// This requires that the linker script contain logic to place the ".rtt" section.
#define SEGGER_RTT_SECTION ".rtt"
#endif
