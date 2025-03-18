/* Copyright (c) 2024 Jamie Smith
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

#ifndef MBED_OS_GENERICETHPHY_H
#define MBED_OS_GENERICETHPHY_H

#include "CompositeEMAC.h"
#include "DigitalOut.h"

#include <optional>

namespace mbed {

namespace GenPhyRegs {
    /// Generic Ethernet phy register definitions
    /// @{
    inline constexpr uint8_t BMCR = 0; ///< Basic Mode Control Register

    inline constexpr size_t BMCR_SW_RST_Pos = 15;
    inline constexpr uint16_t BMCR_SW_RST_Msk = 1 << BMCR_SW_RST_Pos;

    inline constexpr size_t BMCR_SPEED_100M_Pos = 13;
    inline constexpr uint16_t BMCR_SPEED_100M_Msk = 1 << BMCR_SPEED_100M_Pos;

    inline constexpr size_t BMCR_ANEG_EN_Pos = 12;
    inline constexpr uint16_t BMCR_ANEG_EN_Msk = 1 << BMCR_ANEG_EN_Pos;

    inline constexpr size_t BMCR_ANEG_RESTART_Pos = 9;
    inline constexpr uint16_t BMCR_ANEG_RESTART_Msk = 1 << BMCR_ANEG_RESTART_Pos;

    inline constexpr size_t BMCR_DUPLEX_FULL_Pos = 8;
    inline constexpr uint16_t BMCR_DUPLEX_FULL_Msk = 1 << BMCR_DUPLEX_FULL_Pos;

    inline constexpr uint8_t BMSR = 1; ///< Basic Mode Status Register

    inline constexpr size_t BMCR_LINK_UP_Pos = 2;
    inline constexpr uint16_t BMCR_LINK_UP_Msk = 1 << BMCR_LINK_UP_Pos;

    inline constexpr uint8_t PHYIDR1 = 2; ///< PHY ID Register 1
    inline constexpr uint8_t PHYIDR2 = 3; ///< PHY ID Register 2
    inline constexpr uint8_t ANAR = 4; ///< AutoNegotiation Advertisement Register

    inline constexpr size_t ANAR_100BTX_FD_Pos = 8;
    inline constexpr uint16_t ANAR_100BTX_FD_Msk = 1 << ANAR_100BTX_FD_Pos;

    inline constexpr size_t ANAR_100BTX_Pos = 7;
    inline constexpr uint16_t ANAR_100BTX_Msk = 1 << ANAR_100BTX_Pos;

    inline constexpr size_t ANAR_10BT_FD_Pos = 6;
    inline constexpr uint16_t ANAR_10BT_FD_Msk = 1 << ANAR_10BT_FD_Pos;

    inline constexpr size_t ANAR_10BT_Pos = 5;
    inline constexpr uint16_t ANAR_10BT_Msk = 1 << ANAR_10BT_Pos;

    /// Value for ANAR[4:0] that specifies Ethernet as the protocol
    inline constexpr uint16_t ANAR_PROTOCOL_IEE_802_3U_Val = 1;

    inline constexpr uint8_t ANLPAR = 5; ///< AutoNegotiation Link Partner Advertisement Register
    /// @}
}

/**
 * @brief Driver for a generic, Ethernet-standard compliant PHY chip.
 *
 * Passed a configuration which sets most attributes of the phy.
 * May be extended to handle chip-specific quirks.
 */
class GenericEthPhy : public mbed::CompositeEMAC::PHYDriver {
public:
    /// Configuration structure for a generic Ethernet PHY
    struct Config {
        /// 24-bit OUI of the organization that produced the ethernet PHY.
        uint32_t OUI;

        /// 5-bit model number of the phy. This plus the OUI is used to verify that the
        /// chip in hardware matches what's expected.
        uint8_t model;

        /// MDIO address of the phy chip.
        /// NOTE: 0 is *supposed* to be reserved as the general call address but lots of phy chips use
        /// it anyway.
        uint8_t address;

        /// Time to hold reset low on this phy
        std::chrono::microseconds resetLowTime = std::chrono::microseconds(100);

        /// Time to wait after resetting the phy before trying to talk to it.
        /// Also used as the time to wait for power on reset if there is no reset pin.
        /// Note that standards compliant PHYs must reset in <500ms.
        std::chrono::microseconds resetHighTime = std::chrono::milliseconds(500);

        /// Whether autonegotiation shall be enabled on the phy.
        /// NOTE: If this is set to false, the other end of the connection MUST be manually configured to match.
        /// Otherwise you will get sporadic data loss due to an Ethernet duplex mismatch!
        /// However, setting this to false will make the Ethernet link come up much faster (milliseconds instead of seconds, at least if you don't do DHCP)
        bool autonegEnabled = true;

        /// Whether the PHY should advertise full duplex modes
        bool advertiseFullDuplex = true;

        /// Whether the PHY should advertise half duplex modes.
        /// Half duplex allows running only over 1 or 2 twisted pairs, but introduces the possibility of collisions.
        bool advertiseHalfDuplex = true;

        /// Whether the PHY should advertise 100Mbit modes
        bool advertise100M = true;

        /// Whether the PHY should advertise 10Mbit modes.
        /// 10Mbit can be a fallback if signal conditions are poor, but this fallback may not be desired.
        bool advertise10M = true;

        /// If autonegEnabled is false, this gives the fixed link settings to advertise.
        std::pair<CompositeEMAC::LinkSpeed, CompositeEMAC::Duplex> fixedEthSettings = {CompositeEMAC::LinkSpeed::LINK_100MBIT, CompositeEMAC::Duplex::FULL};
    };

protected:

    Config const & config;

    std::optional<DigitalOut> resetDigitalOut;

public:
    GenericEthPhy(Config const & config):
    config(config)
    {}

    CompositeEMAC::ErrCode init() override;

    CompositeEMAC::ErrCode checkLinkStatus(bool &status) override;

    CompositeEMAC::ErrCode checkLinkType(CompositeEMAC::LinkSpeed &speed, CompositeEMAC::Duplex &duplex) override;
};

};

#endif //MBED_OS_GENERICETHPHY_H
