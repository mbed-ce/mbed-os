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

#ifndef ESP32_INTERFACE_H
#define ESP32_INTERFACE_H

#include "mbed.h"
#include "ESP32Stack.h"

/** ESP32Interface class
 *  Implementation of the NetworkStack for the ESP32
 */
class ESP32Interface : public ESP32Stack, public WiFiInterface
{
public:
    /** ESP32Interface lifetime
     * Configuration defined in mbed_lib.json
     */
    ESP32Interface();

    /** ESP32Interface lifetime
     * @param en        EN pin  (If not used this pin, please set "NC")
     * @param io0       IO0 pin (If not used this pin, please set "NC")
     * @param tx        TX pin
     * @param rx        RX pin
     * @param debug     Enable debugging
     * @param rts       RTS pin
     * @param cts       CTS pin
     * @param baudrate  The baudrate of the serial port (default = 230400).
     */
    ESP32Interface(PinName en, PinName io0, PinName tx, PinName rx, bool debug = false,
                   PinName rts = NC, PinName cts = NC, int baudrate = 230400);

    /**
     * @brief ESP32Interface default destructor
     */
    virtual ~ESP32Interface();

    /** ESP32Interface lifetime
     * @param tx        TX pin
     * @param rx        RX pin
     * @param debug     Enable debugging
     */
    ESP32Interface(PinName tx, PinName rx, bool debug = false);

    /** Set a static IP address
     *
     *  Configures this network interface to use a static IP address.
     *  Implicitly disables DHCP, which can be enabled in set_dhcp.
     *  Requires that the network is disconnected.
     *
     *  @param ip_address SocketAddress representation of the local IP address
     *  @param netmask    SocketAddress representation of the local network mask
     *  @param gateway    SocketAddress representation of the local gateway
     *  @return           0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_network(
            const SocketAddress &ip_address, const SocketAddress &netmask,
            const SocketAddress &gateway) override;

    MBED_DEPRECATED_SINCE("mbed-os-5.15", "String-based APIs are deprecated")
    virtual nsapi_error_t set_network(
            const char *ip_address, const char *netmask, const char *gateway);

    /** Enable or disable DHCP on the network
     *
     *  Enables DHCP on connecting the network. Defaults to enabled unless
     *  a static IP address has been assigned. Requires that the network is
     *  disconnected.
     *
     *  @param dhcp     True to enable DHCP
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_dhcp(bool dhcp);

    /** Start the interface
     *
     *  Attempts to connect to a WiFi network. Requires ssid and passphrase to be set.
     *  If passphrase is invalid, NSAPI_ERROR_AUTH_ERROR is returned.
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual int connect();

    /** Start the interface
     *
     *  Attempts to connect to a WiFi network.
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection (Default: NSAPI_SECURITY_NONE)
     *  @param channel   This parameter is not supported, setting it to anything else than 0 will result in NSAPI_ERROR_UNSUPPORTED
     *  @return          0 on success, or error code on failure
     */
    virtual int connect(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE,
                                  uint8_t channel = 0);

    /** Set the WiFi network credentials
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection
     *                   (defaults to NSAPI_SECURITY_NONE)
     *  @return          0 on success, or error code on failure
     */
    virtual int set_credentials(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE);

    /** Set the WiFi network channel - NOT SUPPORTED
     *
     * This function is not supported and will return NSAPI_ERROR_UNSUPPORTED
     *
     *  @param channel   Channel on which the connection is to be made, or 0 for any (Default: 0)
     *  @return          Not supported, returns NSAPI_ERROR_UNSUPPORTED
     */
    virtual int set_channel(uint8_t channel);

    /** Stop the interface
     *  @return             0 on success, negative on failure
     */
    virtual int disconnect();

    /** Get the internally stored IP address
     *  @param          sockAddr SocketAddress pointer to store the local IP address
     *  @retval         NSAPI_ERROR_OK on success
     *  @retval         NSAPI_ERROR_UNSUPPORTED if this feature is not supported
     *  @retval         NSAPI_ERROR_PARAMETER if the provided pointer is invalid
     *  @retval         NSAPI_ERROR_NO_ADDRESS if the address cannot be obtained from stack
     */
    virtual nsapi_error_t get_ip_address(SocketAddress *sockAddr);

    MBED_DEPRECATED_SINCE("mbed-os-5.15", "String-based APIs are deprecated")
    virtual const char *get_ip_address();

    /** Get the internally stored MAC address
     *  @return             MAC address of the interface
     */
    virtual const char *get_mac_address();

