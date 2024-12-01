/*
 * Copyright (c) 2020, Nuvoton Technology Corporation
 *
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

#include <assert.h>
#include "partition_M2354_mem.h"

/* Check MBED_ROM_BANK_NS_CODE_START and friends
 *
 * These symbols must be resolved by TF-M exported region_defs.h.
 */
static_assert(MBED_ROM_BANK_NS_CODE_START == NS_CODE_START,
              "MBED_ROM_BANK_NS_CODE_START not equal TF-M imported NS_CODE_START");
static_assert(MBED_ROM_BANK_NS_CODE_SIZE == NS_CODE_SIZE,
              "MBED_ROM_BANK_NS_CODE_SIZE not equal TF-M imported NS_CODE_SIZE");
static_assert(MBED_RAM_BANK_NS_DATA_START == NS_DATA_START,
              "MBED_RAM_BANK_NS_DATA_START not equal TF-M imported NS_DATA_START");
static_assert(MBED_RAM_BANK_NS_DATA_SIZE == NS_DATA_SIZE,
              "MBED_RAM_BANK_NS_DATA_SIZE not equal TF-M imported NS_DATA_SIZE");
