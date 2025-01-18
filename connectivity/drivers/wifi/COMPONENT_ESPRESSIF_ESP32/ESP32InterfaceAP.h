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

#ifndef ESP32_INTERFACE_AP_H
#define ESP32_INTERFACE_AP_H

#include "ESP32Stack.h"


/** ESP32Interface class
 *  Implementation of the NetworkStack for the ESP32
 */
class ESP32InterfaceAP : public ESP32Stack, public WiFiInterface {
public:
    /** ESP32InterfaceAP lifetime
     * Configuration defined in mbed_lib.json
     */
    ESP32InterfaceAP();

    /** ESP32InterfaceAP lifetime
     * @param en        EN pin  (If not used this pin, please set "NC")
     * @param io0       IO0 pin (If not used this pin, please set "NC")
     * @param tx        TX pin
     * @param rx        RX pin
     * @param debug     Enable debugging
     * @param rts       RTS pin
     * @param cts       CTS pin
     * @param baudrate  The baudrate of the serial port (default = 230400).
     */
    ESP32InterfaceAP(PinName en, PinName io0, PinName tx, PinName rx, bool debug = false,
                     PinName rts = NC, PinName cts = NC, int baudrate = 230400);

    /** ESP32InterfaceAP lifetime
     * @param tx        TX pin
     * @param rx        RX pin
     * @param debug     Enable debugging
     */
    ESP32InterfaceAP(PinName tx, PinName rx, bool debug = false);

    nsapi_error_t set_network(
        const SocketAddress &ip_address, const SocketAddress &netmask,
        const SocketAddress &gateway) override;

    nsapi_error_t set_dhcp(bool dhcp) override;

    int connect() override;

    int connect(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE,
                uint8_t channel = 0) override;

    int set_credentials(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE) override;

    int set_channel(uint8_t channel) override;

    int disconnect() override;

    nsapi_error_t get_ip_address(SocketAddress *address) override;

    const char *get_mac_address() override;

    nsapi_error_t get_gateway(SocketAddress *address) override;

    nsapi_error_t get_netmask(SocketAddress *address) override;

    int8_t get_rssi() override;

    int scan(WiFiAccessPoint *res, unsigned count) override;

    using NetworkInterface::gethostbyname;

    using NetworkInterface::add_dns_server;

    void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb) override;

    nsapi_connection_status_t get_connection_status() const override;

    NetworkStack *get_stack() override
    {
        return this;
    }

private:
    bool _dhcp;
    uint8_t _own_ch;
    char _own_ssid[33]; /* 32 is what 802.11 defines as longest possible name; +1 for the \0 */
    char _own_pass[64]; /* The longest allowed passphrase */
    nsapi_security_t _own_sec;
    SocketAddress _ip_address;
    SocketAddress _netmask;
    SocketAddress _gateway;
    nsapi_connection_status_t _connection_status;
    mbed::Callback<void(nsapi_event_t, intptr_t)> _connection_status_cb;
};

#endif
