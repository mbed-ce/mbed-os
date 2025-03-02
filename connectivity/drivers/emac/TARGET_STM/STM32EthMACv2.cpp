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
#include "STM32EthMACCommon.h"

#include "mbed_power_mgmt.h"
#include "Timer.h"
#include "mbed_error.h"

using namespace std::chrono_literals;

#define TRACE_GROUP "STEMACv2"

// Defined in stm32_eth_init.c
extern "C" void EthInitPinmappings();
extern "C" void EthDeinitPinmappings();
extern "C" PinName EthGetPhyResetPin();

namespace mbed {
    void STM32EthMACv2::TxDMA::startDMA() {
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

    void STM32EthMACv2::TxDMA::stopDMA() {
        // Disable Tx DMA
        base->DMACTCR &= ~ETH_DMACTCR_ST;
    }

#if __DCACHE_PRESENT
    void STM32EthMACv2::TxDMA::cacheInvalidateDescriptor(size_t descIdx) {
        SCB_InvalidateDCache_by_Addr(&txDescs[descIdx], sizeof(stm32_ethv2::EthTxDescriptor));
    }
#endif

    bool STM32EthMACv2::TxDMA::descOwnedByDMA(size_t descIdx) {
        return txDescs[descIdx].formats.fromDMA.dmaOwn;
    }

    bool STM32EthMACv2::TxDMA::isDMAReadableBuffer(uint8_t const *start, const size_t size) const
    {
#ifdef TARGET_STM32H7
        // On STM32H7, the Ethernet DMA cannot access data in DTCM.  So, if someone sends
        // a packet with a data pointer in DTCM (e.g. a stack allocated payload), everything
        // will break if we don't copy it first.
        if(bufferTouchesMemoryBank(start, size, MBED_RAM_BANK_SRAM_DTC_START, MBED_RAM_BANK_SRAM_DTC_SIZE)) {
            return false;
        }
#endif

#ifdef TARGET_STM32H5
        // On STM32H5, the Ethernet DMA cannot access data in backup SRAM.
        if(bufferTouchesMemoryBank(start, size, MBED_RAM_BANK_SRAM_BKUP_START, MBED_RAM_BANK_SRAM_BKUP_SIZE)) {
            return false;
        }
#endif

        return true;
    }

    void STM32EthMACv2::TxDMA::giveToDMA(const size_t descIdx, uint8_t const * const buffer, const size_t len, const bool firstDesc, const bool lastDesc) {
        auto & desc = txDescs[descIdx];

        // Set buffer
        desc.formats.toDMA.buffer1Addr = buffer;
        desc.formats.toDMA.buffer1Len = len;

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

        // Write descriptor back to main memory
#if __DCACHE_PRESENT
        SCB_CleanDCache_by_Addr(&desc, sizeof(stm32_ethv2::EthTxDescriptor));
#else
        __DMB(); // Make sure descriptor is written before the below lines
#endif

        // Move tail pointer register to point to the descriptor after this descriptor.
        // This tells the MAC to transmit until it reaches the given descriptor, then stop.
        const auto nextDescIdx = (descIdx + 1) % MBED_CONF_NSAPI_EMAC_TX_NUM_DESCS;
        base->DMACTDTPR = reinterpret_cast<uint32_t>(&txDescs[nextDescIdx]);
    }

    void STM32EthMACv2::RxDMA::startDMA()
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

    void STM32EthMACv2::RxDMA::stopDMA() {
        // Disable Rx DMA
        base->DMACRCR &= ~ETH_DMACRCR_SR;
    }

#if __DCACHE_PRESENT
    void STM32EthMACv2::RxDMA::cacheInvalidateDescriptor(size_t descIdx) {
        SCB_InvalidateDCache_by_Addr(&rxDescs[descIdx], sizeof(stm32_ethv2::EthRxDescriptor));
    }
#endif

    bool STM32EthMACv2::RxDMA::descOwnedByDMA(size_t descIdx) {
        return rxDescs[descIdx].formats.toDMA.dmaOwn;
    }

    bool STM32EthMACv2::RxDMA::isFirstDesc(size_t descIdx) {
        return rxDescs[descIdx].formats.fromDMA.firstDescriptor;
    }

    bool STM32EthMACv2::RxDMA::isLastDesc(size_t descIdx) {
        return rxDescs[descIdx].formats.fromDMA.lastDescriptor;
    }

    bool STM32EthMACv2::RxDMA::isErrorDesc(size_t descIdx) {
        // For right now, we treat context descriptors equivalent to error descs.
        // Currently we do not use them, so if we did get one, we just want to get rid of it.
        return rxDescs[descIdx].formats.fromDMA.errorSummary || rxDescs[descIdx].formats.fromDMA.context;
    }

    void STM32EthMACv2::RxDMA::returnDescriptor(const size_t descIdx, uint8_t * const buffer) {
        auto & desc = rxDescs[descIdx];

        // Clear out any bits previously set in the descriptor (from when the DMA gave it back to us)
        memset(&desc, 0, sizeof(stm32_ethv2::EthRxDescriptor));

        // Store buffer address
        desc.formats.toDMA.buffer1Addr = buffer;

        // Configure descriptor
        desc.formats.toDMA.buffer1Valid = true;
        desc.formats.toDMA.intrOnCompletion = true;
        desc.formats.toDMA.dmaOwn = true;

#if __DCACHE_PRESENT
        // Flush to main memory
        SCB_CleanDCache_by_Addr(&desc, __SCB_DCACHE_LINE_SIZE);
#else
        __DMB(); // Make sure descriptor is written before the below lines
#endif

        // Update tail ptr to issue "rx poll demand" and mark this descriptor for receive.
        // Rx stops when the current and tail pointers are equal, so we want to set the tail pointer
        // to one location after the last DMA-owned descriptor in the FIFO.
        const auto nextDescIdx = (descIdx + 1) % RX_NUM_DESCS;
        base->DMACRDTPR = reinterpret_cast<uint32_t>(&rxDescs[nextDescIdx]);
    }

    size_t STM32EthMACv2::RxDMA::getTotalLen(const size_t firstDescIdx, const size_t lastDescIdx) {
        // Total length of the packet is in the last descriptor
        return rxDescs[lastDescIdx].formats.fromDMA.pktLength;
    }

    void STM32EthMACv2::MACDriver::ETH_SetMDIOClockRange(ETH_TypeDef * const base)
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

    CompositeEMAC::ErrCode STM32EthMACv2::MACDriver::init() {
        sleep_manager_lock_deep_sleep();

        // Note: Following code is based on HAL_Eth_Init() from the HAL
        /* Init the low level hardware : GPIO, CLOCK, NVIC. */
        EthInitPinmappings();
#ifdef TARGET_STM32H7
        // Use RMII
        HAL_SYSCFG_ETHInterfaceSelect(SYSCFG_ETH_RMII);

        /* Dummy read to sync with ETH */
        (void)SYSCFG->PMCR;
#else
        __HAL_RCC_SBS_CLK_ENABLE();
        HAL_SBS_ETHInterfaceSelect(SBS_ETH_RMII);

        /* Dummy read to sync with ETH */
        (void)SBS->PMCR;
#endif

        /* Ethernet Software reset */
        /* Set the SWR bit: resets all MAC subsystem internal registers and logic */
        /* After reset all the registers holds their respective reset values */
        base->DMAMR |= ETH_DMAMR_SWR;

        const auto ETH_SW_RESET_TIMEOUT = 500us; // used by STM32 HAL
        Timer timeoutTimer;
        timeoutTimer.start();
        while(timeoutTimer.elapsed_time() < ETH_SW_RESET_TIMEOUT && (base->DMAMR & ETH_DMAMR_SWR)) {}
        if(base->DMAMR & ETH_DMAMR_SWR) {
            // Reset failed to complete within expected timeout.
            // Note: This is usually because of a missing RMII clock from the PHY.
            return ErrCode::TIMEOUT;
        }

        /*------------------ MDIO CSR Clock Range Configuration --------------------*/
        ETH_SetMDIOClockRange(base);

        /*------------------ MAC LPI 1US Tic Counter Configuration --------------------*/
        base->MAC1USTCR = (HAL_RCC_GetHCLKFreq() / 1000000U) - 1U;

        // MAC configuration
        base->MACCR = ETH_MACCR_SARC_REPADDR0 | // Replace the SA field in Tx packets with the configured source address
            ETH_MACCR_CST_Msk; // Don't include the CRC when forwarding Rx packets to the application
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
        NVIC_SetVector(ETH_IRQn, reinterpret_cast<uint32_t>(&STM32EthMACv2::irqHandler));
        HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
        HAL_NVIC_EnableIRQ(ETH_IRQn);

        // Enable Tx, Rx, and fatal bus error interrupts.
        // However, don't enable receive buffer unavailable interrupt, because that can
        // trigger if we run out of Rx descriptors, and we don't want to fatal error
        // in that case.
        base->DMACIER = ETH_DMACIER_NIE | ETH_DMACIER_RIE | ETH_DMACIER_TIE | ETH_DMACIER_FBEE | ETH_DMACIER_AIE;


        return ErrCode::SUCCESS;
    }

    CompositeEMAC::ErrCode STM32EthMACv2::MACDriver::deinit()
    {
        // Disable interrupt
        HAL_NVIC_DisableIRQ(ETH_IRQn);

        // Unlock deep sleep
        sleep_manager_unlock_deep_sleep();

        // Unmap pins and turn off clock
        EthDeinitPinmappings();

        return ErrCode::SUCCESS;
    }

    CompositeEMAC::ErrCode STM32EthMACv2::MACDriver::enable(LinkSpeed speed, Duplex duplex)
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

    CompositeEMAC::ErrCode STM32EthMACv2::MACDriver::disable()
    {
        base->MACCR &= ~(ETH_MACCR_TE | ETH_MACCR_RE);

        // Get rid of any packets still in the transmit FIFO
        base->MTLTQOMR |= ETH_MTLTQOMR_FTQ;

        return ErrCode::SUCCESS;
    }

    void STM32EthMACv2::MACDriver::setOwnMACAddr(const MACAddress &ownAddress) {
        // Set MAC address
        writeMACAddress(ownAddress, &base->MACA0HR, &base->MACA0LR);
    }

    CompositeEMAC::ErrCode STM32EthMACv2::MACDriver::mdioRead(uint8_t devAddr, uint8_t regAddr, uint16_t &result) {
        // This code based on HAL_ETH_ReadPHYRegister()
        if(base->MACMDIOAR & ETH_MACMDIOAR_MB_Msk) {
            // MDIO operation already in progress
            return ErrCode::INVALID_USAGE;
        }

        uint32_t tmpreg = base->MACMDIOAR;

        /* Prepare the MDIO Address Register value
         - Set the PHY device address
         - Set the PHY register address
         - Set the read mode
         - Set the MII Busy bit */
        tmpreg &= ~(ETH_MACMDIOAR_PA_Msk | ETH_MACMDIOAR_RDA_Msk | ETH_MACMDIOAR_MOC_Msk);
        tmpreg |= (devAddr << ETH_MACMDIOAR_PA_Pos) | (regAddr << ETH_MACMDIOAR_RDA_Pos) | ETH_MACMDIOAR_MOC_RD | ETH_MACMDIOAR_MB_Msk;
        base->MACMDIOAR = tmpreg;

        Timer timeoutTimer;
        timeoutTimer.start();
        while(timeoutTimer.elapsed_time() < MDIO_TRANSACTION_TIMEOUT && (base->MACMDIOAR & ETH_MACMDIOAR_MB_Msk)) {}
        if(base->MACMDIOAR & ETH_MACMDIOAR_MB_Msk) {
            // Transaction failed to complete within expected timeout
            return ErrCode::TIMEOUT;
        }

        // Get result
        result = base->MACMDIODR & ETH_MACMDIODR_MD_Msk;

        tr_debug("MDIO read devAddr %" PRIu8 ", regAddr 0x%" PRIx8 " -> 0x%" PRIx16, devAddr, regAddr, result);

        return ErrCode::SUCCESS;
    }

    CompositeEMAC::ErrCode STM32EthMACv2::MACDriver::mdioWrite(uint8_t devAddr, uint8_t regAddr, uint16_t data) {
        // This code based on HAL_ETH_WritePHYRegister()
        if(base->MACMDIOAR & ETH_MACMDIOAR_MB_Msk) {
            // MDIO operation already in progress
            return ErrCode::INVALID_USAGE;
        }

        /* Give the value to the MII data register */
        base->MACMDIODR = data << ETH_MACMDIODR_MD_Pos;

        uint32_t tmpreg = base->MACMDIOAR;

        /* Prepare the MDIO Address Register value
         - Set the PHY device address
         - Set the PHY register address
         - Set the write mode
         - Set the MII Busy bit */
        tmpreg &= ~(ETH_MACMDIOAR_PA_Msk | ETH_MACMDIOAR_RDA_Msk | ETH_MACMDIOAR_MOC_Msk);
        tmpreg |= (devAddr << ETH_MACMDIOAR_PA_Pos) | (regAddr << ETH_MACMDIOAR_RDA_Pos) | ETH_MACMDIOAR_MOC_WR | ETH_MACMDIOAR_MB_Msk;
        base->MACMDIOAR = tmpreg;

        Timer timeoutTimer;
        timeoutTimer.start();
        while(timeoutTimer.elapsed_time() < MDIO_TRANSACTION_TIMEOUT && (base->MACMDIOAR & ETH_MACMDIOAR_MB_Msk)) {}
        if(base->MACMDIOAR & ETH_MACMDIOAR_MB_Msk) {
            // Transaction failed to complete within expected timeout
            return ErrCode::TIMEOUT;
        }

        tr_debug("MDIO write devAddr %" PRIu8 ", regAddr 0x%" PRIx8 " <- 0x%" PRIx16, devAddr, regAddr, data);

        return ErrCode::SUCCESS;
    }

    PinName STM32EthMACv2::MACDriver::getPhyResetPin() {
        return EthGetPhyResetPin();
    }

    CompositeEMAC::ErrCode STM32EthMACv2::MACDriver::addMcastMAC(MACAddress mac) {
        if(numPerfectFilterRegsUsed < NUM_PERFECT_FILTER_REGS) {
            size_t perfFiltIdx = numPerfectFilterRegsUsed;
            ++numPerfectFilterRegsUsed;

            tr_debug("Using perfect filtering for %02" PRIx8 ":%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8,
                     mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            writeMACAddress(mac, MAC_ADDR_PERF_FILTER_REGS[perfFiltIdx].first, MAC_ADDR_PERF_FILTER_REGS[perfFiltIdx].second);
        }
        else {
            // Out of spaces in perfect filter, use hash filter instead
            tr_debug("Using hash filtering for %02" PRIx8 ":%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8,
                     mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            addHashFilterMAC(base, mac);
        }
        return ErrCode::SUCCESS;
    }

    CompositeEMAC::ErrCode STM32EthMACv2::MACDriver::clearMcastFilter() {
        // Reset perfect filter registers
        for(auto regPair : MAC_ADDR_PERF_FILTER_REGS) {
            *regPair.first = 0;
            *regPair.second = 0;
        }
        numPerfectFilterRegsUsed = 0;

        // Reset hash filter
        base->MACHT0R = 0;
        base->MACHT1R = 0;

        return ErrCode::SUCCESS;
    }

    void STM32EthMACv2::MACDriver::setPassAllMcast(bool pass) {
        if(pass)
        {
            base->MACPFR |= ETH_MACPFR_PM;
        }
        else
        {
            base->MACPFR &= ~ETH_MACPFR_PM;
        }
    }

    void STM32EthMACv2::MACDriver::setPromiscuous(bool enable) {
        if(enable)
        {
            base->MACPFR |= ETH_MACPFR_PR;
        }
        else
        {
            base->MACPFR &= ~ETH_MACPFR_PR;
        }
    }

    STM32EthMACv2 * STM32EthMACv2::instance = nullptr;

    STM32EthMACv2::STM32EthMACv2():
    CompositeEMAC(txDMA, rxDMA, macDriver),
    base(ETH),
    txDMA(base),
    rxDMA(base),
    macDriver(base)
    {
        instance = this;
    }

    void STM32EthMACv2::irqHandler()
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

// Provide default EMAC driver
MBED_WEAK EMAC &EMAC::get_default_instance()
{
    static mbed::STM32EthMACv2 emac;
    return emac;
}
