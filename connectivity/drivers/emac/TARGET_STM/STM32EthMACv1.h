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

#include "CompositeEMAC.h"
#include "STM32EthV1Descriptors.h"
#include "CacheAlignedBuffer.h"
#include "GenericEthDMA.h"

#if TARGET_STM32F7
#define ENABLE_ERRATA_2_21_6_WORKAROUND 1
#endif

namespace mbed
{

/**
 * @brief EMAC implementation for STM32 MCUs with Ethernet IP v2
 */
class STM32EthMACv1 : public CompositeEMAC
{
    class TxDMA : public GenericTxDMARing
    {
    protected:
        ETH_TypeDef * const base; // Base address of Ethernet peripheral
        StaticCacheAlignedBuffer<stm32_ethv1::TxDescriptor, TX_NUM_DESCS> txDescs; // Tx descriptors

        void startDMA() override;

        void stopDMA() override;

#if __DCACHE_PRESENT
        void cacheInvalidateDescriptor(size_t descIdx) override;
#endif

        bool descOwnedByDMA(size_t descIdx) override;

        bool isDMAReadableBuffer(uint8_t const * start, size_t size) const override;

        void giveToDMA(size_t descIdx, uint8_t const * buffer, size_t len, bool firstDesc, bool lastDesc) override;
    public:
        explicit TxDMA(ETH_TypeDef * const base):
        base(base)
        {}
    };

    class RxDMA : public GenericRxDMARing {
    protected:
        ETH_TypeDef * const base; // Base address of Ethernet peripheral
        StaticCacheAlignedBuffer<stm32_ethv1::RxDescriptor, RX_NUM_DESCS> rxDescs; // Rx descriptors

        void startDMA() override;

        void stopDMA() override;

#if __DCACHE_PRESENT
        void cacheInvalidateDescriptor(size_t descIdx) override;
#endif

        bool descOwnedByDMA(size_t descIdx) override;

        bool isFirstDesc(size_t descIdx) override;

        bool isLastDesc(size_t descIdx) override;

        bool isErrorDesc(size_t descIdx) override;

        void returnDescriptor(size_t descIdx, uint8_t * buffer) override;

        size_t getTotalLen(size_t firstDescIdx, size_t lastDescIdx) override;

    public:
        explicit RxDMA(ETH_TypeDef * const base):
        base(base)
        {}
    };

    class MACDriver : public CompositeEMAC::MACDriver {
        ETH_TypeDef * const base; // Base address of Ethernet peripheral

        // Number of MAC address perfect filter registers used
        size_t numPerfectFilterRegsUsed = 0;

        /**
         * @brief  Configures the Clock range of ETH MDIO interface.
         *
         * Copied from STM32CubeHAL.
         *
         * @param base Base address of Ethernet peripheral
         */
        static void ETH_SetMDIOClockRange(ETH_TypeDef * const base);

#if ENABLE_ERRATA_2_21_6_WORKAROUND
        // Workaround for ETH errata 2.21.6 from the STM32F7 errata sheet
        int rmiiWatchdogHandle;
        std::atomic<bool> rmiiWatchdogRunning = false;
        void rmiiWatchdog();
#endif

    public:
        explicit MACDriver(ETH_TypeDef * const base):
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

    // Pointer to global instance, for ISR to use.
    // TODO if we support more than 1 EMAC per MCU, this will need to be an array
    static STM32EthMACv1 * instance;

    ETH_TypeDef * const base; // Base address of Ethernet peripheral

    // Components of the ethernet MAC
    TxDMA txDMA;
    RxDMA rxDMA;
    MACDriver macDriver;

public:
    STM32EthMACv1();

    // Interrupt callback
    static void irqHandler();
};


}