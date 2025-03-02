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
#include "mbed_error.h"
#include "mbed_events.h"
#include "CriticalSectionLock.h"

#define TRACE_GROUP "STEMACv1"

using namespace std::chrono_literals;

// Defined in stm32_eth_init.c
extern "C" void EthInitPinmappings();
extern "C" void EthDeinitPinmappings();
extern "C" PinName EthGetPhyResetPin();

namespace mbed {
void STM32EthMACv1::TxDMA::startDMA() {
    // Zero all the Tx descriptors
    memset(txDescs.data(), 0, sizeof(stm32_ethv1::TxDescriptor) * TX_NUM_DESCS);

    // Set the end-of-ring bit on the last descriptor
    txDescs[TX_NUM_DESCS - 1].endOfRing = true;

    // Set descriptor list address register
    base->DMATDLAR = reinterpret_cast<ptrdiff_t>(&txDescs[0]);

    // Start Tx DMA
    base->DMAOMR |= ETH_DMAOMR_ST_Msk;
}

void STM32EthMACv1::TxDMA::stopDMA() {
    base->DMAOMR &= ~ETH_DMAOMR_ST_Msk;
}

#if __DCACHE_PRESENT
void STM32EthMACv1::TxDMA::cacheInvalidateDescriptor(const size_t descIdx) {
    SCB_InvalidateDCache_by_Addr(&txDescs[descIdx], sizeof(stm32_ethv1::TxDescriptor));
}
#endif

bool STM32EthMACv1::TxDMA::descOwnedByDMA(size_t descIdx) {
    return txDescs[descIdx].dmaOwn;
}

bool STM32EthMACv1::TxDMA::isDMAReadableBuffer(uint8_t const *start, size_t size) const {
#ifdef TARGET_STM32F7
    if(bufferTouchesMemoryBank(start, size, 0, 1024*16)) {
        // In ITCM memory, not accessible by DMA. Note that ITCM is not included in the CMSIS memory map (yet).
        return false;
    }
#endif

#if TARGET_STM32F2 || TARGET_STM32F4
    // On STM32F2 and F2, ethernet DMA cannot access the flash memory.
    if(bufferTouchesMemoryBank(start, size, MBED_ROM_START, MBED_ROM_SIZE)) {
        return false;
    }
#endif

    return true;
}

void STM32EthMACv1::TxDMA::giveToDMA(size_t descIdx, uint8_t const *buffer, size_t len, bool firstDesc, bool lastDesc) {
    // Configure descriptor with buffer and size
    txDescs[descIdx].buffer1 = buffer;
    txDescs[descIdx].buffer1Size = len;
    txDescs[descIdx].firstSegment = firstDesc;
    txDescs[descIdx].lastSegment = lastDesc;
    txDescs[descIdx].intrOnComplete = true;

    // Return to DMA
    txDescs[descIdx].dmaOwn = true;

    // Flush back to main memory
#ifdef __DCACHE_PRESENT
    SCB_CleanDCache_by_Addr(&txDescs[descIdx], sizeof(stm32_ethv1::TxDescriptor));
#else
    __DMB(); // Make sure descriptor is written before the below lines
#endif

    // Clear buffer unavailable flag (I think this is for information only though)
    base->DMASR = ETH_DMASR_TBUS_Msk;

    // Demand (good sir!) a Tx descriptor poll
    base->DMATPDR = 1;
}

void STM32EthMACv1::RxDMA::startDMA() {

    // Rx buffer size must be a multiple of 4, per the descriptor definition
    MBED_ASSERT(rxPoolPayloadSize % sizeof(uint32_t) == 0);

    // Zero all the Rx descriptors
    memset(rxDescs.data(), 0, sizeof(stm32_ethv1::RxDescriptor) * RX_NUM_DESCS);

    // Set the end-of-ring bit on the last descriptor
    rxDescs[RX_NUM_DESCS - 1].endOfRing = true;

    // Set descriptor list address register
    base->DMARDLAR = reinterpret_cast<ptrdiff_t>(&rxDescs[0]);

    // Start Rx DMA
    base->DMAOMR |= ETH_DMAOMR_SR_Msk;
}

void STM32EthMACv1::RxDMA::stopDMA() {
    base->DMAOMR &= ~ETH_DMAOMR_SR_Msk;
}

#if __DCACHE_PRESENT
void STM32EthMACv1::RxDMA::cacheInvalidateDescriptor(const size_t descIdx) {
    SCB_InvalidateDCache_by_Addr(&rxDescs[descIdx], sizeof(stm32_ethv1::RxDescriptor));
}
#endif

bool STM32EthMACv1::RxDMA::descOwnedByDMA(const size_t descIdx) {
    return rxDescs[descIdx].dmaOwn;
}

bool STM32EthMACv1::RxDMA::isFirstDesc(const size_t descIdx) {
    return rxDescs[descIdx].firstDescriptor;
}

bool STM32EthMACv1::RxDMA::isLastDesc(const size_t descIdx) {
    return rxDescs[descIdx].lastDescriptor;
}

bool STM32EthMACv1::RxDMA::isErrorDesc(const size_t descIdx) {
    return rxDescs[descIdx].errSummary;
}

void STM32EthMACv1::RxDMA::returnDescriptor(const size_t descIdx, uint8_t *buffer)
{
    // Configure descriptor
    rxDescs[descIdx].buffer1 = buffer;
    rxDescs[descIdx].buffer1Size = rxPoolPayloadSize;
    rxDescs[descIdx].dmaOwn = true;

    // Flush back to main memory
#ifdef __DCACHE_PRESENT
    SCB_CleanDCache_by_Addr(&rxDescs[descIdx], sizeof(stm32_ethv1::RxDescriptor));
#else
    __DMB(); // Make sure descriptor is written before the below lines
#endif

    // Clear buffer unavailable flag (I think this is for information only though)
    base->DMASR = ETH_DMASR_RBUS_Msk;

    // Demand (good sir!) an Rx descriptor poll
    base->DMARPDR = 1;
}

size_t STM32EthMACv1::RxDMA::getTotalLen(const size_t firstDescIdx, const size_t lastDescIdx) {
    // Total length of the packet is in the last descriptor
    return rxDescs[lastDescIdx].frameLen;
}

void STM32EthMACv1::MACDriver::ETH_SetMDIOClockRange(ETH_TypeDef * const base) {
        /* Get the ETHERNET MACMIIAR value */
        uint32_t tempreg = base->MACMIIAR;
        /* Clear CSR Clock Range CR[2:0] bits */
        tempreg &= ETH_MACMIIAR_CR_Msk;

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
#ifdef ETH_MACMIIAR_CR_Div102
        else if((hclk >= 150000000)&&(hclk <= 216000000))
        {
            /* CSR Clock Range between 150-216 MHz */
            tempreg |= (uint32_t)ETH_MACMIIAR_CR_Div102;
        }
#endif
        else {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_ETHERNET, EIO), \
                   "STM32 EMAC v1: Unsupported HCLK range\n");
        }

        /* Write to ETHERNET MAC MIIAR: Configure the ETHERNET CSR Clock Range */
        base->MACMIIAR = (uint32_t)tempreg;
    }

