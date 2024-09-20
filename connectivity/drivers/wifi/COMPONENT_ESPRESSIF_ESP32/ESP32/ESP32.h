/* ESP32Interface Example
 * Copyright (c) 2015 ARM Limited
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

#ifndef ESP32_H
#define ESP32_H

#if DEVICE_SERIAL && defined(MBED_CONF_EVENTS_PRESENT) && defined(MBED_CONF_NSAPI_PRESENT) && defined(MBED_CONF_RTOS_PRESENT)
#include <vector>
#include <stdint.h>
#include <stdlib.h>
#include "drivers/DigitalOut.h"
#include "drivers/SerialBase.h"
#include "netsocket/nsapi_types.h"
#include "netsocket/WiFiAccessPoint.h"
#include "PinNames.h"
#include "platform/ATCmdParser.h"
#include "platform/Callback.h"
#include "platform/mbed_error.h"
#include "platform/mbed_version.h"
#include "rtos/Mutex.h"
#include "rtos/ThisThread.h"

#if (MBED_MAJOR_VERSION < 6)
#include "drivers/UARTSerial.h"
#else
#include "drivers/BufferedSerial.h"
#endif

#ifndef ESP32_CONNECT_TIMEOUT
#define ESP32_CONNECT_TIMEOUT 15000
#endif
#ifndef ESP32_SEND_TIMEOUT
#define ESP32_SEND_TIMEOUT    2000
#endif
#ifndef ESP32_RECV_TIMEOUT
#define ESP32_RECV_TIMEOUT    2000
#endif
#ifndef ESP32_MISC_TIMEOUT
#define ESP32_MISC_TIMEOUT    2000
#endif

/** ESP32Interface class.
    This is an interface to a ESP32 radio.
 */
class ESP32
{
public:
    /**
    * Static method to create or retrieve the single ESP32 instance
    */
    static ESP32 * getESP32Inst(PinName en, PinName io0, PinName tx, PinName rx, bool debug,
                                PinName rts, PinName cts, int baudrate);
    static ESP32 * getESP32Inst(bool debug = false);

    ESP32(PinName en, PinName io0, PinName tx, PinName rx, bool debug,
          PinName rts, PinName cts, int baudrate);

    /**
    * Checks Version Information
    *
    * @param ver_info buffer address for storing version information
    * @param buf_size  buffer size
    * @return String of Version Information
    */
    bool get_version_info(char * ver_info, int buf_size);

    /**
    * Sets the Wi-Fi Mode
    *
    * @param mode mode of WIFI 1-client, 2-host, 3-both
    * @return true only if ESP32 was setup correctly
    */
    bool set_mode(int mode);

    /**
    * Enable/Disable DHCP
    *
    * @param enabled DHCP enabled when true
    * @param mode mode of DHCP 0-softAP, 1-station, 2-both
    * @return true only if ESP32 enables/disables DHCP successfully
    */
    bool dhcp(bool enabled, int mode);

    /**
    * Connect ESP32 to AP
    *
    * @param ap the name of the AP
    * @param passPhrase the password of AP
    * @return true only if ESP32 is connected successfully
    */
    bool connect(const char *ap, const char *passPhrase);

    /**
    * Disconnect ESP32 from AP
    *
    * @return true only if ESP32 is disconnected successfully
    */
    bool disconnect(void);

    /**
    * Get the IP address of ESP32
    *
    * @return null-teriminated IP address or null if no IP address is assigned
    */
    const char *getIPAddress(void);
    const char *getIPAddress_ap(void);

    /**
    * Get the MAC address of ESP32
    *
    * @return null-terminated MAC address or null if no MAC address is assigned
    */
    const char *getMACAddress(void);
    const char *getMACAddress_ap(void);

     /** Get the local gateway
     *
     *  @return         Null-terminated representation of the local gateway
     *                  or null if no network mask has been recieved
     */
    const char *getGateway();
    const char *getGateway_ap();

    /** Get the local network mask
     *
     *  @return         Null-terminated representation of the local network mask 
     *                  or null if no network mask has been recieved
     */
    const char *getNetmask();
    const char *getNetmask_ap();

    /* Return RSSI for active connection
     *
     * @return      Measured RSSI
     */
    int8_t getRSSI();

    /**
    * Check if ESP32 is conenected
    *
    * @return true only if the chip has an IP address
    */
    bool isConnected(void);

