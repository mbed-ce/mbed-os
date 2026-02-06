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

#ifndef NET_STACK_MEMORY_MANAGER_H
#define NET_STACK_MEMORY_MANAGER_H

#include <optional>

/**
 * Network Stack interface memory manager
 *
 * \par
 * This interface provides abstraction for memory modules used in different IP stacks (often to accommodate zero
 * copy). NetStack interface is required to accept output packets and provide received data using this stack-
 * independent API. This header should be implemented for each IP stack, so that we keep EMAC module independent.
 *
 * \par
 * NetStack memory interface uses memory buffer chains to store data. Data passed in either direction
 * may either be contiguous (a single-buffer chain), or may consist of multiple buffers.
 * Chaining of the buffers is made using singly-linked list. The NetStack data-passing APIs do not specify
 * alignment or structure of the chain in either direction.
 *
 * \par
 * Memory buffers can be allocated either from heap or from memory pools. Heap buffers are always contiguous.
 * Memory pool buffers may be either contiguous or chained depending on allocation size. By LwIP convention,
 * the pool should only be used for Rx packets -- the EMAC may opt to keep buffers pre-allocated from the pool
 * for receiving packets into. This is done because LwIP will do special stuff when the pool runs out of space,
 * e.g. flushing TCP out-of-sequence segment buffers to free up memory.
 *
 * On NetStack interface buffer chain ownership is transferred. EMAC must free buffer chain that it is given for
 * link output and the stack must free the buffer chain that it is given for link input.
 *
 */

#include "nsapi.h"
#include "Callback.h"

// Opaque struct representing a memory buffer or a chain of memory buffers.
typedef void net_stack_mem_buf_t;

class NetStackMemoryManager {
protected:
    /// Callback which shall be called (if set) by the implementation after one or more buffer spaces
    /// become free in the pool. This is used by zero-copy Ethernet MACs as a hint that
    /// now is a good time to allocate fresh buffers off the pool into Ethernet descriptors.
    /// It *is* legal to call this function if you aren't totally sure new memory is available --
    /// the mac will try to allocate more buffers, and if it can't, oh well.
    /// However, it is not legal for memory to become available without a call to this function.
    /// Such a situation might lead to a lockup of the MAC due to not having memory allocated for Rx.
    // TODO this eventually needs to get converted to a list once we support boards with more than 1 EMAC
    mbed::Callback<void()> onPoolSpaceAvailCallback;

public:

    /**
     * Allocates memory buffer from the heap
     *
     * Memory buffer allocated from heap is always contiguous and can be arbitrary size.
     *
     * @param size     Size of the memory to allocate in bytes
     * @param align    Memory alignment requirement in bytes
     * @return         Allocated memory buffer, or NULL in case of error
     */
    virtual net_stack_mem_buf_t *alloc_heap(uint32_t size, uint32_t align) = 0;

    /**
     * Allocates memory buffer chain from a pool
     *
     * Memory allocated from pool is contiguous if size is equal or less than
     * (aligned) allocation unit, otherwise may be chained. Will typically come from
     * fixed-size packet pool memory.
     *
     * @param  size    Total size of the memory to allocate in bytes
     * @param  align   Memory alignment requirement for each buffer in bytes
     * @return         Allocated memory buffer chain, or NULL in case of error
     */
    virtual net_stack_mem_buf_t *alloc_pool(uint32_t size, uint32_t align) = 0;

    /**
     * @brief Reallocates a buffer or buffer chain as a contiguous (non-chained) heap buffer, freeing the original.
     *
     * Only the visible data in the source buffer is copied, not any skipped headers. Data from chained
     * buffers *will* be copied.
     *
     * @param orig_buf Original buffer. Will be freed whether or not the new buffer is allocated.
     * @param new_align Alignment to allocate the new buffer with
     * @param new_len If set, this length will be used instead of the buffer's original length
     * @param new_header_skip_size If set, this header skip size will be set on the new buffer. If unset, no header skip will be set
     *
     * @return Pointer to new buffer, or nullptr if allocation failed.
     */
    net_stack_mem_buf_t *realloc_heap(net_stack_mem_buf_t *orig_buf, uint32_t new_align, std::optional<uint32_t> new_len = std::nullopt, std::optional<uint16_t> new_header_skip_size = std::nullopt);

