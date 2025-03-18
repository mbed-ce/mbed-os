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

namespace LAN8742 {

/// Driver for the Microchip LAN8742 PHY
/// Datasheet: https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ProductDocuments/DataSheets/DS_LAN8742_00001989A.pdf
/// @{

inline constexpr GenericEthPhy::Config DefaultConfig = {
    .OUI = 0x1F0,
    .model = 0x13,
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