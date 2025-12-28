/* nsapi.h - The network socket API
 * Copyright (c) 2025 Jamie Smith
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

#include "nsapi_types.h"

#include <cstring>

char const *nsapi_strerror(nsapi_error error)
{
    switch (error) {
        case NSAPI_ERROR_OK:
            return "OK";
        case NSAPI_ERROR_WOULD_BLOCK:
            return "WOULD_BLOCK";
        case NSAPI_ERROR_UNSUPPORTED:
            return "UNSUPPORTED";
        case NSAPI_ERROR_PARAMETER:
            return "PARAMETER";
        case NSAPI_ERROR_NO_CONNECTION:
            return "NO_CONNECTION";
        case NSAPI_ERROR_NO_SOCKET:
            return "NO_SOCKET";
        case NSAPI_ERROR_NO_ADDRESS:
            return "NO_ADDRESS";
        case NSAPI_ERROR_NO_MEMORY:
            return "NO_MEMORY";
        case NSAPI_ERROR_NO_SSID:
            return "NO_SSID";
        case NSAPI_ERROR_DNS_FAILURE:
            return "DNS_FAILURE";
        case NSAPI_ERROR_DHCP_FAILURE:
            return "DHCP_FAILURE";
        case NSAPI_ERROR_AUTH_FAILURE:
            return "AUTH_FAILURE";
        case NSAPI_ERROR_DEVICE_ERROR:
            return "DEVICE_ERROR";
        case NSAPI_ERROR_IN_PROGRESS:
            return "IN_PROGRESS";
        case NSAPI_ERROR_ALREADY:
            return "ALREADY";
        case NSAPI_ERROR_IS_CONNECTED:
            return "IS_CONNECTED";
        case NSAPI_ERROR_CONNECTION_LOST:
            return "CONNECTION_LOST";
        case NSAPI_ERROR_CONNECTION_TIMEOUT:
            return "CONNECTION_TIMEOUT";
        case NSAPI_ERROR_ADDRESS_IN_USE:
            return "ADDRESS_IN_USE";
        case NSAPI_ERROR_TIMEOUT:
            return "TIMEOUT";
        case NSAPI_ERROR_BUSY:
            return "BUSY";
        default:
            return nullptr;
    }
}

char const *nsapi_security_to_string(nsapi_security_t sec)
{
    switch (sec) {
        case NSAPI_SECURITY_NONE:
            return "NONE";
        case NSAPI_SECURITY_WEP:
            return "WEP";
        case NSAPI_SECURITY_WPA:
            return "WPA";
        case NSAPI_SECURITY_WPA2:
            return "WPA2";
        case NSAPI_SECURITY_WPA_WPA2:
            return "WPA_WPA2";
        case NSAPI_SECURITY_PAP:
            return "PAP";
        case NSAPI_SECURITY_CHAP:
            return "CHAP";
        case NSAPI_SECURITY_EAP_TLS:
            return "EAP_TLS";
        case NSAPI_SECURITY_PEAP:
            return "PEAP";
        case NSAPI_SECURITY_WPA2_ENT:
            return "WPA2_ENT";
        case NSAPI_SECURITY_WPA3:
            return "WPA3";
        case NSAPI_SECURITY_WPA3_WPA2:
            return "WPA3_WPA2";
        case NSAPI_SECURITY_UNKNOWN:
            return "UNKNOWN";
        default:
            return nullptr;
    }
}

nsapi_security_t nsapi_string_to_security(char const *str)
{
    if (strcmp(str, "NONE") == 0) {
        return NSAPI_SECURITY_NONE;
    }
    if (strcmp(str, "WEP") == 0) {
        return NSAPI_SECURITY_WEP;
    }
    if (strcmp(str, "WPA") == 0) {
        return NSAPI_SECURITY_WPA;
    }
    if (strcmp(str, "WPA2") == 0) {
        return NSAPI_SECURITY_WPA2;
    }
    if (strcmp(str, "WPA_WPA2") == 0) {
        return NSAPI_SECURITY_WPA_WPA2;
    }
    if (strcmp(str, "PAP") == 0) {
        return NSAPI_SECURITY_PAP;
    }
    if (strcmp(str, "CHAP") == 0) {
        return NSAPI_SECURITY_CHAP;
    }
    if (strcmp(str, "EAP_TLS") == 0) {
        return NSAPI_SECURITY_EAP_TLS;
    }
    if (strcmp(str, "PEAP") == 0) {
        return NSAPI_SECURITY_PEAP;
    }
    if (strcmp(str, "WPA2_ENT") == 0) {
        return NSAPI_SECURITY_WPA2_ENT;
    }
    if (strcmp(str, "WPA3") == 0) {
        return NSAPI_SECURITY_WPA3;
    }
    if (strcmp(str, "WPA3_WPA2") == 0) {
        return NSAPI_SECURITY_WPA3_WPA2;
    }
    return NSAPI_SECURITY_UNKNOWN;
}
