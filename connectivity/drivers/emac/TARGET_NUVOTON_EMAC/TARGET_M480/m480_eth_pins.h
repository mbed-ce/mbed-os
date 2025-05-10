/*
 * Copyright (c) 2018 Nuvoton Technology Corp.
 * Copyright (c) 2018 ARM Limited
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
 *
 * Description:   M480 EMAC driver header file
 */


#ifndef M480_ETH_PINS_H
#define M480_ETH_PINS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PinNames.h"

/// Mux pins and enable clock for the Nuvoton EMAC peripheral
void nu_eth_clk_and_pin_init(void);

/// Unmux pins and disable clock for the Nuvoton EMAC peripheral
void nu_eth_clk_and_pin_deinit(void);

/// Get the phy reset pin, or NC if not connected
PinName nu_eth_get_phy_reset_pin(void);

#ifdef __cplusplus
}
#endif

#endif //M480_ETH_PINS_H
