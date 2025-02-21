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

namespace mbed
{

/**
 * @brief EMAC implementation for STM32 MCUs with Ethernet IP v2
 */
class STM32EthMACv1 : public CompositeEMAC
{
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

public:
    // Interrupt callback
    static void irqHandler();
};


}