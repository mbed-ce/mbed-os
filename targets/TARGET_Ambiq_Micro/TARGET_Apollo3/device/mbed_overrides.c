/* mbed Microcontroller Library
 * Copyright (c) 2025 Jamie Smith
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

#include <mbed_boot.h>

#include <stddef.h>

// Needed for am_hal_clkgen.h
#include <stdbool.h>
#include <apollo3.h>

#include <am_hal_clkgen.h>

void mbed_sdk_init(void)
{
    // Turn on frequency adjustment. This improves HFRC clock accuracy by a factor
    // of 10 or more.
    am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_HFADJ_ENABLE, NULL);
}