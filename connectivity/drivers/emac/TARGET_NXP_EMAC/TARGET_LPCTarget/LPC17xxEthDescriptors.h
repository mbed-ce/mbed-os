/* Copyright (c) 2026 Jamie Smith
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

#pragma once

#include <cstdint>

namespace mbed {

/// Struct for an Rx descriptor in the LPC17xx EMAC.
/// See user manual section 10.15.1
struct __attribute__((packed, aligned(4))) LPC17xxEthRxDescriptor
{
    // First word: data pointer
    uint8_t * data;

    // Second word
    uint16_t size: 11;
    uint32_t : 20; // padding
    bool interrupt: 1;
};

/// Struct for an Rx status descriptor. This is written back from the MAC to the application
/// after a packet is received.
/// Note that for some weird reason, only this descriptor is required to have an alignment of 8 -- all the others
/// have alignment of 4!
struct __attribute__((packed, aligned(8))) LPC17xxEthRxStatusDescriptor
{
    // First word
    uint16_t actualSize: 11; // Size in bytes of data written to buffer, minus 1
    uint8_t : 7; // padding
    bool controlFrame : 1;
    bool vlan : 1;
    bool failedFilter : 1;
    bool isMulticast : 1;
    bool isBroadcast : 1;
    bool hadCRCError : 1;
    bool hadSymbolError : 1;
    bool hadLengthError : 1;
    bool rangeError : 1; // note: datasheet says to ignore this, it's for old versions of Ethernet where the ethertype field carried length
    bool alignmentError : 1;
    bool rxOverrun : 1;
    bool noDescriptorError : 1;
    bool lastDescriptor : 1;
    bool anyError : 1;

    // Second word
    uint8_t sourceAddrHashCRC;
    uint16_t : 8;
    uint8_t destAddrHashCRC;
    uint16_t : 8;
};

/// Struct for a Tx descriptor in the LPC17xx EMAC.
/// See user manual section 10.15.1
struct __attribute__((packed, aligned(4))) LPC17xxEthTxDescriptor
{
    // First word: data pointer
    uint8_t const * data;

    // Second word
    uint16_t size: 11;
    uint32_t : 15; // padding

    // Causes the following 3 flags to override the default MAC settings. If false, the next
    // 3 bits will be ignored.
    bool override : 1;

    bool allowHuge : 1;
    bool padTo64Bytes : 1;
    bool appendCRC : 1;

    bool lastDescriptor : 1;
    bool interrupt: 1;
};

/// Struct for a Tx status descriptor. This is written back from the MAC to the application
/// after a packet is transmitted.
struct __attribute__((packed, aligned(4))) LPC17xxEthTxStatusDescriptor
{
    uint32_t : 21; // padding
    uint8_t collisionCount : 4;
    bool defer : 1;
    bool excessiveDefer : 1;
    bool excessiveCollisionError : 1;
    bool lateCollisionError : 1;
    bool underrunError : 1;
    bool noDescriptorError : 1;
    bool anyError : 1;
};

}