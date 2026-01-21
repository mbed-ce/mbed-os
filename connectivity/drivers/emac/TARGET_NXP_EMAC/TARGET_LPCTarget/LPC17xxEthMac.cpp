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

#include "LPC17xxEthMac.h"

#include "DigitalOut.h"
#include "LPC17xxEthMacRegisters.h"
#include "MbedCRC.h"
#include "mbed_error.h"

namespace mbed {
    LPC17xxEthMAC * LPC17xxEthMAC::instance;

    constexpr uint32_t CLKPWR_PCONP_PCENET = 1<<30;

    CompositeEMAC::ErrCode LPC17xxEthMAC::MACDriver::init() {
        /* Enable MII clocking */
        LPC_SC->PCONP |= CLKPWR_PCONP_PCENET;

        // Enable Ethernet clock
        if(ETH_CLOCK_ENABLE != NC) {
            ethClockEnablePin = 1;
        }

        /* Enable P1 Ethernet Pins. */
        LPC_PINCON->PINSEL2 = 0x50150105;
        LPC_PINCON->PINSEL3 = (LPC_PINCON->PINSEL3 & ~0x0000000F) | 0x00000005;

        // Bring MAC out of reset
        base->MAC1 = 0;

        // Enable CRC offload and padding
        base->MAC2 = EMAC_MAC2_CRC_EN | EMAC_MAC2_PAD_EN;

        // Set max frame length (this field includes the CRC)
        base->MAXF = 1518;

        // Reset MII bus
        base->MCFG = EMAC_MCFG_RES_MII;

        // Bring MII out of reset and configure speed. Select host clock / 48, which gives 2.5MHz when CPU clock is
        // 120MHz and slightly slower when not in overdrive mode.
        base->MCFG = EMAC_MCFG_CLK_SEL(0b1011);

        // Set default collision timings (which... aren't applied by default for some reason...)
        base->IPGR = EMAC_IPGR_P1_DEF | EMAC_IPGR_P2_DEF;

        // Enable RMII
        base->Command = EMAC_Command_RMII;

        // Accept matching unicasts (PERFECT), hash-matching multicasts, and broadcasts
        base->RxFilterCtrl = EMAC_RxFilterCtrl_PERFECT_EN | EMAC_RxFilterCtrl_MCAST_HASH_EN | EMAC_RxFilterCtrl_BCAST_EN;

        // Enable interrupts for packet Rx, packet Tx, and fatal errors
        base->IntEnable = EMAC_Int_RX_ERR | EMAC_Int_RX_DONE | EMAC_Int_TX_ERR | EMAC_Int_TX_DONE | EMAC_Int_RX_OVERRUN | EMAC_Int_TX_UNDERRUN;

        return ErrCode::SUCCESS;
    }

    CompositeEMAC::ErrCode LPC17xxEthMAC::MACDriver::deinit() {
        // Disable interrupts
        NVIC_DisableIRQ(ENET_IRQn);

        // Reset all MAC registers
        base->MAC1 = EMAC_MAC1_SOFT_RES;

        // Disable Ethernet peripheral
        LPC_SC->PCONP &= ~CLKPWR_PCONP_PCENET;

        // Disable Ethernet clock
        if(ETH_CLOCK_ENABLE != NC) {
            ethClockEnablePin = 0;
        }

        return ErrCode::SUCCESS;
    }

    CompositeEMAC::ErrCode LPC17xxEthMAC::MACDriver::enable(LinkSpeed speed, Duplex duplex) {
        // Configure speed and duplex
        if (duplex == Duplex::FULL) {
            base->MAC2 |= EMAC_MAC2_FULL_DUP;
            base->IPGT = EMAC_IPGT_FULL_DUP;
            base->Command |= EMAC_Command_FULL_DUP;
        }
        else {
            base->MAC2 &= ~EMAC_MAC2_FULL_DUP;
            base->IPGT = EMAC_IPGT_HALF_DUP;
            base->Command &= ~EMAC_Command_FULL_DUP;
        }
        base->SUPP = speed == LinkSpeed::LINK_100MBIT ? EMAC_SUPP_SPEED : 0;

        // Enable Rx in hardware
        base->MAC1 |= EMAC_MAC1_REC_EN;

        return ErrCode::SUCCESS;
    }

