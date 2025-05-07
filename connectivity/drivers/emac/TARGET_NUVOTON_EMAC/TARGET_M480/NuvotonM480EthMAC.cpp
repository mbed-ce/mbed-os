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

#include "NuvotonM480EthMAC.h"

#include <mbed_power_mgmt.h>
#include <mbed_error.h>

#include "m480_eth_pins.h"

namespace mbed {

void NuvotonM480EthMAC::MACDriver::writeMACAddress(size_t index, MACAddress macAddress) {
    // Find the registers to write the MAC into. Sadly they didn't use an array...
    volatile uint32_t * highReg = (&base->CAM0M) + 2 * index;
    volatile uint32_t * lowReg = (&base->CAM0L) + 2 * index;

    // Write the MAC into the registers.
    *highReg = (static_cast<uint32_t>(macAddress[0]) << 24) | (static_cast<uint32_t>(macAddress[1]) << 16) | (static_cast<uint32_t>(macAddress[2]) << 8) | macAddress[3];
    *lowReg = (static_cast<uint32_t>(macAddress[4]) << 24) | (static_cast<uint32_t>(macAddress[5]) << 16);

    // Mark the address as valid
    base->CAMEN |= (1 << index);
}

CompositeEMAC::ErrCode NuvotonM480EthMAC::MACDriver::init() {
    sleep_manager_lock_deep_sleep();
    nu_eth_clk_and_pin_init();

    // Reset MAC
    base->CTL = EMAC_CTL_RST_Msk;
    while (base->CTL & EMAC_CTL_RST_Msk) {}

    // Reset class vars
    numMulticastSubscriptions = 0;
    passAllMcastEnabled = false;
    promiscuousEnabled = false;

    /* Configure the MAC interrupt enable register. Note that we need to enable interrupts for all types
     * of Rx errors, so that we know when any Rx descriptor has been freed up by the DMA. */
    base->INTEN = EMAC_INTEN_RXIEN_Msk |
                  EMAC_INTEN_TXIEN_Msk |
                  EMAC_INTEN_RXGDIEN_Msk |
                  EMAC_INTEN_TXCPIEN_Msk |
                  EMAC_INTEN_RXBEIEN_Msk |
                  EMAC_INTEN_TXBEIEN_Msk |
                  EMAC_INTEN_CRCEIEN_Msk |
                  EMAC_INTEN_RXOVIEN_Msk |
                  EMAC_INTEN_ALIEIEN_Msk |
                  EMAC_INTEN_RPIEN_Msk |
                  EMAC_INTEN_MFLEIEN_Msk;

    /* Enable interrupts. */
    NVIC_SetVector(EMAC_RX_IRQn, reinterpret_cast<uint32_t>(&NuvotonM480EthMAC::rxIrqHandler));
    NVIC_EnableIRQ(EMAC_RX_IRQn);
    NVIC_SetVector(EMAC_TX_IRQn, reinterpret_cast<uint32_t>(&NuvotonM480EthMAC::txIrqHandler));
    NVIC_EnableIRQ(EMAC_TX_IRQn);

    /* Configure the MAC control register. */
    base->CTL = EMAC_CTL_STRIPCRC_Msk | EMAC_CTL_RMIIEN_Msk;

    /* Accept broadcast packets without using the address filter */
    base->CAMCTL = EMAC_CAMCTL_CMPEN_Msk |
                    EMAC_CAMCTL_ABP_Msk;

    // Maximum frame length.
    // This apparently includes the CRC, so we need to set this 4 bytes higher than the MTU of 1514 bytes
    // or 1514 byte packets get rejected
    base->MRFL = 1518;

    /* Set RX FIFO threshold as 8 words */
    base->FIFOCTL = 0x00200100;

    return ErrCode::SUCCESS;
}

CompositeEMAC::ErrCode NuvotonM480EthMAC::MACDriver::deinit() {
    NVIC_DisableIRQ(EMAC_RX_IRQn);
    NVIC_DisableIRQ(EMAC_TX_IRQn);

    nu_eth_clk_and_pin_deinit();

    sleep_manager_unlock_deep_sleep();

    return ErrCode::SUCCESS;
}

CompositeEMAC::ErrCode NuvotonM480EthMAC::MACDriver::enable(LinkSpeed speed, Duplex duplex) {
    if(speed == LinkSpeed::LINK_100MBIT) {
        base->CTL |= EMAC_CTL_OPMODE_Msk;
    }
    else {
        base->CTL &= ~EMAC_CTL_OPMODE_Msk;
    }

    if(duplex == Duplex::FULL) {
        base->CTL |= EMAC_CTL_FUDUP_Msk;
    }
    else {
        base->CTL &= ~EMAC_CTL_FUDUP_Msk;
    }

    base->CTL |= EMAC_CTL_RXON_Msk | EMAC_CTL_TXON_Msk;

    return ErrCode::SUCCESS;
}

CompositeEMAC::ErrCode NuvotonM480EthMAC::MACDriver::disable() {
    base->CTL &= ~(EMAC_CTL_RXON_Msk | EMAC_CTL_TXON_Msk);

    return ErrCode::SUCCESS;
}

void NuvotonM480EthMAC::MACDriver::setOwnMACAddr(const MACAddress &ownAddress) {
    writeMACAddress(0, ownAddress);
}

CompositeEMAC::ErrCode NuvotonM480EthMAC::MACDriver::mdioRead(uint8_t devAddr, uint8_t regAddr, uint16_t &result) {
    base->MIIMCTL = (devAddr << EMAC_MIIMCTL_PHYADDR_Pos) | regAddr | EMAC_MIIMCTL_BUSY_Msk | EMAC_MIIMCTL_MDCON_Msk;
    while (base->MIIMCTL & EMAC_MIIMCTL_BUSY_Msk);
    result = base->MIIMDAT;

    return ErrCode::SUCCESS;
}

CompositeEMAC::ErrCode NuvotonM480EthMAC::MACDriver::mdioWrite(uint8_t devAddr, uint8_t regAddr, uint16_t data) {
    base->MIIMDAT = data;
    base->MIIMCTL = (devAddr << EMAC_MIIMCTL_PHYADDR_Pos) | regAddr | EMAC_MIIMCTL_BUSY_Msk | EMAC_MIIMCTL_WRITE_Msk | EMAC_MIIMCTL_MDCON_Msk;

    while (base->MIIMCTL & EMAC_MIIMCTL_BUSY_Msk);
    return ErrCode::SUCCESS;
}

PinName NuvotonM480EthMAC::MACDriver::getPhyResetPin() {
    return nu_eth_get_phy_reset_pin();
}

CompositeEMAC::ErrCode NuvotonM480EthMAC::MACDriver::addMcastMAC(MACAddress mac) {
    if(numMulticastSubscriptions >= 14) {
        // 14 is the max we can handle in hardware
        return ErrCode::OUT_OF_MEMORY;
    }
    // We use MAC slots 1 through 14 for the multicast subscriptions
    ++numMulticastSubscriptions;
    writeMACAddress(numMulticastSubscriptions, mac);

    return ErrCode::SUCCESS;
}

CompositeEMAC::ErrCode NuvotonM480EthMAC::MACDriver::clearMcastFilter() {
    // Disable all MAC addresses except CAM0, which is our own unicast MAC
    base->CAMEN = 1;

    return ErrCode::SUCCESS;
}

void NuvotonM480EthMAC::MACDriver::setPassAllMcast(bool pass) {
    passAllMcastEnabled = pass;
    if(pass) {
        base->CAMCTL |= EMAC_CAMCTL_AMP_Msk;
    }
    else if(!promiscuousEnabled){
        base->CAMCTL &= ~EMAC_CAMCTL_AMP_Msk;
    }
}

void NuvotonM480EthMAC::MACDriver::setPromiscuous(bool enable) {
    promiscuousEnabled = enable;

    // To enable promiscuous mode on this MAC, we need to enable pass all multicast and pass all unicast.
    if(enable) {
        base->CAMCTL |= EMAC_CAMCTL_AMP_Msk | EMAC_CAMCTL_AUP_Msk;
    }
    else {
        base->CAMCTL &= ~EMAC_CAMCTL_AUP_Msk;

        // Only disable the AMP bit if we aren't in pass-all-mcast mode
        if(!passAllMcastEnabled) {
            base->CAMCTL &= ~EMAC_CAMCTL_AMP_Msk;
        }
    }
}

void NuvotonM480EthMAC::TxDMA::startDMA() {
    // Set linked list base address
    base->TXDSA = reinterpret_cast<uint32_t>(&txDescs[0]);
}

void NuvotonM480EthMAC::TxDMA::stopDMA() {
    // No specific disable for DMA. DMA will get disabled when the MAC is disabled.
}

bool NuvotonM480EthMAC::TxDMA::descOwnedByDMA(size_t descIdx) {
    return txDescs[descIdx].EMAC_OWN;
}

bool NuvotonM480EthMAC::TxDMA::isDMAReadableBuffer(uint8_t const *start, size_t size) const {
    // No restrictions on what DMA can read
    return true;
}

void NuvotonM480EthMAC::TxDMA::giveToDMA(size_t descIdx, uint8_t const *buffer, size_t len, bool firstDesc,
    bool lastDesc) {

    // Populate Tx descriptor fields
    txDescs[descIdx].PADEN = true;
    txDescs[descIdx].CRCAPP = true;
    txDescs[descIdx].INTEN = true;
    txDescs[descIdx].TXBSA = buffer;
    txDescs[descIdx].TBC = len;
    txDescs[descIdx].NTXDSA = &txDescs[(descIdx + 1) % TX_NUM_DESCS];

    // Give to DMA
    txDescs[descIdx].EMAC_OWN = true;

    // Tell DMA to start writing if stopped
    base->TXST = 1;
}

void NuvotonM480EthMAC::RxDMA::startDMA() {
    // Set linked list base address
    base->RXDSA = reinterpret_cast<uint32_t>(&rxDescs[0]);
}

void NuvotonM480EthMAC::RxDMA::stopDMA() {
    // No specific disable for DMA. DMA will get disabled when the MAC is disabled.
}

bool NuvotonM480EthMAC::RxDMA::descOwnedByDMA(size_t descIdx) {
    return rxDescs[descIdx].EMAC_OWN;
}

// The M480 EMAC enforces a 1:1 descriptor to packet relationship, so every desc is always a first and last desc.
bool NuvotonM480EthMAC::RxDMA::isFirstDesc(size_t descIdx) {
    return true;
}
bool NuvotonM480EthMAC::RxDMA::isLastDesc(size_t descIdx) {
    return true;
}

bool NuvotonM480EthMAC::RxDMA::isErrorDesc(size_t descIdx) {
    // If it's not a good frame, then it's an error.
    return !(rxDescs[descIdx].RXGDIF);
}

void NuvotonM480EthMAC::RxDMA::returnDescriptor(size_t descIdx, uint8_t *buffer) {
    // Populate descriptor
    rxDescs[descIdx].RXBSA = buffer;
    rxDescs[descIdx].NRXDSA = &rxDescs[(descIdx + 1) % RX_NUM_DESCS];

    // Give to DMA
    rxDescs[descIdx].EMAC_OWN = true;

    // Tell DMA to start receiving if stopped
    base->RXST = 1;
}

size_t NuvotonM480EthMAC::RxDMA::getTotalLen(size_t firstDescIdx, size_t lastDescIdx) {
    return rxDescs[firstDescIdx].RBC;
}

NuvotonM480EthMAC * NuvotonM480EthMAC::instance = nullptr;

NuvotonM480EthMAC::NuvotonM480EthMAC():
CompositeEMAC(txDMA, rxDMA, macDriver),
// Note: we can't use the "EMAC" symbol directly because it conflicts with the EMAC class. So we have to
// use the integer address and cast it instead.
base(reinterpret_cast<EMAC_T *>(EMAC_BASE)),
txDMA(base),
rxDMA(base),
macDriver(base)
{
    instance = this;
}

void NuvotonM480EthMAC::txIrqHandler() {
    const auto base = instance->base;
    if(base->INTSTS & EMAC_INTSTS_TXBEIF_Msk) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_ETHERNET, EIO), \
               "M480 EMAC: Hardware reports fatal DMA Tx bus error\n");
    }

    if(base->INTSTS & EMAC_INTSTS_TXCPIF_Msk) {
        // Transmission complete
        instance->txISR();

        // Clear flag
        base->INTSTS = EMAC_INTSTS_TXCPIF_Msk;
    }

    // Clear general Tx interrupt flag
    base->INTSTS = EMAC_INTSTS_TXIF_Msk;
}

void NuvotonM480EthMAC::rxIrqHandler() {
    const auto base = instance->base;
    if(base->INTSTS & EMAC_INTSTS_RXBEIF_Msk) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_ETHERNET, EIO), \
               "M480 EMAC: Hardware reports fatal DMA Rx bus error\n");
    }

    if(base->INTSTS & EMAC_INTSTS_RXIF_Msk) {
        // Frames(s) received (good or otherwise)
        instance->rxISR();

        // Clear flags
        base->INTSTS = EMAC_INTSTS_RXIF_Msk |
            EMAC_INTSTS_CRCEIF_Msk |
            EMAC_INTSTS_RXOVIF_Msk |
            EMAC_INTSTS_LPIF_Msk |
            EMAC_INTSTS_RXGDIF_Msk |
            EMAC_INTSTS_RPIF_Msk |
            EMAC_INTSTS_MFLEIF_Msk;
    }

    // Clear general Tx interrupt flag
    base->INTSTS = EMAC_INTSTS_TXIF_Msk;
}
}

// Provide default EMAC driver
MBED_WEAK EMAC &EMAC::get_default_instance()
{
    static mbed::NuvotonM480EthMAC emac;
    return emac;
}