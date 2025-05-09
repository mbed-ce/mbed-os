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
#if defined(MBED_CONF_RTOS_PRESENT)
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <list>
#include <stdlib.h>

#include "rtos/Mutex.h"

#include "nsdynmemLIB.h"
#include "arm_hal_interrupt_private.h"

#include "EMACMemoryManager.h"
#include "EmacTestMemoryManager.h"

#define BUF_HEAD        "headheadheadhead"
#define BUF_HEAD_SIZE   16
#define BUF_TAIL        "tailtailtailtail"
#define BUF_TAIL_SIZE   16

#define CHECK_ASSERT(value, fmt, ...) check_value(value, fmt, ##__VA_ARGS__)

#define MEM_MNGR_TRACE "test mem mngr: "

char s_trace_buffer[100] = MEM_MNGR_TRACE;

/* For LPC boards define the heap memory bank ourselves to give us section placement
   control */
#ifndef ETHMEM_SECTION
#if defined(TARGET_LPC1768)
#  if defined (__ICCARM__)
#     define ETHMEM_SECTION
#  elif defined(TOOLCHAIN_GCC_CR)
#     define ETHMEM_SECTION __attribute__((section(".data.$RamPeriph32")))
#  else
#     define ETHMEM_SECTION __attribute__((section("AHBSRAM"),aligned))
#  endif
#endif
#endif

#ifdef ETHMEM_SECTION
// Use nanostack libservice dynamic memory library
#define EMAC_HEAP_SIZE 16300

#if defined (__ICCARM__)
#pragma location = ".ethusbram"
#endif
ETHMEM_SECTION static unsigned char ns_heap[EMAC_HEAP_SIZE];

void emac_heap_error_handler(heap_fail_t event)
{
    MBED_ASSERT(0);
}
#endif

EmacTestMemoryManager::EmacTestMemoryManager()
    : m_mem_mutex(),
      m_mem_buffers(),
      m_alloc_unit(MBED_CONF_NSAPI_EMAC_RX_POOL_BUF_SIZE),
      m_memory_available(true),
      m_pool_memory_available(true)
{
#ifdef ETHMEM_SECTION
    static bool ns_heap_init = false;
    if (!ns_heap_init) {
        platform_critical_init(); // Create mutex for dynamic memory library
        ns_dyn_mem_init(ns_heap, EMAC_HEAP_SIZE, emac_heap_error_handler, NULL);
        ns_heap_init = true;
    }
#endif
}

emac_mem_buf_t *EmacTestMemoryManager::alloc_heap(uint32_t size, uint32_t align)
{
    return alloc_heap(size, align, MEM_CHECK);
}

emac_mem_buf_t *EmacTestMemoryManager::alloc_heap(uint32_t size, uint32_t align, uint8_t opt)
{
    validate_list();

    CHECK_ASSERT(size, "alloc_heap() invalid parameter size");

    check_align(align);

    if ((opt & MEM_CHECK) && !m_memory_available) {
        return NULL;
    }

    m_mem_mutex.lock();

    emac_memory_t *buf = new emac_memory_t;

    CHECK_ASSERT(buf, "alloc_heap() no memory");

#ifdef ETHMEM_SECTION
    buf->buffer = ns_dyn_mem_alloc(BUF_HEAD_SIZE + size + align + BUF_TAIL_SIZE);
#else
    buf->buffer = std::malloc(BUF_HEAD_SIZE + size + align + BUF_TAIL_SIZE);
#endif

    CHECK_ASSERT(buf->buffer, "alloc_heap() no memory");

    buf->next = 0;
    buf->ptr = static_cast<char *>(buf->buffer) + BUF_HEAD_SIZE;
    buf->orig_len = size;
    buf->len = size;
    buf->first = true;
    buf->lifetime = Lifetime::HEAP_ALLOCATED;

    if (opt & MEM_NO_ALIGN) {
        if (reinterpret_cast<uint32_t>(buf->ptr) % sizeof(uint16_t) == 0) {
            buf->ptr = static_cast<char *>(buf->ptr) + 1;
        }
    } else if (align) {
        uint32_t remainder = reinterpret_cast<uint32_t>(buf->ptr) % align;
        if (remainder) {
            uint32_t offset = align - remainder;
            if (offset >= align) {
                offset = align;
            }
            buf->ptr = static_cast<char *>(buf->ptr) + offset;
        }
    }

    char *buffer_head = static_cast<char *>(buf->ptr) - BUF_HEAD_SIZE;
    memcpy(buffer_head, BUF_HEAD, BUF_HEAD_SIZE);

    char *buffer_tail = static_cast<char *>(buf->ptr) + buf->len;
    memcpy(buffer_tail, BUF_TAIL, BUF_TAIL_SIZE);

    // Scribble over the buffer contents with 'y' so it's totally obvious if someone uses it uninitialized.
    // Do this in both the cache and the main memory.
    memset(buf->ptr, 'y', buf->orig_len);
#if __DCACHE_PRESENT
    SCB_CleanDCache_by_Addr(buf->ptr, buf->orig_len);
#endif

    m_mem_buffers.push_front(buf);

    m_mem_mutex.unlock();

    return buf;
}

