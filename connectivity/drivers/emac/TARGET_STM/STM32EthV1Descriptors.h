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

#ifndef STM32ETHV1DESCRIPTORS_H
#define STM32ETHV1DESCRIPTORS_H

#include <cinttypes>

namespace mbed {
namespace stm32_ethv1 {
    /// Struct for an enhanced Tx descriptor in the STM32 Eth IP v1.
    /// From STM32F7 Reference Manual, section 42.6.7.
    /// Note that even when there is no CPU cache, descriptors must be 32 bit aligned.
    struct __attribute__((packed, aligned(4))) TxDescriptor
    {
        // TDES0 fields
        bool deferred : 1;
        bool underflowErr : 1;
        bool excessiveDeferral : 1;
        uint8_t collisionCount : 4;
        bool vlanFrame : 1;
        bool excessiveCollision : 1;
        bool lateCollision : 1;
        bool noCarrier : 1;
        bool lossOfCarrier : 1;
        bool ipPayloadErr : 1;
        bool frameFlushed : 1;
        bool jabberTimeout : 1;
        bool errorSummary : 1;
        bool ipHeaderError : 1;
        bool txTimestampStatus : 1;
        uint8_t : 2; // reserved
        bool secAddressChained : 1;
        bool endOfRing : 1;
        uint8_t checksumInsCtrl : 2;
        uint8_t : 1; // reserved
        bool txTimestampEn : 1;
        bool disablePad : 1;
        bool disableCRC : 1;
        bool firstSegment : 1;
        bool lastSegment : 1;
        bool intrOnComplete : 1;
        bool dmaOwn : 1;

        // TDES1 fields
        uint16_t buffer1Size : 13;
        uint8_t : 3;
        uint16_t buffer2Size : 13;
        uint8_t : 3;

        // TDES2 fields
        uint8_t const * buffer1;

        // TDES3 fields
        uint8_t const * buffer2OrNextDesc;

        // TDES4 fields
        uint32_t : 32;

        // TDES5 fields
        uint32_t : 32;

        // TDES6 fields
        uint32_t timestampLow;

        // TDES7 fields
        uint32_t timestampHigh;
    };

    /// Struct for an enhanced Rx descriptor in the STM32 Eth IP v1.
    /// From STM32F7 Reference Manual, section 42.6.8
    /// Note that even when there is no CPU cache, descriptors must be 32 bit aligned.
    struct __attribute__((packed, aligned(4))) RxDescriptor
    {
        // RDES0 fields
        bool extStatusAvail : 1;
        bool crcErr : 1;
        bool dribbleErr : 1;
        bool rxErr : 1;
        bool rxWatchdogTimeout : 1;
        bool isEthernetFrame : 1;
        bool lateCollision : 1;
        bool timestampValid : 1;
        bool lastDescriptor : 1;
        bool firstDescriptor : 1;
        bool hasVLANTag : 1;
        bool overflowErr : 1;
        bool lengthErr : 1;
        bool saFilterFail : 1;
        bool frameTruncated : 1;
        bool errSummary : 1;
        uint16_t frameLen : 14;
        bool daFilterFail : 1;
        bool dmaOwn : 1;

        // RDES1 fields
        uint16_t buffer1Size : 13;
        bool : 1;
        bool secAddressChained : 1;
        bool endOfRing : 1;
        uint16_t buffer2Size : 13;
        uint8_t : 2;
        bool disableInterruptOnComplete : 1;

        // RDES2 fields
        uint8_t const * buffer1;

        // RDES3 fields
        uint8_t const * buffer2OrNextDesc;

        // RDES4 fields
        uint8_t ipPayloadType : 3;
        bool ipHeaderErr : 1;
        bool ipPayloadErr : 1;
        bool ipChksumBypassed : 1;
        bool ipv4Pkt : 1;
        bool ipv6Pkt : 1;
        uint8_t ptpMsgType : 4;
        bool ptpLayer2 : 1;
        bool ptpv2: 1;
        uint32_t : 18;

        // RDES5 fields
        uint32_t : 32;

        // RDES6 fields
        uint32_t timestampLow;

        // RDES7 fields
        uint32_t timestampHigh;
    };

#if __DCACHE_PRESENT
    static_assert(sizeof(RxDescriptor) == __SCB_DCACHE_LINE_SIZE, "Rx descriptor size must equal cache line size");
    static_assert(sizeof(TxDescriptor) == __SCB_DCACHE_LINE_SIZE, "Tx descriptor size must equal cache line size");
#endif

}
}

#endif //STM32ETHV1DESCRIPTORS_H
