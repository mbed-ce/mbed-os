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


class PhyLAN8742 : public GenericEthPhy {
protected:
    std::pair<uint32_t, uint8_t> getOUIAndModel() const override {
        return {0x1F0, 0x13};
    }

    uint8_t getAddress() const override {
        // Address set via PHYAD[0] strap.
        return 0;
    }

    std::pair<std::chrono::microseconds, std::chrono::microseconds> getResetTimes() const override {
        // Per Table 5-11, reset must be low for at least 100us
        // Per section 3.8.6.2, "a software reset will be completed within 0.5s."
        // That's apparently all we get for the post-reset time
        return {100us, 500ms};
    }
};


}