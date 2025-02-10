/* Copyright (c) 2025 Jamie Smith
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

#ifndef MBED_OS_GENERICETHDMA_H
#define MBED_OS_GENERICETHDMA_H

#include "CompositeEMAC.h"
#include "mbed_trace.h"
#include "CacheAlignedBuffer.h"
#include "mbed_critical.h"
#include <atomic>

#define TRACE_GROUP  "GEDMA"

namespace mbed {
    /**
     * @brief Generic transmit DMA loop
     *
     * This implementation of Tx DMA should work for the large majority of embedded MCUs that use a DMA ring-based
     * ethernet MAC.
     *
     * @tparam DescriptorT Type representing an Ethernet descriptor.
     *
     * @note If the MCU has a D-cache, then \c DescriptorT must be padded to an exact number of cache lines in size!
     */
    template<typename DescriptorT>
    class GenericTxDMALoop : public CompositeEMAC::TxDMA
    {
    protected:
        /// Tx descriptor array. It's up to the subclass to allocate these in the correct location.
        CacheAlignedBuffer<DescriptorT> & txDescs;

        /// Pointer to first memory buffer in the chain associated with descriptor n.
        /// The buffer address shall only be set for the *last* descriptor, so that the entire chain is freed
        /// when the last descriptor is returned.
        std::array<net_stack_mem_buf_t *, MBED_CONF_NSAPI_EMAC_TX_NUM_DESCS> descStackBuffers{};

        /// EventFlag used to signal when a Tx descriptor becomes available
        rtos::EventFlags txDescAvailFlag;

        size_t txSendIndex; ///< Index of the next Tx descriptor that can be filled with data
        std::atomic<size_t> txDescsOwnedByApplication; ///< Number of Tx descriptors owned by the application.  Incremented by the mac thread and decremented by the application thread.
        size_t txReclaimIndex; ///< Index of the next Tx descriptor that will be reclaimed by the mac thread.

        /// Configure DMA registers to point to the DMA ring,
        /// and enable DMA. This is done before the MAC itself is enabled.
        virtual void startDMA() = 0;

        /// Stop the DMA running. This is done after MAC transmit & recieve are disabled.
        virtual void stopDMA() = 0;

        /// Get whether the given buffer is in a memory region readable by the Ethernet DMA.
        /// If this returns false for a buffer being transmitted, the buffer will be copied into a new
        /// heap-allocated buffer.
        virtual bool isDMAReadableBuffer(uint8_t const * start, size_t size) const = 0;

        /// Give the descriptor at the given index to DMA to be transmitted. This is called after
        /// the buffer has been set.
        /// Note: if the descriptor needs to be flushed from CPU cache, you need to do that
        /// at the correct point in the implementation of this method!
        virtual void giveToDMA(size_t descIdx, bool firstDesc, bool lastDesc) = 0;

    public:
        /**
         * @brief Construct GenericTxDMALoop
         * @param txDescs Tx descriptor buffer, containing exactly MBED_CONF_NSAPI_EMAC_TX_NUM_DESCS
         *    descriptors. Subclass must allocate this with the correct configuration and location.
         */
        GenericTxDMALoop(CacheAlignedBuffer<DescriptorT> & txDescs):
        txDescs(txDescs)
        {}

        CompositeEMAC::ErrCode init() override {
            // At the start, we own all the descriptors
            txDescsOwnedByApplication = MBED_CONF_NSAPI_EMAC_TX_NUM_DESCS;

            // Next descriptor will be descriptor 0
            txSendIndex = 0;
            txReclaimIndex = 0;

            startDMA();

            return CompositeEMAC::ErrCode::SUCCESS;
        }

        CompositeEMAC::ErrCode deinit() override {
            stopDMA();

            // Deallocate all buffers currently assigned to the DMA ring
            for(auto & buf_addr : descStackBuffers) {
                if(buf_addr != nullptr) {
                    memory_manager->free(buf_addr);
                    buf_addr = nullptr;
                }
            }

            return CompositeEMAC::ErrCode::SUCCESS;
        }

        bool reclaimTxDescs() override {
            bool returnedAnyDescriptors = false;
            while (true)
            {
                if (txReclaimIndex == txSendIndex && txDescsOwnedByApplication > 0) {
                    // If we have reached the Tx send index, we want to stop iterating as this is
                    // the next descriptor that has not been populated by the application yet.
                    // The only exception is if the Tx ring is completely full, in which case we want
                    // to process the entire ring.  In the case where the Tx ring is full,
                    // txDescsOwnedByApplication will be 0.
                    // Note that txSendIndex and txDescsOwnedByApplication are updated in a critical
                    // section so their values will always be in sync with each other.
                    break;
                }

                auto &currDesc = txDescs[txReclaimIndex];

#if __DCACHE_PRESENT
                SCB_InvalidateDCache_by_Addr(&currDesc, sizeof(DescriptorT));
#endif

                if (currDesc.ownedByDMA()) {
                    // This desc is owned by the DMA, so we have reached the part of the ring buffer
                    // that is still being transmitted.
                    // Done for now!
                    break;
                }

                // Free any buffers associated with the descriptor
                if (descStackBuffers[txReclaimIndex] != nullptr) {
                    memory_manager->free(descStackBuffers[txReclaimIndex]);
                }

                // Update counters
                txReclaimIndex = (txReclaimIndex + 1) % MBED_CONF_NSAPI_EMAC_TX_NUM_DESCS;
                ++txDescsOwnedByApplication;

                returnedAnyDescriptors = true;
            }

            if(returnedAnyDescriptors) {
                txDescAvailFlag.set(1);
            }

            return returnedAnyDescriptors;
        }

        CompositeEMAC::ErrCode txPacket(net_stack_mem_buf_t * buf) {
            // Step 1: Figure out if we can send this zero-copy, or if we need to copy it.
            size_t neededDescs = memory_manager->count_buffers(buf);
            bool needToCopy = false;
            if(neededDescs >= MBED_CONF_NSAPI_EMAC_TX_NUM_DESCS)
            {
                // Packet uses too many buffers, we have to copy it into a continuous buffer.
                // Note: Some Eth DMAs (e.g. STM32 v2) cannot enqueue all the descs in the ring at the same time
                // so we can't use every single descriptor to send the packet.
                needToCopy = true;
            }

            if(!needToCopy) {
                net_stack_mem_buf_t * currBuf = buf;
                while(currBuf != nullptr) {
                    // If this buffer is passed down direct from the application, we will need to
                    // copy the packet.
                    if(memory_manager->get_lifetime(currBuf) == NetStackMemoryManager::Lifetime::VOLATILE)
                    {
                        needToCopy = true;
                        break;
                    }

                    // Or, if the buffer is in DMA-inaccessible RAM, we will need to copy it
                    if(!isDMAReadableBuffer(static_cast<uint8_t *>(memory_manager->get_ptr(currBuf)), memory_manager->get_len(currBuf))) {
                        needToCopy = true;
                        break;
                    }

                    currBuf = memory_manager->get_next(currBuf);
                }
            }

            tr_debug("Transmitting packet of length %lu in %zu buffers and %zu descs\n",
               memory_manager.get_total_len(buf), memory_manager.count_buffers(buf), neededDescs);

            // Step 2: Copy packet if needed
            if(needToCopy)
            {
                auto * newBuf = memory_manager->alloc_heap(memory_manager->get_total_len(buf), 0);
                if(newBuf == nullptr)
                {
                    // No free memory, drop packet
                    memory_manager->free(newBuf);
                    return CompositeEMAC::ErrCode::OUT_OF_MEMORY;
                }

                // We should have gotten just one contiguous buffer
                MBED_ASSERT(memory_manager->get_next(newBuf) == nullptr);
                neededDescs = 1;

                // Copy data over
                memory_manager->copy_from_buf(static_cast<uint8_t *>(memory_manager->get_ptr(newBuf)), memory_manager->get_len(newBuf), buf);
                memory_manager->free(buf);
                buf = newBuf;
            }

            // Step 3: Wait for needed amount of buffers to be available.
            // Note that, in my experience, it's better to block here, as dropping the packet
            // due to not having enough buffers can create weird effects when the application sends
            // lots of packets at once.
            while(txDescsOwnedByApplication < neededDescs)
            {
                txDescAvailFlag.wait_any_for(1, rtos::Kernel::wait_for_u32_forever);
            }

            // Step 4: Load buffer into descriptors and send
            net_stack_mem_buf_t * currBuf = buf;
            for(size_t descCount = 0; descCount < neededDescs; descCount++)
            {
                auto & currDesc = txDescs[txSendIndex];

                // Set buffer 1
                currDesc.setBuffer(static_cast<uint8_t *>(memory_manager->get_ptr(currBuf)), memory_manager->get_len(currBuf));
#if __DCACHE_PRESENT
                // Write buffer back to main memory
                SCB_CleanDCache_by_Addr(memory_manager->get_ptr(currBuf), memory_manager->get_len(currBuf));
#endif

                // Move to next buffer
                currBuf = memory_manager->get_next(currBuf);

                if(currBuf == nullptr)
                {
                    // Last descriptor, store buffer address for freeing
                    descStackBuffers[txSendIndex] = buf;
                }
                else
                {
                    descStackBuffers[txSendIndex] = nullptr;
                }

                // Enter a critical section, because we could run into weird corner cases if the
                // interrupt executes while we are half done configuring this descriptor and updating
                // the counters.
                core_util_critical_section_enter();

                // Configure settings.
                giveToDMA(txSendIndex, descCount == 0, currBuf == nullptr);

                // Update descriptor count and index
                --txDescsOwnedByApplication;
                txSendIndex = (txSendIndex + 1) % MBED_CONF_NSAPI_EMAC_TX_NUM_DESCS;

                core_util_critical_section_exit();
            }

            return CompositeEMAC::ErrCode::SUCCESS;
        }

    };
}

#undef TRACE_GROUP

#endif //MBED_OS_GENERICETHDMA_H
