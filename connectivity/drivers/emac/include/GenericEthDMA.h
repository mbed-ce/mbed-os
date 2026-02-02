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
#include "MbedCRC.h"
#include "mbed_critical.h"
#include <atomic>
#include <optional>

#define TRACE_GROUP  "GEDMA"

namespace mbed {
    /**
     * @brief Generic transmit DMA loop
     *
     * This implementation of Tx DMA should work for the large majority of embedded MCUs that use a DMA ring-based
     * ethernet MAC.
     */
    class GenericTxDMARing : public CompositeEMAC::TxDMA
    {
    public:
        /// Number of entries in the Tx descriptor ring
        static constexpr size_t TX_NUM_DESCS = MBED_CONF_NSAPI_EMAC_TX_NUM_DESCS;

    protected:
        /// Extra, unfilled Tx descs to leave in the DMA ring at all times.
        /// This is used to support Eth MACs that don't allow enqueuing every single descriptor at a time.
        const size_t extraTxDescsToLeave;

        /// Whether the hardware supports chaining multiple descriptors together to send one
        /// packet that's split across multiple buffers.
        const bool supportsDescChaining;

        /// Whether we need to manually append the CRC at the end of the frame being transmitted.
        /// Most MACs do this automatically but on some it is bugged (e.g. LPC1768)
        const bool appendCRC;

        /// Whether we need to manually pad short Ethernet frames up to 60 bytes.
        /// Most MACs can do this in HW, but not LPC1768!
        const bool padFrames;

        /// Pointer to first memory buffer in the chain associated with descriptor n.
        /// The buffer address shall only be set for the *last* descriptor, so that the entire chain is freed
        /// when the last descriptor is returned.
        std::array<net_stack_mem_buf_t *, TX_NUM_DESCS> descStackBuffers{};

        /// EventFlag used to signal when a Tx descriptor becomes available
        rtos::EventFlags txDescAvailFlag;

        /// CRC generator. Only used if appendCRC is true.
        MbedCRC<POLY_32BIT_ANSI, 32> crcGenerator;

        // Indexes for descriptor rings.
        // NOTE: when working with these indices, it's important to consider the case where e.g. the send and reclaim indexes are
        // equal.  This could mean *either* that the Tx ring is completely full of data, or that the Tx ring is empty.
        // To resolve this ambiguity, we maintain separate count variables that track how many entries are in the ring at present.
        size_t txSendIndex; ///< Index of the next Tx descriptor that can be filled with data
        std::atomic<size_t> txDescsOwnedByApplication; ///< Number of Tx descriptors owned by the application. Decremented by txPacket() and incremented by reclaimTxDescs()
        size_t txReclaimIndex; ///< Index of the next Tx descriptor that will be reclaimed by the mac thread calling reclaimTxDescs().

        /// Construct, passing a value for extraTxDescsToLeave
        GenericTxDMARing(size_t extraTxDescsToLeave = 0, bool supportsDescChaining = true, bool appendCRC = false, bool padFrames = false):
        extraTxDescsToLeave(extraTxDescsToLeave),
        supportsDescChaining(supportsDescChaining),
        appendCRC(appendCRC),
        padFrames(padFrames),

        // Per Wikipedia and SO, we want input and output reflect on due to Ethernet transmitting
        // the CRC with backwards bit order compared to the rest of the data.
        crcGenerator(0xFFFFFFFF, 0xFFFFFFFF, true, true)
        {}

        /// Configure DMA registers to point to the DMA ring,
        /// and enable DMA. This is done before the MAC itself is enabled.
        virtual void startDMA() = 0;

        /// Stop the DMA running. This is done after MAC transmit & recieve are disabled.
        virtual void stopDMA() = 0;

#if __DCACHE_PRESENT
        /// Invalidate cache for the descriptor at the given index so it gets reloaded from main memory
        virtual void cacheInvalidateDescriptor(size_t descIdx) = 0;
#endif

        /// Is the given descriptor owned by DMA?
        /// Note that the descriptor will already have been invalidated in cache if needed.
        virtual bool descOwnedByDMA(size_t descIdx) = 0;

