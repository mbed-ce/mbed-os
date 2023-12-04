/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
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

#ifndef MBED_CACHEALIGNEDBUFFER_H
#define MBED_CACHEALIGNEDBUFFER_H

#include <cstdlib>

#include "device.h"

namespace mbed {


/**
 * @brief CacheAlignedBuffer is used by Mbed in locations where we need a cache-aligned buffer.
 *
 * Cache alignment is desirable in several different situations in embedded programming; often
 * when working with DMA or other peripherals which write their results back to main memory.
 * In order to read those results from memory without risk of getting old data from the
 * CPU cache, one needs to align the buffer so it takes up an integer number of cache lines,
 * then invalidate the cache lines so that the data gets reread from RAM.
 *
 * CacheAlignedBuffer provides an easy way to allocate the correct amount of space so that
 * a buffer of any size can be made cache-aligned.
 *
 * @tparam DataT Type of the data to store in the buffer.  Note: %CacheAlignedBuffer is not designed for
 *    using class types as DataT, and will not call constructors.
 * @tparam BufferSize Buffer size (number of elements) needed by the application for the buffer.
 */
template<typename DataT, size_t BufferSize>
struct
CacheAlignedBuffer {
private:
#if __DCACHE_PRESENT
    // Allocate enough extra space that we can shift the start of the buffer forward to be on a cache line.
    // The worst case for this is when the first byte is allocated 1 byte past the start of a cache line, so we
    // will need an additional (cache line size - 1) bytes.
    // Additionally, since we are going to be invalidating this buffer, we can't allow any other variables to be
    // in the same cache lines, or they might get corrupted.
    // So, we need to round up the backing buffer size to the nearest multiple of the cache line size.
    // The math for rounding up can be found here:
    // https://community.st.com/t5/stm32-mcus-products/maintaining-cpu-data-cache-coherence-for-dma-buffers/td-p/95746
    constexpr static size_t requiredSizeRoundedUp =  (BufferSize * sizeof(DataT) + __SCB_DCACHE_LINE_SIZE - 1) & ~((__SCB_DCACHE_LINE_SIZE) - 1);
    constexpr static size_t backingBufferSizeBytes = requiredSizeRoundedUp + __SCB_DCACHE_LINE_SIZE - 1;
#else
    constexpr static size_t backingBufferSizeBytes = BufferSize * sizeof(DataT);
#endif

    uint8_t _backingBuffer[backingBufferSizeBytes];
    DataT * _alignedArrayPtr;

    /**
     * Find and return the first location in the given buffer that starts on a cache line.
     * If this MCU does not use a cache, this function is a no-op.
     *
     * @param buffer Pointer to buffer
     *
     * @return Pointer to first data item, aligned at the start of a cache line.
     */
    inline DataT * findCacheLineStart(uint8_t * buffer)
    {
#if __DCACHE_PRESENT
        // Use integer division to divide the address down to the cache line size, which
        // rounds to the cache line before the given address.
        // So that we always go one cache line back even if the given address is on a cache line,
        // subtract 1.
        ptrdiff_t prevCacheLine = ((ptrdiff_t)(buffer - 1)) / __SCB_DCACHE_LINE_SIZE;

        // Now we just have to multiply up again to get an address (adding 1 to go forward by 1 cache line)
        return reinterpret_cast<DataT *>((prevCacheLine + 1) * __SCB_DCACHE_LINE_SIZE);
#else
        return reinterpret_cast<DataT *>(buffer);
#endif
    }

public:

    // Iterator types
    typedef DataT * iterator;
    typedef DataT const * const_iterator;

    /**
     * @brief Construct new cache-aligned buffer.  Buffer will be zero-initialized.
     */
    CacheAlignedBuffer():
    _backingBuffer{},
    _alignedArrayPtr(findCacheLineStart(_backingBuffer))
    {}

    /**
     * @brief Copy from other cache-aligned buffer.  Buffer memory will be copied.
     */
    CacheAlignedBuffer(CacheAlignedBuffer const & other):
    _alignedArrayPtr(findCacheLineStart(_backingBuffer))
    {
        memcpy(this->_alignedArrayPtr, other._alignedArrayPtr, BufferSize * sizeof(DataT));
    }

    /**
     * @brief Assign from other cache-aligned buffer.  Buffer memory will be assigned.
     */
    CacheAlignedBuffer & operator=(CacheAlignedBuffer const & other)
    {
        memcpy(this->_alignedArrayPtr, other._alignedArrayPtr, BufferSize * sizeof(DataT));
    }

    /**
     * @brief Get a pointer to the aligned data array inside the buffer
     */
    DataT * data() { return _alignedArrayPtr; }

    /**
     * @brief Get a pointer to the aligned data array inside the buffer (const version)
     */
    DataT const * data() const { return _alignedArrayPtr; }

    /**
     * @brief Element access
     */
    DataT & operator[](size_t index) { return _alignedArrayPtr[index]; }

    /**
     * @brief Element access (const)
     */
    DataT operator[](size_t index) const { return _alignedArrayPtr[index]; }

    /**
     * @brief Get iterator for start of buffer
     */
    iterator begin() { return _alignedArrayPtr; }

    /**
     * @brief Get iterator for start of buffer
     */
    const_iterator begin() const { return _alignedArrayPtr; }

    /**
     * @brief Get iterator for end of buffer
     */
    iterator end() { return _alignedArrayPtr + BufferSize; }

    /**
     * @brief Get iterator for end of buffer
     */
    const_iterator end() const { return _alignedArrayPtr + BufferSize; }

    /**
     * @return The maximum amount of DataT elements that this buffer can hold
     */
    constexpr size_t capacity() { return BufferSize; }

    /**
     * @brief If this MCU has a data cache, this function _invalidates_ the buffer in the data cache.
     *
     * Invalidation means that the next time code access the buffer data, it is guaranteed to be fetched from
     * main memory rather than from the CPU cache.  If there are changes made to the data which have not been
     * flushed back to main memory, those changes will be lost!
     */
    void invalidate()
    {
#if __DCACHE_PRESENT
        SCB_InvalidateDCache_by_Addr(_alignedArrayPtr, BufferSize * sizeof(DataT));
#endif
    }
};

}

#endif //MBED_CACHEALIGNEDBUFFER_H
