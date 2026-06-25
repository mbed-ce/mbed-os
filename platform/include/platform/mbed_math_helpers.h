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

#include <stdint.h>

/**
 * @brief Check whether an integer is an exact power of two.
 */
// from https://stackoverflow.com/a/600306/7083698
static inline bool mbed_is_power_of_two(const uint32_t x)
{
    return x > 0 && (x & (x - 1)) == 0;
}

/**
 * @brief Get the log2 of an integer.
 *
 * Rounds down to the nearest power of 2, i.e. \c mbed_integer_log_2(3) is 1.
 */
static inline uint32_t mbed_integer_log_2(uint32_t x) {
    return sizeof(uint32_t)*8 - 1 - __builtin_clz(x);
}
