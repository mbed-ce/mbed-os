/*
 * Copyright (c) 2024 Jamie Smith, All Rights Reserved
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


#ifndef MBED_OS_GREENTEA_GET_NETWORK_INTERFACE_H
#define MBED_OS_GREENTEA_GET_NETWORK_INTERFACE_H

#include "WiFiInterface.h"
#include "NetworkInterface.h"

#define WIFI 2
#if !defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE) || \
    (MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI && !defined(MBED_GREENTEA_WIFI_SECURE_SSID))
#error [NOT_SUPPORTED] No network configuration found for this target.
#endif

#if MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI
/*
 * Read the MBED_GREENTEA_WIFI_SECURE_PROTOCOL define from CMake and get the type of wifi
 * security in use
 */
inline nsapi_security get_wifi_security()
{
#define JOIN(x, y) JOIN_AGAIN(x, y)
#define JOIN_AGAIN(x, y) x ## y
    return JOIN(NSAPI_SECURITY_, MBED_GREENTEA_WIFI_SECURE_PROTOCOL);
#undef JOIN
#undef JOIN_AGAIN
}

/*
 * Get the wifi interface for this board, or nullptr if there is none
 */
inline WiFiInterface *get_wifi_interface()
{
    auto *const wifi_interface = WiFiInterface::get_default_instance();
    if (wifi_interface == nullptr) {
        return nullptr;
    }

    // Set the credentials based on CMake settings so it actually can connect to the internet
    wifi_interface->set_credentials(MBED_GREENTEA_WIFI_SECURE_SSID,
                                    MBED_GREENTEA_WIFI_SECURE_PASSWORD,
                                    get_wifi_security());

    return wifi_interface;
}
#endif

/*
 * Get the default network interface for this board.
 * For wifi, this also configures in the credentials passed to CMake.
 */
inline NetworkInterface *get_network_interface()
{
#if MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI
    return get_wifi_interface();
#else
    return NetworkInterface::get_default_instance();
#endif

}

#endif //MBED_OS_GREENTEA_GET_NETWORK_INTERFACE_H
