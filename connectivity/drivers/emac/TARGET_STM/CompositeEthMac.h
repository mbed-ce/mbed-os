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

#ifndef MBED_OS_COMPOSITEETHMAC_H
#define MBED_OS_COMPOSITEETHMAC_H

#include "EMAC.h"

namespace mbed
{

/**
 * @brief Implementation of the EMAC API built up from several components implemented by device-specific classes.
 *
 * \par Motivation
 * Originally, Mbed interfaced with the Ethernet hardware on an MCU via a single class, an implementation of the
 * EMAC interface.  However, EMAC is a large interface, encompassing many responsibilities: setting up the pins,
 * managing the Ethernet peripheral, controlling the phy chip, and putting data into and out of DMA.  Many of these
 * pieces, such as PHY control and managing memory buffers, are common, but others are unique to each MCU family.
 * To better divide responsibility into common and target-specific parts, and to allow Ethernet drivers to
 * be organized more logically, the CompositeEMAC class was created.
 *
 * \par Division
 * CompositeEMAC divides Ethernet functionality up into several different classes, each with different
 * responsibilities:
 * <ul>
 * <li>\c MACDriver : Driver for the MAC peripheral itself.  Provides functionality to map MAC pins, start up and
 *   configure the MAC, configure interrupts, and communicate with the phy over MDIO.</li>
 * <li>\c PHYDriver : Communicates with the phy.  Responsible for configuring it and setting its status.</li>
 * <li>\c TxDMA : Driver for the Tx DMA ring.  Takes in Tx packets and queues them for transmission in the
 *   hardware.</li>
 * <li>\c RxDMA : Driver for the Rx DMA ring.  Sets up Rx descriptors and dequeues them once packets are received.</li>
 * </ul>
 *
 * \note CompositeEMAC itself does not use any global data and supports multiple instances for MCUs that have
 *    multiple EMACs.  However, the implementation for a specific MCU may or may not use global data -- if
 *    there's only one EMAC on the MCU, there isn't really a reason not to.
 */
class CompositeEMAC : public EMAC
{

};

}
#endif //MBED_OS_COMPOSITEETHMAC_H