        /// Get whether the given buffer is in a memory region readable by the Ethernet DMA.
        /// If this returns false for a buffer being transmitted, the buffer will be copied into a new
        /// heap-allocated buffer.
        virtual bool isDMAReadableBuffer(uint8_t const * start, size_t size) const = 0;

        /// Give the descriptor at the given index to DMA to be transmitted with the given buffer.
        /// Note: if the descriptor needs to be flushed from CPU cache, you need to do that
        /// at the correct point in the implementation of this method!
        /// Also, if the DMA ran out of data to transmit, you may need to do a "poke"/"wake" operation
        /// to tell it to start running again.
        virtual void giveToDMA(size_t descIdx, uint8_t const * buffer, size_t len, bool firstDesc, bool lastDesc) = 0;

        // Utility function for implementing isDMAReadableBuffer().
        // 1D intersection test between a buffer and a memory bank.
        static bool bufferTouchesMemoryBank(uint8_t const * start, const size_t size, const uint32_t bankStartAddr, const uint32_t bankSize) {
            const auto startAddrInt = reinterpret_cast<uint32_t>(start);

            if(startAddrInt < bankStartAddr) {
                // Case 1: buffer begins before bank
                return (startAddrInt + size) > bankStartAddr;
            }
            else if(startAddrInt < (bankStartAddr + bankSize)) {
                // Case 2: buffer begins inside bank
                return true;
            }
            else {
                // Case 3: buffer begins after bank
                return false;
            }
        }

    public:
        CompositeEMAC::ErrCode init() override {
            // At the start, we own all the descriptors
            txDescsOwnedByApplication = TX_NUM_DESCS;

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


#if __DCACHE_PRESENT
                cacheInvalidateDescriptor(txReclaimIndex);
#endif

                if (descOwnedByDMA(txReclaimIndex)) {
                    // This desc is owned by the DMA, so we have reached the part of the ring buffer
                    // that is still being transmitted.
                    // Done for now!
                    break;
                }

                // Free any buffers associated with the descriptor
                if (descStackBuffers[txReclaimIndex] != nullptr) {
                    memory_manager->free(descStackBuffers[txReclaimIndex]);
                    descStackBuffers[txReclaimIndex] = nullptr;
                }

                tr_debug("Reclaimed Tx descriptor %zu", txReclaimIndex);

                // Update counters
                txReclaimIndex = (txReclaimIndex + 1) % TX_NUM_DESCS;
                ++txDescsOwnedByApplication;

                returnedAnyDescriptors = true;
            }

            if(returnedAnyDescriptors) {
                txDescAvailFlag.set(1);
            }

            return returnedAnyDescriptors;
        }

