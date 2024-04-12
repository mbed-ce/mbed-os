/*
 * Copyright 2020 Arduino SA
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

#include "cyhal.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Enter a critical section
 *
 * Disables interrupts and returns a value indicating whether the interrupts were previously
 * enabled.
 *
 * @return Returns the state before entering the critical section. This value must be provided
 * to \ref cyhal_system_critical_section_exit() to properly restore the state
 *
 * See \ref subsection_system_snippet1 for code snippet on critical section
 */
uint32_t cyhal_system_critical_section_enter(void);

/** Exit a critical section
 *
 * Re-enables the interrupts if they were enabled before
 *  cyhal_system_critical_section_enter() was called. The argument should be the value
 *  returned from \ref cyhal_system_critical_section_enter().
 *
 * @param[in] old_state The state of interrupts from cyhal_system_critical_section_enter()
 *
 * See \ref subsection_system_snippet1 for code snippet on critical section
 */
void cyhal_system_critical_section_exit(uint32_t old_state);

#if defined(__cplusplus)
}
#endif
