/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if defined(MBED_CONF_RTOS_PRESENT)

#include "mbed.h"
#include "mbed_trace.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "NetworkInterface.h"
#include "EMAC.h"
#include "EmacTestNetworkStack.h"
#include "emac_tests.h"
#include "emac_util.h"
#include "greentea_get_network_interface.h"

using namespace utest::v1;

void test_emac_initialize()
{
    worker_loop_init();
    mbed_trace_init();

    static NetworkInterface *network_interface = get_network_interface();

    // Power up the interface and emac driver
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, network_interface->connect());

    worker_loop_link_up_wait();
}

/*
 * Test which powers the EMAC down and then up again
 */
void test_emac_power_down_and_power_up()
{
    EmacTestNetworkStack::get_instance().get_emac()->power_down();

    // Note: Currently the EMAC does not deliver a link state change to down callback when powered down.
    // Might change that in the future but for now we need to deliver the callback manually.
    emac_if_link_state_change_cb(false);

    TEST_ASSERT_TRUE(EmacTestNetworkStack::get_instance().get_emac()->power_up());

    // Currently EMACs may expect set_hwaddr() to be called after power up as this API is not well defined.
    EmacTestNetworkStack::get_instance().get_emac()->set_hwaddr(EmacTestNetworkStack::get_instance().get_mac_addr());

    // Wait for link to come back up before continuing
    worker_loop_link_up_wait();
}

#endif // defined(MBED_CONF_RTOS_PRESENT)