emac_mem_buf_t *EmacTestMemoryManager::alloc_pool(uint32_t size, uint32_t align)
{
    return alloc_pool(size, align, MEM_CHECK);
}

emac_mem_buf_t *EmacTestMemoryManager::alloc_pool(uint32_t size, uint32_t align, uint8_t opt)
{
    validate_list();

    CHECK_ASSERT(size, "alloc_pool() invalid parameter size");

    check_align(align);

    if ((opt & MEM_CHECK) && (!m_memory_available || !m_pool_memory_available)) {
        return NULL;
    }

    // Lock memory mutex
    rtos::ScopedMutexLock lock(m_mem_mutex);

    // Contiguous allocation
    if (size + align <= m_alloc_unit) {
        if (m_pool_bufs_used > MBED_CONF_NSAPI_EMAC_RX_POOL_NUM_BUFS) {
            return nullptr;
        }

        auto *buf = alloc_heap(size, align, opt);

        static_cast<emac_memory_t *>(buf)->lifetime = Lifetime::POOL_ALLOCATED;
        ++m_pool_bufs_used;

        return buf;
    }

    unsigned int pool_buffer_max_size = m_alloc_unit - align;

    CHECK_ASSERT(pool_buffer_max_size > 0, "alloc_pool() invalid align");

    emac_memory_t *first_buf = 0;
    emac_memory_t *prev_buf = 0;
    unsigned int size_left = size;

    while (size_left != 0) {
        unsigned int alloc_size;

        // New alloc size buffer needed
        if (size_left > pool_buffer_max_size) {
            size_left = size_left - pool_buffer_max_size;
            alloc_size = pool_buffer_max_size;
            // New smaller than alloc size buffer needed
        } else {
            alloc_size = size_left;
            size_left = 0;
        }

        if (m_pool_bufs_used > MBED_CONF_NSAPI_EMAC_RX_POOL_NUM_BUFS) {
            // No simulated pool space left, free and return nullptr
            if (first_buf != nullptr) {
                free(first_buf);
            }
            return nullptr;
        }

        emac_memory_t *new_buf = static_cast<emac_memory_t *>(alloc_heap(alloc_size, align, opt));

        static_cast<emac_memory_t *>(new_buf)->lifetime = Lifetime::POOL_ALLOCATED;
        ++m_pool_bufs_used;

        if (prev_buf) {
            new_buf->first = false;
            prev_buf->next = new_buf;
        } else {
            first_buf = new_buf;
        }
        prev_buf = new_buf;
    }

    return first_buf;
}

uint32_t EmacTestMemoryManager::get_pool_alloc_unit(uint32_t align) const
{
    validate_list();

    check_align(align);

    return m_alloc_unit - align;
}

