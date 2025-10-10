/*
 * Copyright (c) 2018 ARM Limited
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

#include "NetStackMemoryManager.h"

net_stack_mem_buf_t * NetStackMemoryManager::realloc_heap(net_stack_mem_buf_t * orig_buf, uint32_t new_align, std::optional<uint32_t> new_len, std::optional<uint16_t> new_header_skip_size) {

    const uint32_t orig_buf_len = get_total_len(orig_buf);
    const uint32_t new_total_len = new_len.value_or(orig_buf_len) + new_header_skip_size.value_or(0);
    auto * new_buf = alloc_heap(new_total_len, new_align);

    if(!new_buf) {
        free(orig_buf);
        return nullptr;
    }

    if(new_header_skip_size.has_value()) {
        skip_header_space(new_buf, *new_header_skip_size);
    }

    // We should have gotten just one contiguous buffer
    MBED_ASSERT(get_next(new_buf) == nullptr);

    // Copy data over
    const uint32_t len_to_copy = std::min(new_len.value_or(orig_buf_len), orig_buf_len);
    copy_from_buf(get_ptr(new_buf), len_to_copy, orig_buf);
    free(orig_buf);
    return new_buf;
}

void NetStackMemoryManager::copy_to_buf(net_stack_mem_buf_t *to_buf, const void *ptr, uint32_t len)
{
    while (to_buf && len) {
        void *copy_to_ptr = get_ptr(to_buf);
        uint32_t copy_to_len = get_len(to_buf);

        if (copy_to_len > len) {
            copy_to_len = len;
            len = 0;
        } else {
            len -= copy_to_len;
        }

        memcpy(copy_to_ptr, ptr, copy_to_len);
        ptr = static_cast<const uint8_t *>(ptr) + copy_to_len;

        to_buf = get_next(to_buf);
    }
}

uint32_t NetStackMemoryManager::copy_from_buf(void *ptr, uint32_t len, const net_stack_mem_buf_t *from_buf) const
{
    uint32_t copied_len = 0;

    while (from_buf && len) {
        void *copy_from_ptr = get_ptr(from_buf);
        uint32_t copy_from_len = get_len(from_buf);

        if (copy_from_len > len) {
            copy_from_len = len;
            len = 0;
        } else {
            len -= copy_from_len;
        }

        memcpy(ptr, copy_from_ptr, copy_from_len);
        ptr = static_cast<uint8_t *>(ptr) + copy_from_len;
        copied_len += copy_from_len;

        from_buf = get_next(from_buf);
    }

    return copied_len;
}
