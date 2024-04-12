/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
=======
/*
 * Copyright 2024 Arduino SA
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
#ifndef MBED_DEVICE_H
#define MBED_DEVICE_H

#include "objects.h"

#endif
=======
#include "cyhal_system.h"

#include "mbed_critical.h"

uint32_t cyhal_system_critical_section_enter(void)
{
    bool were_interrupts_enabled = !core_util_in_critical_section();

    core_util_critical_section_enter();

    return were_interrupts_enabled;
}

void cyhal_system_critical_section_exit(uint32_t old_state)
{
    (void)old_state;
    core_util_critical_section_exit();
}
