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

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "wifi_tests.h"

#include "greentea_get_network_interface.h"

using namespace utest::v1;

#if defined(MBED_GREENTEA_WIFI_SECURE_SSID)

void wifi_connect_disconnect_repeat(void)
{
    WiFiInterface *wifi = get_wifi_interface();
    TEST_ASSERT(wifi);
    if (wifi == NULL) {
        return;
    }

    if (wifi->get_connection_status() != NSAPI_STATUS_DISCONNECTED) {
        wifi->disconnect();
    }

    nsapi_error_t error;

    for (int i = 0; i < 10; i++) {
        printf("#%u connecting...\n", i);
        error = wifi->connect();
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, error);
        printf("#%u diconnecting...\n", i);
        error = wifi->disconnect();
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, error);
    }
}

#endif // defined(MBED_GREENTEA_WIFI_SECURE_SSID)