    /** Scan for available networks
     *
     * @param  ap    Pointer to allocated array to store discovered AP
     * @param  limit Size of allocated @a res array, or 0 to only count available AP
     * @return       Number of entries in @a res, or if @a count was 0 number of available networks, negative on error
     *               see @a nsapi_error
     */
    int scan(WiFiAccessPoint *res, unsigned limit);

    /**
    * Open a socketed connection
    *
    * @param type the type of socket to open "UDP" or "TCP"
    * @param id id to give the new socket, valid 0-4
    * @param port port to open connection with
    * @param addr the IP address of the destination
    * @param addr the IP address of the destination
    * @param opt  type=" UDP" : UDP socket's local port, zero means any
    *             type=" TCP" : TCP connection's keep alive time, zero means disabled
    * @return true only if socket opened successfully
    */
    bool open(const char *type, int id, const char* addr, int port, int opt = 0);

    /**
    * Sends data to an open socket
    *
    * @param id id of socket to send to
    * @param data data to be sent
    * @param amount amount of data to be sent - max 1024
    * @return true only if data sent successfully
    */
    bool send(int id, const void *data, uint32_t amount);

    /**
    * Receives data from an open socket
    *
    * @param id id to receive from
    * @param data placeholder for returned information
    * @param amount number of bytes to be received
    * @return the number of bytes received
    */
    int32_t recv(int id, void *data, uint32_t amount, uint32_t timeout = ESP32_RECV_TIMEOUT);

    /**
    * Closes a socket
    *
    * @param id id of socket to close, valid only 0-4
    * @param wait_close 
    * @return true only if socket is closed successfully
    */
    bool close(int id, bool wait_close = false);

    /**
    * Allows timeout to be changed between commands
    *
    * @param timeout_ms timeout of the connection
    */
    void setTimeout(uint32_t timeout_ms = ESP32_MISC_TIMEOUT);

    /**
    * Checks if data is available
    */
    bool readable();

    /**
    * Checks if data can be written
    */
    bool writeable();

    void socket_attach(int id, void (*callback)(void *), void *data);
    int get_free_id();

    bool config_soft_ap(const char *ap, const char *passPhrase, uint8_t chl, uint8_t ecn);

    bool restart();
    bool get_ssid(char *ap);
    bool cre_server(int port);
    bool del_server();
    bool accept(int * p_id);

    bool set_network(const char *ip_address, const char *netmask, const char *gateway);
    bool set_network_ap(const char *ip_address, const char *netmask, const char *gateway);

    /**
    * Attach a function to call whenever network state has changed
    *
    * @param func A pointer to a void function, or 0 to set as none
    */
    void attach_wifi_status(mbed::Callback<void(int8_t)> status_cb);

    /** Get the connection status
     *
     *  @return         The connection status according to ConnectionStatusType
     */
    int8_t get_wifi_status() const;

    /**
     * Flush the serial port input buffers.
     *
     * If you do HW reset for ESP module, you should
     * flush the input buffers from existing responses
     * from the device.
     */
    void flush();

    static const int8_t WIFIMODE_STATION = 1;
    static const int8_t WIFIMODE_SOFTAP = 2;
    static const int8_t WIFIMODE_STATION_SOFTAP = 3;
    static const int8_t SOCKET_COUNT = 5;

    static const int8_t STATUS_DISCONNECTED = 0;
    static const int8_t STATUS_CONNECTED = 1;
    static const int8_t STATUS_GOT_IP = 2;

private:
    mbed::DigitalOut * _p_wifi_en;
    mbed::DigitalOut * _p_wifi_io0;
    bool _init_end_common;
    bool _init_end_wifi;
#if (MBED_MAJOR_VERSION < 6)
    mbed::UARTSerial _serial;
#else
    mbed::BufferedSerial _serial;
#endif
    mbed::ATCmdParser _parser;
    struct packet {
        struct packet *next;
        int id;
        uint32_t len;
        uint32_t index;
        // data follows
    } *_packets, **_packets_end;
    int _wifi_mode;
    int _baudrate;
    PinName _rts;
    PinName _cts;
    int _flow_control;
    uint32_t last_timeout_ms;

    std::vector<int> _accept_id;
    uint32_t _id_bits;
    bool _server_act;
    rtos::Mutex _smutex; // Protect serial port access
    static ESP32 * instESP32;
    int8_t _wifi_status;
    mbed::Callback<void(int8_t)> _wifi_status_cb;
    uint32_t _at_version;

