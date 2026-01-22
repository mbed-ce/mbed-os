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

#include <optional>

#include "DigitalOut.h"
#include "GenericEthDMA.h"
#include "LPC17xxEthDescriptors.h"

namespace mbed
{

class LPC17xxEthMAC : public CompositeEMAC
{
    class MACDriver : public CompositeEMAC::MACDriver
    {
        LPC_EMAC_TypeDef * const base;

        bool passAllMcastEnabled = false;
        bool promiscuousEnabled = false;

        DigitalOut ethClockEnablePin;

    public:
        explicit MACDriver(LPC_EMAC_TypeDef * const base):
        base(base),
        ethClockEnablePin(ETH_CLOCK_ENABLE)
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
        LPC_EMAC_TypeDef * const base; // Base address of Ethernet peripheral

        // Tx descriptors (Application to MAC)
        volatile LPC17xxEthTxDescriptor txDescs[TX_NUM_DESCS]{};

        // Gx status descriptors (MAC to application)
        volatile LPC17xxEthTxStatusDescriptor txStatusDescs[TX_NUM_DESCS]{};

        void startDMA() override;

        void stopDMA() override;

        bool descOwnedByDMA(size_t descIdx) override;

        bool isDMAReadableBuffer(uint8_t const * start, size_t size) const override;

        void giveToDMA(size_t descIdx, uint8_t const * buffer, size_t len, bool firstDesc, bool lastDesc) override;
    public:
        explicit TxDMA(LPC_EMAC_TypeDef * const base):
        GenericTxDMARing(0, false), // we do NOT support multiple descriptors in the hardware
        base(base)
        {}
    };

    class RxDMA : public GenericRxDMARing {
    protected:
        LPC_EMAC_TypeDef * const base; // Base address of Ethernet peripheral

        // Rx descriptors (Application to MAC)
        volatile LPC17xxEthRxDescriptor rxDescs[RX_NUM_DESCS]{};

        // Rx status descriptors (MAC to application)
        volatile LPC17xxEthRxStatusDescriptor rxStatusDescs[RX_NUM_DESCS]{};

        void startDMA() override;

        void stopDMA() override;

        bool descOwnedByDMA(size_t descIdx) override;

        bool isFirstDesc(size_t descIdx) override;

        bool isLastDesc(size_t descIdx) override;

        bool isErrorDesc(size_t descIdx) override;

        void returnDescriptor(size_t descIdx, uint8_t * buffer) override;

        size_t getTotalLen(size_t firstDescIdx, size_t lastDescIdx) override;

    public:
        explicit RxDMA(LPC_EMAC_TypeDef * const base):
        GenericRxDMARing(false, true),
        base(base)
        {}
    };

    // Pointer to global instance, for ISR to use.
    // TODO if we support more than 1 EMAC per MCU, this will need to be an array
    static LPC17xxEthMAC * instance;

    LPC_EMAC_TypeDef * const base; // Base address of Ethernet peripheral

    // Components of the ethernet MAC
    TxDMA txDMA;
    RxDMA rxDMA;
    MACDriver macDriver;

public:
    explicit LPC17xxEthMAC(LPC_EMAC_TypeDef * const base);

    // Interrupt callbacks
    static void irqHandler();
};

}