void EmacTestMemoryManager::free(emac_mem_buf_t *buf)
{
    validate_list();

    CHECK_ASSERT(buf, "free(): buffer parameter is null");

    emac_memory_t *mem_buf = static_cast<emac_memory_t *>(buf);

    CHECK_ASSERT(mem_buf->first, "free() not first in chain");

    std::list<emac_memory_t *>::iterator mem_buf_entry;

    m_mem_mutex.lock();

    bool pool_buf_freed = false;

    while (mem_buf) {
        for (mem_buf_entry = m_mem_buffers.begin(); mem_buf_entry != m_mem_buffers.end(); ++mem_buf_entry) {
            if (*mem_buf_entry == mem_buf) {
                break;
            }
        }

        if (mem_buf_entry == m_mem_buffers.end()) {
            CHECK_ASSERT(0, "free(): %p buffer already freed", mem_buf);
            m_mem_mutex.unlock();
            return;
        }

        char *buffer_head = static_cast<char *>(mem_buf->ptr) - BUF_HEAD_SIZE;
        if (memcmp(buffer_head, BUF_HEAD, BUF_HEAD_SIZE) != 0) {
            CHECK_ASSERT(0, "free(): %p head overwrite", mem_buf);
        }

        char *buffer_tail = static_cast<char *>(mem_buf->ptr) + mem_buf->orig_len;
        if (memcmp(buffer_tail, BUF_TAIL, BUF_TAIL_SIZE) != 0) {
            CHECK_ASSERT(0, "free(): %p tail overwrite", mem_buf);
        }

        // Scribble over the buffer contents with 'z' so it's totally obvious if someone reuses it later.
        // Do this in both the cache and the main memory.
        memset(mem_buf->ptr, 'z', mem_buf->orig_len);
#if __DCACHE_PRESENT
        SCB_CleanDCache_by_Addr(mem_buf->ptr, mem_buf->orig_len);
#endif

        // Update pool size
        if (mem_buf->lifetime == Lifetime::POOL_ALLOCATED) {
            --m_pool_bufs_used;
            pool_buf_freed = true;
        }

        emac_memory_t *next = mem_buf->next;

        m_mem_buffers.erase(mem_buf_entry);

#ifdef ETHMEM_SECTION
        ns_dyn_mem_free(mem_buf->buffer);
#else
        std::free(mem_buf->buffer);
#endif
        delete mem_buf;

        mem_buf = next;
    }

    m_mem_mutex.unlock();

    if (pool_buf_freed && onPoolSpaceAvailCallback) {
        onPoolSpaceAvailCallback();
    }
}

uint32_t EmacTestMemoryManager::get_total_len(const emac_mem_buf_t *buf) const
{
    validate_list();

    if (!validate_ptr(buf)) {
        CHECK_ASSERT(0, "get_total_len(): %p invalid buffer\n", buf);
        return 0;
    }

    uint32_t total_len = 0;

    for (emac_memory_t *mem_buf = (emac_memory_t *) buf; mem_buf != NULL; mem_buf = mem_buf->next) {
        total_len += mem_buf->len;
    }

    return total_len;
}

