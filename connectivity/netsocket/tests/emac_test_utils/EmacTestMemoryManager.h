/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef EMAC_TEST_MEMORY_MANAGER_H
#define EMAC_TEST_MEMORY_MANAGER_H

#include <list>

#include "EMACMemoryManager.h"

#define MEM_CHECK       0x01
#define MEM_NO_ALIGN    0x02

typedef struct emac_memory {
    struct emac_memory *next;
    void *buffer;              /**< Pointer to allocated buffer */
    unsigned int orig_len;     /**< Original buffer length (set_len() does not change) */
    unsigned int len;          /**< Buffer length */
    void *ptr;                 /**< Aligned pointer */
    bool first;
} emac_memory_t;

class EmacTestMemoryManager : public EMACMemoryManager {
public:

    /**
     * Creates emac test memory manager
     */
    EmacTestMemoryManager();

    /*
     * Gets static instance
     */
    static EmacTestMemoryManager &get_instance();

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

    /**
     * Allocates memory buffer from the heap
     *
     * Memory buffer allocated from heap is always contiguous and can be arbitrary size.
     *
     * @param size        Size of the memory to allocate in bytes
     * @param align       Memory alignment requirement in bytes
     * @param opt         Options
     * @return            Allocated memory buffer, or NULL in case of error
     */
    emac_mem_buf_t *alloc_heap(uint32_t size, uint32_t align, uint8_t opt);

    /**
     * Allocates memory buffer chain from a pool
     *
     * Memory allocated from pool is contiguous if size is equal or less than
     * (aligned) allocation unit, otherwise may be chained. Will typically come from
     * fixed-size packet pool memory.
     *
     * @param size        Total size of the memory to allocate in bytes
     * @param align       Memory alignment requirement for each buffer in bytes
     * @param opt         Options
     * @return            Allocated memory buffer chain, or NULL in case of error
     */
    emac_mem_buf_t *alloc_pool(uint32_t size, uint32_t align, uint8_t opt);

    /**
     * Sets memory buffer pool allocation unit
     *
     * Sets the maximum size of contiguous memory that can be allocated from a pool.
     *
     * @param alloc_unit   Contiguous memory size
     */
    virtual void set_alloc_unit(uint32_t alloc_unit);

    /**
     * Sets whether memory is available
     *
     * Can be used to disable memory allocation request from emac.
     *
     * @param memory    True if memory is available
     */
    void set_memory_available(bool memory);

    /**
     * Gets memory statistics
     *
     * Gets memory usage statistics
     *
     * @param buffers   Number of buffers that are reserved
     * @param memory    Reserved memory in bytes
     */
    void get_memory_statistics(int *buffers, int *memory);

private:
    void validate_list() const;
    template <typename TYPE> void check_value(TYPE value, const char *fmt, ...) const;
    bool validate_ptr(const emac_mem_buf_t *buf) const;
    void check_align(uint32_t align) const;
    mutable rtos::Mutex m_mem_mutex;
    std::list<emac_memory_t *> m_mem_buffers;
    unsigned int m_alloc_unit;
    bool m_memory_available;
};

#endif /* EMAC_TEST_MEMORY_MANAGER_H */
