/* Copyright (c) 2024 Jamie Smith
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

#include "STM32EthIPv2DMARings.h"

#include "ThisThread.h"
#include "mbed_trace.h"

#define TRACE_GROUP  "CEMAC"

namespace mbed {

// Thread flag constants
static uint32_t THREAD_FLAG_TX_DESC_AVAILABLE = 1;
static uint32_t THREAD_FLAG_RX_DESC_AVAILABLE = 2;
static uint32_t THREAD_FLAG_SHUTDOWN = 4;

// Event flag constants
static uint32_t EVENT_FLAG_TX_DESC_AVAILABLE = 1;

void STM32EthIPv2DMARings::buildRxDescriptors() {

    // Note: With this Ethernet peripheral, you can never give back every single descriptor to
    // the hardware, because then it thinks there are 0 descriptors left.
    while (rxDescsOwnedByApplication > 1) {
        auto &currRxDesc = rxDescs[rxBuildIndex];

        // Allocate new buffer
        auto *const buffer = memory_manager.alloc_pool(rxPoolPayloadSize, RX_BUFFER_ALIGN);
        if (buffer == nullptr) {
            // No memory, cannot return any more descriptors.
            return;
        }

        // Clear out any bits previously set in the descriptor
        memset(&currRxDesc.rxDesc.toDMAFmt, 0, sizeof(EthRxDescriptor));

        // Store buffer address
        currRxDesc.buffer = buffer;
        currRxDesc.rxDesc.toDMAFmt.buffer1Addr = memory_manager.get_ptr(buffer);

        // Configure descriptor
        currRxDesc.rxDesc.toDMAFmt.buffer1Valid = true;
        currRxDesc.rxDesc.toDMAFmt.buffer2Valid = false;
        currRxDesc.rxDesc.toDMAFmt.intrOnCompletion = true;
        currRxDesc.rxDesc.toDMAFmt.dmaOwn = true;

#if __DCACHE_PRESENT
        // Flush to main memory
        SCB_CleanDCache_by_Addr(&currRxDesc, __SCB_DCACHE_LINE_SIZE);
#endif

        // Move to next descriptor
        --rxDescsOwnedByApplication;
        rxBuildIndex = (rxBuildIndex + 1) % rxPoolSize;

        // Update tail ptr to issue "rx poll demand" and mark this descriptor for receive.
        // Rx stops when the current and tail pointers are equal, so we want to set the tail pointer
        // to one location after the last DMA-owned descriptor in the FIFO.
        heth.Instance->DMACRDTPR = reinterpret_cast<uint32_t>(&rxDescs[rxBuildIndex]);
    }
}

// TODO monitor for Rx buffer exhaustion conditions.  This happens when we have too few
// built Rx buffers and won't be able to receive another packet.  Example: if the size of
// one buffer is 512 bytes, then we need at least 3 built buffers to be able to receive
// the next packet if it's 1 MTU.
// If we are in a potential buffer exhaustion situation, we need to poll at a reasonably
// high rate (10ms) until the application has freed up at least some of the pool buffers.
// NOTE: In LwIP, the failed allocation of a buffer from the pool will trigger a cleanup
// of the TCP reassembly buffer, potentially freeing some memory in the near future.

net_stack_mem_buf_t *STM32EthIPv2DMARings::rxNextPacket() {
    // Indices of the first and last descriptors for the packet will be saved here
    size_t firstDescIdx = rxPoolSize;
    size_t lastDescIdx = rxPoolSize;

    // Prevent looping around into descriptors waiting for rebuild by limiting how many
    // we can process.
    const size_t maxDescsToProcess = rxPoolSize - rxDescsOwnedByApplication;

    const size_t startIdx = rxNextIndex;

    for (size_t descCount = 0; descCount < maxDescsToProcess && lastDescIdx == rxPoolSize; descCount++) {
        size_t descIdx = (startIdx + descCount) % rxPoolSize;
        auto &descriptor = rxDescs[descIdx];

#if __DCACHE_PRESENT
        SCB_InvalidateDCache_by_Addr(&descriptor, sizeof(EthRxDescriptor));
#endif

        if (descriptor.rxDesc.fromDMAFmt.dmaOwn) {
            // Descriptor owned by DMA and has not been filled in yet.  We are out of descriptors to process.
            break;
        }

        if (descriptor.rxDesc.fromDMAFmt.context || descriptor.rxDesc.fromDMAFmt.errorSummary ||
            (!descriptor.rxDesc.fromDMAFmt.firstDescriptor && firstDescIdx == rxPoolSize)) {
            // Context or error descriptor, or a non-first-descriptor before a first descriptor
            // (could be caused by incomplete packets/junk in the DMA buffer).
            // Ignore, free associated memory, and schedule for rebuild.
            memory_manager.free(descriptor.buffer);
            ++rxDescsOwnedByApplication;
            ++rxNextIndex;

            // We should only get one of these error descriptors before the start of the packet, not
            // during it.
            MBED_ASSERT(firstDescIdx == rxPoolSize);

            continue;
        }

        if (descriptor.rxDesc.fromDMAFmt.firstDescriptor) {
            // We should see first descriptor only once and before last descriptor. If this rule is violated, it's
            // because we ran out of descriptors during receive earlier and the MAC tossed out the rest of the packet.
            if(firstDescIdx != rxPoolSize) {
                // Clean up the old first descriptor
                auto & oldFirstDesc = rxDescs[firstDescIdx];
                memory_manager.free(oldFirstDesc.buffer);
                ++rxDescsOwnedByApplication;
                ++rxNextIndex;
            }

            firstDescIdx = descIdx;
        }

        if (descriptor.rxDesc.fromDMAFmt.lastDescriptor) {
            lastDescIdx = descIdx;
        }
    }

    if (lastDescIdx == rxPoolSize) {
        // No complete packet identified.
        // Take the chance to rebuild any available descriptors, then return.
        tr_debug("No complete packets in Rx descs\n");
        buildRxDescriptors();
        return nullptr;
    }

    // We will receive next into the descriptor after this one.
    // Update this now to tell the ISR to search for descriptors after lastDescIdx only.
    rxNextIndex = (lastDescIdx + 1) % rxPoolSize;

    // Set length of first buffer
    net_stack_mem_buf_t *const headBuffer = rxDescs[firstDescIdx].buffer;
    size_t lenRemaining = rxDescs[lastDescIdx].rxDesc.fromDMAFmt.pktLength;
    memory_manager.set_len(headBuffer, std::min(lenRemaining, rxPoolPayloadSize));
    lenRemaining -= std::min(lenRemaining, rxPoolPayloadSize);

    // Iterate through the subsequent descriptors in this packet and link the buffers
    // Note that this also transfers ownership of subsequent buffers to the first buffer,
    // so if the first buffer is deleted, the others will be as well.
    ++rxDescsOwnedByApplication; // for first buffer
    rxDescs[firstDescIdx].buffer = nullptr;
    for (size_t descIdx = (firstDescIdx + 1) % rxPoolSize;
         descIdx != (lastDescIdx + 1) % rxPoolSize;
         descIdx = (descIdx + 1) % rxPoolSize) {

        // We have to set the buffer length first before concatenating it to the chain
        memory_manager.set_len(rxDescs[descIdx].buffer, std::min(lenRemaining, rxPoolPayloadSize));
        lenRemaining -= std::min(lenRemaining, rxPoolPayloadSize);

        memory_manager.cat(headBuffer, rxDescs[descIdx].buffer);
        rxDescs[descIdx].buffer = nullptr;
        ++rxDescsOwnedByApplication;
    }

    // Invalidate cache for all data buffers, as these were written by the DMA to main memory
#if __DCACHE_PRESENT
    auto * bufToInvalidate = headBuffer;
    while(bufToInvalidate != nullptr)
    {
        SCB_InvalidateDCache_by_Addr(memory_manager.get_ptr(bufToInvalidate), rxPoolPayloadSize);
        bufToInvalidate = memory_manager.get_next(bufToInvalidate);
    }
#endif

    tr_info("Returning packet of length %lu, start %p from Rx descriptors %zu-%zu (%p-%p)\n",
           memory_manager.get_total_len(headBuffer), memory_manager.get_ptr(headBuffer), firstDescIdx, lastDescIdx,
           &rxDescs[firstDescIdx], &rxDescs[lastDescIdx]);

    // Rebuild descriptors if possible
    buildRxDescriptors();

    return headBuffer;
}

void STM32EthIPv2DMARings::reclaimTxDescs() {
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
        SCB_InvalidateDCache_by_Addr(&currDesc, sizeof(EthTxDescriptor));
#endif

        if (currDesc.txDesc.fromDMAFmt.dmaOwn) {
            // This desc is owned by the DMA, so we have reached the part of the ring buffer
            // that is still being transmitted.
            // Done for now!
            break;
        }

        // Free any buffers associated with the descriptor
        if (currDesc.packetFirstBuf != nullptr) {
            memory_manager.free(currDesc.packetFirstBuf);
        }

        // Update counters
        txReclaimIndex = (txReclaimIndex + 1) % MBED_CONF_STM32_EMAC_ETH_TXBUFNB;
        txDescsOwnedByApplication++;

        returnedAnyDescriptors = true;
    }

    if (returnedAnyDescriptors) {
        eventFlags.set(EVENT_FLAG_TX_DESC_AVAILABLE);
    }
}

void STM32EthIPv2DMARings::macThread()
{
    while(true)
    {
        // Wait for something to happen
        uint32_t flags = rtos::ThisThread::flags_wait_any(THREAD_FLAG_TX_DESC_AVAILABLE | THREAD_FLAG_SHUTDOWN | THREAD_FLAG_RX_DESC_AVAILABLE);
        if(flags & THREAD_FLAG_SHUTDOWN)
        {
            return;
        }
        if(flags & (THREAD_FLAG_RX_DESC_AVAILABLE | THREAD_FLAG_TX_DESC_AVAILABLE)) // TODO temp
        {
            // Receive any available packets.
            // Note that if the ISR was delayed, we might get multiple packets per ISR, so we need to loop.
            while(true)
            {
                auto * packet = rxNextPacket();
                if(!packet) {
                    break;
                }

                if(emac_link_input_cb)
                {
                    emac_link_input_cb(packet);
                }
                else
                {
                    memory_manager.free(packet);
                }
            }
        }
        if(flags & THREAD_FLAG_TX_DESC_AVAILABLE)
        {
            reclaimTxDescs();
        }
    }
}


STM32EthIPv2DMARings::STM32EthIPv2DMARings(EMACMemoryManager &memory_manager, ETH_HandleTypeDef & heth, EMAC::emac_link_input_cb_t emac_link_input_cb):
        memory_manager(memory_manager),
        heth(heth),
        emac_link_input_cb(emac_link_input_cb),
        thread(osPriorityHigh, MBED_CONF_STM32_EMAC_THREAD_STACKSIZE, nullptr, "stm32_emac_rx_thread"),
        rxPoolSize(memory_manager.get_pool_size() - RX_POOL_EXTRA_BUFFERS + 1), // + 1 because we have to always keep one descriptor owned by the application
        rxDescs(rxPoolSize),
        rxPoolPayloadSize(memory_manager.get_pool_alloc_unit(RX_BUFFER_ALIGN))
{
    // Make sure we have enough space in the pool for RX_POOL_EXTRA_BUFFERS plus a minimum of two descriptors
    MBED_ASSERT(memory_manager.get_pool_size() >= RX_POOL_EXTRA_BUFFERS + 2);
}

STM32EthIPv2DMARings::~STM32EthIPv2DMARings()
{
    if(thread.get_state() != rtos::Thread::Deleted)
    {
        thread.flags_set(THREAD_FLAG_SHUTDOWN);
        thread.join();
    }
}


HAL_StatusTypeDef STM32EthIPv2DMARings::startDMA()
{
    if (heth.gState == HAL_ETH_STATE_READY)
    {
        heth.gState = HAL_ETH_STATE_BUSY;

        // At the start, we own all the descriptors
        rxDescsOwnedByApplication = rxPoolSize;
        txDescsOwnedByApplication = MBED_CONF_STM32_EMAC_ETH_TXBUFNB;

        // Flush Tx queue
        heth.Instance->MTLTQOMR |= ETH_MTLTQOMR_FTQ;

        // Configure Rx buffer size.  Per the datasheet and HAL code, we need to round this down to
        // the nearest multiple of 4.
        size_t rxBufferSize = memory_manager.get_pool_alloc_unit(RX_BUFFER_ALIGN);
        rxBufferSize = (rxBufferSize / sizeof(uint32_t)) * sizeof(uint32_t);
        heth.Instance->DMACRCR |= rxBufferSize << ETH_DMACRCR_RBSZ_Pos;

        // Configure spacing between descriptors.  This will be different depending on
        // cache line sizes.
        // NOTE: Cast pointers to uint8_t so that the difference will be returned in bytes instead
        // of elements.
        const size_t rxSpacing = reinterpret_cast<uint8_t const *>(&rxDescs[1]) - reinterpret_cast<uint8_t const *>(&rxDescs[0]);

        // Check that spacing seems valid
#ifndef NDEBUG
        const size_t txSpacing = reinterpret_cast<uint8_t const *>(&txDescs[1]) - reinterpret_cast<uint8_t const *>(&txDescs[0]);
        MBED_ASSERT(rxSpacing == txSpacing);
        MBED_ASSERT(rxSpacing % sizeof(uint32_t) == 0);
#endif

        // The spacing bitfield is configured as the number of 32-bit words to skip between descriptors.
        // The descriptors have a default size of 16 bytes.
        const size_t wordsToSkip = (rxSpacing - 16) / sizeof(uint32_t);
        MBED_ASSERT(wordsToSkip <= 7);
        heth.Instance->DMACCR &= ~ETH_DMACCR_DSL_Msk;
        heth.Instance->DMACCR |= wordsToSkip << ETH_DMACCR_DSL_Pos;

        // Configure Rx descriptor ring
        heth.Instance->DMACRDRLR = rxPoolSize - 1; // Ring size
        heth.Instance->DMACRDLAR = reinterpret_cast<uint32_t>(&rxDescs[0]); // Ring base address
        heth.Instance->DMACRDTPR = reinterpret_cast<uint32_t>(&rxDescs[0]); // Next descriptor (tail) pointer

        buildRxDescriptors();

        // Configure Tx descriptor ring
        heth.Instance->DMACTDRLR = MBED_CONF_STM32_EMAC_ETH_TXBUFNB - 1; // Ring size
        heth.Instance->DMACTDLAR = reinterpret_cast<uint32_t>(&txDescs[0]); // Ring base address
        heth.Instance->DMACTDTPR = reinterpret_cast<uint32_t>(&txDescs[0]); // Next descriptor (tail) pointer

        // Enable Rx and Tx DMA.  NOTE: Typo in C++ headers, these should be called
        // "DMACTXCR" and "DMACRXCR"
        heth.Instance->DMACTCR |= ETH_DMACTCR_ST;
        heth.Instance->DMACRCR |= ETH_DMACRCR_SR;

        // Clear Tx and Rx process stopped flags
        heth.Instance->DMACSR = (ETH_DMACSR_TPS | ETH_DMACSR_RPS);

        // Start Rx thread
        thread.start(mbed::callback(this, &STM32EthIPv2DMARings::macThread));

        heth.gState = HAL_ETH_STATE_STARTED;

        return HAL_OK;
    }
    else
    {
        return HAL_ERROR;
    }
}

void STM32EthIPv2DMARings::rxISR()
{
    // First, we need to check if at least one DMA descriptor that is owned by the application
    // has its last descriptor flag or error flag set, indicating we have received at least one complete packet
    // or there is an error descriptor that can be reclaimed by the application.
    // Note that we want to bias towards false positives here, because false positives just waste CPU time,
    // while false negatives would cause packets to be dropped.
    // So, for simplicity, we just check every descriptor currently owned by the application until we
    // find one with the FS bit set or the error bits set.
    // This could potentially produce a false positive if we do this in the middle of receiving
    // an existing packet, but that is unlikely and will not cause anything bad to happen if it does.

    for(size_t descCount = 0; descCount < rxPoolSize; descCount++)
    {
        auto &descriptor = rxDescs[rxNextIndex];

#if __DCACHE_PRESENT
        SCB_InvalidateDCache_by_Addr(&descriptor, sizeof(ETH_DMADescTypeDef));
#endif

        if (descriptor.rxDesc.fromDMAFmt.dmaOwn)
        {
            // Descriptor owned by DMA.  We are out of descriptors to process.
            return;
        }
        if (descriptor.rxDesc.fromDMAFmt.context || descriptor.rxDesc.fromDMAFmt.errorSummary || descriptor.rxDesc.fromDMAFmt.lastDescriptor)
        {
            // Reclaimable descriptor or complete packet detected.
            thread.flags_set(THREAD_FLAG_RX_DESC_AVAILABLE);
            return;
        }
    }
}

void STM32EthIPv2DMARings::txISR()
{
    thread.flags_set(THREAD_FLAG_TX_DESC_AVAILABLE);
}

HAL_StatusTypeDef STM32EthIPv2DMARings::txPacket(net_stack_mem_buf_t * buf)
{
    // Step 1: Figure out if we can send this zero-copy, or if we need to copy it.
    // Also note that each descriptor can store 2 buffers, so we need half as many descriptors
    // as we have buffers, rounding up.
    size_t neededDescs = (memory_manager.count_buffers(buf) + 1) / 2;
    bool needToCopy = false;
    if(neededDescs > MBED_CONF_STM32_EMAC_ETH_TXBUFNB)
    {
        // Packet uses too many buffers, we have to copy it into a continuous buffer.
        needToCopy = true;
    }

    if(!needToCopy)
    {
        net_stack_mem_buf_t * currBuf = buf;
        while(currBuf != nullptr)
        {
            // If this buffer is passed down direct from the application, we will need to
            // copy the packet.
            if(memory_manager.get_lifetime(currBuf) == NetStackMemoryManager::Lifetime::VOLATILE)
            {
                needToCopy = true;
            }

            // On STM32H7, the Ethernet DMA cannot access data in DTCM.  So, if someone sends
            // a packet with a data pointer in DTCM (e.g. a stack allocated payload), everything
            // will break if we don't copy it first.
#ifdef MBED_RAM_BANK_SRAM_DTC_START
            if(reinterpret_cast<ptrdiff_t>(memory_manager.get_ptr(currBuf)) >= MBED_RAM_BANK_SRAM_DTC_START &&
                reinterpret_cast<ptrdiff_t>(memory_manager.get_ptr(currBuf)) <= MBED_RAM_BANK_SRAM_DTC_START + MBED_RAM_BANK_SRAM_DTC_SIZE)
            {
                needToCopy = true;
            }
#endif

            currBuf = memory_manager.get_next(currBuf);
        }
    }

    printf("Transmitting packet of length %lu in %zu buffers and %zu descs\n",
           memory_manager.get_total_len(buf), memory_manager.count_buffers(buf), neededDescs);

    // Step 2: Copy packet if needed
    if(needToCopy)
    {
        auto * newBuf = memory_manager.alloc_heap(memory_manager.get_total_len(buf), 0);
        if(newBuf == nullptr)
        {
            // No free memory, drop packet
            memory_manager.free(newBuf);
            return HAL_ERROR;
        }

        // We should have gotten just one contiguous buffer
        MBED_ASSERT(memory_manager.get_next(newBuf) == nullptr);
        neededDescs = 1;

        // Copy data over
        memory_manager.copy_from_buf(memory_manager.get_ptr(newBuf), memory_manager.get_len(newBuf), buf);
        memory_manager.free(buf);
        buf = newBuf;
    }

    // Step 3: Wait for needed amount of buffers to be available.
    // Note that, in my experience, it's better to block here, as dropping the packet
    // due to not having enough buffers can create weird effects when the application sends
    // lots of packets at once.
    while(txDescsOwnedByApplication < neededDescs)
    {
        eventFlags.wait_any_for(EVENT_FLAG_TX_DESC_AVAILABLE, rtos::Kernel::wait_for_u32_forever);
    }

    // Step 4: Load buffer into descriptors and send
    net_stack_mem_buf_t * currBuf = buf;
    for(size_t descCount = 0; descCount < neededDescs; descCount++)
    {
        auto & currDesc = txDescs[txSendIndex];

        // Set buffer 1
        currDesc.txDesc.toDMAFmt.buffer1Addr = static_cast<uint8_t *>(memory_manager.get_ptr(currBuf));
        currDesc.txDesc.toDMAFmt.buffer1Len = memory_manager.get_len(currBuf);

        // Set buffer 2
        currBuf = memory_manager.get_next(currBuf);
        if(currBuf != nullptr)
        {
            currDesc.txDesc.toDMAFmt.buffer2Addr = memory_manager.get_ptr(currBuf);
            currDesc.txDesc.toDMAFmt.buffer2Len = memory_manager.get_len(currBuf);

            // Move to next buffer
            currBuf = memory_manager.get_next(currBuf);
        }
        else
        {
            currDesc.txDesc.toDMAFmt.buffer2Addr = nullptr;
            currDesc.txDesc.toDMAFmt.buffer2Len = 0;
        }

        if(currBuf == nullptr)
        {
            // Last descriptor, store buffer address for freeing
            currDesc.packetFirstBuf = buf;
        }
        else
        {
            currDesc.packetFirstBuf = nullptr;
        }

//        printf("Tx Ethernet buffer:");
//        for(size_t byteIdx = 0; byteIdx < currDesc.txDesc.toDMAFmt.buffer1Len; ++byteIdx)
//        {
//            printf(" %02" PRIx8, reinterpret_cast<uint8_t const *>(currDesc.txDesc.toDMAFmt.buffer1Addr)[byteIdx]);
//        }
//        printf("\n");
//
//        printf("Tx Ethernet buffer2:");
//        for(size_t byteIdx = 0; byteIdx < currDesc.txDesc.toDMAFmt.buffer2Len; ++byteIdx)
//        {
//            printf(" %02" PRIx8, reinterpret_cast<uint8_t const *>(currDesc.txDesc.toDMAFmt.buffer2Addr)[byteIdx]);
//        }
//        printf("\n");

#if __DCACHE_PRESENT
        // Write buffers back to main memory
        SCB_CleanDCache_by_Addr(const_cast<void*>(currDesc.txDesc.toDMAFmt.buffer1Addr), currDesc.txDesc.toDMAFmt.buffer1Len);
        if(currDesc.txDesc.toDMAFmt.buffer2Addr != nullptr)
        {
            SCB_CleanDCache_by_Addr(const_cast<void*>(currDesc.txDesc.toDMAFmt.buffer2Addr), currDesc.txDesc.toDMAFmt.buffer2Len);
        }
#endif

        // Enter a critical section, because we could run into weird corner cases if the
        // interrupt executes while we are half done configuring this descriptor and updating
        // the counters.
        core_util_critical_section_enter();

        // Configure settings.  Note that we have to configure these every time as
        // they get wiped away when the DMA gives back the descriptor
        currDesc.txDesc.toDMAFmt._reserved = 0;
        currDesc.txDesc.toDMAFmt.checksumInsertionCtrl = 0; // Mbed does not do checksum offload for now
        currDesc.txDesc.toDMAFmt.tcpSegmentationEnable = false; // No TCP offload
        currDesc.txDesc.toDMAFmt.tcpUDPHeaderLen = 0; // No TCP offload
        currDesc.txDesc.toDMAFmt.srcMACInsertionCtrl = 0; // No MAC insertion
        currDesc.txDesc.toDMAFmt.crcPadCtrl = 0; // Insert CRC and padding
        currDesc.txDesc.toDMAFmt.lastDescriptor = currBuf == nullptr;
        currDesc.txDesc.toDMAFmt.firstDescriptor = descCount == 0;
        currDesc.txDesc.toDMAFmt.isContext = false;
        currDesc.txDesc.toDMAFmt.vlanTagCtrl = 0; // No VLAN tag
        currDesc.txDesc.toDMAFmt.intrOnCompletion = true;
        currDesc.txDesc.toDMAFmt.timestampEnable = false;
        currDesc.txDesc.toDMAFmt.dmaOwn = true;

#if __DCACHE_PRESENT
        // Write descriptor back to main memory
        SCB_CleanDCache_by_Addr(&currDesc, sizeof(EthTxDescriptor));
#endif

        // Update descriptor count and index
        txDescsOwnedByApplication--;
        txSendIndex = (txSendIndex + 1) % MBED_CONF_STM32_EMAC_ETH_TXBUFNB;

        core_util_critical_section_exit();

        // Move tail pointer register to point to the descriptor after this descriptor.
        // This tells the MAC to transmit until it reaches the given descriptor, then stop.
        heth.Instance->DMACTDTPR = reinterpret_cast<uint32_t>(&txDescs[txSendIndex]);
    }

    return HAL_OK;
}

}