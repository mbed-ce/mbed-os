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

#ifndef NUVOTONM480ETHMAC_H
#define NUVOTONM480ETHMAC_H

#include "CompositeEMAC.h"
#include "GenericEthDMA.h"
#include "m480_eth_descriptors.h"

namespace mbed
{

class NuvotonM480EthMAC : public CompositeEMAC
{
    class MACDriver : public CompositeEMAC::MACDriver
    {
        EMAC_T * const base;

        /// Number of multicast MACs we are currently subscribed to
        size_t numMulticastSubscriptions = 0;

        bool passAllMcastEnabled = false;
        bool promiscuousEnabled = false;

        /// Write a MAC address into the CAM (mac filter) registers
        void writeMACAddress(size_t index, MACAddress macAddress);

    public:
        explicit MACDriver(EMAC_T * const base):
        base(base)
        {}

        ErrCode init() override;

        ErrCode deinit() override;

        ErrCode enable(LinkSpeed speed, Duplex duplex) override;

        ErrCode disable() override;

        void setOwnMACAddr(const MACAddress &ownAddress) override;

        ErrCode mdioRead(uint8_t devAddr, uint8_t regAddr, uint16_t &result) override;

        ErrCode mdioWrite(uint8_t devAddr, uint8_t regAddr, uint16_t data) override;

        PinName getPhyResetPin() override;

        ErrCode addMcastMAC(MACAddress mac) override;

        ErrCode clearMcastFilter() override;

        void setPassAllMcast(bool pass) override;

        void setPromiscuous(bool enable) override;
    };

    class TxDMA : public GenericTxDMARing
    {
    protected:
        EMAC_T * const base; // Base address of Ethernet peripheral
        volatile M480EthTxDescriptor txDescs[TX_NUM_DESCS]{}; // Tx descriptors

        void startDMA() override;

        void stopDMA() override;

        bool descOwnedByDMA(size_t descIdx) override;

        bool isDMAReadableBuffer(uint8_t const * start, size_t size) const override;

        void giveToDMA(size_t descIdx, uint8_t const * buffer, size_t len, bool firstDesc, bool lastDesc) override;
    public:
        explicit TxDMA(EMAC_T * const base):
        GenericTxDMARing(0, false), // we do NOT support multiple descriptors in the hardware
        base(base)
        {}
    };

    class RxDMA : public GenericRxDMARing {
    protected:
        EMAC_T * const base; // Base address of Ethernet peripheral
        volatile M480EthRxDescriptor rxDescs[RX_NUM_DESCS]{}; // Rx descriptors

        void startDMA() override;

        void stopDMA() override;

        bool descOwnedByDMA(size_t descIdx) override;

        bool isFirstDesc(size_t descIdx) override;

        bool isLastDesc(size_t descIdx) override;

        bool isErrorDesc(size_t descIdx) override;

        void returnDescriptor(size_t descIdx, uint8_t * buffer) override;

        size_t getTotalLen(size_t firstDescIdx, size_t lastDescIdx) override;

    public:
        explicit RxDMA(EMAC_T * const base):
        base(base)
        {}
    };

    // Pointer to global instance, for ISR to use.
    // TODO if we support more than 1 EMAC per MCU, this will need to be an array
    static NuvotonM480EthMAC * instance;

    EMAC_T * const base; // Base address of Ethernet peripheral

    // Components of the ethernet MAC
    TxDMA txDMA;
    RxDMA rxDMA;
    MACDriver macDriver;

public:
    NuvotonM480EthMAC();

    // Interrupt callbacks
    static void txIrqHandler();
    static void rxIrqHandler();
};

}

#endif //NUVOTONM480ETHMAC_H
