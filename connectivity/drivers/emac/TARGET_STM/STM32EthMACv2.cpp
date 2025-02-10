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
}
