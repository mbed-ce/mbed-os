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

#pragma once

#include "device.h"
#include "CompositeEMAC.h"
#include "MbedCRC.h"

// Figure out the Ethernet IP version in use
#if defined(TARGET_STM32H5) || defined(TARGET_STM32H7)
#define ETH_IP_VERSION_V2
#else
#define ETH_IP_VERSION_V1
#endif

namespace mbed
{
constexpr auto MDIO_TRANSACTION_TIMEOUT = std::chrono::milliseconds(1); // used by STMicro HAL

inline constexpr size_t NUM_PERFECT_FILTER_REGS = 3;
static const std::pair<volatile uint32_t *, volatile uint32_t *> MAC_ADDR_PERF_FILTER_REGS[NUM_PERFECT_FILTER_REGS] = {
    {&ETH->MACA1HR, &ETH->MACA1LR},
    {&ETH->MACA2HR, &ETH->MACA2LR},
    {&ETH->MACA3HR, &ETH->MACA3LR}
};

/// Write a MAC address into the given registers with the needed encoding
static inline void writeMACAddress(const CompositeEMAC::MACAddress & mac, volatile uint32_t *addrHighReg, volatile uint32_t *addrLowReg)
{
    /* Set MAC addr bits 32 to 47 */
    *addrHighReg = (static_cast<uint32_t>(mac[5]) << 8) | static_cast<uint32_t>(mac[4]) | ETH_MACA1HR_AE_Msk;
    /* Set MAC addr bits 0 to 31 */
    *addrLowReg = (static_cast<uint32_t>(mac[3]) << 24) | (static_cast<uint32_t>(mac[2]) << 16) |
                  (static_cast<uint32_t>(mac[1]) << 8) | static_cast<uint32_t>(mac[0]);
}

/// Add a MAC address to the multicast hash filter.
void addHashFilterMAC(ETH_TypeDef * base, const CompositeEMAC::MACAddress & mac) {
#if defined(ETH_IP_VERSION_V2)
    uint32_t volatile * hashRegs[] = {
        &base->MACHT0R,
        &base->MACHT1R
    };
#else
    uint32_t volatile * hashRegs[] = {
        &base->MACHTLR,
        &base->MACHTHR
    };
#endif

    // Note: as always, the datasheet description of how to do this CRC was vague and slightly wrong.
    // This forum thread figured it out: https://community.st.com/t5/stm32-mcus-security/calculating-ethernet-multicast-filter-hash-value/td-p/416984
    // What the datasheet SHOULD say is:
    // Compute the Ethernet CRC-32 of the MAC address, with initial value of 1s, final XOR of ones, and input reflection on but output reflection off
    // Then, take the upper 6 bits and use that to index the hash table.

    mbed::MbedCRC<POLY_32BIT_ANSI> crcCalc(0xFFFFFFFF, 0xFFFFFFFF, true, false);

    // Compute Ethernet CRC-32 of the MAC address
    uint32_t crc;
    crcCalc.compute(mac.data(), mac.size(), &crc);

    // Take upper 6 bits
    uint32_t hashVal = crc >> 26;

    // Set correct bit in hash filter
    *hashRegs[hashVal >> 5] |= (1 << (hashVal & 0x1F));
}

}