    bool _ids[SOCKET_COUNT];
    struct {
        void (*callback)(void *);
        void *data;
        int  Notified;
    } _cbs[SOCKET_COUNT];

    void _startup_common();
    bool _startup_wifi();
    bool reset(void);
    void debugOn(bool debug);
    void socket_handler(bool connect, int id);
    void _connect_handler_0();
    void _connect_handler_1();
    void _connect_handler_2();
    void _connect_handler_3();
    void _connect_handler_4();
    void _closed_handler_0();
    void _closed_handler_1();
    void _closed_handler_2();
    void _closed_handler_3();
    void _closed_handler_4();
    void _connection_status_handler();
    void _packet_handler();
    void _clear_socket_packets(int id);
    void event();
    bool recv_ap(nsapi_wifi_ap_t *ap);

    char _ip_buffer[16];
    char _gateway_buffer[16];
    char _netmask_buffer[16];
    char _mac_buffer[18];

    char _ip_buffer_ap[16];
    char _gateway_buffer_ap[16];
    char _netmask_buffer_ap[16];
    char _mac_buffer_ap[18];

#if defined(TARGET_ESP32AT_BLE)
public:
    typedef struct {
        int      srv_index;          /**< service's index starting from 1 */
        int      char_index;         /**< characteristic's index starting from 1 */
        int      desc_index;         /**< descriptor's index */
        void *   data;               /**< data buffer address */
        uint32_t len;                /**< data len */
    } ble_packet_t;

    typedef union {
        const uint8_t * addr;        /**< buffer address */
        uint32_t        data;        /**< data */
    } union_data_t;

    typedef struct {
        union_data_t uuid;           /**< UUID value */
        uint8_t      uuid_type;      /**< UUID type  0: addr 1:data */
        uint16_t     uuid_size;      /**< UUID size */
        uint16_t     val_max_len;    /**< max allow value length (the max length when you dynamic set value) */
        union_data_t value;          /**< initial value */
        uint8_t      value_type;     /**< initial value  type 0: addr 1:data */
        uint16_t     value_size;     /**< initial value size */
        uint8_t      permissions;    /**< permission (refer to BLE Spec for definition) */
    } gatt_service_t;

    typedef struct {
        uint16_t  adv_int_min;       /**< minimum value of advertising interval; range: 0x0020 ~ 0x4000 */
        uint16_t  adv_int_max;       /**< maximum value of advertising interval; range: 0x0020 ~ 0x4000 */
        uint8_t   adv_type;          /**< 0:FADV_TYPE_IND, 2:FADV_TYPE_SCAN_IND, 3:FADV_TYPE_NONCONN_IND */
        uint8_t   own_addr_type;     /**< own BLE address type; 0:FBLE_ADDR_TYPE_PUBLIC, 1:FBLE_ADDR_TYPE_RANDOM */
        uint8_t   channel_map;       /**< channel of advertising; ADV_CHNL_~ */
        uint8_t   adv_filter_policy; /**< filter policy of advertising; ADV_FILTER_ALLOW_SCAN_~ */
        uint8_t   peer_addr_type;    /**< remote BLE address type; 0:FPUBLIC, 1:FRANDOM */
        uint8_t   peer_addr[6];      /**< remote BLE address */
    } advertising_param_t;

    typedef struct {
        uint8_t   addr[6];           /**< BLE address */
        int8_t    rssi;              /**< signal strength */
        uint8_t   adv_data[31];      /**< advertising data */
        uint8_t   adv_data_len;      /**< advertising data length */
        uint8_t   scan_rsp_data[31]; /**< scan response data */
        uint8_t   scan_rsp_data_len; /**< scan response data length */
        uint8_t   addr_type;         /**< the address type of broadcasters */
    } ble_scan_t;

    typedef struct {
        int      srv_index;          /**< service's index starting from 1 */
        uint16_t srv_uuid;           /**< service's UUID */
        int      srv_type;           /**< service's type */
    } ble_primary_service_t;

    typedef struct {
        int      srv_index;          /**< service's index starting from 1 */
        uint16_t srv_uuid;           /**< service's UUID */
        int      srv_type;           /**< service's type */
    } ble_characteristics_t;

    typedef struct {
        int      char_index;         /**< Characteristic's index starting from 1 */
        uint16_t char_uuid;          /**< Characteristic's UUID */
        uint8_t  char_prop;          /**< Characteristic's properties */
    } ble_discovers_char_t;

