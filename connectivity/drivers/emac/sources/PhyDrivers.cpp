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

namespace mbed {

using namespace std::chrono_literals;

namespace LAN87XX {

/// Driver for the Microchip LAN8742 & LAN8720 PHY
/// Datasheet: https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ProductDocuments/DataSheets/DS_LAN8742_00001989A.pdf
///            https://ww1.microchip.com/downloads/en/devicedoc/8720a.pdf
/// @{

inline constexpr GenericEthPhy::Config DefaultConfig = {
    .OUI = 0x1F0,
    .model_min = 0x0F, // LAN8720
    .model_max = 0x13, // LAN8742
    .address = 0, // Address set via PHYAD[0] strap.
};

class Driver : public GenericEthPhy {
public:
    explicit Driver(GenericEthPhy::Config const & config = DefaultConfig):
    GenericEthPhy(config)
    {}
};


/// @}

}

namespace IP101G {

    /// Driver for the IC+ IP101Gx PHY
    /// Datasheet: https://www.lcsc.com/datasheet/lcsc_datasheet_IC-Plus-IP101GR_C79324.pdf
    /// @{

    inline constexpr GenericEthPhy::Config DefaultConfig = {
        .OUI = 0x90C3,
        .model_min = 0x5,
        .model_max = 0x5,
        .address = 1, // Address set via strapping pins, 1 is used on Nuvoton boards
    };

    class Driver : public GenericEthPhy {
    public:
        explicit Driver(GenericEthPhy::Config const & config = DefaultConfig):
        GenericEthPhy(config)
        {}
    };


    /// @}

}

namespace DP8384X {

    /// Driver for the  DP8384X PHY
    /// Datasheet: https://www.ti.com/lit/ds/symlink/dp83848c.pdf
    ///            https://www.ti.com/lit/ds/symlink/dp83849i.pdf
    /// @{

    inline constexpr GenericEthPhy::Config DefaultConfig = {
        .OUI = 0x80017,
        .model_min = 0x09, // DP83848VV, DP83849I
        .model_max = 0x0A, // DP83848C/I/VYB/YB
        .address = 1, // Address set via PHYAD[0] strap.
    };

    class Driver : public GenericEthPhy {
    public:
        explicit Driver(GenericEthPhy::Config const & config = DefaultConfig):
        GenericEthPhy(config)
        {}
    };


    /// @}

}

/**
 * @brief Obtains the PHY driver for Ethernet port 0.
 *
 * The default implementation constructs a built-in driver (given by \c nsapi.emac-phy-model ) using the
 * configured MDIO address ( \c nsapi.emac-phy-mdio-address ).  However, it can be overridden by the
 * application if it wishes to use another phy driver class!
 *
 * @return Phy driver class instance, or nullptr if none is configured.
 */
MBED_WEAK CompositeEMAC::PHYDriver * get_eth_phy_driver()
{
#ifdef MBED_CONF_NSAPI_EMAC_PHY_MODEL
    static GenericEthPhy::Config driverConfig = MBED_CONF_NSAPI_EMAC_PHY_MODEL::DefaultConfig;
#ifdef MBED_CONF_NSAPI_EMAC_PHY_MDIO_ADDRESS
    driverConfig.address = MBED_CONF_NSAPI_EMAC_PHY_MDIO_ADDRESS;
#endif
    static MBED_CONF_NSAPI_EMAC_PHY_MODEL::Driver driver(driverConfig);
    return &driver;
#else
    return nullptr;
#endif
};

}