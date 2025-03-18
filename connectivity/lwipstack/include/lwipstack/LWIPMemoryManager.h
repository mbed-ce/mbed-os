/* Copyright (c) 2017 ARM Limited
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

#ifndef LWIP_MEMORY_MANAGER_H
#define LWIP_MEMORY_MANAGER_H

#include "EMACMemoryManager.h"

// Hook called from LwIP whenever a pbuf chain containing at least one pool pbuf has been freed
extern "C" void mbed_lwip_on_pbuf_pool_free_hook();

class LWIPMemoryManager final : public EMACMemoryManager {
public:

    net_stack_mem_buf_t *alloc_heap(uint32_t size, uint32_t align) override;

    net_stack_mem_buf_t *alloc_pool(uint32_t size, uint32_t align) override;

    uint32_t get_pool_alloc_unit(uint32_t align) const override;

    void free(net_stack_mem_buf_t *buf) override;

    uint32_t get_total_len(const net_stack_mem_buf_t *buf) const override;

    void copy(net_stack_mem_buf_t *to_buf, const net_stack_mem_buf_t *from_buf) override;

    void copy_to_buf(net_stack_mem_buf_t *to_buf, const void *ptr, uint32_t len) override;

    uint32_t copy_from_buf(void *ptr, uint32_t len, const net_stack_mem_buf_t *from_buf) const override;

    void cat(net_stack_mem_buf_t *to_buf, net_stack_mem_buf_t *cat_buf) override;

    net_stack_mem_buf_t *get_next(const net_stack_mem_buf_t *buf) const override;

    void *get_ptr(const net_stack_mem_buf_t *buf) const override;

    uint32_t get_len(const net_stack_mem_buf_t *buf) const override;

    void set_len(net_stack_mem_buf_t *buf, uint32_t len) override;

    Lifetime get_lifetime(const net_stack_mem_buf_t *buf) const override;

private:

    // Allow callback to access private vars
    friend void mbed_lwip_on_pbuf_pool_free_hook();

    /**
     * Returns a total memory alignment size
     *
     * Calculates the total memory alignment size for a memory buffer chain.
     * Used internally on pool allocation.
     *
     * @param  size    Size of the memory to allocate in bytes
     * @param  align   Memory alignment requirement for each buffer in bytes
     * @return         Total alignment needed in bytes
     */
    uint32_t count_total_align(uint32_t size, uint32_t align);

    /**
     * Aligns a memory buffer chain
     *
     * Aligns a memory buffer chain and updates lengths and total lengths
     * accordingly. There needs to be enough overhead to do the alignment
     * for all buffers.
     *
     * @param pbuf     Memory buffer
     * @param align    Memory alignment requirement for each buffer in bytes
     */
    void align_memory(struct pbuf *pbuf, uint32_t align);

    /**
     * Sets total lengths of a memory buffer chain
     *
     * Sets total length fields for a memory buffer chain based on buffer
     * \c len fields. All total lengths are calculated again.
     *
     * @param pbuf     Memory buffer
     */
    void set_total_len(struct pbuf *pbuf);
};

#endif /* LWIP_MEMORY_MANAGER_H */
