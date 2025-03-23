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

#include "GenericEthPhy.h"
#include "CompositeEMAC.h"

#include <mbed_wait_api.h>
#include <mbed_trace.h>
#include <ThisThread.h>
#include <Timer.h>

#define TRACE_GROUP "GEPHY"

namespace mbed {

using namespace std::chrono_literals;

#define FORWARD_ERR(call) {auto const err_code = call; if(err_code != CompositeEMAC::ErrCode::SUCCESS) { return err_code; }}

CompositeEMAC::ErrCode GenericEthPhy::init() {

    // If we have a hardware reset pin, reset the PHY in hardware
    if(mac->getPhyResetPin() != NC) {
        // Output low on the reset pin, then bring high and wait
        resetDigitalOut.emplace(mac->getPhyResetPin(), 0);
        wait_us(config.resetLowTime);
        resetDigitalOut->write(1);
    }

    // Wait for the reset time to expire. Even if we didn't just actuate the hardware reset line,
    // this is important because some PHYs need a long power on reset time, and the board may have
    // just powered on.
    wait_us(config.resetHighTime);

    // Try and detect the phy.
    // ID1 should be the upper 18 MSBits of the OUI, with the two MSBits chopped off.
    const uint16_t expectedID1 = config.OUI >> 6;
    // Bits 10-15 of ID2 are the 6 LSBits of the OUI. Bits 4-9 are the model number. Bits 0-3 are the revision and may be anything.
    const uint16_t expectedID2 = (config.OUI << 10) | (config.model << 4);
    const uint16_t expectedID2Mask = 0xFFF0;

    // Read IDs
    uint16_t actualID1;
    uint16_t actualID2;
    FORWARD_ERR(mac->mdioRead(config.address, GenPhyRegs::PHYIDR1, actualID1));
    FORWARD_ERR(mac->mdioRead(config.address, GenPhyRegs::PHYIDR2, actualID2));

    if(actualID1 == expectedID1 && (actualID2 & expectedID2Mask) == expectedID2) {
        // OK
        tr_info("Detected ethernet PHY at MDIO addr %" PRIu8 " with OUI 0x%" PRIx32 ", model 0x%" PRIx8 ", and revision number %" PRIu8, config.address, config.OUI, config.model, actualID2 % 0xF);
    }
    else if(actualID1 == std::numeric_limits<uint16_t>::max() && actualID2 == std::numeric_limits<uint16_t>::max()) {
        tr_error("Got all 0xFFs when reading Ethernet PHY. Since MDIO is an open drain bus, this means the phy is not connected or not responding.");
        return CompositeEMAC::ErrCode::PHY_NOT_RESPONDING;
    }
    else {
        tr_error("Ethernet phy model number verification mismatch. Expected PHYIDR1 = %" PRIu16 ", PHYIDR2 = %" PRIu16 ", got PHYIDR1 = %" PRIu16 ", PHYIDR2 = %" PRIu16 " [note: bottom 4 bits of PHYIDR2 ignored]", expectedID1, expectedID2, actualID1, actualID2);
        return CompositeEMAC::ErrCode::PHY_NOT_RESPONDING;
    }

    // Software reset, if we couldn't use the hardware reset line earlier
    if(mac->getPhyResetPin() == NC) {
        uint16_t bmcrVal;
        FORWARD_ERR(mac->mdioRead(config.address, GenPhyRegs::BMCR, bmcrVal));
        FORWARD_ERR(mac->mdioWrite(config.address, GenPhyRegs::BMCR, bmcrVal | GenPhyRegs::BMCR_SW_RST_Msk));

        // Wait for SW reset bit to clear
        Timer timer;
        timer.start();
        do {
            FORWARD_ERR(mac->mdioRead(config.address, GenPhyRegs::BMCR, bmcrVal));
            rtos::ThisThread::sleep_for(1ms);
        }
        while(timer.elapsed_time() < config.resetHighTime && (bmcrVal & GenPhyRegs::BMCR_SW_RST_Msk));

        // If the reset bit has not cleared yet, we have hit a timeout. Check one more time to avoid race condition.
        FORWARD_ERR(mac->mdioRead(config.address, GenPhyRegs::BMCR, bmcrVal));
        if(bmcrVal & GenPhyRegs::BMCR_SW_RST_Msk) {
            return CompositeEMAC::ErrCode::TIMEOUT;
        }
    }

    // If using autonegotiation, program ANAR and then restart autonegotiation
    if(config.autonegEnabled) {
        uint16_t anarVal = GenPhyRegs::ANAR_PROTOCOL_IEE_802_3U_Val;
        if(config.advertise100M && config.advertiseFullDuplex) {
            anarVal |= GenPhyRegs::ANAR_100BTX_FD_Msk;
        }
        if(config.advertise100M && config.advertiseHalfDuplex) {
            anarVal |= GenPhyRegs::ANAR_100BTX_Msk;
        }
        if(config.advertise10M && config.advertiseFullDuplex) {
            anarVal |= GenPhyRegs::ANAR_10BT_FD_Msk;
        }
        if(config.advertise10M && config.advertiseHalfDuplex) {
            anarVal |= GenPhyRegs::ANAR_10BT_Msk;
        }
        FORWARD_ERR(mac->mdioWrite(config.address, GenPhyRegs::ANAR, anarVal));

        // Now restart and enable autoneg
        uint16_t bmcrVal;
        FORWARD_ERR(mac->mdioRead(config.address, GenPhyRegs::BMCR, bmcrVal));
        bmcrVal |= GenPhyRegs::BMCR_ANEG_EN_Msk | GenPhyRegs::BMCR_ANEG_RESTART_Msk;
        FORWARD_ERR(mac->mdioWrite(config.address, GenPhyRegs::BMCR, bmcrVal));
    }
    else {
        // Set fixed speed and duplex
        uint16_t bmcrVal;
        FORWARD_ERR(mac->mdioRead(config.address, GenPhyRegs::BMCR, bmcrVal));

        // Disable autonegotiation
        bmcrVal &= ~GenPhyRegs::BMCR_ANEG_EN_Msk;

        if(config.fixedEthSettings.first == CompositeEMAC::LinkSpeed::LINK_100MBIT) {
            bmcrVal |= GenPhyRegs::BMCR_SPEED_100M_Msk;
        }
        else { // 10Mbit. TODO handle 1Gbit
            bmcrVal &= ~GenPhyRegs::BMCR_SPEED_100M_Msk;
        }
        if(config.fixedEthSettings.second == CompositeEMAC::Duplex::FULL) {
            bmcrVal |= GenPhyRegs::BMCR_DUPLEX_FULL_Msk;
        }
        else { // half duplex
            bmcrVal &= ~GenPhyRegs::BMCR_DUPLEX_FULL_Msk;
        }
        FORWARD_ERR(mac->mdioWrite(config.address, GenPhyRegs::BMCR, bmcrVal));
    }

    return CompositeEMAC::ErrCode::SUCCESS;
}

CompositeEMAC::ErrCode GenericEthPhy::checkLinkStatus(bool &status) {
    // Note: PHYs latch the link state bit as low, so if the link has ever gone down here since the
    // last poll, we will read 0 for the link status.
    uint16_t bmsrVal;
    FORWARD_ERR(mac->mdioRead(config.address, GenPhyRegs::BMSR, bmsrVal));
    status = bmsrVal & GenPhyRegs::BMCR_LINK_UP_Msk;

    return CompositeEMAC::ErrCode::SUCCESS;
}

CompositeEMAC::ErrCode GenericEthPhy::checkLinkType(CompositeEMAC::LinkSpeed &speed, CompositeEMAC::Duplex &duplex) {

    if(config.autonegEnabled) {
        // What a lot of people don't know is, you can actually get the link type of a phy just by reading its
        // ANLPAR register! You don't need to read implementation specific registers.
        // I got this trick from the Linux kernel: https://github.com/torvalds/linux/blob/d79bc8f79baacdd2549ec4af6d963ce3e69d7330/drivers/net/phy/phy-core.c#L475
        uint16_t anlparVal;
        FORWARD_ERR(mac->mdioRead(config.address, GenPhyRegs::ANLPAR, anlparVal));

        // Check settings in order of priority. This mirrors the logic done inside the PHY to determine the
        // negotiated link type.
        if((anlparVal & GenPhyRegs::ANAR_100BTX_FD_Msk) && config.advertise100M && config.advertiseFullDuplex) {
            speed = CompositeEMAC::LinkSpeed::LINK_100MBIT;
            duplex = CompositeEMAC::Duplex::FULL;
        }
        else if((anlparVal & GenPhyRegs::ANAR_100BTX_Msk) && config.advertise100M && config.advertiseHalfDuplex) {
            speed = CompositeEMAC::LinkSpeed::LINK_100MBIT;
            duplex = CompositeEMAC::Duplex::HALF;
        }
        else if((anlparVal & GenPhyRegs::ANAR_10BT_FD_Msk) && config.advertise10M && config.advertiseFullDuplex) {
            speed = CompositeEMAC::LinkSpeed::LINK_10MBIT;
            duplex = CompositeEMAC::Duplex::FULL;
        }
        else if((anlparVal & GenPhyRegs::ANAR_10BT_Msk) && config.advertise10M && config.advertiseHalfDuplex) {
            speed = CompositeEMAC::LinkSpeed::LINK_10MBIT;
            duplex = CompositeEMAC::Duplex::HALF;
        }
        else {
            // No matching duplex settings
            return CompositeEMAC::ErrCode::NEGOTIATION_FAILED;
        }
    }
    else {
        // Use configured setting
        speed = config.fixedEthSettings.first;
        duplex = config.fixedEthSettings.second;
    }
    return CompositeEMAC::ErrCode::SUCCESS;
}
}
