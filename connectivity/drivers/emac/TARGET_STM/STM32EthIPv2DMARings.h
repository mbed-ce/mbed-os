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


#ifndef MBED_OS_STM32ETHIPV2DMARINGS_H
#define MBED_OS_STM32ETHIPV2DMARINGS_H

#include "EMACMemoryManager.h"
#include "EMAC.h"
#include "CacheAlignedBuffer.h"

#include "stm32xx_emac_config.h"
#include "STM32IPv2EthDescriptors.h"

#include "rtos/Thread.h"
#include "rtos/EventFlags.h"

#include <mstd_atomic>


#ifdef ETH_IP_VERSION_V2

namespace mbed
{

struct WrappedEthTxDescriptor
{
    EthTxDescriptor txDesc;

    // Pointer to first memory buffer in the chain associated with this descriptor.
    // This shall only be filled in on the *last* packet, so that the entire chain is freed
    // when the last descriptor is returned.
    net_stack_mem_buf_t * packetFirstBuf;

    // If we have a data cache, we need each descriptor to be in its own cache line.  So,
    // pad up to 32 byte cache line size
#if __DCACHE_PRESENT
    uint8_t _padding[__SCB_DCACHE_LINE_SIZE - sizeof(EthTxDescriptor) - sizeof(net_stack_mem_buf_t *)];
#endif
};

#if __DCACHE_PRESENT
static_assert(sizeof(WrappedEthTxDescriptor) == __SCB_DCACHE_LINE_SIZE, "Tx descriptor size must equal cache line size");
#endif

struct WrappedEthRxDescriptor
{
    EthRxDescriptor rxDesc;

    // Memory buffers filled in to this descriptor.
    // These will be passed to the application if the reception was successful.
    net_stack_mem_buf_t * buffer;

    // If we have a data cache, we need each descriptor to be in its own cache line.  So,
    // pad up to 32 byte cache line size
#if __DCACHE_PRESENT
    uint8_t _padding[__SCB_DCACHE_LINE_SIZE - sizeof(EthRxDescriptor) - sizeof(net_stack_mem_buf_t *)];
#endif
};

#if __DCACHE_PRESENT
static_assert(sizeof(WrappedEthRxDescriptor) == __SCB_DCACHE_LINE_SIZE, "Rx descriptor size must equal cache line size");
#endif


/**
 * @brief Implementation of Ethernet DMA rings for STM32 Ethernet IP v2.
 *
 * This class contains logic largely adapted from the STM32 Ethernet HAL driver.
 * It would have been nice to just use that driver directly, but it cannot adapt to
 * support Mbed's memory manager implementation.
 */
class STM32EthIPv2DMARings
{
    EMACMemoryManager & memory_manager; /**< Memory manager */
    ETH_HandleTypeDef & heth; ///< Handle to Ethernet peripheral
    EMAC::emac_link_input_cb_t emac_link_input_cb; /**< Callback for incoming packets */

    // Event flags used to signal application threads from ISRs
    rtos::EventFlags eventFlags;

    // Thread which runs the receive loop and the transmit buffer reclamation process
    rtos::Thread thread;

    const size_t rxPoolSize; ///< Number of entries in the Rx buffer pool

    // Indexes for descriptor rings.
    // NOTE: when working with these indices, it's important to consider the case where e.g. the send and reclaim indexes are
    // equal.  This could mean *either* that the Tx ring is completely full of data, or that the Tx ring is empty.
    // To resolve this ambiguity, we maintain separate count variables that track how many entries are in the ring at present.
    size_t rxBuildIndex; ///< Index of the next Rx descriptor that needs to be built.  Updated by application and used by ISR.
    size_t rxDescsOwnedByApplication; ///< Number of Rx descriptors owned by the application and needing buffers allocated.
    mstd::atomic<size_t> rxNextIndex; ///< Index of the next frame that the DMA shall populate.  Updated by application but used by ISR.

    size_t txSendIndex; ///< Index of the next Tx descriptor that can be filled with data
    mstd::atomic<size_t> txDescsOwnedByApplication; ///< Number of Tx descriptors owned by the application.  Incremented by the mac thread and decremented by the application thread.
    size_t txReclaimIndex; ///< Index of the next Tx descriptor that will be reclaimed by the mac thread.

    // Descriptors
    DynamicCacheAlignedBuffer<WrappedEthRxDescriptor> rxDescs;
    StaticCacheAlignedBuffer<WrappedEthTxDescriptor, MBED_CONF_STM32_EMAC_ETH_TXBUFNB> txDescs;

    // Tx buffers just need to be aligned to the nearest 4 bytes.
    uint32_t txBuffers[MBED_CONF_STM32_EMAC_ETH_TXBUFNB][ETH_MAX_PACKET_SIZE / sizeof(uint32_t)];

    // Return Rx descriptors to the Ethernet MAC.
    // Descriptors can only be returned if there are free buffers in the pool to allocate to them.
    // The first descriptor returned will be the one at RxBuildDescIdx
    void buildRxDescriptors();

    /// Receive the next packet.  Call from the Rx thread when signal is delivered.
    /// Returns nullptr if nothing was received.
    net_stack_mem_buf_t * rxNextPacket();

    /// Reclaims Tx buffers and frees their memory after packet transmission.
    /// Invoked by the MAC thread when it sees a Tx interrupt.
    void reclaimTxDescs();

    /// MAC thread loop
    void macThread();

public:
    // Alignment required for Rx memory buffers.  Normally they don't need alignment but
    // if we are doing cache operations they need to be cache aligned.
#if __DCACHE_PRESENT
    static constexpr size_t RX_BUFFER_ALIGN = __SCB_DCACHE_LINE_SIZE;
#else
    static constexpr size_t RX_BUFFER_ALIGN = 2;
#endif

    /// How many extra buffers to leave in the Rx pool, relative to how many we keep assigned to Rx descriptors.
    /// We want to keep some amount of extra buffers because constantly hitting the network stack with failed pool
    /// allocations can produce some negative consequences in some cases.
    static constexpr size_t RX_POOL_EXTRA_BUFFERS = 3;

    /// Payload size of buffers allocated from the Rx pool.  This is the allocation unit size
    /// of the pool minus any overhead needed for alignment.
    const size_t rxPoolPayloadSize;

    STM32EthIPv2DMARings(EMACMemoryManager & memory_manager, ETH_HandleTypeDef & heth, EMAC::emac_link_input_cb_t emac_link_input_cb);

    ~STM32EthIPv2DMARings();

    /**
     * @brief Start DMA rings going.  Based on HAL_ETH_Start().
     */
    HAL_StatusTypeDef startDMA();

    /// Call when EMAC generates receive interrupt.  Signals the Rx thread if there is a
    /// new packet to receive.
    void rxISR();

    /// Call when EMAC generates transmit interrupt
    void txISR();

    /// Transmit a packet out of the Tx DMA ring.  Note that this function
    /// *takes ownership of* the passed packet and will free it either now or after
    /// it's been transmitted.
    /// Will block until there is space to transmit the packet.
    HAL_StatusTypeDef txPacket(net_stack_mem_buf_t * buf);
};

}

#endif

#endif //MBED_OS_STM32ETHIPV2DMARINGS_H