    /**
     * Get memory buffer pool allocation unit
     *
     * Returns the maximum size of contiguous memory that can be allocated from a pool.
     *
     * @param align    Memory alignment requirement in bytes
     * @return         Contiguous memory size
     */
    virtual uint32_t get_pool_alloc_unit(uint32_t align) const = 0;

    /**
     * Get memory buffer pool size.
     *
     * @return The number of pool buffers that can be allocated at any one time
     */
    uint32_t get_pool_size()
    {
#if UNITTEST
        return 0;
#else
        return MBED_CONF_NSAPI_EMAC_RX_POOL_NUM_BUFS;
#endif
    }

    /**
     * Free memory buffer chain
     *
     * Frees all buffers from the chained list.
     *
     * @param buf      Memory buffer chain to be freed.
     */
    virtual void free(net_stack_mem_buf_t *buf) = 0;

    /**
     * Return total length of a memory buffer chain
     *
     * Returns a total length of this buffer and any following buffers in the chain.
     *
     * @param buf      Memory buffer chain
     * @return         Total length in bytes
     */
    virtual uint32_t get_total_len(const net_stack_mem_buf_t *buf) const = 0;

    /**
     * @brief Copy from a raw buffer in memory to a memory buffer chain
     *
     * Copies data to a buffer chain. Copy operation does not adjust the lengths
     * of the copied-to memory buffer chain, so chain total length must match the
     * copied length.
     *
     * @param to_buf    Memory buffer chain to copy to
     * @param ptr       Pointer to data
     * @param len       Data length
     */
    virtual void copy_to_buf(net_stack_mem_buf_t *to_buf, const void *ptr, uint32_t len);

    /**
     * @brief Copy from a memory buffer chain to a raw buffer in memory.
     *
     * Header skip bytes are processed, so the copy will begin AFTER the header bytes of
     * \c from_buf
     *
     * @param len       Data length
     * @param ptr       Pointer to data
     * @param from_buf  Memory buffer chain to copy from
     * @return          Length of the data that was copied
     */
    virtual uint32_t copy_from_buf(void *ptr, uint32_t len, const net_stack_mem_buf_t *from_buf) const;

    /**
     * Concatenate two memory buffer chains
     *
     * Concatenates buffer chain to end of the other buffer chain. Concatenated-to buffer total length
     * is adjusted accordingly. cat_buf must point to the start of the chain. After concatenation
     * to_buf's chain now owns those buffers, and they will be freed when the to_buf chain is freed.
     *
     * @warning It is forbidden for \c cat_buf to have skipped header bytes.
     *
     * @param to_buf   Memory buffer chain to concatenate to
     * @param cat_buf  Memory buffer chain to concatenate
     */
    virtual void cat(net_stack_mem_buf_t *to_buf, net_stack_mem_buf_t *cat_buf) = 0;

    /**
     * Returns the next buffer
     *
     * Returns the next buffer from the memory buffer chain.
     *
     * @param buf      Memory buffer
     * @return         The next memory buffer, or NULL if last
     */
    virtual net_stack_mem_buf_t *get_next(const net_stack_mem_buf_t *buf) const = 0;

    /**
     * @brief Count the number of buffers in a buffer chain
     *
     * @param buf      Memory buffer
     * @return         The number of buffers in the chain
     */
    size_t count_buffers(const net_stack_mem_buf_t *buf)
    {
        size_t count = 0;
        while (buf != nullptr) {
            count += 1;
            buf = get_next(buf);
        }
        return count;
    }

    /**
     * Return pointer to the payload of the buffer
     *
     * Note that this is affected by the current header skip size (see below)
     *
     * @param buf      Memory buffer
     * @return         Pointer to the payload
     */
    virtual void *get_ptr(const net_stack_mem_buf_t *buf) const = 0;