    typedef struct {
        int      char_index;         /**< Characteristic's index starting from 1 */
        int      desc_index;         /**< Descriptor's index */
        uint16_t desc_uuid;          /**< Descriptor's UUID */
    } ble_discovers_desc_t;

    // advertising_param_t:adv_type
    #define ADV_TYPE_IND           0
    #define ADV_TYPE_SCAN_IND      2
    #define ADV_TYPE_NONCONN_IND   3

    // advertising_param_t:own_addr_type and peer_addr_type
    #define BLE_ADDR_TYPE_PUBLIC   0
    #define BLE_ADDR_TYPE_RANDOM   1

    // advertising_param_t:channel_map
    #define ADV_CHNL_37            0x01
    #define ADV_CHNL_38            0x02
    #define ADV_CHNL_39            0x04
    #define ADV_CHNL_ALL           0x07

    // advertising_param_t:adv_filter_policy
    #define ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY    0
    #define ADV_FILTER_ALLOW_SCAN_WLST_CON_ANY   1
    #define ADV_FILTER_ALLOW_SCAN_ANY_CON_WLST   2
    #define ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST  3

    // ble_set_role: role
    #define INIT_CLIENT_ROLE       1
    #define INIT_SERVER_ROLE       2

    /** Sets BLE Role
     *
     *  @param role         INIT_CLIENT_ROLE: client role, INIT_SERVER_ROLE: server role
     *  @return             true: success, false: failure
     */
    bool ble_set_role(int role);

    /** Gets BLE Role
     *
     *  @param role         INIT_CLIENT_ROLE: client role, INIT_SERVER_ROLE: server role
     *  @return             true: success, false: failure
     */
    bool ble_get_role(int * role);

    /** Sets BLE Device's Name
     *
     *  @param name         The BLE device name
     *  @return             true: success, false: failure
     */
    bool ble_set_device_name(const char * name);

    /** Gets BLE Device's Name
     *
     *  @param name         The BLE device name
     *  @return             true: success, false: failure
     */
    bool ble_get_device_name(char * name);

    /** GATTS Creates and Starts Services
     *
     *  @return             true: success, false: failure
     */
    bool ble_start_services();

    /** Sets BLE Scan Response
     *
     *  @param data         Scan response data
     *  @param len          Data len
     *  @return             true: success, false: failure
     */
    bool ble_set_scan_response(const uint8_t * data, int len);

    /** Starts Advertising
     *
     *  @return             true: success, false: failure
     */
    bool ble_start_advertising();

    /** Stops Advertising
     *
     *  @return             true: success, false: failure
     */
    bool ble_stop_advertising();

    /** Sets BLE Device's Address
     *
     *  @param addr_type    0: public address, 1: random address
     *  @param addr         Random address data. Valid only when addr_type is 1.
     *  @return             true: success, false: failure
     */
    bool ble_set_addr(int addr_type, const uint8_t * random_addr = NULL);

    /** Gets BLE Device's Address
     *
     *  @param public_addr  BLE public address
     *  @return             true: success, false: failure
     */
    bool ble_get_addr(uint8_t * public_addr);

    /** Sets Parameters of Advertising
     *
     *  @param param        Parameters. See advertising_param_t.
     *  @return             true: success, false: failure
     */
    bool ble_set_advertising_param(const advertising_param_t * param);

    /** Sets Advertising Data
     *
     *  @param data         Advertising data
     *  @param len          Data len
     *  @return             true: success, false: failure
     */
    bool ble_set_advertising_data(const uint8_t * data, int len);

    /** GATT Sets Service
     *
     *  @param service_list GATT service list. see gatt_service_t.
     *  @param num          Number of GATT service list
     *  @return             true: success, false: failure
     */
    bool ble_set_service(const gatt_service_t * service_list, int num);

    /** GATTS Sets Characteristic
     *
     *  @param srv_index    Service's index starting from 1
     *  @param char_index   Characteristic's index starting from 1
     *  @param data         Data buffer address
     *  @param len          Data len
     *  @return             true: success, false: failure
     */
    bool ble_set_characteristic(int srv_index, int char_index, const uint8_t * data, int len);

    /** GATTS Notifies of Characteristics
     *
     *  @param srv_index    Service's index starting from 1
     *  @param char_index   Characteristic's index starting from 1
     *  @param data         Data buffer address
     *  @param len          Data len
     *  @return             true: success, false: failure
     */
    bool ble_notifies_characteristic(int srv_index, int char_index, const uint8_t * data, int len);