        CompositeEMAC::ErrCode txPacket(net_stack_mem_buf_t * buf) {

            // Whether we had to copy the packet. If we copied the packet,
            // it will be copied to one contiguous buffer and will have room for the CRC at the end (if needed)
            bool packetCopied = false;

            // Step 0: Pad to 60/64 bytes (if appending CRC) if needed
            constexpr size_t ETH_MIN_SIZE = 60; // Ethernet packet, not including CRC at end, must be padded to at least this size
            if(padFrames && memory_manager->get_total_len(buf) < ETH_MIN_SIZE) {
                const size_t sizeToPadTo = ETH_MIN_SIZE + (appendCRC ? sizeof(uint32_t) : 0);
                const size_t numPaddingBytes = sizeToPadTo - memory_manager->get_total_len(buf);

                buf = memory_manager->realloc_heap(buf, 0, sizeToPadTo);
                if(buf == nullptr)
                {
                    // No free memory, drop packet
                    return CompositeEMAC::ErrCode::OUT_OF_MEMORY;
                }
                packetCopied = true;

                // Clear to zeros
                memset(static_cast<uint8_t *>(memory_manager->get_ptr(buf)) + (sizeToPadTo - numPaddingBytes), 0, numPaddingBytes);
            }

            // Step 1: Figure out if we can send this zero-copy, or if we need to copy it.
            size_t packetDescsUsed = memory_manager->count_buffers(buf);
            size_t neededFreeDescs = packetDescsUsed + extraTxDescsToLeave;
            bool needToCopy = false;
            if(!packetCopied) {
                if(packetDescsUsed > 1 && !supportsDescChaining)
                {
                    /// Packet uses more than 1 descriptor and the hardware doesn't support that so
                    /// we have to copy it into one single descriptor.
                    needToCopy = true;
                }

                if(neededFreeDescs >= TX_NUM_DESCS)
                {
                    // Packet uses too many buffers, we have to copy it into a continuous buffer.
                    // Note: Some Eth DMAs (e.g. STM32 v2) cannot enqueue all the descs in the ring at the same time
                    // so we can't use every single descriptor to send the packet.
                    needToCopy = true;
                }

                if(!needToCopy && (neededFreeDescs > txDescsOwnedByApplication && txDescsOwnedByApplication > extraTxDescsToLeave)) {
                    // Packet uses more buffers than we have descriptors, but we can send it immediately if we copy
                    // it into a single buffer.
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
            }

            // Step 2: Copy packet if needed
            if(needToCopy)
            {
                const size_t newLen = memory_manager->get_total_len(buf) + (appendCRC ? sizeof(uint32_t) : 0);
                buf = memory_manager->realloc_heap(buf, 0, newLen);
                if(buf == nullptr)
                {
                    // No free memory, drop packet
                    return CompositeEMAC::ErrCode::OUT_OF_MEMORY;
                }
                packetCopied = true;

                packetDescsUsed = 1;
                neededFreeDescs = packetDescsUsed + extraTxDescsToLeave;
            }

            tr_debug("Transmitting packet of length %lu in %zu buffers (starting at 0x%" PRIx32 ") and %zu descs (starting at %zu)",
                memory_manager->get_total_len(buf), memory_manager->count_buffers(buf), reinterpret_cast<uint32_t>(memory_manager->get_ptr(buf)), packetDescsUsed, txSendIndex);

            // Step 3: Calculate CRC and add to packet if possible.
            // If we did not copy the packet, we are not allowed to modify the buffer sent from the network stack
            // (as it may be e.g. kept by the stack for TCP retransmission later) so we have to tack on the CRC
            // via another descriptor.
            uint32_t crc;
            net_stack_mem_buf_t * crcBuf = nullptr; // CRC is saved here if it needs to be appended later
            if(appendCRC) {
                if(packetCopied) {
                    // Compute the CRC. Easy since we have a contiguous buffer, we just need to not do it over
                    // the last 4 bytes.
                    crcGenerator.compute(memory_manager->get_ptr(buf), memory_manager->get_len(buf) - sizeof(uint32_t), &crc);

                    // Just memcpy the little-endian CRC onto the end of the message.
                    // Per here, that should give correct results:
                    // https://stackoverflow.com/a/65108067/7083698
                    memcpy(static_cast<uint8_t *>(memory_manager->get_ptr(buf)) + memory_manager->get_len(buf) - sizeof(uint32_t),
                        &crc, sizeof(uint32_t));
                    tr_debug("Appending calculated Ethernet CRC 0x%08" PRIx32 " into copied buffer.", crc);
                }
                else {
                    // Iterate over all data in the packet and compute the CRC
                    net_stack_mem_buf_t * bufBeingChecksummed = buf;

                    crcGenerator.compute_partial_start(&crc);
                    do {
                        crcGenerator.compute_partial(memory_manager->get_ptr(bufBeingChecksummed), memory_manager->get_len(bufBeingChecksummed), &crc);
                        bufBeingChecksummed = memory_manager->get_next(bufBeingChecksummed);
                    }
                    while(bufBeingChecksummed != nullptr);
                    crcGenerator.compute_partial_stop(&crc);

                    // Allocate a new buffer at the end for the CRC.
                    // We need to do this now because we need to be able to bail if the allocation fails,
                    // and we can't do that if we already enqueued the rest of the packet
                    crcBuf = memory_manager->alloc_heap(sizeof(uint32_t), 0);
                    if(crcBuf == nullptr)
                    {
                        // No free memory, drop packet
                        memory_manager->free(buf);
                        return CompositeEMAC::ErrCode::OUT_OF_MEMORY;
                    }
                    memcpy(memory_manager->get_ptr(crcBuf), &crc, sizeof(uint32_t));

                    // We will need one more descriptor
                    ++neededFreeDescs;
                }
            }


            // Step 4: Wait for needed amount of buffers to be available.
            // Note that, in my experience, it's better to block here, as dropping the packet
            // due to not having enough buffers can create weird effects when the application sends
            // lots of packets at once.
            while(txDescsOwnedByApplication < neededFreeDescs)
            {
                txDescAvailFlag.wait_any_for(1, rtos::Kernel::wait_for_u32_forever);
            }

            // Step 5: Load buffer into descriptors and send
            net_stack_mem_buf_t * currBuf = buf;
            for(size_t descCount = 0; descCount < packetDescsUsed; descCount++)
            {
#if __DCACHE_PRESENT
                // Write buffer back to main memory
                SCB_CleanDCache_by_Addr(memory_manager->get_ptr(currBuf), memory_manager->get_len(currBuf));
#endif

                // Get next buffer
                const auto nextBuf = memory_manager->get_next(currBuf);
                if(nextBuf == nullptr)
                {
                    // Last descriptor, store head buffer address for freeing
                    descStackBuffers[txSendIndex] = buf;
                }
                else
                {
                    descStackBuffers[txSendIndex] = nullptr;
                }

                // Get the pointer and length of the packet because this might not be doable in a critical section
                const auto bufferPtr = static_cast<uint8_t *>(memory_manager->get_ptr(currBuf));
                const auto bufferLen = memory_manager->get_len(currBuf);

                // Enter a critical section, because we could run into weird corner cases if the
                // interrupt executes while we are half done configuring this descriptor and updating
                // the counters.
                core_util_critical_section_enter();

                // Give the packet to DMA.
                // If we are in appendCRC mode AND the packet was not copied, suppress the last desc flag
                // since we need to enqueue one more later.
                const bool isLast = nextBuf == nullptr && !(appendCRC && !packetCopied);
                giveToDMA(txSendIndex, bufferPtr, bufferLen, descCount == 0, isLast);

                // Update descriptor count and index
                --txDescsOwnedByApplication;
                txSendIndex = (txSendIndex + 1) % TX_NUM_DESCS;

                core_util_critical_section_exit();

                // Move to next buffer
                currBuf = nextBuf;
            }

            // Step 6: Append CRC descriptor (if needed)
            // We do this by appending one more descriptor to the end of the packet in the Tx ring containing
            // the CRC. Logic here works the same as above.
            if(appendCRC && !packetCopied) {
                descStackBuffers[txSendIndex] = crcBuf;
                const auto bufferPtr = static_cast<uint8_t *>(memory_manager->get_ptr(crcBuf));

                tr_debug("Appending calculated Ethernet CRC 0x%08" PRIx32 " via Tx descriptor %zu", crc, txSendIndex);

                core_util_critical_section_enter();
                giveToDMA(txSendIndex, bufferPtr, sizeof(uint32_t), false, true);
                --txDescsOwnedByApplication;
                txSendIndex = (txSendIndex + 1) % TX_NUM_DESCS;
                core_util_critical_section_exit();
            }

            return CompositeEMAC::ErrCode::SUCCESS;
        }

    };