    /**
     * Return payload size of this individual buffer (NOT including any chained buffers)
     *
     * Note that this is affected by the current header skip size (see below)
     *
     * @param buf      Memory buffer
     * @return         Size in bytes
     */
    virtual uint32_t get_len(const net_stack_mem_buf_t *buf) const = 0;

    /**
     * Sets the payload size of the buffer
     *
     * The allocated payload size will not change. It is not permitted
     * to change the length of a buffer that is not the first (or only) in a chain.
     *
     * Note that this is affected by the current header skip size (see below)
     *
     * *Note as of Dec 2024: Different implementations (Nanostack vs LwIP) disagree about
     * how to implement this operation.  Specifically, if called on the head of a buffer
     * chain, the LwIP implementation allows changing the length of the chain as a whole.
     * However, the Nanostack implementation does not and only can change the length of the head buffer.
     * For fear of breaking existing code, I do not want to change this behavior.
     * So, if constructing a buffer chain, it is safest to set the buffer lengths first before
     * building the chain.
     *
     * @param buf      Memory buffer
     * @param len      Payload size, must be less or equal to the allocated size
     */
    virtual void set_len(net_stack_mem_buf_t *buf, uint32_t len) = 0;

    /**
     * @brief Skips (or un-skips) header space from the buffer.
     *
     * Skipping n bytes of header space causes the buffer's payload pointer to refer to a location n bytes after
     * the base address of the packet buffer, and the length of the buffer to be decreased by n.
     *
     * This is commonly used to skip protocol headers in network
     * packets. For example, if you have an Ethernet frame, skipping 14 bytes of header space will cause
     * the "start" of the packet buffer to point to the IP header instead.
     *
     * Multiple calls to this function add together, so for example if you first skip 14 bytes, then -4, then
     * 10, the result will be 20 total bytes of skipped header.
     *
     * @param buf Buffer to operate on.
     * @param amount Amount of header space to skip. Negative values are allowed and cause
     *    previously skipped header space to be removed.
     *
     * @warning Skipping a larger total header space than the size of the first buffer in the chain, or skipping a negative
     *    total header space, results in undefined behavior.
     */
    virtual void skip_header_space(net_stack_mem_buf_t *buf, int32_t amount) = 0;

    /**
     * @brief Restores previously skipped header space to the buffer.
     *
     * This function is the inverse of \c skip_header_space().
     *
     * @param buf Buffer to operate on.
     * @param amount Amount of header space to skip. Negative values are allowed and cause
     *    previously skipped header space to be removed.
     */
    inline void restore_header_space(net_stack_mem_buf_t *buf, const int32_t amount)
    {
        skip_header_space(buf, -1 * amount);
    }

    /**
     * @brief Get the total number of header bytes that are currently being skipped.
     *
     * This is the aggregate result of all \c skip_header_space() / \c restore_header_space() calls.
     *
     * @param buf Buffer to operate on.
     */
    virtual int32_t get_header_skip_size(net_stack_mem_buf_t *buf) = 0;

    enum class Lifetime {
        POOL_ALLOCATED, ///< Allocated from the memory manager's pool
        HEAP_ALLOCATED, ///< Allocated from the memory manager's heap
        CONSTANT, ///< Buffer points to constant data (e.g. in ROM) that will live forever
        VOLATILE ///< Buffer points to data from the application that will not live past the current network stack call.
    };

    /**
     * Gets the lifetime of the buffer
     *
     * @param buf      Memory buffer
     */
    virtual Lifetime get_lifetime(net_stack_mem_buf_t const *buf) const = 0;

    /**
     * @brief Set callback which will be called when pool space becomes available
     *
     * \warning The callback could be called from any thread, and should make no assumptions about
     *    being in the same thread as anything else.
     *
     * @param cb Callback to call
     */
    void set_on_pool_space_avail_cb(mbed::Callback<void()> cb)
    {
        onPoolSpaceAvailCallback = cb;
    }

protected:
    ~NetStackMemoryManager() = default;
};

#endif /* NET_STACK_MEMORY_MANAGER_H */
