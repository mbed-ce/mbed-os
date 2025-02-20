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

#include "STM32EthMACv1.h"
#include "STM32EthMACCommon.h"

#include <mbed_power_mgmt.h>
#include <Timer.h>
#include <mbed_trace.h>

#define TRACE_GROUP "STEMACv1"

using namespace std::chrono_literals;

// Defined in stm32_eth_init.c
extern "C" void EthInitPinmappings();
extern "C" void EthDeinitPinmappings();
extern "C" PinName EthGetPhyResetPin();

namespace mbed {
    void STM32EthMACv1::MACDriver::ETH_SetMDIOClockRange(ETH_TypeDef * const base) {
        /* Get the ETHERNET MACMIIAR value */
        uint32_t tempreg = base->MACMIIAR;
        /* Clear CSR Clock Range CR[2:0] bits */
        tempreg &= ETH_MACMIIAR_CR_MASK;

        /* Get hclk frequency value */
        uint32_t hclk = HAL_RCC_GetHCLKFreq();

        /* Set CR bits depending on hclk value */
        if((hclk >= 20000000)&&(hclk < 35000000))
        {
            /* CSR Clock Range between 20-35 MHz */
            tempreg |= (uint32_t)ETH_MACMIIAR_CR_Div16;
        }
        else if((hclk >= 35000000)&&(hclk < 60000000))
        {
            /* CSR Clock Range between 35-60 MHz */
            tempreg |= (uint32_t)ETH_MACMIIAR_CR_Div26;
        }
        else if((hclk >= 60000000)&&(hclk < 100000000))
        {
            /* CSR Clock Range between 60-100 MHz */
            tempreg |= (uint32_t)ETH_MACMIIAR_CR_Div42;
        }
        else if((hclk >= 100000000)&&(hclk < 150000000))
        {
            /* CSR Clock Range between 100-150 MHz */
            tempreg |= (uint32_t)ETH_MACMIIAR_CR_Div62;
        }
        else /* ((hclk >= 150000000)&&(hclk <= 216000000)) */
        {
            /* CSR Clock Range between 150-216 MHz */
            tempreg |= (uint32_t)ETH_MACMIIAR_CR_Div102;
        }

        /* Write to ETHERNET MAC MIIAR: Configure the ETHERNET CSR Clock Range */
        base->MACMIIAR = (uint32_t)tempreg;
    }

CompositeEMAC::ErrCode STM32EthMACv1::MACDriver::init() {
    sleep_manager_lock_deep_sleep();

    // Note: Following code is based on HAL_Eth_Init() from the HAL

    /* Enable SYSCFG Clock */
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* Select RMII Mode*/
    SYSCFG->PMC |= SYSCFG_PMC_MII_RMII_SEL;

    /* Init the low level hardware : GPIO, CLOCK, NVIC. */
    EthInitPinmappings();

    /* Ethernet Software reset */
    /* Set the SWR bit: resets all MAC subsystem internal registers and logic */
    /* After reset all the registers holds their respective reset values */
    base->DMABMR |= ETH_DMABMR_SR;

    const auto ETH_SW_RESET_TIMEOUT = 500us; // used by STM32 HAL
    Timer timeoutTimer;
    timeoutTimer.start();
    while(timeoutTimer.elapsed_time() < ETH_SW_RESET_TIMEOUT && (base->DMABMR & ETH_DMABMR_SR)) {}
    if(base->DMABMR & ETH_DMABMR_SR) {
        // Reset failed to complete within expected timeout.
        // Note: This is usually because of a missing RMII clock from the PHY.
        return ErrCode::TIMEOUT;
    }

    // Configure MDIO clock
    ETH_SetMDIOClockRange(base);

    // Configure MAC settings
    base->MACCR |= ETH_MACCR_APCS_Msk; // Strip padding and CRC from frames
    base->MACFFR = ETH_MACFFR_HPF_Msk | ETH_MACFFR_HM_Msk; // Use perfect and hash filters for multicast

    // Configure DMA settings. Default STM32CubeHAL settings used.
    base->DMAOMR = ETH_DMAOMR_RSF_Msk |
        ETH_DMAOMR_TSF_Msk;

    base->DMABMR = ETH_DMABMR_AAB_Msk |
        ETH_DMABMR_USP_Msk |
        ETH_DMABMR_RDP_32Beat |
        ETH_DMABMR_FB_Msk |
        ETH_DMABMR_PBL_32Beat |
        ETH_DMABMR_EDE_Msk;

    // Set up interrupt handler
    NVIC_SetVector(ETH_IRQn, reinterpret_cast<uint32_t>(&STM32EthMACv1::irqHandler));
    HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
    HAL_NVIC_EnableIRQ(ETH_IRQn);

    // Enable Tx, Rx, and fatal bus error interrupts.
    // However, don't enable receive buffer unavailable interrupt, because that can
    // trigger if we run out of Rx descriptors, and we don't want to fatal error
    // in that case.
    base->DMAIER = ETH_DMAIER_NISE | ETH_DMAIER_RIE | ETH_DMAIER_TIE | ETH_DMAIER_FBEIE | ETH_DMAIER_AISE;
}

CompositeEMAC::ErrCode STM32EthMACv1::MACDriver::deinit() {
    // Disable interrupt
    HAL_NVIC_DisableIRQ(ETH_IRQn);

    // Unlock deep sleep
    sleep_manager_unlock_deep_sleep();

    // Unmap pins and turn off clock
    EthDeinitPinmappings();

    return ErrCode::SUCCESS;
}

CompositeEMAC::ErrCode STM32EthMACv1::MACDriver::enable(LinkSpeed speed, Duplex duplex) {
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

CompositeEMAC::ErrCode STM32EthMACv1::MACDriver::disable() {
    base->MACCR &= ~(ETH_MACCR_TE | ETH_MACCR_RE);

    // Note: Don't flush Tx FIFO because of STM32F7 errata 2.21.3, which can cause
    // the MAC to get stuck if the Tx FIFO is flushed at the wrong time

    return ErrCode::SUCCESS;
}

void STM32EthMACv1::MACDriver::setOwnMACAddr(const MACAddress &ownAddress) {
    // Set MAC address
    writeMACAddress(ownAddress, &base->MACA0HR, &base->MACA0LR);
}

CompositeEMAC::ErrCode STM32EthMACv1::MACDriver::mdioRead(uint8_t devAddr, uint8_t regAddr, uint16_t &result) {
    // This code based on HAL_ETH_ReadPHYRegister()
    if(base->MACMIIAR & ETH_MACMIIAR_MB_Msk) {
        // MDIO operation already in progress
        return ErrCode::INVALID_USAGE;
    }

    uint32_t tmpreg = base->MACMIIAR;

    /* Prepare the MDIO Address Register value
     - Set the PHY device address
     - Set the PHY register address
     - Set the read mode
     - Set the MII Busy bit */
    tmpreg &= ~(ETH_MACMIIAR_MW_Msk | ETH_MACMIIAR_PA_Msk | ETH_MACMIIAR_MR_Msk);
    tmpreg |= (devAddr << ETH_MACMIIAR_PA_Pos) | (regAddr << ETH_MACMIIAR_MR_Pos) | ETH_MACMIIAR_MB_Msk;
    base->MACMIIAR = tmpreg;

    Timer timeoutTimer;
    timeoutTimer.start();
    while(timeoutTimer.elapsed_time() < MDIO_TRANSACTION_TIMEOUT && (base->MACMIIAR & ETH_MACMIIAR_MB_Msk)) {}
    if(base->MACMIIAR & ETH_MACMIIAR_MB_Msk) {
        // Transaction failed to complete within expected timeout
        return ErrCode::TIMEOUT;
    }

    // Get result
    result = base->MACMIIDR;

    tr_info("MDIO read devAddr %" PRIu8 ", regAddr 0x%" PRIx8 " -> 0x%" PRIx16, devAddr, regAddr, result);

    return ErrCode::SUCCESS;
}

CompositeEMAC::ErrCode STM32EthMACv1::MACDriver::mdioWrite(uint8_t devAddr, uint8_t regAddr, uint16_t data) {
    // This code based on HAL_ETH_WritePHYRegister()
    if(base->MACMIIAR & ETH_MACMIIAR_MB_Msk) {
        // MDIO operation already in progress
        return ErrCode::INVALID_USAGE;
    }

    /* Give the value to the MII data register */
    base->MACMIIDR = data;

    uint32_t tmpreg = base->MACMIIAR;

    /* Prepare the MDIO Address Register value
     - Set the PHY device address
     - Set the PHY register address
     - Set the write mode
     - Set the MII Busy bit */
    tmpreg &= ~(ETH_MACMIIAR_MW_Msk | ETH_MACMIIAR_PA_Msk | ETH_MACMIIAR_MR_Msk);
    tmpreg |= (devAddr << ETH_MACMIIAR_PA_Pos) | (regAddr << ETH_MACMIIAR_MR_Pos) | ETH_MACMIIAR_MB_Msk | ETH_MACMIIAR_MW_Msk;
    base->MACMIIAR = tmpreg;

    Timer timeoutTimer;
    timeoutTimer.start();
    while(timeoutTimer.elapsed_time() < MDIO_TRANSACTION_TIMEOUT && (base->MACMIIAR & ETH_MACMIIAR_MB_Msk)) {}
    if(base->MACMIIAR & ETH_MACMIIAR_MB_Msk) {
        // Transaction failed to complete within expected timeout
        return ErrCode::TIMEOUT;
    }

    tr_debug("MDIO write devAddr %" PRIu8 ", regAddr 0x%" PRIx8 " <- 0x%" PRIx16, devAddr, regAddr, data);

    return ErrCode::SUCCESS;
}

PinName STM32EthMACv1::MACDriver::getPhyResetPin() {
    return EthGetPhyResetPin();
}

CompositeEMAC::ErrCode STM32EthMACv1::MACDriver::addMcastMAC(MACAddress mac) {
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

CompositeEMAC::ErrCode STM32EthMACv1::MACDriver::clearMcastFilter() {
    // Reset perfect filter registers
    for(auto regPair : MAC_ADDR_PERF_FILTER_REGS) {
        *regPair.first = 0;
        *regPair.second = 0;
    }
    numPerfectFilterRegsUsed = 0;

    // Reset hash filter
    base->MACHTLR = 0;
    base->MACHTHR = 0;

    return ErrCode::SUCCESS;
}

void STM32EthMACv1::MACDriver::setPassAllMcast(bool pass) {
    if(pass)
    {
        base->MACFFR |= ETH_MACFFR_PAM_Msk;
    }
    else
    {
        base->MACFFR &= ~ETH_MACFFR_PAM_Msk;
    }
}

void STM32EthMACv1::MACDriver::setPromiscuous(bool enable) {
    if(enable)
    {
        base->MACFFR |= ETH_MACFFR_PM_Msk;
    }
    else
    {
        base->MACFFR &= ~ETH_MACFFR_PM_Msk;
    }
}
}
