/*
 * Copyright (c) 2018-2019 ARM Limited
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

#ifndef WHD_EMAC_H_
#define WHD_EMAC_H_

#include "EMAC.h"
#include "EMACInterface.h"
#include "WiFiInterface.h"
#include "whd_int.h"
#include <optional>

#include "rtos/Semaphore.h"
#include "rtos/Mutex.h"

class WHD_EMAC : public EMAC {
public:
    WHD_EMAC(whd_interface_role_t itype = WHD_STA_ROLE, const uint8_t *mac_addr = NULL);

    static WHD_EMAC &get_instance(whd_interface_role_t role = WHD_STA_ROLE, const uint8_t *mac_addr = NULL);

    uint32_t get_mtu_size() const override;

    uint32_t get_align_preference() const override;

    void get_ifname(char *name, uint8_t size) const override;

    uint8_t get_hwaddr_size() const override;

    bool get_hwaddr(uint8_t *addr) const override;

    void set_hwaddr(const uint8_t *addr) override;

    bool link_out(emac_mem_buf_t *buf) override;

    bool power_up() override;

    void power_down() override;

    void set_link_input_cb(emac_link_input_cb_t input_cb) override;

    void set_link_state_cb(emac_link_state_change_cb_t state_cb) override;

    void add_multicast_group(const uint8_t *address) override;

    void remove_multicast_group(const uint8_t *address) override;

    void set_all_multicast(bool all) override;

    void set_memory_manager(EMACMemoryManager &mem_mngr) override;

    virtual void set_activity_cb(mbed::Callback<void(bool is_tx_activity)> activity_cb);

    emac_link_input_cb_t emac_link_input_cb = NULL; /**< Callback for incoming data */
    emac_link_state_change_cb_t emac_link_state_cb = NULL;
    EMACMemoryManager *memory_manager;
    bool powered_up = false;
    bool link_state = false;
    bool ap_sta_concur = false;
    whd_interface_role_t interface_type;
    whd_driver_t drvp = NULL;
    whd_interface_t ifp = NULL;
    std::optional<whd_buffer_funcs_t> buffer_funcs;
    whd_mac_t unicast_addr;
    whd_mac_t multicast_addr;
    mbed::Callback<void(bool)> activity_cb;
};

#endif /* WHD_EMAC_H_ */