void EmacTestMemoryManager::copy(emac_mem_buf_t *to_buf, const emac_mem_buf_t *from_buf)
{
    validate_list();

    if (!validate_ptr(to_buf)) {
        CHECK_ASSERT(0, "copy(): %p invalid to buffer\n", to_buf);
        return;
    }

    if (!validate_ptr(from_buf)) {
        CHECK_ASSERT(0, "copy(): %p invalid from buffer\n", from_buf);
        return;
    }

    if (get_total_len(to_buf) != get_total_len(from_buf)) {
        CHECK_ASSERT(0, "copy(): %p to and %p from buffer total lengths not same\n", to_buf, from_buf);
        return;
    }

    unsigned int to_buf_offset = 0;
    unsigned int from_buf_offset = 0;

    emac_memory_t *to_mem_buf = static_cast<emac_memory_t *>(to_buf);
    const emac_memory_t *from_mem_buf = static_cast<const emac_memory_t *>(from_buf);

    while (to_mem_buf && from_mem_buf) {
        unsigned int buf_copy_len;

        // Is there data in from buffer
        buf_copy_len = from_mem_buf->len - from_buf_offset;
        if (buf_copy_len == 0) {
            from_mem_buf = from_mem_buf->next;
            from_buf_offset = 0;
            continue;
        }

        // Is there space left in to buffer
        if (buf_copy_len > to_mem_buf->len - to_buf_offset) {
            buf_copy_len = to_mem_buf->len - to_buf_offset;
        }
        if (buf_copy_len == 0) {
            to_mem_buf = to_mem_buf->next;
            to_buf_offset = 0;
            continue;
        }

        // Copy data
        memcpy(static_cast<char *>(to_mem_buf->ptr) + to_buf_offset, static_cast<const char *>(from_mem_buf->ptr) + from_buf_offset, buf_copy_len);
        from_buf_offset += buf_copy_len;
        to_buf_offset += buf_copy_len;
    }
}

void EmacTestMemoryManager::cat(emac_mem_buf_t *to_buf, emac_mem_buf_t *cat_buf)
{
    validate_list();

    if (!validate_ptr(to_buf)) {
        CHECK_ASSERT(0, "cat(): %p invalid to buffer\n", to_buf);
        return;
    }

    if (!validate_ptr(cat_buf)) {
        CHECK_ASSERT(0, "cat(): %p invalid cat buffer\n", cat_buf);
        return;
    }

    emac_memory_t *cat_mem_buf = static_cast<emac_memory_t *>(cat_buf);

    if (!cat_mem_buf->first) {
        CHECK_ASSERT(0, "cat(): %p cat buffer does not point to head of chain\n", cat_buf);
        return;
    }

    emac_memory_t *to_mem_buf = static_cast<emac_memory_t *>(to_buf);

    while (to_mem_buf->next != 0) {
        to_mem_buf = to_mem_buf->next;
    }

    to_mem_buf->next = cat_mem_buf;
    cat_mem_buf->first = false;
}

emac_mem_buf_t *EmacTestMemoryManager::get_next(const emac_mem_buf_t *buf) const
{
    validate_list();

    if (!validate_ptr(buf)) {
        CHECK_ASSERT(0, "get_next(): %p invalid buffer\n", buf);
        return NULL;
    }

    const emac_memory_t *mem_buf = static_cast<const emac_memory_t *>(buf);
    return mem_buf->next;
}

void *EmacTestMemoryManager::get_ptr(const emac_mem_buf_t *buf) const
{
    validate_list();

    if (!validate_ptr(buf)) {
        CHECK_ASSERT(0, "get_ptr(): %p invalid buffer\n", buf);
        return NULL;
    }

    const emac_memory_t *mem_buf = static_cast<const emac_memory_t *>(buf);
    return mem_buf->ptr;
}

uint32_t EmacTestMemoryManager::get_len(const emac_mem_buf_t *buf) const
{
    validate_list();

    if (!validate_ptr(buf)) {
        CHECK_ASSERT(0, "get_len(): %p invalid buffer\n", buf);
        return 0;
    }

    const emac_memory_t *mem_buf = static_cast<const emac_memory_t *>(buf);
    return mem_buf->len;
}

void EmacTestMemoryManager::set_len(emac_mem_buf_t *buf, uint32_t len)
{
    validate_list();

    if (!validate_ptr(buf)) {
        CHECK_ASSERT(0, "set_len(): %p invalid buffer\n", buf);
        return;
    }

    emac_memory_t *mem_buf = static_cast<emac_memory_t *>(buf);

    if (len > mem_buf->orig_len) {
        CHECK_ASSERT(0, "set_len(): %p new length %i must be less or equal allocated size %i\n", buf, len, mem_buf->orig_len);
        return;
    }

    if (!mem_buf->first) {
        CHECK_ASSERT(0, "set_len(): %p buffer does not point to head of chain\n", buf);
        return;
    }

    mem_buf->len = len;
}

