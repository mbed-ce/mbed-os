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

#include "WhdMemoryManagerInterface.h"

#include <ThisThread.h>
#include <Timer.h>

// The WHD driver likes to use SDIO block mode for reads, meaning that an additional up-to 63 bytes can be stored at the
// end of the buffer relative to its actual size. This means that this layer needs to allocate the buffers
// with additional space at the end relative to what was asked for, and then NOT report this space
// when asked about a buffer's length
constexpr size_t WHD_MEM_BUFFER_EXTRA_SPACE = 64;

// Note: Implementations adapted from Infineon's code for LwIP
// https://github.com/Infineon/wifi-host-driver/blob/master/docs/html/cy_network_buffer.c

// Note: In this code, both "whd_buffer_t" and "net_stack_mem_buf_t *" represent pointers to opaque network
// stack buffers, and we use them interchangeably.

whd_result_t mbed_whd_host_buffer_get(void *instance, whd_buffer_t*buffer, const whd_buffer_dir_t direction, const unsigned short size,
    const unsigned long timeout_ms) {
    auto * const memoryManager = static_cast<NetStackMemoryManager *>(instance);

    mbed::Timer allocTimer;
    allocTimer.start();

    do {

        // Pool should only be used for Rx buffers and only if we can do a contiguous allocation.
        // Note that IMO the official example code is wrong here -- pool buffers should only be used for Rx as
        // they have some special properties in LwIP (a failed allocation triggers a cleanup of nonessential
        // internal memory buffers). So I changed this from Tx to Rx.
        const bool usePool = direction == WHD_NETWORK_RX && ((size + WHD_MEM_BUFFER_EXTRA_SPACE) <= memoryManager->get_pool_alloc_unit(WHD_MEM_BUFFER_ALIGNMENT));

        if(usePool) {
            // Try to allocate from the pool if possible to avoid dynamic memory allocation
            *buffer = memoryManager->alloc_pool(size + WHD_MEM_BUFFER_EXTRA_SPACE, WHD_MEM_BUFFER_ALIGNMENT);
            if(*buffer) {
                return WHD_SUCCESS;
            }
        }

        // Now try using the heap
        *buffer = memoryManager->alloc_heap(size + WHD_MEM_BUFFER_EXTRA_SPACE, WHD_MEM_BUFFER_ALIGNMENT);
        if(*buffer) {
            return WHD_SUCCESS;
        }

        // Both failed? wait
        rtos::ThisThread::sleep_for(std::chrono::milliseconds(1));
    } while(allocTimer.elapsed_time() < std::chrono::milliseconds(timeout_ms));

    return WHD_BUFFER_ALLOC_FAIL;
}

void mbed_whd_buffer_release(void *instance, const whd_buffer_t buffer, const whd_buffer_dir_t direction) {
    (void)direction;
    MBED_ASSERT(buffer != nullptr);

    auto * const memoryManager = static_cast<NetStackMemoryManager *>(instance);
    memoryManager->free(buffer);
}

uint8_t * mbed_whd_buffer_get_current_piece_data_pointer(void *instance, whd_buffer_t buffer) {
    MBED_ASSERT(buffer != nullptr);
    auto * const memoryManager = static_cast<NetStackMemoryManager *>(instance);
    return static_cast<uint8_t *>(memoryManager->get_ptr(buffer));
}

uint16_t mbed_whd_buffer_get_current_piece_size(void *instance, whd_buffer_t buffer) {
    MBED_ASSERT(buffer != nullptr);
    auto * const memoryManager = static_cast<NetStackMemoryManager *>(instance);
    return memoryManager->get_len(buffer) - WHD_MEM_BUFFER_EXTRA_SPACE;
}

whd_result_t mbed_whd_buffer_set_size(void *instance, whd_buffer_t buffer, unsigned short size) {
    MBED_ASSERT(buffer != nullptr);
    auto * const memoryManager = static_cast<NetStackMemoryManager *>(instance);

    // Note that the example code has a rather confusing sanity check on size here. However, the Mbed
    // net stack memory manager interface doesn't provide a way to check the original size of a buffer,
    // so it's hard to port over properly.
    // However, when we run this code in the EMAC test, the EmacTestMemoryManager will catch any incorrect
    // usage here
    memoryManager->set_len(buffer, size + WHD_MEM_BUFFER_EXTRA_SPACE);

    return WHD_SUCCESS;
}

whd_result_t mbed_whd_buffer_add_remove_at_front(void *instance, whd_buffer_t*buffer, int32_t add_remove_amount) {
    MBED_ASSERT(buffer != nullptr);
    auto * const memoryManager = static_cast<NetStackMemoryManager *>(instance);

    // Make sure we aren't trying to skip before the start of the buffer or after the end
    if(add_remove_amount + memoryManager->get_header_skip_size(*buffer) < 0) {
        return WHD_PMK_WRONG_LENGTH;
    }
    if(add_remove_amount > static_cast<int32_t>(memoryManager->get_len(*buffer) - WHD_MEM_BUFFER_EXTRA_SPACE)) {
        return WHD_PMK_WRONG_LENGTH;
    }

    memoryManager->skip_header_space(*buffer, add_remove_amount);

    return WHD_SUCCESS;
}