    /** Sets Parameters of BLE Scanning
     *
     *  @param scan_type     0: passive scan  1: active scan
     *  @param own_addr_type 0: public address  1: random address  2: RPA public address  3: RPA random address
     *  @param filter_policy 0: BLE_SCAN_FILTER_ALLOW_ALL
     *                       1: BLE_SCAN_FILTER_ALLOW_ONLY_WLST
     *                       2: BLE_SCAN_FILTER_ALLOW_UND_RPA_DIR
     *                       3: BLE_SCAN_FILTER_ALLOW_WLIST_PRA_DIR
     *  @param scan_interval scan interval
     *  @param scan_window   scan window
     *  @return              rue: success, false: failure
     */
    bool ble_set_scan_param(int scan_type, int own_addr_type, int filter_policy, int scan_interval, int scan_window);

    /** Enables BLE Scanning
     *
     *  @param interval     0: scanning is continuous
     *                      !0: scanning should last for <interval> seconds and then stop automatically
     *  @return             true: success, false: failure
     */
    bool ble_start_scan(int interval = 0);

    /** Disables BLE scan
     *
     *  @return             true: success, false: failure
     */
    bool ble_stop_scan();

    /** Establishes BLE connection
     *
     *  @param conn_index   Index of BLE connection; only 0 is supported for the single connection right now,
     *                      but multiple BLE connections will be supported in the future.
     *  @param remote_addr  Remote BLE address
     *  @return             true: success, false: failure
     */
    bool ble_connect(int conn_index, const uint8_t * remote_addr);

    /** Ends BLE connection
     *
     *  @param conn_index   Index of BLE connection; only 0 is supported for the single connection right now,
     *                      but multiple BLE connections will be supported in the future.
     *  @return             true: success, false: failure
     */
    bool ble_disconnect(int conn_index);

    /** GATTC Discovers Primary Services
     *
     *  @param conn_index   Index of BLE connection; only 0 is supported for the single connection right now,
     *                      but multiple BLE connections will be supported in the future.
     *  @param service      Service info
     *  @param num          Number of service info
     *  @return             true: success, false: failure
     */
    bool ble_discovery_service(int conn_index, ble_primary_service_t * service, int * num);

    /** GATTC Discovers Characteristics
     *
     *  @param conn_index     Index of BLE connection; only 0 is supported for the single connection right now,
     *                        but multiple BLE connections will be supported in the future.
     *  @param srv_index      Service's index. It can be fetched with "ble_discovery_service()"
     *  @param discovers_char Characteristic info
     *  @param char_num       Number of characteristic info
     *  @param discovers_desc Descriptor info
     *  @param desc_num       Number of descriptor info
     *  @return               true: success, false: failure
     */
    bool ble_discovery_characteristics(
             int conn_index, int srv_index,
             ble_discovers_char_t * discovers_char, int * char_num,
             ble_discovers_desc_t * discovers_desc, int * desc_num
         );

    /** GATTC Reads a Characteristic
     *
     *  @param conn_index   Index of BLE connection; only 0 is supported for the single connection right now,
     *                      but multiple BLE connections will be supported in the future.
     *  @param srv_index    Service's index. It can be fetched with "ble_discovery_service()"
     *  @param char_index   Characteristic's index. It can be fetched with "ble_discovery_characteristics()"
     *  @param data         Read data buffer
     *  @param amount       Amount of bytes to be received
     *  @return             Data size of received
     */
    int32_t ble_read_characteristic(int conn_index, int srv_index, int char_index, uint8_t * data, int amount);

    /** GATTC Reads a Descriptor
     *
     *  @param conn_index   Index of BLE connection; only 0 is supported for the single connection right now,
     *                      but multiple BLE connections will be supported in the future.
     *  @param srv_index    Service's index. It can be fetched with "ble_discovery_service()"
     *  @param char_index   Characteristic's index. It can be fetched with "ble_discovery_characteristics()"
     *  @param desc_index   Descriptor's index. It can be fetched with "ble_discovery_characteristics()"
     *  @param data         Read data buffer
     *  @param amount       Amount of bytes to be received
     *  @return             true: success, false: failure
     */
    int32_t ble_read_descriptor(int conn_index, int srv_index, int char_index, int desc_index, uint8_t * data, int amount);

