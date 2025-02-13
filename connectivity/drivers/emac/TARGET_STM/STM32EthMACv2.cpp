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

#include "STM32EthMACv2.h"

#include "mbed_power_mgmt.h"
#include "Timer.h"
#include "mbed_error.h"

using namespace std::chrono_literals;

// Defined in stm32_eth_init.c
extern "C" void EthInitPinmappings(void);

namespace mbed {
    void STM32EthMacV2::TxDMA::startDMA() {
        // Flush Tx queue
        base->MTLTQOMR |= ETH_MTLTQOMR_FTQ;

        // Configure Tx descriptor ring
        base->DMACTDRLR = MBED_CONF_NSAPI_EMAC_TX_NUM_DESCS - 1; // Ring size
        base->DMACTDLAR = reinterpret_cast<uint32_t>(&txDescs[0]); // Ring base address
        base->DMACTDTPR = reinterpret_cast<uint32_t>(&txDescs[0]); // Next descriptor (tail) pointer

        // Enable Tx DMA
        // NOTE: Typo in C++ headers, should be called "DMACTXCR"
        base->DMACTCR |= ETH_DMACTCR_ST;

        // Clear Tx process stopped flag
        base->DMACSR = ETH_DMACSR_TPS;
    }

    void STM32EthMacV2::TxDMA::stopDMA() {
        // Disable Tx DMA
        base->DMACTCR &= ~ETH_DMACTCR_ST;
    }

    bool STM32EthMacV2::TxDMA::isDMAReadableBuffer(uint8_t const *start, const size_t size) const
    {
        // On STM32H7, the Ethernet DMA cannot access data in DTCM.  So, if someone sends
        // a packet with a data pointer in DTCM (e.g. a stack allocated payload), everything
        // will break if we don't copy it first.
#ifdef MBED_RAM_BANK_SRAM_DTC_START
        if(reinterpret_cast<ptrdiff_t>(start) >= MBED_RAM_BANK_SRAM_DTC_START &&
            reinterpret_cast<ptrdiff_t>(start + size) <= MBED_RAM_BANK_SRAM_DTC_START + MBED_RAM_BANK_SRAM_DTC_SIZE)
        {
            return false;
        }
#endif
        return true;
    }

    void STM32EthMacV2::TxDMA::giveToDMA(const size_t descIdx, const bool firstDesc, const bool lastDesc) {
        auto & desc = txDescs[descIdx];

        // Note that we have to configure these every time as
        // they get wiped away when the DMA gives back the descriptor
        desc.formats.toDMA._reserved = 0;
        desc.formats.toDMA.checksumInsertionCtrl = 0; // Mbed does not do checksum offload for now
        desc.formats.toDMA.tcpSegmentationEnable = false; // No TCP offload
        desc.formats.toDMA.tcpUDPHeaderLen = 0; // No TCP offload
        desc.formats.toDMA.srcMACInsertionCtrl = 0; // No MAC insertion
        desc.formats.toDMA.crcPadCtrl = 0; // Insert CRC and padding
        desc.formats.toDMA.lastDescriptor = lastDesc;
        desc.formats.toDMA.firstDescriptor = firstDesc;
        desc.formats.toDMA.isContext = false;
        desc.formats.toDMA.vlanTagCtrl = 0; // No VLAN tag
        desc.formats.toDMA.intrOnCompletion = true;
        desc.formats.toDMA.timestampEnable = false;
        desc.formats.toDMA.dmaOwn = true;

#if __DCACHE_PRESENT
        // Write descriptor back to main memory
        SCB_CleanDCache_by_Addr(&desc, sizeof(stm32_ethv2::EthTxDescriptor));
#endif

        // Move tail pointer register to point to the descriptor after this descriptor.
        // This tells the MAC to transmit until it reaches the given descriptor, then stop.
        base->DMACTDTPR = reinterpret_cast<uint32_t>(&txDescs[txSendIndex]);
    }

    void STM32EthMacV2::RxDMA::startDMA()
    {
        // Configure Rx buffer size.  Per the datasheet and HAL code, we need to round this down to
        // the nearest multiple of 4.
        MBED_ASSERT(rxPoolPayloadSize % sizeof(uint32_t) == 0);
        base->DMACRCR |= rxPoolPayloadSize << ETH_DMACRCR_RBSZ_Pos;

        // Configure Rx descriptor ring
        base->DMACRDRLR = RX_NUM_DESCS - 1; // Ring size
        base->DMACRDLAR = reinterpret_cast<uint32_t>(&rxDescs[0]); // Ring base address
        base->DMACRDTPR = reinterpret_cast<uint32_t>(&rxDescs[0]); // Next descriptor (tail) pointer

        // Enable Rx DMA.
        base->DMACRCR |= ETH_DMACRCR_SR;

        // Clear Rx process stopped flag
        base->DMACSR = ETH_DMACSR_RPS;
    }