#if ENABLE_ERRATA_2_21_6_WORKAROUND
void STM32EthMACv1::MACDriver::rmiiWatchdog() {
    // mbed_event_queue() is not ISR safe, so get the pointer before entering the critical section
    auto * const equeue = mbed_event_queue();

    CriticalSectionLock lock;

    if(!rmiiWatchdogRunning) {
        // Already canceled by main thread, bail
        return;
    }

    /* some good packets are received */
    if (base->MMCRGUFCR > 0) {
        /* RMII Init is OK - cancel watchdog task */
        equeue->cancel(rmiiWatchdogHandle);
        rmiiWatchdogRunning = false;
    } else if (base->MMCRFCECR > 10) {
        /* ETH received too many packets with CRC errors, resetting RMII */
        SYSCFG->PMC &= ~SYSCFG_PMC_MII_RMII_SEL;
        SYSCFG->PMC |= SYSCFG_PMC_MII_RMII_SEL;
        base->MMCCR |= ETH_MMCCR_CR;
    }
}
#endif

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
    base->MACCR |= (1 << 25); // Strip CRC from frames. CSTF bit definition missing from CMSIS header for some reason?
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

#if ENABLE_ERRATA_2_21_6_WORKAROUND
    // Start RMII watchdog task
    rmiiWatchdogHandle = mbed_event_queue()->call_every(std::chrono::milliseconds(MBED_CONF_NSAPI_EMAC_PHY_POLL_PERIOD),
                callback(this, &STM32EthMACv1::MACDriver::rmiiWatchdog));
    rmiiWatchdogRunning = true;
#endif

    return CompositeEMAC::ErrCode::SUCCESS;
}

CompositeEMAC::ErrCode STM32EthMACv1::MACDriver::deinit() {
    // Disable interrupt
    HAL_NVIC_DisableIRQ(ETH_IRQn);

#if ENABLE_ERRATA_2_21_6_WORKAROUND
    // Disable RMII watchdog if still running
    if(rmiiWatchdogRunning) {
        // mbed_event_queue() is not ISR safe, so get the pointer before entering the critical section
        auto * const equeue = mbed_event_queue();

        CriticalSectionLock lock;
        if(rmiiWatchdogRunning) { // Recheck flag inside critical section
            equeue->cancel(rmiiWatchdogHandle);
            rmiiWatchdogRunning = false;
        }
    }
#endif

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

    tr_debug("MDIO read devAddr %" PRIu8 ", regAddr 0x%" PRIx8 " -> 0x%" PRIx16, devAddr, regAddr, result);

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

STM32EthMACv1 * STM32EthMACv1::instance = nullptr;

STM32EthMACv1::STM32EthMACv1():
CompositeEMAC(txDMA, rxDMA, macDriver),
base(ETH),
txDMA(base),
rxDMA(base),
macDriver(base)
{
    instance = this;
}

void STM32EthMACv1::irqHandler() {
    const auto emacInst = instance;
    uint32_t dma_flag = emacInst->base->DMASR;

    /* Packet received */
    if ((dma_flag & ETH_DMASR_RS_Msk) != 0U)
    {
        /* Clear the Eth DMA Rx IT pending bits */
        ETH->DMASR = ETH_DMASR_RS_Msk | ETH_DMASR_NIS_Msk;

        emacInst->rxISR();
    }

    /* Packet transmitted */
    if ((dma_flag & ETH_DMASR_TS_Msk) != 0U)
    {
        /* Clear the Eth DMA Tx IT pending bits */
        ETH->DMASR = ETH_DMASR_TS_Msk | ETH_DMASR_NIS_Msk;

        emacInst->txISR();
    }

    /* ETH DMA Error */
    if(dma_flag & ETH_DMASR_FBES_Msk)
    {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_ETHERNET, EIO), \
               "STM32 EMAC v1: Hardware reports fatal DMA error\n");
    }
}
}

// Provide default EMAC driver
MBED_WEAK EMAC &EMAC::get_default_instance()
{
    static mbed::STM32EthMACv1 emac;
    return emac;
}