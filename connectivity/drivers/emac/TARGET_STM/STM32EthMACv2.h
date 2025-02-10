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

#ifndef MBED_OS_STM32ETHMACV2_H
#define MBED_OS_STM32ETHMACV2_H

#include "CompositeEMAC.h"
#include "GenericEthDMA.h"
#include "STM32EthV2Descriptors.h"

namespace mbed {
    /**
     * @brief EMAC implementation for STM32 MCUs with Ethernet IP v2
     */
    class STM32EthMacV2 : public CompositeEMAC {

        class TxDMA : public GenericTxDMALoop<stm32_ethv2::EthTxDescriptor>
        {
        protected:
            ETH_TypeDef * const base; // Base address of Ethernet peripheral
            StaticCacheAlignedBuffer<stm32_ethv2::EthTxDescriptor, MBED_CONF_NSAPI_EMAC_TX_NUM_DESCS> txDescs; // Tx descriptors

            void startDMA() override;

            void stopDMA() override;

            bool isDMAReadableBuffer(uint8_t const * start, size_t size) const override;

            void giveToDMA(size_t descIdx, bool firstDesc, bool lastDesc) override;
        public:
            explicit TxDMA(ETH_TypeDef * const base):
            GenericTxDMALoop<stm32_ethv2::EthTxDescriptor>(txDescs),
            base(base)
            {}
        };

        class RxDMA : public GenericRxDMALoop<stm32_ethv2::EthRxDescriptor> {
        protected:
            ETH_TypeDef * const base; // Base address of Ethernet peripheral
            StaticCacheAlignedBuffer<stm32_ethv2::EthRxDescriptor, RX_NUM_DESCS> rxDescs; // Rx descriptors

            void startDMA() override;

            void stopDMA() override;

            void returnDescriptor(size_t descIdx, uint8_t *buffer) override;

            DescriptorType getType(const stm32_ethv2::EthRxDescriptor &desc) override;

        public:
            explicit RxDMA(ETH_TypeDef * const base):
            GenericRxDMALoop<mbed::stm32_ethv2::EthRxDescriptor>(rxDescs),
            base(base)
            {}
        };

        // Components of the ethernet MAC
        TxDMA txDMA;
    };
}



#endif // MBED_OS_STM32ETHMACV2_H