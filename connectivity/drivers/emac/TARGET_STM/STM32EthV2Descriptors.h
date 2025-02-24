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

#ifndef MBED_OS_STM32ETHV2DESCRIPTORS_H
#define MBED_OS_STM32ETHV2DESCRIPTORS_H

#include <cstdint>
#include <cstdlib>

namespace mbed {
    namespace stm32_ethv2
    {

        // Tx descriptor ------------------------------------------------------------------------------------------

        // Descriptor format written by the application to queue a packet for transmission.
        // Note: Datasheet calls this the "read format" which is just nuts...
        struct __attribute__((packed)) EthTxDescriptorToDMAFmt
        {
            uint8_t const * buffer1Addr;
            uint8_t const * buffer2Addr;
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
        };

        // Write-back descriptor (DMA updates the desc with this format when complete)
        struct __attribute__((packed)) EthTxDescriptorFromDMAFmt
        {
            uint32_t timestampLow;
            uint32_t timestampHigh;
            uint32_t _reserved;
            // TDES3 fields
            bool ipHeaderError : 1;
            bool deferred : 1;
            bool underflowError : 1;
            bool excessiveDeferral : 1;
            uint8_t collisionCount : 4;
            bool excessiveCollisions : 1;
            bool lateCollision : 1;
            bool noCarrier : 1;
            bool lossOfCarrier : 1;
            bool payloadChecksumError : 1;
            bool packetFlushed : 1;
            bool jabberTimeout : 1;
            bool errorSummary: 1;
            uint8_t _reserved0: 1;
            bool txTimestampCaptured : 1;
            uint16_t _reserved1 : 10;
            bool lastDescriptor: 1;
            bool firstDescriptor : 1;
            bool context : 1;
            bool dmaOwn : 1;
        };

        // Top-level descriptor type
        // Note that per the datasheet, Tx descriptors must be word aligned.
        struct alignas(uint32_t) EthTxDescriptor {
            union {
                EthTxDescriptorToDMAFmt toDMA;
                EthTxDescriptorFromDMAFmt fromDMA;
            } formats;

            // If we have a data cache, we need each descriptor to be in its own cache line.  So,
    		// pad up to 32 byte cache line size
#if __DCACHE_PRESENT
    		uint8_t _padding[__SCB_DCACHE_LINE_SIZE - sizeof(decltype(formats))];
#endif
        };
#if __DCACHE_PRESENT
		static_assert(sizeof(EthTxDescriptor) == __SCB_DCACHE_LINE_SIZE, "Tx descriptor size must equal cache line size");
#endif

        // Rx descriptor ------------------------------------------------------------------------------------------

        // Format when an Rx descriptor is returned to the DMA.
        // This is called the "read format" in the datasheet.
        struct __attribute__((packed)) EthRxDescriptorToDMAFmt
        {
            uint8_t * buffer1Addr;
            uint32_t _reserved0;
            uint8_t * buffer2Addr;
            uint32_t _reserved1: 24;
            bool buffer1Valid: 1;
            bool buffer2Valid: 1;
            uint8_t _reserved2: 4;
            bool intrOnCompletion: 1;
            bool dmaOwn: 1;
        };

        // Format when an Rx descriptor is given to the application
        // This is called the "write-back format" in the datasheet.
        struct __attribute__((packed)) EthRxDescriptorFromDMAFmt
        {
            // RDES0 fields
            uint16_t outerVLANTag;
            uint16_t innerVLANTag;

            // RDES1 fields
            uint8_t payloadType: 3;
            bool ipHeaderError: 1;
            bool ipv4HeaderPresent: 1;
            bool ipv6HeaderPresent: 1;
            bool checksumOffloadBypassed: 1;
            bool ipPayloadError: 1;
            uint8_t ptpMsgType: 4;
            bool ptpType: 1;
            bool isPTPv2: 1;
            bool tsAvailable: 1;
            bool tsDropped: 1;
            uint16_t oamOrMacCtrl;

            // RDES2 fields
            uint16_t _reserved0: 10;
            bool arpNotGenerated: 1;
            uint8_t _reserved1: 4;
            bool vlanFiltPassed : 1;
            bool sourceAddrFail: 1;
            bool destAddrFail: 1;
            bool hashFilterStatus: 1;
            uint8_t hashValOrMatchIdx: 8;
            bool l3FiltMatch: 1;
            bool l4FiltMatch: 1;
            uint8_t filterMatchNo: 3;

            // RDES3 fields
            uint16_t pktLength: 15;
            bool errorSummary: 1;
            uint8_t lengthType: 3;
            bool dribbleError: 1;
            bool phyRxErr: 1;
            bool overflowErr: 1;
            bool rxWdogTimeout: 1;
            bool giantPkt: 1;
            bool crcErr: 1;
            bool rdes0Valid: 1;
            bool rdes1Valid: 1;
            bool rdes2Valid: 1;
            bool lastDescriptor: 1;
            bool firstDescriptor: 1;
            bool context: 1;
            bool dmaOwn: 1;
        };

        struct alignas(uint32_t) EthRxDescriptor {
          	union {
                EthRxDescriptorToDMAFmt toDMA;
                EthRxDescriptorFromDMAFmt fromDMA;
            } formats;

            // If we have a data cache, we need each descriptor to be in its own cache line.  So,
    		// pad up to 32 byte cache line size
#if __DCACHE_PRESENT
    		uint8_t _padding[__SCB_DCACHE_LINE_SIZE - sizeof(decltype(formats))];
#endif
        };

#if __DCACHE_PRESENT
		static_assert(sizeof(EthRxDescriptor) == __SCB_DCACHE_LINE_SIZE, "Rx descriptor size must equal cache line size");
#endif


    }
}

#endif
