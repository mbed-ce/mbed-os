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

#include "STM32EthMACv2.h"

namespace mbed {
    void STM32EthMacV2::TxDMA::startDMA() {
        // Flush Tx queue
        base->MTLTQOMR |= ETH_MTLTQOMR_FTQ;

        // Configure Tx descriptor ring
        base->DMACTDRLR = MBED_CONF_NSAPI_EMAC_TX_NUM_DESCS - 1; // Ring size
        base->DMACTDLAR = reinterpret_cast<uint32_t>(&txDescs[0]); // Ring base address
        base->DMACTDTPR = reinterpret_cast<uint32_t>(&txDescs[0]); // Next descriptor (tail) pointer

        // Enable Tx DMA
        // NOTE: Typo in C++ headers, should be called "DMACTXCR"
        base->DMACTCR |= ETH_DMACTCR_ST;

        // Clear Tx process stopped flag
        base->DMACSR = ETH_DMACSR_TPS;
    }

    void STM32EthMacV2::TxDMA::stopDMA() {
        // Disable Tx DMA
        base->DMACTCR &= ~ETH_DMACTCR_ST;
    }

    bool STM32EthMacV2::TxDMA::isDMAReadableBuffer(uint8_t const *start, size_t size) const
    {
        // On STM32H7, the Ethernet DMA cannot access data in DTCM.  So, if someone sends
        // a packet with a data pointer in DTCM (e.g. a stack allocated payload), everything
        // will break if we don't copy it first.
#ifdef MBED_RAM_BANK_SRAM_DTC_START
        if(reinterpret_cast<ptrdiff_t>(start) >= MBED_RAM_BANK_SRAM_DTC_START &&
            reinterpret_cast<ptrdiff_t>(start + size) <= MBED_RAM_BANK_SRAM_DTC_START + MBED_RAM_BANK_SRAM_DTC_SIZE)
        {
            return false;
        }
#endif
        return true;
    }

    void STM32EthMacV2::TxDMA::giveToDMA(size_t descIdx, bool firstDesc, bool lastDesc) {
        auto & desc = txDescs[descIdx];

        // Note that we have to configure these every time as
        // they get wiped away when the DMA gives back the descriptor
        desc.formats.toDMA._reserved = 0;
        desc.formats.toDMA.checksumInsertionCtrl = 0; // Mbed does not do checksum offload for now
        desc.formats.toDMA.tcpSegmentationEnable = false; // No TCP offload
        desc.formats.toDMA.tcpUDPHeaderLen = 0; // No TCP offload
        desc.formats.toDMA.srcMACInsertionCtrl = 0; // No MAC insertion
        desc.formats.toDMA.crcPadCtrl = 0; // Insert CRC and padding
        desc.formats.toDMA.lastDescriptor = lastDesc;
        desc.formats.toDMA.firstDescriptor = firstDesc;
        desc.formats.toDMA.isContext = false;
        desc.formats.toDMA.vlanTagCtrl = 0; // No VLAN tag
        desc.formats.toDMA.intrOnCompletion = true;
        desc.formats.toDMA.timestampEnable = false;
        desc.formats.toDMA.dmaOwn = true;

#if __DCACHE_PRESENT
        // Write descriptor back to main memory
        SCB_CleanDCache_by_Addr(&desc, sizeof(stm32_ethv2::EthTxDescriptor));
#endif

        // Move tail pointer register to point to the descriptor after this descriptor.
        // This tells the MAC to transmit until it reaches the given descriptor, then stop.
        base->DMACTDTPR = reinterpret_cast<uint32_t>(&txDescs[txSendIndex]);
    }

    void STM32EthMacV2::RxDMA::startDMA()
    {
        // Configure Rx buffer size.  Per the datasheet and HAL code, we need to round this down to
        // the nearest multiple of 4.
        MBED_ASSERT(rxPoolPayloadSize % sizeof(uint32_t) == 0);
        base->DMACRCR |= rxPoolPayloadSize << ETH_DMACRCR_RBSZ_Pos;

        // Configure Rx descriptor ring
        base->DMACRDRLR = RX_NUM_DESCS - 1; // Ring size
        base->DMACRDLAR = reinterpret_cast<uint32_t>(&rxDescs[0]); // Ring base address
        base->DMACRDTPR = reinterpret_cast<uint32_t>(&rxDescs[0]); // Next descriptor (tail) pointer

        // Enable Rx DMA.
        base->DMACRCR |= ETH_DMACRCR_SR;

        // Clear Rx process stopped flag
        base->DMACSR = ETH_DMACSR_RPS;
    }

    void STM32EthMacV2::RxDMA::returnDescriptor(size_t descIdx, uint8_t *buffer) {
        auto & desc = rxDescs[descIdx];

        // Clear out any bits previously set in the descriptor (from when the DMA gave it back to us)
        memset(&desc, 0, sizeof(stm32_ethv2::EthRxDescriptor));

        // Store buffer address
        desc.formats.toDMA.buffer1Addr = memory_manager->get_ptr(buffer);

        // Configure descriptor
        desc.formats.toDMA.buffer1Valid = true;
        desc.formats.toDMA.intrOnCompletion = true;
        desc.formats.toDMA.dmaOwn = true;

#if __DCACHE_PRESENT
        // Flush to main memory
        SCB_CleanDCache_by_Addr(&desc, __SCB_DCACHE_LINE_SIZE);
#endif

        // Update tail ptr to issue "rx poll demand" and mark this descriptor for receive.
        // Rx stops when the current and tail pointers are equal, so we want to set the tail pointer
        // to one location after the last DMA-owned descriptor in the FIFO.
        base->DMACRDTPR = reinterpret_cast<uint32_t>(&rxDescs[rxBuildIndex]);
    }
}
