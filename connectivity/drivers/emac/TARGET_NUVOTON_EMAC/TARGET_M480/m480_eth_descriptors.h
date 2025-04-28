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

#ifndef M480_ETH_DESCRIPTORS_H
#define M480_ETH_DESCRIPTORS_H

namespace mbed {
/// Struct for a Tx descriptor in the M480 EMAC.
/// See M480 TRM page 1263.
struct __attribute__((packed, aligned(4))) M480EthTxDescriptor
{
    // TXDES0 fields
    bool PADEN : 1;
    bool CRCAPP : 1;
    bool INTEN : 1;
    bool TTSEN : 1;
    uint32_t : 27;
    bool EMAC_OWN : 1;

    // TXDES1 fields
    union {
        uint8_t const * TXBSA;
        uint32_t TSSUBSEC;
    };

    // TXDES2 fields
    uint16_t TBC;
    bool TXIF : 1;
    bool DEF : 1;
    bool : 1;
    bool TXCPIF : 1;
    bool EXDEFIF : 1;
    bool NCSIF : 1;
    bool TXABTIF : 1;
    bool LCIF : 1;
    bool TXHALT : 1;
    bool TXPAUSED : 1;
    bool SQE : 1;
    bool TTSAS : 1;
    uint8_t COLCNT : 4;

    // TXDES3 fields
    union {
        M480EthTxDescriptor volatile * NTXDSA;
        uint32_t TSSEC;
    };
};

/// Struct for an Rx descriptor in the M480 EMAC.
/// See M480 TRM page 1257.
struct __attribute__((packed, aligned(4))) M480EthRxDescriptor
{
    // RXDES0 fields
    uint16_t RBC;
    bool RXIF : 1;
    bool CRCEIF : 1;
    bool : 1;
    bool LPIF : 1;
    bool RXGDIF : 1;
    bool ALIEIF : 1;
    bool RPIF : 1;
    bool RTSAS : 1;
    uint8_t : 7;
    bool EMAC_OWN : 1;

    // RXDES1 fields
    union {
        uint8_t * RXBSA;
        uint32_t TSSUBSEC;
    };

    // RXDES2 fields
    uint32_t : 32;

    // RXDES3 fields
    union {
        M480EthRxDescriptor volatile * NRXDSA;
        uint32_t TSSEC;
    };
};
}

#endif //M480_ETH_DESCRIPTORS_H