    /**
     * @brief Generic receive DMA loop
     *
     * This implementation of Rx DMA should work for the large majority of embedded MCUs that use a DMA ring-based
     * ethernet MAC.
     *
     * The subclass must allocate the DMA descriptors, and all access to them is done through virtual functions
     * that the subclass must override.
     */
    class GenericRxDMARing : public CompositeEMAC::RxDMA {
    public:
        /// How many extra buffers to leave in the Rx pool, relative to how many we keep assigned to Rx descriptors.
        /// We want to keep some amount of extra buffers because constantly hitting the network stack with failed pool
        /// allocations can produce some negative consequences in some cases.
        static constexpr size_t RX_POOL_EXTRA_BUFFERS = 3;

        /// Number of entries in the Rx descriptor ring
        /// Note: + 1 because for some EMACs (e.g. STM32 v2) we have to always keep one descriptor owned by the application
        // TODO: When we add multiple Ethernet support, this calculation may need to be changed, because the pool buffers will be split between multiple EMACs
        static constexpr size_t RX_NUM_DESCS = MBED_CONF_NSAPI_EMAC_RX_POOL_NUM_BUFS - RX_POOL_EXTRA_BUFFERS + 1;

        // Alignment required for Rx memory buffers.  Normally they don't need more than word alignment but
        // if we are doing cache operations they need to be cache aligned.
#if __DCACHE_PRESENT
        static constexpr size_t RX_BUFFER_ALIGN = __SCB_DCACHE_LINE_SIZE;
#else
        static constexpr size_t RX_BUFFER_ALIGN = sizeof(uint32_t);
#endif
    protected:
        /// Whether the hardware has a first descriptor flag. If this is false, isFirstDesc() will always
        /// return false. This reduces our ability to detect bad data in the DMA buffer.
        const bool supportsFirstDescFlag;

