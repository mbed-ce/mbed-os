/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#include "nsdynmemLIB.h"
#include <string.h>
#include "mbed_assert.h"
#include "NanostackMemoryManager.h"
#include "NanostackEthernetInterface.h"

struct ns_stack_mem_t {
    ns_stack_mem_t *next;
    uint8_t * payload;
    uint32_t len; // Original requested length of buffer (not including extra space allocated for alignment, or header size)
    uint16_t header_size; // Number of header bytes being skipped
    uint8_t mem[];
};

emac_mem_buf_t *NanostackMemoryManager::alloc_heap(uint32_t size, uint32_t align)
{
    ns_stack_mem_t *buf = static_cast<ns_stack_mem_t *>(ns_dyn_mem_temporary_alloc(sizeof(ns_stack_mem_t) + size + align));
    if (buf == NULL) {
        return NULL;
    }

    buf->next = NULL;
    buf->payload = buf->mem;
    buf->len = size;
    buf->header_size = 0;

    if (align) {
        uint32_t remainder = reinterpret_cast<uint32_t>(buf->payload) % align;
        if (remainder) {
            uint32_t offset = align - remainder;
            if (offset >= align) {
                offset = align;
            }

            buf->payload = static_cast<char *>(buf->payload) + offset;
        }
    }

    return static_cast<emac_mem_buf_t *>(buf);
}

emac_mem_buf_t *NanostackMemoryManager::alloc_pool(uint32_t size, uint32_t align)
{
    // Unlike LwIP, Nanostack does not treat pool buffers any differently from heap ones.
    return alloc_heap(size, align);
}

uint32_t NanostackMemoryManager::get_pool_alloc_unit(uint32_t align) const
{
    return MBED_CONF_NSAPI_EMAC_RX_POOL_BUF_SIZE - align;
}

void NanostackMemoryManager::free(emac_mem_buf_t *mem)
{
    ns_dyn_mem_free(mem);
}

uint32_t NanostackMemoryManager::get_total_len(const emac_mem_buf_t *buf) const
{
    const ns_stack_mem_t *mem = static_cast<const ns_stack_mem_t *>(buf);
    uint32_t total = 0;

    while (mem) {
        total += mem->len - mem->header_size; // note: header size can only legally be set on the first buffer
        mem = mem->next;
    }
}

void NanostackMemoryManager::cat(emac_mem_buf_t *to_buf, emac_mem_buf_t *cat_buf)
{
    ns_stack_mem_t *to_mem = static_cast<ns_stack_mem_t *>(to_buf);
    ns_stack_mem_t *cat_mem = static_cast<ns_stack_mem_t *>(cat_buf);

    MBED_ASSERT(cat_mem->header_size == 0);

    while (to_mem->next) {
        to_mem = to_mem->next;
    }

    to_mem->next = cat_mem;
}

emac_mem_buf_t *NanostackMemoryManager::get_next(const emac_mem_buf_t *buf) const
{
    return static_cast<const ns_stack_mem_t *>(buf)->next;
}

void *NanostackMemoryManager::get_ptr(const emac_mem_buf_t *buf) const
{
    auto mem = static_cast<const ns_stack_mem_t *>(buf);
    return mem->payload + mem->header_size;
}

uint32_t NanostackMemoryManager::get_len(const emac_mem_buf_t *buf) const
{
    auto mem = static_cast<const ns_stack_mem_t *>(buf);
    return mem->len - mem->header_size;
}

void NanostackMemoryManager::set_len(emac_mem_buf_t *buf, uint32_t len)
{
    auto *mem = static_cast<ns_stack_mem_t *>(buf);
    mem->len = len + mem->header_size;
}

NetStackMemoryManager::Lifetime NanostackMemoryManager::get_lifetime(const net_stack_mem_buf_t *buf) const
{
    // For Nanostack, all buffers are heap allocated and can be kept around as long as
    // is needed by the EMAC driver.
    return Lifetime::HEAP_ALLOCATED;
}

void NanostackMemoryManager::skip_header_space(net_stack_mem_buf_t *buf, int32_t amount) {
    auto * const mem = static_cast<ns_stack_mem_t *>(buf);

    if(amount > 0) {
        MBED_ASSERT(amount + mem->header_size > mem->len); // header_size cannot exceed len
    }
    else {
        MBED_ASSERT(-1 * amount <= mem->header_size); // header_size cannot go below 0
    }

    mem->header_size += amount;
}

void mbed_ns_heap_free_hook()
{
    auto &callback = Nanostack::get_instance().get_memory_manager().onPoolSpaceAvailCallback;
    if (callback) {
        callback();
    }
}
