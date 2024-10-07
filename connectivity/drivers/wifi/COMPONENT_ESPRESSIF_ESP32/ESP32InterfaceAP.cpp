/* ESP32 implementation of NetworkInterfaceAPI
 * Copyright (c) 2017 Renesas Electronics Corporation
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

#include <string.h>
#include "ESP32InterfaceAP.h"

// ESP32InterfaceAP implementation
ESP32InterfaceAP::ESP32InterfaceAP() :
    ESP32Stack(MBED_CONF_ESP32_WIFI_EN, MBED_CONF_ESP32_WIFI_IO0, MBED_CONF_ESP32_WIFI_TX, MBED_CONF_ESP32_WIFI_RX, MBED_CONF_ESP32_WIFI_DEBUG,
               MBED_CONF_ESP32_WIFI_RTS, MBED_CONF_ESP32_WIFI_CTS, MBED_CONF_ESP32_WIFI_BAUDRATE, 1),
    _dhcp(true),
    _own_ch(1),
    _own_ssid(),
    _own_pass(),
    _own_sec(NSAPI_SECURITY_NONE),
    _ip_address(),
    _netmask(),
    _gateway(),
    _connection_status(NSAPI_STATUS_DISCONNECTED),
    _connection_status_cb(NULL)
{
}

ESP32InterfaceAP::ESP32InterfaceAP(PinName en, PinName io0, PinName tx, PinName rx, bool debug,
    PinName rts, PinName cts, int baudrate) :
    ESP32Stack(en, io0, tx, rx, debug, rts, cts, baudrate, 1),
    _dhcp(true),
    _own_ch(1),
    _own_ssid(),
    _own_pass(),
    _own_sec(NSAPI_SECURITY_NONE),
    _ip_address(),
    _netmask(),
    _gateway(),
    _connection_status(NSAPI_STATUS_DISCONNECTED),
    _connection_status_cb(NULL)
{
}

ESP32InterfaceAP::ESP32InterfaceAP(PinName tx, PinName rx, bool debug) :
    ESP32Stack(NC, NC, tx, rx, debug, NC, NC, 230400, 1),
    _dhcp(true),
    _own_ch(1),
    _own_ssid(),
    _own_pass(),
    _own_sec(NSAPI_SECURITY_NONE),
    _ip_address(),
    _netmask(),
    _gateway(),
    _connection_status(NSAPI_STATUS_DISCONNECTED),
    _connection_status_cb(NULL)
{
}

nsapi_error_t ESP32InterfaceAP::set_network(const SocketAddress &ip_address, const SocketAddress &netmask, const SocketAddress &gateway)
{
    _dhcp = false;
    _ip_address = ip_address;
    _netmask = netmask;
    _gateway = gateway;

    return NSAPI_ERROR_OK;
}

nsapi_error_t ESP32InterfaceAP::set_network(const char *ip_address, const char *netmask, const char *gateway)
{
    _dhcp = false;

    _ip_address.set_ip_address(ip_address);
    _netmask.set_ip_address(netmask);
    _gateway.set_ip_address(gateway);

    return NSAPI_ERROR_OK;
}

nsapi_error_t ESP32InterfaceAP::set_dhcp(bool dhcp)
{
    _dhcp = dhcp;

    return NSAPI_ERROR_OK;
}

int ESP32InterfaceAP::connect(const char *ssid, const char *pass, nsapi_security_t security,
                                        uint8_t channel)
{
    int ret;

    ret = set_credentials(ssid, pass, security);
    if (ret != 0) {
        return ret;
    }

    ret = set_channel(channel);
    if (ret != 0) {
        return ret;
    }

    return connect();
}

int ESP32InterfaceAP::connect()
{
    if (!_esp->set_mode(ESP32::WIFIMODE_STATION_SOFTAP)) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (!_esp->dhcp(_dhcp, 1)) {
        return NSAPI_ERROR_DHCP_FAILURE;
    }

    if (!_dhcp) {
        if (!_esp->set_network_ap(_ip_address.get_ip_address(), _netmask.get_ip_address(), _gateway.get_ip_address())) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
    }

    if (!_esp->config_soft_ap(_own_ssid, _own_pass, _own_ch, (uint8_t)_own_sec)) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    _connection_status = NSAPI_STATUS_GLOBAL_UP;
    if (_connection_status_cb) {
        _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, _connection_status);
    }

    return NSAPI_ERROR_OK;
}

int ESP32InterfaceAP::set_credentials(const char *ssid, const char *pass, nsapi_security_t security)
{
    switch (security) {
        case NSAPI_SECURITY_NONE:
        case NSAPI_SECURITY_WPA:
        case NSAPI_SECURITY_WPA2:
        case NSAPI_SECURITY_WPA_WPA2:
            _own_sec = security;
            break;
        case NSAPI_SECURITY_UNKNOWN:
        case NSAPI_SECURITY_WEP:
        default:
            return NSAPI_ERROR_UNSUPPORTED;
    }

    memset(_own_ssid, 0, sizeof(_own_ssid));
    strncpy(_own_ssid, ssid, sizeof(_own_ssid));

    memset(_own_pass, 0, sizeof(_own_pass));
    strncpy(_own_pass, pass, sizeof(_own_pass));

    return 0;
}

int ESP32InterfaceAP::set_channel(uint8_t channel)
{
    if (channel != 0) {
        _own_ch = channel;
    }

    return 0;
}

int ESP32InterfaceAP::disconnect()
{
    if (!_esp->set_mode(ESP32::WIFIMODE_STATION)) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    _connection_status = NSAPI_STATUS_DISCONNECTED;
    if (_connection_status_cb) {
        _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, _connection_status);
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t ESP32InterfaceAP::get_ip_address(SocketAddress* sockAddr)
{
    if (sockAddr->set_ip_address(_esp->getIPAddress_ap())) {
        return NSAPI_ERROR_OK;
    }
    return NSAPI_ERROR_NO_ADDRESS;
}

const char *ESP32InterfaceAP::get_ip_address()
{
    return _esp->getIPAddress_ap();
}

const char *ESP32InterfaceAP::get_mac_address()
{
    return _esp->getMACAddress_ap();
}

nsapi_error_t ESP32InterfaceAP::get_gateway(SocketAddress* sockAddr)
{
    if (sockAddr->set_ip_address(_esp->getGateway_ap())) {
        return NSAPI_ERROR_OK;
    }
    return NSAPI_ERROR_NO_ADDRESS;
}

const char *ESP32InterfaceAP::get_gateway()
{
    return _esp->getGateway_ap();
}

nsapi_error_t ESP32InterfaceAP::get_netmask(SocketAddress* sockAddr)
{
    if (sockAddr->set_ip_address(_esp->getNetmask_ap())) {
        return NSAPI_ERROR_OK;
    }
    return NSAPI_ERROR_NO_ADDRESS;
}


const char *ESP32InterfaceAP::get_netmask()
{
    return _esp->getNetmask_ap();
}

int8_t ESP32InterfaceAP::get_rssi()
{
    return 0;
}

int ESP32InterfaceAP::scan(WiFiAccessPoint *res, unsigned count)
{
    return _esp->scan(res, count);
}

void ESP32InterfaceAP::attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _connection_status_cb = status_cb;
}

nsapi_connection_status_t ESP32InterfaceAP::get_connection_status() const
{
    return _connection_status;
}