        /// Whether it is possible to return all Ethernet Rx descriptors to the MAC at the same time.
        /// Some DMA arrangements do not support this.
        const bool canReturnAllDescriptors;

        /// This many bytes at the end of the packet will be discarded. This is used for EMACs which append
        /// extra data to the end of the received Ethernet frame, such as the CRC.
        const size_t packetEndDiscardSize;

        /// Pointer to the network stack buffer associated with the corresponding Rx descriptor.
        net_stack_mem_buf_t * rxDescStackBufs[RX_NUM_DESCS];

        // Indexes for descriptor rings.
        size_t rxBuildIndex; ///< Index of the next Rx descriptor that needs to be built.  Updated by application and used by ISR.
        size_t rxDescsOwnedByApplication; ///< Number of Rx descriptors owned by the application and needing buffers allocated.
        std::atomic<size_t> rxNextIndex; ///< Index of the next descriptor that the DMA will populate.  Updated by application but used by ISR.

    protected:
        /// Payload size of buffers allocated from the Rx pool.  This is the allocation unit size
        /// of the pool minus any overhead needed for alignment.
        size_t rxPoolPayloadSize;

        /// Constructor. Subclass must allocate descriptor array of size RX_NUM_DESCS
        GenericRxDMARing(const bool supportsFirstDescFlag = true, const bool canReturnAllDescriptors = false, const size_t packetEndDiscardSize = 0):
        supportsFirstDescFlag(supportsFirstDescFlag),
        canReturnAllDescriptors(canReturnAllDescriptors),
        packetEndDiscardSize(packetEndDiscardSize)
        {}

        /// Configure DMA registers to point to the DMA ring,
        /// and enable DMA. This is done before the MAC itself is enabled, and before any descriptors
        /// are given to DMA.
        virtual void startDMA() = 0;

        /// Stop the DMA running. This is done after MAC transmit & receive are disabled.
        virtual void stopDMA() = 0;

#if __DCACHE_PRESENT
        /// Invalidate cache for the descriptor at the given index so it gets reloaded from main memory
        virtual void cacheInvalidateDescriptor(size_t descIdx) = 0;
#endif

        /// Is the given descriptor owned by DMA?
        /// Note that the descriptor will already have been invalidated in cache if needed.
        virtual bool descOwnedByDMA(size_t descIdx) = 0;

        /// Does the given descriptor contain the start of a packet?
        /// Note that the descriptor will already have been invalidated in cache if needed.
        virtual bool isFirstDesc(size_t descIdx) = 0;

        /// Does the given descriptor contain the end of a packet?
        /// Note that the descriptor will already have been invalidated in cache if needed.
        virtual bool isLastDesc(size_t descIdx) = 0;