    void STM32EthMacV2::RxDMA::returnDescriptor(const size_t descIdx, uint8_t * const buffer) {
        auto & desc = rxDescs[descIdx];

        // Clear out any bits previously set in the descriptor (from when the DMA gave it back to us)
        memset(&desc, 0, sizeof(stm32_ethv2::EthRxDescriptor));

        // Store buffer address
        desc.formats.toDMA.buffer1Addr = memory_manager->get_ptr(buffer);

        // Configure descriptor
        desc.formats.toDMA.buffer1Valid = true;
        desc.formats.toDMA.intrOnCompletion = true;
        desc.formats.toDMA.dmaOwn = true;

#if __DCACHE_PRESENT
        // Flush to main memory
        SCB_CleanDCache_by_Addr(&desc, __SCB_DCACHE_LINE_SIZE);
#endif

        // Update tail ptr to issue "rx poll demand" and mark this descriptor for receive.
        // Rx stops when the current and tail pointers are equal, so we want to set the tail pointer
        // to one location after the last DMA-owned descriptor in the FIFO.
        base->DMACRDTPR = reinterpret_cast<uint32_t>(&rxDescs[rxBuildIndex]);
    }

    size_t STM32EthMacV2::RxDMA::getTotalLen(const size_t firstDescIdx) {
        // Total length of the packet is in the first descriptor
        return rxDescs[firstDescIdx].formats.fromDMA.pktLength;
    }

    void STM32EthMacV2::MacDriver::ETH_SetMDIOClockRange(ETH_TypeDef * const base)
    {
        uint32_t hclk;
        uint32_t tmpreg;

        /* Get the ETHERNET MACMDIOAR value */
        tmpreg = base->MACMDIOAR;

        /* Clear CSR Clock Range bits */
        tmpreg &= ~ETH_MACMDIOAR_CR;

        /* Get hclk frequency value */
        hclk = HAL_RCC_GetHCLKFreq();

        /* Set CR bits depending on hclk value */
        if (hclk < 35000000U)
        {
            /* CSR Clock Range between 0-35 MHz */
            tmpreg |= (uint32_t)ETH_MACMDIOAR_CR_DIV16;
        }
        else if (hclk < 60000000U)
        {
            /* CSR Clock Range between 35-60 MHz */
            tmpreg |= (uint32_t)ETH_MACMDIOAR_CR_DIV26;
        }
        else if (hclk < 100000000U)
        {
            /* CSR Clock Range between 60-100 MHz */
            tmpreg |= (uint32_t)ETH_MACMDIOAR_CR_DIV42;
        }
        else if (hclk < 150000000U)
        {
            /* CSR Clock Range between 100-150 MHz */
            tmpreg |= (uint32_t)ETH_MACMDIOAR_CR_DIV62;
        }
        else if (hclk < 250000000U)
        {
            /* CSR Clock Range between 150-250 MHz */
            tmpreg |= (uint32_t)ETH_MACMDIOAR_CR_DIV102;
        }
        else /* (hclk >= 250000000U) */
        {
            /* CSR Clock >= 250 MHz */
            tmpreg |= (uint32_t)(ETH_MACMDIOAR_CR_DIV124);
        }

        /* Configure the CSR Clock Range */
        base->MACMDIOAR = (uint32_t)tmpreg;
    }


