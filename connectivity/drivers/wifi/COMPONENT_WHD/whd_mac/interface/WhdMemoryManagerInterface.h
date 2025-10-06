/*
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

#ifndef WHDMEMORYMANAGERINTERFACE_H
#define WHDMEMORYMANAGERINTERFACE_H

#include "whd_network_types.h"
#include "whd.h"

#include "NetStackMemoryManager.h"

// Default to 4 byte alignment
constexpr size_t WHD_MEM_BUFFER_ALIGNMENT = sizeof(uint32_t);

// These functions interface the Cypress WHD driver with Mbed's memory manager interface.
// For documentation on each function, see "struct whd_buffer_funcs" in whd_network_types.h

whd_result_t mbed_whd_host_buffer_get(void* instance, whd_buffer_t *buffer, whd_buffer_dir_t direction, unsigned short size,
                                        unsigned long timeout_ms);
void mbed_whd_buffer_release(void* instance, whd_buffer_t buffer, whd_buffer_dir_t direction);
uint8_t * mbed_whd_buffer_get_current_piece_data_pointer(void* instance, whd_buffer_t buffer);
uint16_t mbed_whd_buffer_get_current_piece_size(void* instance, whd_buffer_t buffer);
whd_result_t mbed_whd_buffer_set_size(void* instance, whd_buffer_t buffer, unsigned short size);
whd_result_t mbed_whd_buffer_add_remove_at_front(void* instance, whd_buffer_t *buffer, int32_t add_remove_amount);

static inline whd_buffer_funcs_t getMbedWHDBufferFuncs(NetStackMemoryManager & memManager) {
    return {
        .instance = &memManager,
        .whd_host_buffer_get = mbed_whd_host_buffer_get,
        .whd_buffer_release = mbed_whd_buffer_release,
        .whd_buffer_get_current_piece_data_pointer = mbed_whd_buffer_get_current_piece_data_pointer,
        .whd_buffer_get_current_piece_size = mbed_whd_buffer_get_current_piece_size,
        .whd_buffer_set_size = mbed_whd_buffer_set_size,
        .whd_buffer_add_remove_at_front = mbed_whd_buffer_add_remove_at_front
    };
}

#endif //WHDMEMORYMANAGERINTERFACE_H