        /// Is the given descriptor an error descriptor?
        /// Note that the descriptor will already have been invalidated in cache if needed.
        virtual bool isErrorDesc(size_t descIdx) = 0;

        /// Return a descriptor to DMA so that DMA can receive into it.
        /// Is passed the buffer address (fixed size equal to rxPoolPayloadSize) to attach to this descriptor.
        /// Note: if the descriptor needs to be flushed from CPU cache, you need to do that
        /// at the correct point in the implementation of this method!
        /// Also, if the DMA ran out of data to transmit, you may need to do a "poke"/"wake" operation
        /// to tell it to start running again.
        virtual void returnDescriptor(size_t descIdx, uint8_t * buffer) = 0;

        /// Get the length of the packet starting at firstDescIdx and continuing until
        /// lastDescIdx (which might or might not be the same as firstDescIdx). Descriptors have already been
        /// validated to contain a complete packet at this point.
        virtual size_t getTotalLen(size_t firstDescIdx, size_t lastDescIdx) = 0;

    public:
        CompositeEMAC::ErrCode init() override {
            rxPoolPayloadSize = memory_manager->get_pool_alloc_unit(RX_BUFFER_ALIGN);
            rxBuildIndex = 0;
            rxNextIndex = 0;

            // At the start, we own all the descriptors
            rxDescsOwnedByApplication = RX_NUM_DESCS;

            // init DMA peripheral
            startDMA();

            // Build all descriptors
            rebuildDescriptors();

            return CompositeEMAC::ErrCode::SUCCESS;
        }

        CompositeEMAC::ErrCode deinit() override {
            stopDMA();

            // Deallocate buffers associated with all descriptors
            for(size_t descIdx = 0; descIdx < RX_NUM_DESCS; ++descIdx) {
                if(rxDescStackBufs[descIdx] != nullptr) {
                    memory_manager->free(rxDescStackBufs[descIdx]);
                }
            }

            return CompositeEMAC::ErrCode::SUCCESS;
        }

        void rebuildDescriptors() override {
            const size_t origRxDescsOwnedByApplication [[maybe_unused]] = rxDescsOwnedByApplication;

            // Note: With some Ethernet peripherals, you can never give back every single descriptor to
            // the hardware, because then it thinks there are 0 descriptors left.
            while (rxDescsOwnedByApplication > (canReturnAllDescriptors ? 0 : 1)) {
                // Allocate new buffer
                auto *const buffer = memory_manager->alloc_pool(rxPoolPayloadSize, RX_BUFFER_ALIGN);
                if (buffer == nullptr) {
                    // No memory, cannot return any more descriptors.
                    return;
                }

                // Store buffer address
                rxDescStackBufs[rxBuildIndex] = buffer;

                // Send descriptor to DMA
                returnDescriptor(rxBuildIndex, static_cast<uint8_t *>(memory_manager->get_ptr(buffer)));

                // Move to next descriptor
                --rxDescsOwnedByApplication;
                rxBuildIndex = (rxBuildIndex + 1) % RX_NUM_DESCS;
            }

            tr_debug("buildRxDescriptors(): Returned %zu descriptors.", origRxDescsOwnedByApplication - rxDescsOwnedByApplication);
        }

        bool rxHasPackets_ISR() override {
            // First, we need to check if at least one DMA descriptor that is owned by the application
            // has its last descriptor flag or error flag set, indicating we have received at least one complete packet
            // or there is an error descriptor that can be reclaimed by the application.
            // Note that we want to bias towards false positives here, because false positives just waste CPU time,
            // while false negatives would cause packets to be missed.
            // So, for simplicity, we just check every descriptor currently owned by the application until we
            // find one with the FS bit set or the error bits set.
            // This could potentially produce a false positive if we do this in the middle of receiving
            // an existing packet, but that is unlikely and will not cause anything bad to happen if it does.

            bool seenFirstDesc = false;

            for(size_t descCount = 0; descCount < RX_NUM_DESCS; descCount++)
            {
                size_t descIdx = (rxNextIndex + descCount) % RX_NUM_DESCS;

#if __DCACHE_PRESENT
                cacheInvalidateDescriptor(descIdx);
#endif

                if(descOwnedByDMA(descIdx))
                {
                    // Descriptor owned by DMA.  We are out of descriptors to process.
                    return false;
                }

                if(supportsFirstDescFlag && isFirstDesc(descIdx))
                {
                    if(seenFirstDesc)
                    {
                        // First desc seen after another first desc.
                        // Some MACs do this if they run out of Rx descs when halfway through a packet.
                        // dequeuePacket() can clean this up and reclaim the partial packet desc(s).
                        return true;
                    }
                    else
                    {
                        seenFirstDesc = true;
                    }
                }
                if(isErrorDesc(descIdx) || isLastDesc(descIdx))
                {
                    // Reclaimable descriptor or complete packet detected.
                    return true;
                }
            }

            // Processed all descriptors.
            return false;
        }

