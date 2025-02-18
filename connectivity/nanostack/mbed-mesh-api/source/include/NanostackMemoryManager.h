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

#ifndef NANOSTACK_MEMORY_MANAGER_H
#define NANOSTACK_MEMORY_MANAGER_H

#include "EMACMemoryManager.h"

extern "C" void mbed_ns_heap_free_hook();

class NanostackMemoryManager final : public EMACMemoryManager {

    // Allow the hook to call the callback from the superclass
    friend void mbed_ns_heap_free_hook();
public:

    virtual ~NanostackMemoryManager() = default;

    emac_mem_buf_t *alloc_heap(uint32_t size, uint32_t align) override;

    emac_mem_buf_t *alloc_pool(uint32_t size, uint32_t align) override;

    uint32_t get_pool_alloc_unit(uint32_t align) const override;

    void free(emac_mem_buf_t *buf) override;

    uint32_t get_total_len(const emac_mem_buf_t *buf) const override;

    void copy(emac_mem_buf_t *to_buf, const emac_mem_buf_t *from_buf) override;

    void cat(emac_mem_buf_t *to_buf, emac_mem_buf_t *cat_buf) override;

    emac_mem_buf_t *get_next(const emac_mem_buf_t *buf) const override;

    void *get_ptr(const emac_mem_buf_t *buf) const override;

    uint32_t get_len(const emac_mem_buf_t *buf) const override;

    void set_len(emac_mem_buf_t *buf, uint32_t len) override;

    Lifetime get_lifetime(const net_stack_mem_buf_t *buf) const override;
};

#endif /* NANOSTACK_MEMORY_MANAGER_H */