    CompositeEMAC::ErrCode LPC17xxEthMAC::MACDriver::disable() {
        // Disable Rx in hardware (tho we'll leave it enabled in DMA so packets don't get stuck)
        base->MAC1 &= ~EMAC_MAC1_REC_EN;

        // Disable Tx in DMA
        base->Command &= ~EMAC_Command_TX_EN;

        return ErrCode::SUCCESS;
    }

    void LPC17xxEthMAC::MACDriver::setOwnMACAddr(const MACAddress &ownAddress) {
        base->SA2 = (uint32_t) ownAddress[0] | (((uint32_t) ownAddress[1]) << 8);
        base->SA1 = (uint32_t) ownAddress[2] | (((uint32_t) ownAddress[3]) << 8);
        base->SA0 = (uint32_t) ownAddress[4] | (((uint32_t) ownAddress[5]) << 8);
    }

    CompositeEMAC::ErrCode LPC17xxEthMAC::MACDriver::mdioRead(uint8_t devAddr, uint8_t regAddr,
        uint16_t &result) {
        // Start the read
        base->MADR = EMAC_MADR_PHY_ADR(devAddr) | EMAC_MADR_REG_ADR(regAddr);
        base->MCMD = EMAC_MCMD_READ;

        // Wait for read to complete
        while (base->MIND & EMAC_MIND_BUSY) {}

        // Clear command
        base->MCMD = 0;

        // Check if the operation failed.
        // Note: I am rather confused what the "link fail" bit here does. Seems like it sometimes is set when
        // doing reads, but those reads seem to still work. The old driver code also ignored it.
        if (base->MIND & EMAC_MIND_NOT_VAL) {
            return ErrCode::HW_ERROR;
        }

        result = base->MRDD;
        return ErrCode::SUCCESS;
    }

    CompositeEMAC::ErrCode LPC17xxEthMAC::MACDriver::mdioWrite(uint8_t devAddr, uint8_t regAddr, uint16_t data) {
        // Start the write
        base->MCMD = 0;
        base->MADR = EMAC_MADR_PHY_ADR(devAddr) | EMAC_MADR_REG_ADR(regAddr);
        base->MWTD = data;

        // Wait for write to complete
        while (base->MIND & EMAC_MIND_BUSY) {}

        return ErrCode::SUCCESS;
    }

    PinName LPC17xxEthMAC::MACDriver::getPhyResetPin() {
        return ETH_PHY_RESET;
    }

    CompositeEMAC::ErrCode LPC17xxEthMAC::MACDriver::addMcastMAC(const MACAddress mac) {
        // Per reference manual section 10.17.10:
        // The standard Ethernet cyclic redundancy check (CRC) function is calculated from
        // the 6 byte destination address in the Ethernet frame (this CRC is calculated
        // anyway as part of calculating the CRC of the whole frame), then bits [28:23] out of
        // the 32-bit CRC result are taken to form the hash. The 6-bit hash is used to access
        // the hash table: it is used as an index in the 64-bit HashFilter register that has been
        // programmed with accept values. If the selected accept value is 1, the frame is
        // accepted.
        MbedCRC<POLY_32BIT_ANSI, 32> crcCalc;
        uint32_t crc;
        if (!crcCalc.compute(mac.data(), mac.size(), &crc) != 0) {
            return ErrCode::INVALID_USAGE;
        }

        // Grab correct index for hash table
        const uint8_t hashTableIndex = (crc >> 23) & 0b111111;

        // Set correct bit in hash table
        if (hashTableIndex >= 32) {
            base->HashFilterH |= 1 << (hashTableIndex - 32);
        }
        else {
            base->HashFilterL |= 1 << hashTableIndex;
        }

        return ErrCode::SUCCESS;
    }

    CompositeEMAC::ErrCode LPC17xxEthMAC::MACDriver::clearMcastFilter() {
        base->HashFilterH = 0;
        base->HashFilterL = 0;
        return ErrCode::SUCCESS;
    }

    void LPC17xxEthMAC::MACDriver::setPassAllMcast(const bool pass) {
        passAllMcastEnabled = pass;
        if(pass) {
            base->RxFilterCtrl |= EMAC_RxFilterCtrl_MCAST_EN;
        }
        else if(!promiscuousEnabled){ // don't disable pass all unicast if we are in promiscuous mode
            base->RxFilterCtrl &= ~EMAC_RxFilterCtrl_MCAST_EN;
        }
    }