    private:

        /// Helper function: Discard received Rx descriptors from a given start index (inclusive) to stop index (exclusive)
        void discardRxDescs(size_t startIdx, size_t stopIdx)
        {
            for(size_t descToCleanIdx = startIdx; descToCleanIdx != stopIdx; descToCleanIdx = (descToCleanIdx + 1) % RX_NUM_DESCS) {
                // Free Rx buffer attached to this desc
                memory_manager->free(rxDescStackBufs[descToCleanIdx]);
                rxDescStackBufs[descToCleanIdx] = nullptr;

                // Allow desc to be rebuilt
                ++rxDescsOwnedByApplication;
                ++rxNextIndex;
            }
        }

    public:

        net_stack_mem_buf_t * dequeuePacket() override {
            // Indices of the first and last descriptors for the packet will be saved here
            std::optional<size_t> firstDescIdx, lastDescIdx;

            // Prevent looping around into descriptors waiting for rebuild by limiting how many
            // we can process.
            const size_t maxDescsToProcess = RX_NUM_DESCS - rxDescsOwnedByApplication;

            const size_t startIdx = rxNextIndex;

            for (size_t descCount = 0; descCount < maxDescsToProcess && !lastDescIdx.has_value(); descCount++) {
                size_t descIdx = (startIdx + descCount) % RX_NUM_DESCS;

#if __DCACHE_PRESENT
                cacheInvalidateDescriptor(descIdx);
#endif

                if (descOwnedByDMA(descIdx)) {
                    // Descriptor owned by DMA and has not been filled in yet.  We are out of descriptors to process.
                    break;
                }

                const bool isError = isErrorDesc(descIdx);
                const bool isFirst = isFirstDesc(descIdx);
                const bool isLast = isLastDesc(descIdx);

                if(isError) {
                    if (!firstDescIdx.has_value()) {
                        // Error before a first descriptor.
                        // (could be caused by incomplete packets/junk in the DMA buffer).
                        // Ignore, free associated memory, and schedule for rebuild.
                        discardRxDescs(descIdx, (descIdx + 1) % RX_NUM_DESCS);
                        tr_debug("Rx descriptor %zu has error, discarding...\n", descIdx);
                        continue;
                    }
                    else {
                        // Already seen a first descriptor, but we have an error descriptor.
                        // So, delete the in-progress packet up to this point.
                        discardRxDescs(*firstDescIdx, (descIdx + 1) % RX_NUM_DESCS);
                        tr_debug("Rx descriptor %zu has error, discarding packet starting at index %zu...\n", descIdx, *firstDescIdx);
                        firstDescIdx.reset();
                        continue;
                    }
                }

                if (supportsFirstDescFlag) {
                    if (!firstDescIdx.has_value() && !isFirst) {
                        // Non-first-descriptor before a first descriptor
                        // (could be caused by incomplete packets/junk in the DMA buffer).
                        // Ignore, free associated memory, and schedule for rebuild.
                        discardRxDescs(descIdx, (descIdx + 1) % RX_NUM_DESCS);
                        continue;
                    }
                    else if(firstDescIdx.has_value() && isFirst)
                    {
                        // Already seen a first descriptor, but we have another first descriptor.
                        // Some MACs do this if they run out of Rx descs when halfway through a packet.
                        // Delete the in-progress packet up to this point and start over from descIdx.
                        discardRxDescs(*firstDescIdx, descIdx);
                        firstDescIdx = descIdx;
                    }
                    else if(isFirst)
                    {
                        // Normal first descriptor.
                        firstDescIdx = descIdx;
                    }
                }
                else {
                    // Without more information, assume the first desc we see is the first descriptor
                    if(!firstDescIdx.has_value()) {
                        firstDescIdx = descIdx;
                    }
                }

                if(isLast) {
                    lastDescIdx = descIdx;
                }
            }

            if (!firstDescIdx.has_value() || !lastDescIdx.has_value()) {
                // No complete packet identified.
                // Take the chance to rebuild any available descriptors, then return.
                rebuildDescriptors();
                if(!firstDescIdx.has_value()) {
                    tr_debug("No packet at all seen in Rx descs\n");
                }
                else {
                    tr_debug("No last descriptor was seen for packet beginning at descriptor %zu\n", *firstDescIdx);
                }

                return nullptr;
            }

            // We will receive next into the descriptor after this one.
            // Update this now to tell the ISR to search for descriptors after lastDescIdx only.
            rxNextIndex = (*lastDescIdx + 1) % RX_NUM_DESCS;

            // Calculate packet length
            size_t pktLen = getTotalLen(*firstDescIdx, *lastDescIdx);
            MBED_ASSERT(pktLen > packetEndDiscardSize);
            pktLen -= packetEndDiscardSize;

            // Set length of first buffer
            net_stack_mem_buf_t *const headBuffer = rxDescStackBufs[*firstDescIdx];
            memory_manager->set_len(headBuffer, std::min(pktLen, rxPoolPayloadSize));
            size_t lenRemaining = pktLen - std::min(pktLen, rxPoolPayloadSize);

            // Iterate through the subsequent descriptors in this packet and link the buffers
            // Note that this also transfers ownership of subsequent buffers to the first buffer,
            // so if the first buffer is deleted, the others will be as well.

            ++rxDescsOwnedByApplication; // for first buffer
            rxDescStackBufs[*firstDescIdx] = nullptr;
            for (size_t descIdx = (*firstDescIdx + 1) % RX_NUM_DESCS;
                 descIdx != (*lastDescIdx + 1) % RX_NUM_DESCS;
                 descIdx = (descIdx + 1) % RX_NUM_DESCS) {

                if(lenRemaining == 0) {
                    // No real data bytes left to keep. This can happen because of the end discard size, i.e.
                    // if there is a final descriptor containing only the last few bytes we want to discard.
                    // In this case just free the buffer.
                    memory_manager->free(rxDescStackBufs[descIdx]);
                }
                else {
                    // Otherwise, append this buffer to the chain
                    memory_manager->set_len(rxDescStackBufs[descIdx], std::min(lenRemaining, rxPoolPayloadSize));
                    lenRemaining -= std::min(lenRemaining, rxPoolPayloadSize);

                    memory_manager->cat(headBuffer, rxDescStackBufs[descIdx]);
                }

                rxDescStackBufs[descIdx] = nullptr;
                ++rxDescsOwnedByApplication;
            }

            // Invalidate cache for all data buffers, as these were written by the DMA to main memory
#if __DCACHE_PRESENT
            auto * bufToInvalidate = headBuffer;
            while(bufToInvalidate != nullptr)
            {
                SCB_InvalidateDCache_by_Addr(memory_manager->get_ptr(bufToInvalidate), rxPoolPayloadSize);
                bufToInvalidate = memory_manager->get_next(bufToInvalidate);
            }
#endif

            tr_debug("Returning packet of length %lu, start %p from Rx descriptors %zu-%zu\n",
                   memory_manager->get_total_len(headBuffer), memory_manager->get_ptr(headBuffer), *firstDescIdx, *lastDescIdx);

            return headBuffer;
        }
    };
}

#undef TRACE_GROUP

#endif //MBED_OS_GENERICETHDMA_H