    CompositeEMAC::ErrCode STM32EthMacV2::MacDriver::init() {
        sleep_manager_lock_deep_sleep();

        // Note: Following code is based on HAL_Eth_Init() from the HAL
        /* Init the low level hardware : GPIO, CLOCK, NVIC. */
        EthInitPinmappings();

        // Use RMII
        HAL_SYSCFG_ETHInterfaceSelect(SYSCFG_ETH_RMII);

        /* Dummy read to sync with ETH */
        (void)SYSCFG->PMCR;

        /* Ethernet Software reset */
        /* Set the SWR bit: resets all MAC subsystem internal registers and logic */
        /* After reset all the registers holds their respective reset values */
        base->DMAMR |= ETH_DMAMR_SWR;

        const auto ETH_SW_RESET_TIMEOUT = 500us; // used by STM32 HAL
        Timer timeoutTimer;
        timeoutTimer.start();
        while(timeoutTimer.elapsed_time() < ETH_SW_RESET_TIMEOUT && (base->DMAMR & ETH_DMAMR_SWR)) {}
        if(base->DMAMR & ETH_DMAMR_SWR) {
            // Reset failed to complete within expected timeout
            return ErrCode::TIMEOUT;
        }

        /*------------------ MDIO CSR Clock Range Configuration --------------------*/
        ETH_SetMDIOClockRange(base);

        /*------------------ MAC LPI 1US Tic Counter Configuration --------------------*/
        base->MAC1USTCR = (HAL_RCC_GetHCLKFreq() / 1000000U) - 1U;

        // MAC configuration
        base->MACCR = ETH_MACCR_SARC_REPADDR0; // Replace the SA field in Tx packets with the configured source address
        base->MTLTQOMR |= ETH_MTLTQOMR_TSF_Msk; // Enable store and forward mode for transmission (default in the HAL)

        // Enable multicast hash and perfect filter
        base->MACPFR = ETH_MACPFR_HMC | ETH_MACPFR_HPF;

        // Default CubeHAL DMA settings
        base->DMASBMR = ETH_DMASBMR_AAL_Msk | ETH_DMASBMR_FB_Msk;
        base->DMACTCR = ETH_DMACTCR_TPBL_32PBL;
        base->DMACRCR = ETH_DMACRCR_RPBL_32PBL;

        // Configure spacing between DMA descriptors.  This will be different depending on
        // cache line sizes.
        // NOTE: Cast pointers to uint8_t so that the difference will be returned in bytes instead
        // of elements.
        const size_t rxSpacing = sizeof(stm32_ethv2::EthRxDescriptor);

        // Check that spacing seems valid
#ifndef NDEBUG
        const size_t txSpacing = sizeof(stm32_ethv2::EthTxDescriptor);
        MBED_ASSERT(rxSpacing == txSpacing);
        MBED_ASSERT(rxSpacing % sizeof(uint32_t) == 0);
#endif

        // The spacing bitfield is configured as the number of 32-bit words to skip between descriptors.
        // The descriptors have a default size of 16 bytes.
        const size_t wordsToSkip = (rxSpacing - 16) / sizeof(uint32_t);
        MBED_ASSERT(wordsToSkip <= 7);
        base->DMACCR &= ~ETH_DMACCR_DSL_Msk;
        base->DMACCR |= wordsToSkip << ETH_DMACCR_DSL_Pos;

        // Set up interrupt handler
        NVIC_SetVector(ETH_IRQn, reinterpret_cast<uint32_t>(&STM32EthMacV2::irqHandler));
        HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
        HAL_NVIC_EnableIRQ(ETH_IRQn);

        // Enable Tx, Rx, and fatal bus error interrupts.
        // However, don't enable receive buffer unavailable interrupt, because that can
        // trigger if we run out of Rx descriptors, and we don't want to fatal error
        // in that case.
        base->DMACIER = ETH_DMACIER_NIE | ETH_DMACIER_RIE | ETH_DMACIER_TIE | ETH_DMACIER_FBEE | ETH_DMACIER_AIE;


        return ErrCode::SUCCESS;
    }

    CompositeEMAC::ErrCode STM32EthMacV2::MacDriver::deinit()
    {
        // Disable transmission and reception
        disable();

        // Disable interrupt
        HAL_NVIC_DisableIRQ(ETH_IRQn);

        // Unlock deep sleep
        sleep_manager_unlock_deep_sleep();

        return ErrCode::SUCCESS;
    }

    CompositeEMAC::ErrCode STM32EthMacV2::MacDriver::enable(LinkSpeed speed, Duplex duplex)
    {
        if(speed == LinkSpeed::LINK_1GBIT) {
            return ErrCode::INVALID_ARGUMENT;
        }

        auto maccrVal = base->MACCR;
        if(speed == LinkSpeed::LINK_100MBIT) {
            maccrVal |= ETH_MACCR_FES_Msk;
        }
        else {
            maccrVal &= ~ETH_MACCR_FES_Msk;
        }
        if(duplex == Duplex::FULL) {
            maccrVal |= ETH_MACCR_DM_Msk;
        }
        else {
            maccrVal &= ~ETH_MACCR_DM_Msk;
        }

        // Enable the MAC transmission & reception
        maccrVal |= ETH_MACCR_TE | ETH_MACCR_RE;
        base->MACCR = maccrVal;
        return ErrCode::SUCCESS;
    }

    CompositeEMAC::ErrCode STM32EthMacV2::MacDriver::disable()
    {
        base->MACCR &= ~(ETH_MACCR_TE | ETH_MACCR_RE);
        return ErrCode::SUCCESS;
    }

    void STM32EthMacV2::MacDriver::setOwnMACAddr(const MACAddress &ownAddress) {
    }

    STM32EthMacV2 * STM32EthMacV2::instance = nullptr;

    STM32EthMacV2::STM32EthMacV2():
    CompositeEMAC(txDMA, rxDMA),
    base(ETH),
    txDMA(base),
    rxDMA(base),
    macDriver(base)
    {
        instance = this;
    }

    void STM32EthMacV2::irqHandler()
    {
        const auto emacInst = instance;
        uint32_t dma_flag = emacInst->base->DMACSR;

        /* Packet received */
        if ((dma_flag & ETH_DMACSR_RI) != 0U)
        {
            /* Clear the Eth DMA Rx IT pending bits */
            ETH->DMACSR = ETH_DMACSR_RI | ETH_DMACSR_NIS;

            emacInst->rxISR();
        }

        /* Packet transmitted */
        if ((dma_flag & ETH_DMACSR_TI) != 0U)
        {
            /* Clear the Eth DMA Tx IT pending bits */
            ETH->DMACSR = ETH_DMACSR_TI | ETH_DMACSR_NIS;

            emacInst->txISR();
        }

        /* ETH DMA Error */
        if(dma_flag & ETH_DMACSR_FBE)
        {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_ETHERNET, EIO), \
                   "STM32 EMAC v2: Hardware reports fatal DMA error\n");
        }
    }

}
