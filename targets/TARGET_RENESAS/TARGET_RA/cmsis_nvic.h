/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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

#ifndef CMSIS_NVIC_H
#define CMSIS_NVIC_H

#include <stdint.h>

#include "vector_data.h"   // FSP generated

/* Cortex-M has 16 system exceptions */
#define NVIC_NUM_SYSTEM_EXCEPTIONS   (16)

/* Total number of vectors = system exceptions + IRQs */
#define NVIC_NUM_VECTORS  (NVIC_NUM_SYSTEM_EXCEPTIONS + VECTOR_DATA_IRQ_COUNT)

#endif