    void LPC17xxEthMAC::MACDriver::setPromiscuous(bool enable) {
        promiscuousEnabled = enable;

        // To enable promiscuous mode on this MAC, we need to enable pass all multicast and pass all unicast.
        if(enable) {
            base->RxFilterCtrl |= EMAC_RxFilterCtrl_MCAST_EN | EMAC_RxFilterCtrl_UCAST_EN;
        }
        else {
            base->RxFilterCtrl &= ~EMAC_RxFilterCtrl_UCAST_EN;

            // Only disable the MCAST_EN bit if we aren't in pass-all-mcast mode
            if(!passAllMcastEnabled) {
                base->RxFilterCtrl &= ~EMAC_RxFilterCtrl_MCAST_EN;
            }
        }
    }

    void LPC17xxEthMAC::TxDMA::startDMA() {
        /* Setup pointers to TX structures */
        base->TxDescriptor = reinterpret_cast<uint32_t>(txDescs);
        base->TxStatus = reinterpret_cast<uint32_t>(txStatusDescs);
        base->TxDescriptorNumber = TX_NUM_DESCS;

        // We're going to produce index 0 next
        base->TxProduceIndex = 0;

        // Enable Tx DMA
        base->Command |= EMAC_Command_TX_EN;
    }

    void LPC17xxEthMAC::TxDMA::stopDMA() {
        // Disable Tx DMA
        base->Command &= ~EMAC_Command_TX_EN;

        // Reset Tx DMA (hopefully will clear out any partial state)
        base->Command |= EMAC_Command_TX_RES;
    }

    bool LPC17xxEthMAC::TxDMA::descOwnedByDMA(size_t descIdx) {

        // Read registers once (since they might change during the function call)
        const uint32_t consumeIdx = base->TxConsumeIndex;
        const uint32_t produceIdx = base->TxProduceIndex;

        // Case 1: produce index > consume index (no wrapping)
        if(produceIdx > consumeIdx) {
            // Examples:
            // If TxConsumeIndex = 3 and TxProduceIndex = 5, descIdxes 3 and 4 are owned by DMA
            const bool ownedByDMA = descIdx >= consumeIdx && descIdx < produceIdx;
            return ownedByDMA;
        }
        // Case 2: produce index < consume index (wrapping)
        else {
            // Examples:
            // If TX_NUM_DESCS = 8, TxConsumeIndex = 6 and TxProduceIndex = 2, descIdxes 6, 7, 0, and 1 are owned by DMA
            const bool ownedByDMA = descIdx >= consumeIdx || descIdx < produceIdx;
            return ownedByDMA;
        }
    }

    bool LPC17xxEthMAC::TxDMA::isDMAReadableBuffer(uint8_t const *start, size_t size) const {
        // Note: the datasheet recommends using Ethernet with only AHBSRAM buffers, but this is only
        // a recommendation for best performance, not an actual restriction of the HW
        return true;
    }

    void LPC17xxEthMAC::TxDMA::giveToDMA(size_t descIdx, uint8_t const *buffer, size_t len, bool firstDesc,
        bool lastDesc) {
        // Fill in descriptor
        txDescs[descIdx].data = buffer;
        txDescs[descIdx].size = len;
        txDescs[descIdx].interrupt = true;
        txDescs[descIdx].lastDescriptor = lastDesc;

        // Have DMA process it
        base->TxProduceIndex = (descIdx + 1) % TX_NUM_DESCS;
    }

    void LPC17xxEthMAC::RxDMA::startDMA() {
        /* Setup pointers to RX structures */
        base->RxDescriptor = reinterpret_cast<uint32_t>(rxDescs);
        base->RxStatus = reinterpret_cast<uint32_t>(rxStatusDescs);
        base->RxDescriptorNumber = RX_NUM_DESCS;

        // We're going to consume index 0 next
        base->RxConsumeIndex = 0;

        // Enable Rx DMA
        base->Command |= EMAC_Command_RX_EN;
    }

    void LPC17xxEthMAC::RxDMA::stopDMA() {
        // Disable Rx DMA
        base->Command &= ~EMAC_Command_RX_EN;

        // Reset Rx DMA (hopefully will clear out any partial state)
        base->Command |= EMAC_Command_RX_RES;
    }