     /** Get the local gateway
     *
     *  @param          sockAddr SocketAddress representation of gateway address
     *  @retval         NSAPI_ERROR_OK on success
     *  @retval         NSAPI_ERROR_UNSUPPORTED if this feature is not supported
     *  @retval         NSAPI_ERROR_PARAMETER if the provided pointer is invalid
     *  @retval         NSAPI_ERROR_NO_ADDRESS if the address cannot be obtained from stack
     */
    virtual nsapi_error_t get_gateway(SocketAddress *sockAddr);

    MBED_DEPRECATED_SINCE("mbed-os-5.15", "String-based APIs are deprecated")
    virtual const char *get_gateway();

    /** Get the local network mask
     *
     *  @param          sockAddr SocketAddress representation of netmask
     *  @retval         NSAPI_ERROR_OK on success
     *  @retval         NSAPI_ERROR_UNSUPPORTED if this feature is not supported
     *  @retval         NSAPI_ERROR_PARAMETER if the provided pointer is invalid
     *  @retval         NSAPI_ERROR_NO_ADDRESS if the address cannot be obtained from stack
     */
    virtual nsapi_error_t get_netmask(SocketAddress *sockAddr);

    MBED_DEPRECATED_SINCE("mbed-os-5.15", "String-based APIs are deprecated")
    virtual const char *get_netmask();

    /** Gets the current radio signal strength for active connection
     *
     * @return          Connection strength in dBm (negative value)
     */
    virtual int8_t get_rssi();

    /** Scan for available networks
     *
     * This function will block.
     *
     * @param  ap       Pointer to allocated array to store discovered AP
     * @param  count    Size of allocated @a res array, or 0 to only count available AP
     * @param  timeout  Timeout in milliseconds; 0 for no timeout (Default: 0)
     * @return          Number of entries in @a, or if @a count was 0 number of available networks, negative on error
     *                  see @a nsapi_error
     */
    virtual int scan(WiFiAccessPoint *res, unsigned count);

    /** Translates a hostname to an IP address with specific version
     *
     *  The hostname may be either a domain name or an IP address. If the
     *  hostname is an IP address, no network transactions will be performed.
     *
     *  If no stack-specific DNS resolution is provided, the hostname
     *  will be resolve using a UDP socket on the stack.
     *
     *  @param address  Destination for the host SocketAddress
     *  @param host     Hostname to resolve
     *  @param version  IP version of address to resolve, NSAPI_UNSPEC indicates
     *                  version is chosen by the stack (defaults to NSAPI_UNSPEC)
     *  @return         0 on success, negative error code on failure
     */
    using NetworkInterface::gethostbyname;

    /** Add a domain name server to list of servers to query
     *
     *  @param addr     Destination for the host address
     *  @return         0 on success, negative error code on failure
     */
    using NetworkInterface::add_dns_server;

    /** Register callback for status reporting
     *
     *  The specified status callback function will be called on status changes
     *  on the network. The parameters on the callback are the event type and
     *  event-type dependent reason parameter.
     *
     *  In ESP32 the callback will be called when processing OOB-messages via
     *  AT-parser. Do NOT call any ESP8266Interface -functions or do extensive
     *  processing in the callback.
     *
     *  @param status_cb The callback for status changes
     */
    virtual void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb);

    /** Get the connection status
     *
     *  @return         The connection status according to ConnectionStatusType
     */
    virtual nsapi_connection_status_t get_connection_status() const;

    /** Provide access to the NetworkStack object
     *
     *  @return The underlying NetworkStack object
     */
    virtual NetworkStack *get_stack()
    {
        return this;
    }

private:

    // HW reset pin
    class ResetPin {
    public:
        ResetPin(PinName rst_pin);
        void rst_assert();
        void rst_deassert();
        bool is_connected();
    private:
        mbed::DigitalOut  _rst_pin;
    } _rst_pin;

    int _initialized;
    bool _dhcp;
    char _ap_ssid[33]; /* 32 is what 802.11 defines as longest possible name; +1 for the \0 */
    char _ap_pass[64]; /* The longest allowed passphrase */
    nsapi_security_t _ap_sec;
    SocketAddress _ip_address;
    SocketAddress _netmask;
    SocketAddress _gateway;
    nsapi_connection_status_t _connection_status;
    mbed::Callback<void(nsapi_event_t, intptr_t)> _connection_status_cb;

    void set_connection_status(nsapi_connection_status_t connection_status);
    void wifi_status_cb(int8_t wifi_status);
    nsapi_error_t _init(void);
    nsapi_error_t _reset(void);
};

#endif