NetStackMemoryManager::Lifetime EmacTestMemoryManager::get_lifetime(const net_stack_mem_buf_t *buf) const
{
    return static_cast<emac_memory_t const *>(buf)->lifetime;
}

void EmacTestMemoryManager::set_alloc_unit(uint32_t alloc_unit)
{
    validate_list();

    m_alloc_unit = alloc_unit;
}

void EmacTestMemoryManager::set_memory_available(bool memory)
{
    m_memory_available = memory;

    // Poke the EMAC in case it can allocate buffers
    if (m_memory_available && onPoolSpaceAvailCallback) {
        onPoolSpaceAvailCallback();
    }
}

void EmacTestMemoryManager::set_pool_memory_available(bool memory)
{
    m_pool_memory_available = memory;

    // Poke the EMAC in case it can allocate buffers
    if (m_pool_memory_available && m_memory_available && onPoolSpaceAvailCallback) {
        onPoolSpaceAvailCallback();
    }
}

void EmacTestMemoryManager::get_memory_statistics(int *buffers, int *memory)
{
    if (!buffers || !memory) {
        return;
    }

    *buffers = 0;
    *memory = 0;

    m_mem_mutex.lock();

    const emac_memory_t *mem_buf;

    for (std::list<emac_memory_t *>::const_iterator it = m_mem_buffers.begin(); it != m_mem_buffers.end(); ++it) {
        mem_buf = static_cast<const emac_memory_t *>(*it);
        ++(*buffers);
        *memory += mem_buf->orig_len;
    }

    m_mem_mutex.unlock();
}

template <typename TYPE> void EmacTestMemoryManager::check_value(TYPE value, const char *fmt, ...) const
{
    if (!value) {
        va_list ap;
        va_start(ap, fmt);
        vfprintf(stderr, fmt, ap);
        MBED_ASSERT(false);
        va_end(ap);
    }
}

bool EmacTestMemoryManager::validate_ptr(const emac_mem_buf_t *buf) const
{
    if (!buf) {
        return false;
    }

    m_mem_mutex.lock();

    const emac_memory_t *mem_buf = static_cast<const emac_memory_t *>(buf);

    for (std::list<emac_memory_t *>::const_iterator it = m_mem_buffers.begin(); it != m_mem_buffers.end(); ++it) {
        emac_memory_t *list_buf = static_cast<emac_memory_t *>(*it);
        if (list_buf == mem_buf) {
            m_mem_mutex.unlock();
            return true;
        }
    }

    m_mem_mutex.unlock();

    return false;
}

void EmacTestMemoryManager::check_align(uint32_t align) const
{
    if (align > 64) {
        CHECK_ASSERT(0, "check_align(): invalid alignment value");
    }
}

void EmacTestMemoryManager::validate_list() const
{
    m_mem_mutex.lock();

    const emac_memory_t *mem_buf;

    for (std::list<emac_memory_t *>::const_iterator it = m_mem_buffers.begin(); it != m_mem_buffers.end(); ++it) {
        mem_buf = static_cast<const emac_memory_t *>(*it);

        char *buffer_head = static_cast<char *>(mem_buf->ptr) - BUF_HEAD_SIZE;
        if (memcmp(buffer_head, BUF_HEAD, BUF_HEAD_SIZE) != 0) {
            CHECK_ASSERT(0, "validate_list(): %p head overwrite", mem_buf);
        }

        char *buffer_tail = static_cast<char *>(mem_buf->ptr) + mem_buf->orig_len;
        if (memcmp(buffer_tail, BUF_TAIL, BUF_TAIL_SIZE) != 0) {
            CHECK_ASSERT(0, "validate_list(): %p tail overwrite", mem_buf);
        }
    }

    m_mem_mutex.unlock();
}

EmacTestMemoryManager &EmacTestMemoryManager::get_instance()
{
    static EmacTestMemoryManager test_memory_manager;
    return test_memory_manager;
}


#endif // defined(MBED_CONF_RTOS_PRESENT)