    /** GATTC Writes Characteristic
     *
     *  @param conn_index   Index of BLE connection; only 0 is supported for the single connection right now,
     *                      but multiple BLE connections will be supported in the future.
     *  @param srv_index    Service's index. It can be fetched with "ble_discovery_service()"
     *  @param char_index   Characteristic's index. It can be fetched with "ble_discovery_characteristics()"
     *  @param data         Write data buffer
     *  @param amount       Amount of data to be written
     *  @return             true: success, false: failure
     */
    bool ble_write_characteristic(int conn_index, int srv_index, int char_index, const uint8_t * data, int amount);

    /** GATTC Writes Descriptor
     *
     *  @param conn_index   Index of BLE connection; only 0 is supported for the single connection right now,
     *                      but multiple BLE connections will be supported in the future.
     *  @param srv_index    Service's index. It can be fetched with "ble_discovery_service()"
     *  @param char_index   Characteristic's index. It can be fetched with "ble_discovery_characteristics()"
     *  @param desc_index   Descriptor's index. It can be fetched with "ble_discovery_characteristics()"
     *  @param data         Write data buffer
     *  @param amount       Amount of data to be written
     *  @return             true: success, false: failure
     */
    bool ble_write_descriptor(int conn_index, int srv_index, int char_index, int desc_index, const uint8_t * data, int amount);

    /** For executing OOB processing on background
     *
     *  @param timeout      AT parser receive timeout
     *  @param all          if TRUE, process all OOBs instead of only one
     */
    void ble_process_oob(uint32_t timeout, bool all);

    /** Register a callback on state change.
     *
     *  The specified callback will be called on state changes.
     *
     *  The callback may be called in an interrupt context and should not
     *  perform expensive operations.
     *
     *  Note! This is not intended as an attach-like asynchronous api, but rather
     *  as a building block for constructing  such functionality.
     *
     *  The exact timing of when the registered function
     *  is called is not guaranteed and susceptible to change. It should be used
     *  as a cue to make ble_process_oobl calls to find the current state.
     *
     *  @param cb_func      function to call on state change
     */
    void ble_attach_sigio(mbed::Callback<void()> cb_func);

    /**
     * Attach a function to call whenever the BLE connection establishes
     *
     * @param cb_func       Pointer to the function to be calledt
     *                      cb_func argument  0: disconnect, 1: connect
     */
    void ble_attach_conn(mbed::Callback<void(int, uint8_t *)> cb_func);

    /**
     * Attach a function to call whenever the BLE connection ends
     *
     * @param cb_func       Pointer to the function to be calledt
     *                      cb_func argument  0: disconnect, 1: connect
     */
    void ble_attach_disconn(mbed::Callback<void(int)> cb_func);

    /**
     * Attach a function to call whenever characteristic data is written
     *
     * @param cb_func       Pointer to the function to be called
     */
    void ble_attach_write(mbed::Callback<void(ble_packet_t *)> cb_func);

    /**
     * Attach a function to call whenever scan data is received
     *
     * @param cb_func       Pointer to the function to be called
     */
    void ble_attach_scan(mbed::Callback<void(ble_scan_t *)> cb_func);

private:
    #define PRIMARY_SERVICE_BUF_NUM    16
    #define DISCOVERS_CHAR_BUF_NUM     16
    #define DISCOVERS_DESC_BUF_NUM     16

    struct {
        mbed::Callback<void()> callback;
        int  Notified;
    } _cbs_ble;

    mbed::Callback<void(int, uint8_t *)> _ble_conn_cb;
    mbed::Callback<void(int)> _ble_disconn_cb;
    mbed::Callback<void(ble_packet_t *)> _ble_write_cb;
    mbed::Callback<void(ble_scan_t *)> _ble_scan_cb;
    int _ble_role;
    bool _init_end_ble;
    int _primary_service_idx;
    int _discovers_char_idx;
    int _discovers_desc_idx;
    ble_primary_service_t _primary_service[PRIMARY_SERVICE_BUF_NUM];
    ble_discovers_char_t _discovers_char[DISCOVERS_CHAR_BUF_NUM];
    ble_discovers_desc_t _discovers_desc[DISCOVERS_DESC_BUF_NUM];

    bool _startup_ble();
    void _ble_conn();
    void _ble_disconn();
    void _ble_write();
    void _ble_scan();
    void _ble_primsrv();
    void _ble_discovers_char();
    char _int2char(int data);
    int _char2int(char c);
    int _set_char(char * buf1, const uint8_t * buf2, int size);
#endif /* TARGET_ESP32AT_BLE */

};
#endif
#endif /* ESP32_H */
