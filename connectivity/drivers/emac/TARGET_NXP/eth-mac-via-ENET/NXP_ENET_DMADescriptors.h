/*
 * Header mapping the "enhanced" buffer descriptors for the ENET peripheral
 * used on NXP Kinetis and iMXRT MCUs.
 *
 * Copyright (c) 2026 Jamie Smith
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cinttypes>

namespace mbed::NXP_ENET {

struct __attribute__((packed)) EthTxDescriptor
{
    // Flags to Ethernet MAC
    uint16_t dataLength;
    uint16_t _reserved0: 10;
    bool transmitCRC: 1;
    bool lastDescriptor: 1;
    bool _reserved1: 1;
    bool wrapToStartOfRing: 1;
    bool _reserved2: 1;
    bool ready: 1;
    uint8_t const * dataBuffer;

    uint8_t _reserved3;

    // Error result data from Eth MAC
    bool timestampError: 1;
    bool overflowError: 1;
    bool lateCollisionError: 1;
    bool frameError: 1;
    bool excessCollisionError: 1;
    bool underflowError: 1;
    bool _reserved4: 1;
    bool transmitError: 1; // Summary field, indicates if any other error flags tripped

    uint16_t _reserved5: 11;

    // More control fields
    bool insertIPChecksum: 1;
    bool insertProtocolChecksum: 1;
    bool recordTimestamp: 1;
    bool interruptOnTx: 1;
    bool _reserved6: 1;

    uint16_t _reserved7[3];
    uint16_t _reserved8: 15;

    bool lastDescUpdateDone: 1;

    uint32_t timestamp;

    uint64_t _reserved9;
};

#if __DCACHE_PRESENT
    static_assert(sizeof(EthTxDescriptor) == __SCB_DCACHE_LINE_SIZE, "Tx descriptor size must equal cache line size");
#endif

}