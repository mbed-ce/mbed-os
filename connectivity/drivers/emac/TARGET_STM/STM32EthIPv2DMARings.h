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

#include "rtos/Thread.h"
#include "rtos/EventFlags.h"

#include <mstd_atomic>


#ifdef ETH_IP_VERSION_V2

namespace mbed
{

/// Structure representing one Tx descriptor.
/// Only the first 4 words are processed by the hardware; we are free to add our own
/// stuff in after that.
/// Note that per the datasheet, Tx descriptors must be word aligned.
struct __attribute__((packed)) alignas(uint32_t) EthTxDescriptor
{
    void const * buffer1Addr;
    void const * buffer2Addr;
    // TDES2 fields
    uint16_t buffer1Len : 14;
    uint8_t vlanTagCtrl : 2;
    uint16_t buffer2Len : 14;
    bool timestampEnable : 1;
    bool intrOnCompletion : 1;
    // TDES3 fields (not dealing with TCP offload for now)
    uint16_t _reserved : 16;
    uint8_t checksumInsertionCtrl : 2;
    bool tcpSegmentationEnable : 1;
    uint8_t tcpUDPHeaderLen : 4;
    uint8_t srcMACInsertionCtrl : 3;
    uint8_t crcPadCtrl : 2;
    bool lastDescriptor : 1;
    bool firstDescriptor: 1;
    bool isContext : 1;
    bool dmaOwn : 1;

    // Memory buffers filled in to this descriptor.
    // These must be freed when the descriptor is done transmitting.
    net_stack_mem_buf_t * buffer1;
    net_stack_mem_buf_t * buffer2;
};

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

    // Thread which runs the receive loop
    rtos::Thread rxThread;

    const size_t rxPoolSize; ///< Number of entries in the Rx buffer pool

    // Indexes for descriptor rings.
    // NOTE: when working with these indices, it's important to consider the case where e.g. the send and reclaim indexes are
    // equal.  This could mean *either* that the Tx ring is completely full of data, or that the Tx ring is empty.
    // To resolve this ambiguity, we maintain separate count variables that track how many entries are in the ring at present.
    size_t rxBuildIndex; ///< Index of the next Rx descriptor that needs to be built.  Updated by application and used by ISR.
    size_t rxDescsOwnedByApplication; ///< Number of Rx descriptors owned by the application and needing buffers allocated.
    mstd::atomic<size_t> rxNextIndex; ///< Index of the next frame that the DMA shall populate.  Updated by application but used by ISR.

    size_t txSendIndex; ///< Index of the next Tx descriptor that can be filled with data
    mstd::atomic<size_t> txDescsOwnedByApplication; ///< Number of Tx descriptors owned by the application.  Incremented by the ISR and decremented by the application.
    size_t txReclaimIndex; ///< Index of the next Tx descriptor that will be reclaimed by the ISR.

    // Descriptors
    StaticCacheAlignedBuffer<ETH_DMADescTypeDef, 1> * rxDescs;
    StaticCacheAlignedBuffer<EthTxDescriptor, 1> txDescs[MBED_CONF_STM32_EMAC_ETH_TXBUFNB];

    // Tx buffers just need to be aligned to the nearest 4 bytes.
    uint32_t txBuffers[MBED_CONF_STM32_EMAC_ETH_TXBUFNB][ETH_MAX_PACKET_SIZE / sizeof(uint32_t)];

    // Buffers associated with each Rx descriptor.  These will be passed to the application
    // when data is received into them.
    net_stack_mem_buf_t ** rxDescBuffers;

    // Return Rx descriptors to the Ethernet MAC.
    // Descriptors can only be returned if there are free buffers in the pool to allocate to them.
    // The first descriptor returned will be the one at RxBuildDescIdx
    void buildRxDescriptors();

    /// Receive the next packet.  Call from the Rx thread when signal is delivered.
    /// Returns nullptr if nothing was received.
    net_stack_mem_buf_t * rxNextPacket();

    /// Receive main loop
    void rxLoop();

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

    STM32EthIPv2DMARings(EMACMemoryManager & memory_manager, ETH_HandleTypeDef & heth);

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

    /**
     * Sets a callback that needs to be called for packets received for that interface
     *
     * @param input_cb Function to be register as a callback
     */
    void set_link_input_cb(EMAC::emac_link_input_cb_t input_cb)
    {
        emac_link_input_cb = input_cb;
    }

};

}

#endif

#endif //MBED_OS_STM32ETHIPV2DMARINGS_H
