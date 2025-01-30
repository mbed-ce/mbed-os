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

#include "CompositeEthMac.h"

namespace mbed {

class GenericEthPhy : public mbed::CompositeEMAC::PhyDriver {
protected:

    // Methods for subclasses to implement
    /**
     * @brief Get the expected OUI and model number for this phy chip. These will be used
     *    to verify that the phy available on the board matches the expected model.
     *
     * @return Pair of OUI and model number.
     */
    virtual std::pair<uint8_t, uint8_t> getOUIAndModel() const = 0;

    /**
     * @brief Get the default MDIO address of this phy chip
     *
     * @return Pair of OUI and model number.
     */
    virtual uint8_t getDefaultAddress() const = 0;

    /**
     * @brief Get the reset times for this phy
     *
     * @return Pair of (time reset should be held low, time after reset until we should try to contact the phy)
     */
    virtual std::pair<std::chrono::microseconds, std::chrono::microseconds> getResetTimes() const {
        return {100us, 500ms}; // Apparently the 802.3u standard dictates PHYs should boot in <=0.5s
    }

public:


};

};

#endif //MBED_OS_GENERICETHPHY_H