    bool LPC17xxEthMAC::RxDMA::descOwnedByDMA(const size_t descIdx) {

        // Read registers once (since they might change during the function call)
        const uint32_t consumeIdx = base->TxConsumeIndex;
        const uint32_t produceIdx = base->TxProduceIndex;

        // Case 1: produce index > consume index (no wrapping)
        if(produceIdx > consumeIdx) {
            // Examples:
            // If RxConsumeIndex = 3 and RxProduceIndex = 5, descIdxes 3 and 4 are owned by us
            const bool ownedByMCU = descIdx >= consumeIdx && descIdx < produceIdx;
            return !ownedByMCU;
        }
        // Case 2: produce index < consume index (wrapping)
        else {
            // Examples:
            // If RX_NUM_DESCS = 8, RxConsumeIndex = 6 and RxProduceIndex = 2, descIdxes 6, 7, 0, and 1 are owned by us
            const bool ownedByMCU = descIdx >= consumeIdx || descIdx < produceIdx;
            return !ownedByMCU;
        }
    }

    bool LPC17xxEthMAC::RxDMA::isFirstDesc(const size_t descIdx) {
        return false; // Not supported
    }

    bool LPC17xxEthMAC::RxDMA::isLastDesc(const size_t descIdx) {
        return rxStatusDescs[descIdx].lastDescriptor;
    }

    bool LPC17xxEthMAC::RxDMA::isErrorDesc(const size_t descIdx) {
        return rxStatusDescs[descIdx].hadCRCError ||
            rxStatusDescs[descIdx].hadSymbolError ||
            rxStatusDescs[descIdx].alignmentError ||
            rxStatusDescs[descIdx].rxOverrun ||
            rxStatusDescs[descIdx].noDescriptorError;
    }

    void LPC17xxEthMAC::RxDMA::returnDescriptor(const size_t descIdx, uint8_t * const buffer) {
        rxDescs[descIdx].data = buffer;
        rxDescs[descIdx].size = rxPoolPayloadSize;
        rxDescs[descIdx].interrupt = true;
        base->RxConsumeIndex = (descIdx + 1) % RX_NUM_DESCS;
    }

    size_t LPC17xxEthMAC::RxDMA::getTotalLen(const size_t firstDescIdx, const size_t lastDescIdx) {
        size_t totalSize = 0;
        for(size_t descIdx = firstDescIdx; descIdx != (lastDescIdx + 1) % RX_NUM_DESCS; descIdx = (descIdx + 1) % RX_NUM_DESCS) {
            totalSize += rxStatusDescs[descIdx].actualSize + 1; // size is minus 1 encoded
        }
        return totalSize;
    }

    LPC17xxEthMAC::LPC17xxEthMAC(LPC_EMAC_TypeDef * const base):
    CompositeEMAC(txDMA, rxDMA, macDriver),
    base(base),
    txDMA(base),
    rxDMA(base),
    macDriver(base)
    {
        instance = this;
    }

    void LPC17xxEthMAC::irqHandler() {
        const auto emacInst = instance;

        /* Packet received */
        if(emacInst->base->IntStatus & (EMAC_Int_RX_DONE | EMAC_Int_RX_ERR))
        {
            // Clear Rx interrupts
            emacInst->base->IntStatus = EMAC_Int_RX_DONE | EMAC_Int_RX_ERR;

            emacInst->rxISR();
        }

        /* Packet transmitted */
        if(emacInst->base->IntStatus & (EMAC_Int_TX_DONE | EMAC_Int_TX_ERR))
        {
            // Clear Tx interrupts
            emacInst->base->IntStatus = EMAC_Int_TX_DONE | EMAC_Int_TX_ERR;

            emacInst->txISR();
        }

        // Check for fatal underrun/overrun errors
        if(emacInst->base->IntStatus & (EMAC_Int_RX_OVERRUN | EMAC_Int_TX_UNDERRUN))
        {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_ETHERNET, EIO), \
                   "LPC17xx EMAC: Hardware reports fatal DMA error\n");
        }
    }
}

// Provide default EMAC driver
MBED_WEAK EMAC &EMAC::get_default_instance()
{
    static mbed::LPC17xxEthMAC emac(LPC_EMAC);
    return emac;
}
