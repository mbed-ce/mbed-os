/* ESP32 Example
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

#if DEVICE_SERIAL && defined(MBED_CONF_EVENTS_PRESENT) && defined(MBED_CONF_NSAPI_PRESENT) && defined(MBED_CONF_RTOS_PRESENT)
#include "ESP32.h"
#include "platform/mbed_interface.h"

#define ESP32_DEFAULT_BAUD_RATE   115200
#define ESP32_ALL_SOCKET_IDS      -1

using namespace mbed;
using namespace rtos;
using namespace std::chrono;
using std::milli;

ESP32 * ESP32::instESP32 = NULL;

ESP32 * ESP32::getESP32Inst(PinName en, PinName io0, PinName tx, PinName rx, bool debug,
                            PinName rts, PinName cts, int baudrate)
{
    if (instESP32 == NULL) {
        instESP32 = new ESP32(en, io0, tx, rx, debug, rts, cts, baudrate);
    } else {
        if (debug) {
            instESP32->debugOn(debug);
        }
    }
    return instESP32;
}

ESP32 * ESP32::getESP32Inst(bool debug)
{
    return getESP32Inst(MBED_CONF_ESP32_WIFI_EN, MBED_CONF_ESP32_WIFI_IO0,
                        MBED_CONF_ESP32_WIFI_TX, MBED_CONF_ESP32_WIFI_RX, debug,
                        MBED_CONF_ESP32_WIFI_RTS, MBED_CONF_ESP32_WIFI_CTS,
                        MBED_CONF_ESP32_WIFI_BAUDRATE);
}

ESP32::ESP32(PinName en, PinName io0, PinName tx, PinName rx, bool debug,
    PinName rts, PinName cts, int baudrate)
    : _p_wifi_en(NULL), _p_wifi_io0(NULL), _init_end_common(false), _init_end_wifi(false)
    , _serial(tx, rx, ESP32_DEFAULT_BAUD_RATE), _parser(&_serial, "\r\n")
    , _packets(0), _packets_end(&_packets)
    , _id_bits(0), _server_act(false)
    , _wifi_status(STATUS_DISCONNECTED)
    , _wifi_status_cb(NULL), _at_version(0)
#if defined(TARGET_ESP32AT_BLE)
    , _ble_conn_cb(NULL), _ble_disconn_cb(NULL), _ble_write_cb(NULL), _ble_scan_cb(NULL)
    , _ble_role(INIT_SERVER_ROLE), _init_end_ble(false)
    , _primary_service_idx(0), _discovers_char_idx(0), _discovers_desc_idx(0)
#endif /* TARGET_ESP32AT_BLE */
{
    if ((int)en != NC) {
        _p_wifi_en = new DigitalOut(en);
    }
    if ((int)io0 != NC) {
        _p_wifi_io0 = new DigitalOut(io0);
    }

    _wifi_mode = WIFIMODE_STATION;
    _baudrate = baudrate;
    memset(_ids, 0, sizeof(_ids));
    memset(_cbs, 0, sizeof(_cbs));
#if defined(TARGET_ESP32AT_BLE)
    memset(&_cbs_ble, 0, sizeof(_cbs_ble));
#endif /* TARGET_ESP32AT_BLE */

    _rts = rts;
    _cts = cts;

    if ((_rts != NC) && (_cts != NC)) {
        _flow_control = 3;
    } else if (_rts != NC) {
        _flow_control = 1;
    } else if (_cts != NC) {
        _flow_control = 2;
    } else {
        _flow_control = 0;
    }

    _serial.set_baud(ESP32_DEFAULT_BAUD_RATE);
    debugOn(debug);

    _parser.oob("+IPD", callback(this, &ESP32::_packet_handler));
    _parser.oob("0,CONNECT", callback(this, &ESP32::_connect_handler_0));
    _parser.oob("1,CONNECT", callback(this, &ESP32::_connect_handler_1));
    _parser.oob("2,CONNECT", callback(this, &ESP32::_connect_handler_2));
    _parser.oob("3,CONNECT", callback(this, &ESP32::_connect_handler_3));
    _parser.oob("4,CONNECT", callback(this, &ESP32::_connect_handler_4));
    _parser.oob("0,CLOSED", callback(this, &ESP32::_closed_handler_0));
    _parser.oob("1,CLOSED", callback(this, &ESP32::_closed_handler_1));
    _parser.oob("2,CLOSED", callback(this, &ESP32::_closed_handler_2));
    _parser.oob("3,CLOSED", callback(this, &ESP32::_closed_handler_3));
    _parser.oob("4,CLOSED", callback(this, &ESP32::_closed_handler_4));
    _parser.oob("WIFI ", callback(this, &ESP32::_connection_status_handler));
#if defined(TARGET_ESP32AT_BLE)
    _parser.oob("+BLECONN:", callback(this, &ESP32::_ble_conn));
    _parser.oob("+BLEDISCONN:", callback(this, &ESP32::_ble_disconn));
    _parser.oob("+WRITE:", callback(this, &ESP32::_ble_write));
    _parser.oob("+BLESCAN:", callback(this, &ESP32::_ble_scan));
    _parser.oob("+BLEGATTCPRIMSRV:", callback(this, &ESP32::_ble_primsrv));
    _parser.oob("+BLEGATTCCHAR:", callback(this, &ESP32::_ble_discovers_char));
#endif /* TARGET_ESP32AT_BLE */

    _serial.sigio(Callback<void()>(this, &ESP32::event));

    setTimeout();
}

void ESP32::debugOn(bool debug)
{
    _parser.debug_on((debug) ? 1 : 0);
}

bool ESP32::get_version_info(char * ver_info, int buf_size)
{
    bool ret;
    int idx = 0;
    const char key_word[6] = "\nOK\r\n";
    char wk_buf[5];
    int wk_idx;
    char c;

    if (ver_info == NULL) {
        return false;
    }
    _smutex.lock();
    _startup_common();
    setTimeout(500);
    ret = _parser.send("AT+GMR");
    if (!ret) {
        setTimeout();
        _smutex.unlock();
        return false;
    }
    for (int i = 0; i < 10; i++) {
        _parser.getc();   // dummy read
    }
    while (idx < buf_size) {
        wk_idx = 0;
        for (int i = 0; i < 5; i++) {
            c = _parser.getc();
            wk_buf[wk_idx++] = c;
            if (c != key_word[i]) {
                break;
            }
        }
        if (wk_idx >= 5) {
            break;
        }
        for (int i = 0; (i < wk_idx) && (idx < buf_size); i++) {
            ver_info[idx++] = wk_buf[i];
        }
    }
    setTimeout();
    _smutex.unlock();

    ver_info[idx] = '\0';
    while (idx > 0) {
        idx--;
        if (ver_info[idx] != '\r' && ver_info[idx] != '\n') {
            break;
        }
        ver_info[idx] = '\0';
    }

    return true;
}

void ESP32::_startup_common()
{
    if (_init_end_common) {
        return;
    }

    _serial.set_baud(ESP32_DEFAULT_BAUD_RATE);
    if (_p_wifi_io0 != NULL) {
        _p_wifi_io0->write(1);
    }
    if (_p_wifi_en != NULL) {
        _p_wifi_en->write(0);
        ThisThread::sleep_for(10ms);
        _p_wifi_en->write(1);
        _parser.recv("ready");
    } else {
        setTimeout(100);
        _parser.recv("ready");
    }

    reset();

    _init_end_common = true;

    return;
}

bool ESP32::_startup_wifi()
{
    _startup_common();

    if (_init_end_wifi) {
        return true;
    }

    bool success = _parser.send("AT+CWMODE=%d", _wifi_mode)
                && _parser.recv("OK")
                && _parser.send("AT+CIPMUX=1")
                && _parser.recv("OK")
                && _parser.send("AT+CWAUTOCONN=0")
                && _parser.recv("OK")
                && _parser.send("AT+CWQAP")
                && _parser.recv("OK");
    if (success) {
        _init_end_wifi = true;
    }

    return success;
}

bool ESP32::restart()
{
    bool success = true;;
    bool ret;

    _smutex.lock();
    setTimeout();
    reset();
    if (_init_end_wifi) {
        ret = _parser.send("AT+CWMODE=%d", _wifi_mode)
           && _parser.recv("OK")
           && _parser.send("AT+CIPMUX=1")
           && _parser.recv("OK");
        if (!ret) {
            success = false;
        }
    }
#if defined(TARGET_ESP32AT_BLE)
    if (_init_end_ble) {
        ret = _parser.send("AT+BLEINIT=%d", _ble_role)
           && _parser.recv("OK");
        if (!ret) {
            success = false;
        }
    }
#endif
    _smutex.unlock();

    return success;
}

bool ESP32::set_mode(int mode)
{
    //only 3 valid modes
    if (mode < 1 || mode > 3) {
        return false;
    }
    if (_wifi_mode != mode) {
        _wifi_mode = mode;
        if (_init_end_wifi) {
            return restart();
        }
    }
    return true;
}

bool ESP32::cre_server(int port)
{
    if (_server_act) {
        return false;
    }
    _smutex.lock();
    _startup_wifi();
    if (!(_parser.send("AT+CIPSERVER=1,%d", port)
        && _parser.recv("OK"))) {
        _smutex.unlock();
        return false;
    }
    _server_act = true;
    _smutex.unlock();
    return true;
}

bool ESP32::del_server()
{
    _smutex.lock();
    _startup_wifi();
    if (!(_parser.send("AT+CIPSERVER=0")
        && _parser.recv("OK"))) {
        _smutex.unlock();
        return false;
    }
    _server_act = false;
    _smutex.unlock();
    return true;
}

void ESP32::socket_handler(bool connect, int id)
{
    _cbs[id].Notified = 0;
    if (connect) {
        _id_bits |= (1 << id);
        if (_server_act) {
            _accept_id.push_back(id);
        }
    } else {
        _id_bits &= ~(1 << id);
        if (_server_act) {
            for (size_t i = 0; i < _accept_id.size(); i++) {
                if (id == _accept_id[i]) {
                    _accept_id.erase(_accept_id.begin() + i);
                }
            }
        }
    }
}

bool ESP32::accept(int * p_id)
{
    bool ret = false;

    while (!ret) {
        if (!_server_act) {
            break;
        }

        _smutex.lock();
        _startup_wifi();
        if (!_accept_id.empty()) {
            ret = true;
        } else {
            _parser.process_oob(); // Poll for inbound packets
            if (!_accept_id.empty()) {
                ret = true;
            }
        }
        if (ret) {
            *p_id = _accept_id[0];
            _accept_id.erase(_accept_id.begin());
        }
        _smutex.unlock();
        if (!ret) {
            ThisThread::sleep_for(5ms);
        }
    }

    return ret;
}

bool ESP32::reset(void)
{
    for (int i = 0; i < 2; i++) {
        if (_parser.send("AT+RST")
            && _parser.recv("OK")) {
            _serial.set_baud(ESP32_DEFAULT_BAUD_RATE);
#if DEVICE_SERIAL_FC
            _serial.set_flow_control(SerialBase::Disabled);
#endif
            _parser.recv("ready");
            _clear_socket_packets(ESP32_ALL_SOCKET_IDS);

            if (_parser.send("AT+UART_CUR=%d,8,1,0,%d", _baudrate, _flow_control)
                && _parser.recv("OK")) {
                _serial.set_baud(_baudrate);
#if DEVICE_SERIAL_FC
                switch (_flow_control) {
                    case 1:
                        _serial.set_flow_control(SerialBase::RTS, _rts);
                        break;
                    case 2:
                        _serial.set_flow_control(SerialBase::CTS, _cts);
                        break;
                    case 3:
                        _serial.set_flow_control(SerialBase::RTSCTS, _rts, _cts);
                        break;
                    case 0:
                    default:
                        // do nothing
                        break;
                }
#endif
            }

            ThisThread::sleep_for(5ms);

            uint8_t wk_ver[4+1]; /* It needs 1 byte extra. */

            if (_parser.send("AT+GMR")
                && _parser.recv("AT version:%hhx.%hhx.%hhx.%hhx", &wk_ver[0], &wk_ver[1], &wk_ver[2], &wk_ver[3])
                && _parser.recv("OK")
            ) {
                _at_version = (wk_ver[0] << 24)
                            | (wk_ver[1] << 16)
                            | (wk_ver[2] << 8)
                            | (wk_ver[3] << 0);
            }

            return true;
        }
    }

    return false;
}

bool ESP32::dhcp(bool enabled, int mode)
{
    //only 3 valid modes
    if (mode < 0 || mode > 2) {
        return false;
    }

    _smutex.lock();
    _startup_wifi();
    bool done = _parser.send("AT+CWDHCP=%d,%d", enabled?1:0, mode)
       && _parser.recv("OK");
    _smutex.unlock();

    return done;
}

bool ESP32::connect(const char *ap, const char *passPhrase)
{
    bool ret;

    _wifi_status = STATUS_DISCONNECTED;

    _smutex.lock();
    _startup_wifi();

    setTimeout(ESP32_CONNECT_TIMEOUT);
    ret = _parser.send("AT+CWJAP=\"%s\",\"%s\"", ap, passPhrase)
       && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();
    return ret;
}

bool ESP32::config_soft_ap(const char *ap, const char *passPhrase, uint8_t chl, uint8_t ecn)
{
    bool ret;

    _smutex.lock();
    _startup_wifi();
    ret = _parser.send("AT+CWSAP=\"%s\",\"%s\",%hhu,%hhu", ap, passPhrase, chl, ecn)
       && _parser.recv("OK");
    _smutex.unlock();
    return ret;
}

bool ESP32::get_ssid(char *ap)
{
    bool ret;

    _smutex.lock();
    _startup_wifi();
    ret = _parser.send("AT+CWJAP?")
       && _parser.recv("+CWJAP:\"%33[^\"]\",", ap)
       && _parser.recv("OK");
    _smutex.unlock();
    return ret;
}

bool ESP32::disconnect(void)
{
    bool ret;

    _smutex.lock();
    _startup_wifi();
    ret = _parser.send("AT+CWQAP") && _parser.recv("OK");
    _smutex.unlock();
    return ret;
}

const char *ESP32::getIPAddress(void)
{
    bool ret;

    _smutex.lock();
    _startup_wifi();
    ret = _parser.send("AT+CIFSR")
       && _parser.recv("+CIFSR:STAIP,\"%15[^\"]\"", _ip_buffer)
       && _parser.recv("OK");
    _smutex.unlock();
    if (!ret) {
        return 0;
    }
    return _ip_buffer;
}

const char *ESP32::getIPAddress_ap(void)
{
    bool ret;

    _smutex.lock();
    _startup_wifi();
    ret = _parser.send("AT+CIFSR")
       && _parser.recv("+CIFSR:APIP,\"%15[^\"]\"", _ip_buffer_ap)
       && _parser.recv("OK");
    _smutex.unlock();
    if (!ret) {
        return 0;
    }
    return _ip_buffer_ap;
}

const char *ESP32::getMACAddress(void)
{
    bool ret;

    _smutex.lock();
    _startup_wifi();
    ret = _parser.send("AT+CIFSR")
       && _parser.recv("+CIFSR:STAMAC,\"%17[^\"]\"", _mac_buffer)
       && _parser.recv("OK");
    _smutex.unlock();

    if (!ret) {
        return 0;
    }
    return _mac_buffer;
}

const char *ESP32::getMACAddress_ap(void)
{
    bool ret;

    _smutex.lock();
    _startup_wifi();
    ret = _parser.send("AT+CIFSR")
       && _parser.recv("+CIFSR:APMAC,\"%17[^\"]\"", _mac_buffer_ap)
       && _parser.recv("OK");
    _smutex.unlock();

    if (!ret) {
        return 0;
    }
    return _mac_buffer_ap;
}

const char *ESP32::getGateway()
{
    bool ret;

    _smutex.lock();
    _startup_wifi();
    ret = _parser.send("AT+CIPSTA?")
       && _parser.recv("+CIPSTA:gateway:\"%15[^\"]\"", _gateway_buffer)
       && _parser.recv("OK");
    _smutex.unlock();

    if (!ret) {
        return 0;
    }
    return _gateway_buffer;
}

const char *ESP32::getGateway_ap()
{
    bool ret;

    _smutex.lock();
    _startup_wifi();
    ret = _parser.send("AT+CIPAP?")
       && _parser.recv("+CIPAP:gateway:\"%15[^\"]\"", _gateway_buffer_ap)
       && _parser.recv("OK");
    _smutex.unlock();

    if (!ret) {
        return 0;
    }
    return _gateway_buffer_ap;
}

const char *ESP32::getNetmask()
{
    bool ret;

    _smutex.lock();
    _startup_wifi();
    ret = _parser.send("AT+CIPSTA?")
       && _parser.recv("+CIPSTA:netmask:\"%15[^\"]\"", _netmask_buffer)
       && _parser.recv("OK");
    _smutex.unlock();

    if (!ret) {
        return 0;
    }
    return _netmask_buffer;
}

const char *ESP32::getNetmask_ap()
{
    bool ret;

    _smutex.lock();
    _startup_wifi();
    ret = _parser.send("AT+CIPAP?")
       && _parser.recv("+CIPAP:netmask:\"%15[^\"]\"", _netmask_buffer_ap)
       && _parser.recv("OK");
    _smutex.unlock();

    if (!ret) {
        return 0;
    }
    return _netmask_buffer_ap;
}

int8_t ESP32::getRSSI()
{
    bool ret;
    int8_t rssi[2]; /* It needs 1 byte extra. */
    char ssid[33];
    char bssid[18];

    _smutex.lock();
    _startup_wifi();
    ret = _parser.send("AT+CWJAP?")
       && _parser.recv("+CWJAP:\"%32[^\"]\",\"%17[^\"]\"", ssid, bssid)
       && _parser.recv("OK");
    if (!ret) {
        _smutex.unlock();
        return 0;
    }

    ret = _parser.send("AT+CWLAP=\"%s\",\"%s\"", ssid, bssid)
       && _parser.recv("+CWLAP:(%*d,\"%*[^\"]\",%hhd,", &rssi[0])
       && _parser.recv("OK");
    _smutex.unlock();

    if (!ret) {
        return 0;
    }

    return rssi[0];
}

int ESP32::scan(WiFiAccessPoint *res, unsigned limit)
{
    unsigned cnt = 0;
    nsapi_wifi_ap_t ap;

    if (!_init_end_wifi) {
        _smutex.lock();
        _startup_wifi();
        _smutex.unlock();
        ThisThread::sleep_for(1500ms);
    }

    _smutex.lock();
    setTimeout(5000);
    if (!_parser.send("AT+CWLAP")) {
        _smutex.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    while (recv_ap(&ap)) {
        if (cnt < limit) {
            res[cnt] = WiFiAccessPoint(ap);
        }

        cnt++;
        if ((limit != 0) && (cnt >= limit)) {
            setTimeout(10);
            _parser.recv("OK");
            break;
        }
        setTimeout(500);
    }
    setTimeout();
    _smutex.unlock();

    return cnt;
}

bool ESP32::isConnected(void)
{
    return getIPAddress() != 0;
}

bool ESP32::open(const char *type, int id, const char* addr, int port, int opt)
{
    bool ret;

    if (id >= SOCKET_COUNT) {
        return false;
    }
    _cbs[id].Notified = 0;

    _smutex.lock();
    _startup_wifi();
    setTimeout(ESP32_SEND_TIMEOUT);
    if (opt != 0) {
        ret = _parser.send("AT+CIPSTART=%d,\"%s\",\"%s\",%d,%d", id, type, addr, port, opt)
           && _parser.recv("OK");
    } else {
        ret = _parser.send("AT+CIPSTART=%d,\"%s\",\"%s\",%d", id, type, addr, port)
           && _parser.recv("OK");
    }
    setTimeout();
    _clear_socket_packets(id);
    _smutex.unlock();

    return ret;
}

bool ESP32::send(int id, const void *data, uint32_t amount)
{
    int send_size;
    int max_send_size = 2048;
    bool ret;
    int error_cnt = 0;
    int index = 0;

    _cbs[id].Notified = 0;
    if (amount == 0) {
        return true;
    }

    if (_cts == NC) {
        max_send_size = 512;
    }

    //May take a second try if device is busy
    while (error_cnt < 2) {
        _smutex.lock();
        if ((_id_bits & (1 << id)) == 0) {
            _smutex.unlock();
            return false;
        }
        send_size = amount;
        if (send_size > max_send_size) {
            send_size = max_send_size;
        }
        _startup_wifi();
        setTimeout(ESP32_SEND_TIMEOUT);
        ret = _parser.send("AT+CIPSEND=%d,%d", id, send_size)
           && _parser.recv(">")
           && (_parser.write((char*)data + index, (int)send_size) >= 0)
           && _parser.recv("SEND OK");
        setTimeout();
        _smutex.unlock();
        if (ret) {
            amount -= send_size;
            index += send_size;
            error_cnt = 0;
            if (amount == 0) {
                return true;
            }
        } else {
            error_cnt++;
        }
    }

    return false;
}

void ESP32::_packet_handler()
{
    int id;
    int amount;
    uint32_t tmp_timeout;

    // parse out the packet
    if (!_parser.recv(",%d,%d:", &id, &amount)) {
        return;
    }

    struct packet *packet = (struct packet*)malloc(
            sizeof(struct packet) + amount);
    if (!packet) {
        return;
    }

    packet->id = id;
    packet->len = amount;
    packet->next = 0;
    packet->index = 0;

    tmp_timeout = last_timeout_ms;
    setTimeout(500);
    if (!(_parser.read((char*)(packet + 1), amount))) {
        free(packet);
        setTimeout(tmp_timeout);
        return;
    }

    // append to packet list
    *_packets_end = packet;
    _packets_end = &packet->next;
}

int32_t ESP32::recv(int id, void *data, uint32_t amount, uint32_t timeout)
{
    struct packet **p;
    uint32_t idx = 0;

    _cbs[id].Notified = 0;

    _smutex.lock();
    setTimeout(timeout);
    if (_rts == NC) {
        while (_parser.process_oob()); // Poll for inbound packets
    } else {
        _parser.process_oob(); // Poll for inbound packets
    }
    setTimeout();

    // check if any packets are ready for us
    p = &_packets;
    while (*p) {
        if ((*p)->id == id) {
            struct packet *q = *p;

            if (q->len <= amount) { // Return and remove full packet
                memcpy(&(((uint8_t *)data)[idx]), (uint8_t*)(q+1) + q->index, q->len);
                if (_packets_end == &(*p)->next) {
                    _packets_end = p;
                }
                *p = (*p)->next;
                idx += q->len;
                amount -= q->len;
                free(q);
            } else { // return only partial packet
                memcpy(&(((uint8_t *)data)[idx]), (uint8_t*)(q+1) + q->index, amount);
                q->len -= amount;
                q->index += amount;
                idx += amount;
            }
            break;
        } else {
            p = &(*p)->next;
        }
    }
    _smutex.unlock();

    if (idx > 0) {
        return idx;
    } else if ((_id_bits & (1 << id)) == 0) {
        return 0;
    } else {
        _cbs[id].Notified = 0;
        return -1;
    }
}

void ESP32::_clear_socket_packets(int id)
{
    struct packet **p = &_packets;

    while (*p) {
        if ((*p)->id == id || id == ESP32_ALL_SOCKET_IDS) {
            struct packet *q = *p;

            if (_packets_end == &(*p)->next) {
                _packets_end = p; // Set last packet next field/_packets
            }
            *p = (*p)->next;

            free(q);
        } else {
            // Point to last packet next field
            p = &(*p)->next;
        }
    }
}

bool ESP32::close(int id, bool wait_close)
{
    if (wait_close) {
        _smutex.lock();
        for (int j = 0; j < 2; j++) {
            if ((_id_bits & (1 << id)) == 0) {
                _ids[id] = false;
                _clear_socket_packets(id);
                _smutex.unlock();
                return true;
            }
            _startup_wifi();
            setTimeout(500);
            _parser.process_oob(); // Poll for inbound packets
            setTimeout();
        }
        _smutex.unlock();
    }

    //May take a second try if device is busy
    for (unsigned i = 0; i < 2; i++) {
        _smutex.lock();
        if ((_id_bits & (1 << id)) == 0) {
            _ids[id] = false;
            _clear_socket_packets(id);
            _smutex.unlock();
            return true;
        }
        _startup_wifi();
        setTimeout(500);
        if (_parser.send("AT+CIPCLOSE=%d", id)
            && _parser.recv("OK")) {
            setTimeout();
            _clear_socket_packets(id);
            _ids[id] = false;
            _smutex.unlock();
            return true;
        }
        setTimeout();
        _smutex.unlock();
    }

    _ids[id] = false;
    return false;
}

void ESP32::setTimeout(uint32_t timeout_ms)
{
    last_timeout_ms = timeout_ms;
    _parser.set_timeout(timeout_ms);
}

bool ESP32::readable()
{
    return _serial.FileHandle::readable();
}

bool ESP32::writeable()
{
    return _serial.FileHandle::writable();
}

void ESP32::socket_attach(int id, void (*callback)(void *), void *data)
{
    _cbs[id].callback = callback;
    _cbs[id].data = data;
    _cbs[id].Notified = 0;
}

bool ESP32::recv_ap(nsapi_wifi_ap_t *ap)
{
    bool ret;
    int c;
    const char keyword_0[8] = "+CWLAP:";
    const char keyword_1[6] = "\nOK\r\n";
    int idx_0 = 0;
    int idx_1 = 0;

    while (true) {
        c = _parser.getc();
        if (c < 0) {
            ret = false;
            break;
        }
        if ((char)c == keyword_0[idx_0]) {
            idx_0++;
        } else {
            idx_0 = 0;
        }
        if ((char)c == keyword_1[idx_1]) {
            idx_1++;
        } else {
            idx_1 = 0;
        }

        // "+CWLAP:"
        if (idx_0 >= (int)(sizeof(keyword_0) - 1)) {
            int sec;
            uint8_t work_buf[6+1]; /* It needs 1 byte extra. */
            int8_t  work_rssi[2];  /* It needs 1 byte extra. */

            ret = _parser.recv("(%d,\"%32[^\"]\",%hhd,\"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx\",%hhu)", &sec, ap->ssid,
                               &work_rssi[0], &work_buf[0], &work_buf[1], &work_buf[2], &work_buf[3], &work_buf[4],
                               &work_buf[5], &ap->channel);
            ap->rssi = work_rssi[0];
            memcpy(ap->bssid, work_buf, 6);
            ap->security = sec < 5 ? (nsapi_security_t)sec : NSAPI_SECURITY_UNKNOWN;
            break;
        }

        // "\nOK\r\n"
        if (idx_1 >= (int)(sizeof(keyword_1) - 1)) {
            ret = false;
            break;
        }
    }

    return ret;
}

void ESP32::_connect_handler_0() { socket_handler(true, 0);  }
void ESP32::_connect_handler_1() { socket_handler(true, 1);  }
void ESP32::_connect_handler_2() { socket_handler(true, 2);  }
void ESP32::_connect_handler_3() { socket_handler(true, 3);  }
void ESP32::_connect_handler_4() { socket_handler(true, 4);  }
void ESP32::_closed_handler_0()  { socket_handler(false, 0); }
void ESP32::_closed_handler_1()  { socket_handler(false, 1); }
void ESP32::_closed_handler_2()  { socket_handler(false, 2); }
void ESP32::_closed_handler_3()  { socket_handler(false, 3); }
void ESP32::_closed_handler_4()  { socket_handler(false, 4); }

void ESP32::_connection_status_handler()
{
    char status[13];
    if (_parser.recv("%12[^\"]\n", status)) {
        if (strcmp(status, "CONNECTED\n") == 0) {
            _wifi_status = STATUS_CONNECTED;
        } else if (strcmp(status, "GOT IP\n") == 0) {
            _wifi_status = STATUS_GOT_IP;
        } else if (strcmp(status, "DISCONNECT\n") == 0) {
            _wifi_status = STATUS_DISCONNECTED;
        } else {
            return;
        }

        if(_wifi_status_cb) {
            _wifi_status_cb(_wifi_status);
        }
    }
}

int ESP32::get_free_id()
{
    // Look for an unused socket
    int id = -1;

    for (int i = 0; i < SOCKET_COUNT; i++) {
        if ((!_ids[i]) && ((_id_bits & (1 << i)) == 0)) {
            id = i;
            _ids[i] = true;
            break;
        }
    }

    return id;
}

void ESP32::event() {
#if defined(TARGET_ESP32AT_BLE)
    if ((_cbs_ble.callback) && (_cbs_ble.Notified == 0)) {
        _cbs_ble.Notified = 1;
        _cbs_ble.callback();
    }
#endif /* TARGET_ESP32AT_BLE */
    for (int i = 0; i < SOCKET_COUNT; i++) {
        if ((_cbs[i].callback) && (_cbs[i].Notified == 0)) {
            _cbs[i].Notified = 1;
            _cbs[i].callback(_cbs[i].data);
        }
    }
}

bool ESP32::set_network(const char *ip_address, const char *netmask, const char *gateway)
{
    bool ret;

    if (ip_address == NULL) {
        return false;
    }

    _smutex.lock();
    if ((netmask != NULL) && (gateway != NULL)) {
        ret = _parser.send("AT+CIPSTA=\"%s\",\"%s\",\"%s\"", ip_address, gateway, netmask)
           && _parser.recv("OK");
    } else {
        ret = _parser.send("AT+CIPSTA=\"%s\"", ip_address)
           && _parser.recv("OK");
    }
    _smutex.unlock();

    return ret;
}

bool ESP32::set_network_ap(const char *ip_address, const char *netmask, const char *gateway)
{
    bool ret;

    if (ip_address == NULL) {
        return false;
    }

    _smutex.lock();
    if ((netmask != NULL) && (gateway != NULL)) {
        ret = _parser.send("AT+CIPAP=\"%s\",\"%s\",\"%s\"", ip_address, gateway, netmask)
           && _parser.recv("OK");
    } else {
        ret = _parser.send("AT+CIPAP=\"%s\"", ip_address)
           && _parser.recv("OK");
    }
    _smutex.unlock();

    return ret;
}

void ESP32::attach_wifi_status(mbed::Callback<void(int8_t)> status_cb)
{
    _wifi_status_cb = status_cb;
}

int8_t ESP32::get_wifi_status() const
{
    return _wifi_status;
}

void ESP32::flush()
{
    _smutex.lock();
    _parser.flush();
    _smutex.unlock();
}


#if defined(TARGET_ESP32AT_BLE)
bool ESP32::ble_set_role(int role)
{
    if ((role != INIT_CLIENT_ROLE) && (role != INIT_SERVER_ROLE)) {
        return false;
    }
    if (_ble_role != role) {
        _ble_role = role;
        if (_init_end_ble) {
            return restart();
        }
    }
    return true;
}

bool ESP32::ble_get_role(int * role)
{
    *role = _ble_role;
    return true;
}

bool ESP32::ble_set_device_name(const char * name)
{
    bool ret;

    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("AT+BLENAME=\"%s\"", name)
       && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_get_device_name(char * name)
{
    bool ret;

    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("AT+BLENAME?")
       && _parser.recv("+BLENAME:%s\n", name);
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}


bool ESP32::ble_start_services()
{
    bool ret;

    ble_set_role(INIT_SERVER_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("AT+BLEGATTSSRVCRE")
       && _parser.recv("OK")
       && _parser.send("AT+BLEGATTSSRVSTART")
       && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_set_scan_response(const uint8_t * data, int len)
{
    bool ret;
    int size = 0;
    char * wk_buf = new char[1024];

    if (wk_buf == NULL) {
        return false;
    }

    sprintf(&wk_buf[size], "AT+BLESCANRSPDATA=\"");
    size = strlen(wk_buf);
    _set_char(&wk_buf[size], data, len);
    strcat(wk_buf, "\"");

    ble_set_role(INIT_SERVER_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("%s", wk_buf)
        && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();

    delete [] wk_buf;

    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_start_advertising()
{
    bool ret;

    ble_set_role(INIT_SERVER_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("AT+BLEADVSTART")
       && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_stop_advertising()
{
    bool ret;

    ble_set_role(INIT_SERVER_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("AT+BLEADVSTOP")
       && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_set_addr(int addr_type, const uint8_t * random_addr)
{
    bool ret;

    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    if ((addr_type == 1) && (random_addr != NULL)) {
        ret = _parser.send("AT+BLEADDR=1,\"%02x:%02x:%02x:%02x:%02x:%02x\"",
                           random_addr[0], random_addr[1], random_addr[2], random_addr[3], random_addr[4], random_addr[5])
           && _parser.recv("OK");
    } else {
        ret = _parser.send("AT+BLEADDR=%d", addr_type)
           && _parser.recv("OK");
    }
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_get_addr(uint8_t * public_addr)
{
    bool ret;
    uint8_t work_buf[6+1]; /* It needs 1 byte extra. */

    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("AT+BLEADDR?")
       && _parser.recv("+BLEADDR:%hhx:%hhx:%hhx:%hhx:%hhx:%hhx\n",
                       &work_buf[0], &work_buf[1], &work_buf[2], &work_buf[3], &work_buf[4], &work_buf[5])
       && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    memcpy(public_addr, work_buf, 6);

    return true;
}

bool ESP32::ble_set_advertising_param(const advertising_param_t * param)
{
    bool ret;

    ble_set_role(INIT_SERVER_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("AT+BLEADVPARAM=%d,%d,%d,%d,%d,%d,%d,\"%02x:%02x:%02x:%02x:%02x:%02x\"",
                        param->adv_int_min, param->adv_int_max, param->adv_type, param->own_addr_type,
                        param->channel_map, param->adv_filter_policy, param->peer_addr_type,
                        param->peer_addr[0], param->peer_addr[1], param->peer_addr[2],
                        param->peer_addr[3], param->peer_addr[4], param->peer_addr[5])
       && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_set_advertising_data(const uint8_t * data, int len)
{
    bool ret;
    int size = 0;
    char * wk_buf = new char[1024];

    if (wk_buf == NULL) {
        return false;
    }

    sprintf(&wk_buf[size], "AT+BLEADVDATA=\"");
    size = strlen(wk_buf);
    _set_char(&wk_buf[size], data, len);
    strcat(wk_buf, "\"");

    ble_set_role(INIT_SERVER_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("%s", wk_buf)
        && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();

    delete [] wk_buf;

    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_set_service(const gatt_service_t * service_list, int num)
{
    uint8_t header[17] = {0x9D,0x10,0x27,0x95,0x7B,0x22,0x53,0x65,0x72,0x76,0x69,0x63,0x65,0x22,0x3A,0x20,0x5B};
    bool ret;
    int idx = 0;
    int size = 0;
    uint8_t wk_data[4];
    char * wk_buf = new char[1024];

    if (wk_buf == NULL) {
        return false;
    }

    ble_set_role(INIT_SERVER_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);

    size = sizeof(header);
    ret = _parser.send("AT+SYSFLASH=0,\"ble_data\"")
       && _parser.recv("OK")
       && _parser.send("AT+SYSFLASH=1,\"ble_data\",0,%d", size)
       && _parser.recv(">")
       && (_parser.write((char*)header, size) >= 0)
       && _parser.recv("OK");
    idx += size;
    if (!ret) {
        _smutex.unlock();
        delete [] wk_buf;
        return false;
    }

    for (int i = 0; i < num; i++) {
        size = 0;
        sprintf(&wk_buf[size], "{\"index\": %d, \"uuid\": \"", i);
        size = strlen(wk_buf);
        if ((service_list->uuid_type != 0) && (service_list->uuid_size <= 4)) {
            for (int j = 0; j < service_list->uuid_size; j++) {
                wk_data[j] = (service_list->uuid.data >> (8 * (service_list->uuid_size - 1 - j))) & 0x00FF;
            }
            _set_char(&wk_buf[size], wk_data, service_list->uuid_size);
        } else {
            _set_char(&wk_buf[size], service_list->uuid.addr, service_list->uuid_size);
        }
        size = strlen(wk_buf);

        sprintf(&wk_buf[size], "\", \"uuid_len\": %d, \"val_max_len\": %d, \"value\": \"",
                service_list->uuid_size * 8, service_list->val_max_len);
        size = strlen(wk_buf);

        if ((service_list->value_type != 0) && (service_list->value_size <= 4)) {
            for (int j = 0; j < service_list->value_size; j++) {
                wk_data[j] = (service_list->value.data >> (8 * (service_list->value_size - 1 - j))) & 0x00FF;
            }
            _set_char(&wk_buf[size], wk_data, service_list->value_size);
        } else {
            _set_char(&wk_buf[size], service_list->value.addr, service_list->value_size);
        }
        size = strlen(wk_buf);

        sprintf(&wk_buf[size], "\", \"perm\": %d, \"val_cur_len\": %d}",
                service_list->permissions, service_list->value_size);
        if ((i + 1) == num) {
            strcat(wk_buf, "]}");
        } else {
            strcat(wk_buf, ", ");
        }
        size = strlen(wk_buf);

        ret = _parser.send("AT+SYSFLASH=1,\"ble_data\",%d,%d", idx, size)
           && _parser.recv(">")
           && (_parser.write((char*)wk_buf, size) >= 0)
           && _parser.recv("OK");
        idx += size;
        if (!ret) {
            break;
        }
        service_list++;
    }
    delete [] wk_buf;

    setTimeout();
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_set_characteristic(int srv_index, int char_index, const uint8_t * data, int len)
{
    bool ret;

    ble_set_role(INIT_SERVER_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("AT+BLEGATTSSETATTR=%d,%d,,%d", srv_index, char_index, len)
       && _parser.recv(">")
       && (_parser.write((char*)data, len) >= 0)
       && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_notifies_characteristic(int srv_index, int char_index, const uint8_t * data, int len)
{
    bool ret;

    ble_set_role(INIT_SERVER_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("AT+BLEGATTSNTFY=0,%d,%d,%d", srv_index, char_index, len)
       && _parser.recv(">")
       && (_parser.write((char*)data, len) >= 0)
       && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_set_scan_param(int scan_type, int own_addr_type, int filter_policy, int scan_interval, int scan_window)
{
    bool ret;

    ble_set_role(INIT_CLIENT_ROLE);
    _smutex.lock();
    _startup_ble();
    ret = _parser.send("AT+BLESCANPARAM=%d,%d,%d,%d,%d",
                       scan_type, own_addr_type, filter_policy, scan_interval, scan_window)
       && _parser.recv("OK");
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_start_scan(int interval)
{
    bool ret;

    ble_set_role(INIT_CLIENT_ROLE);
    _smutex.lock();
    _startup_ble();
    ret = _parser.send("AT+BLESCAN=1,%d", interval);
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_stop_scan()
{
    bool ret;

    ble_set_role(INIT_CLIENT_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("AT+BLESCAN=0")
       && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_connect(int conn_index, const uint8_t * remote_addr)
{
    bool ret;

    ble_set_role(INIT_CLIENT_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("AT+BLECONN=%d,\"%02x:%02x:%02x:%02x:%02x:%02x\"", conn_index,
                       remote_addr[0], remote_addr[1], remote_addr[2],
                       remote_addr[3], remote_addr[4], remote_addr[5])
       && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_disconnect(int conn_index)
{
    bool ret;

    ble_set_role(INIT_CLIENT_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("AT+BLEDISCONN=%d", conn_index)
       && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_discovery_service(int conn_index, ble_primary_service_t * service, int * num)
{
    bool ret;
    int cpy_num;

    if ((service == NULL) || (num == NULL)) {
        return false;
    }

    ble_set_role(INIT_CLIENT_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    _primary_service_idx = 0;
    ret = _parser.send("AT+BLEGATTCPRIMSRV=%d", conn_index)
       && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        *num = 0;
        return false;
    }

    cpy_num = *num;
    if (cpy_num > _primary_service_idx) {
        cpy_num = _primary_service_idx;
    }
    for (int i = 0; i < cpy_num; i++) {
        service[i] = _primary_service[i];
    }
    *num = cpy_num;

    return true;
}

bool ESP32::ble_discovery_characteristics(
    int conn_index, int srv_index,
    ble_discovers_char_t * discovers_char, int * char_num,
    ble_discovers_desc_t * discovers_desc, int * desc_num
)
{
    bool ret;
    int cpy_num;

    ble_set_role(INIT_CLIENT_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    _discovers_char_idx = 0;
    _discovers_desc_idx = 0;
    ret = _parser.send("AT+BLEGATTCCHAR=%d,%d", conn_index, srv_index)
       && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        if (char_num != NULL) {
            *char_num = 0;
        }
        if (desc_num != NULL) {
            *desc_num = 0;
        }
        return false;
    }

    if ((discovers_char != NULL) && (char_num != NULL)) {
        cpy_num = *char_num;
        if (cpy_num > _discovers_char_idx) {
            cpy_num = _discovers_char_idx;
        }
        for (int i = 0; i < cpy_num; i++) {
            discovers_char[i] = _discovers_char[i];
        }
        *char_num = cpy_num;
    }

    if ((discovers_desc != NULL) && (desc_num != NULL)) {
        cpy_num = *desc_num;
        if (cpy_num > _discovers_desc_idx) {
            cpy_num = _discovers_desc_idx;
        }
        for (int i = 0; i < cpy_num; i++) {
            discovers_desc[i] = _discovers_desc[i];
        }
        *desc_num = cpy_num;
    }

    return true;
}

int32_t ESP32::ble_read_characteristic(int conn_index, int srv_index, int char_index, uint8_t * data, int amount)
{
    bool ret;
    int wk_conn_index;
    int data_len;
    int idx = 0;;
    char c;

    ble_set_role(INIT_CLIENT_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("AT+BLEGATTCRD=%d,%d,%d", conn_index, srv_index, char_index)
       && _parser.recv("+BLEGATTCRD:%d,%d,", &wk_conn_index, &data_len);
    if (!ret) {
        setTimeout();
        _smutex.unlock();
        return -1;
    }
    for (int i = 0; i < data_len; i++) {
        c = _parser.getc();
        if (idx < amount) {
            data[idx++] = (uint8_t)c;
        }
    }
    _parser.recv("OK");

    setTimeout();
    _smutex.unlock();

    return idx;
}

int32_t ESP32::ble_read_descriptor(int conn_index, int srv_index, int char_index, int desc_index, uint8_t * data, int amount)
{
    bool ret;
    int wk_conn_index;
    int data_len;
    int idx = 0;;
    char c;

    ble_set_role(INIT_CLIENT_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("AT+BLEGATTCRD=%d,%d,%d,%d", conn_index, srv_index, char_index, desc_index)
       && _parser.recv("+BLEGATTCRD:%d,%d,", &wk_conn_index, &data_len);
    if (!ret) {
        setTimeout();
        _smutex.unlock();
        return 0;
    }
    for (int i = 0; i < data_len; i++) {
        c = _parser.getc();
        if (idx < amount) {
            data[idx++] = (uint8_t)c;
        }
    }
    _parser.recv("OK");

    setTimeout();
    _smutex.unlock();

    return idx;
}

bool ESP32::ble_write_characteristic(int conn_index, int srv_index, int char_index, const uint8_t * data, int amount)
{
    bool ret;

    ble_set_role(INIT_CLIENT_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("AT+BLEGATTCWR=%d,%d,%d,,%d", conn_index, srv_index, char_index, amount)
       && _parser.recv(">")
       && (_parser.write((char*)data, amount) >= 0)
       && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}

bool ESP32::ble_write_descriptor(int conn_index, int srv_index, int char_index, int desc_index, const uint8_t * data, int amount)
{
    bool ret;

    ble_set_role(INIT_CLIENT_ROLE);
    _smutex.lock();
    _startup_ble();
    setTimeout(ESP32_MISC_TIMEOUT);
    ret = _parser.send("AT+BLEGATTCWR=%d,%d,%d,%d,%d", conn_index, srv_index, char_index, desc_index, amount)
       && _parser.recv(">")
       && (_parser.write((char*)data, amount) >= 0)
       && _parser.recv("OK");
    setTimeout();
    _smutex.unlock();
    if (!ret) {
        return false;
    }
    return true;
}

void ESP32::ble_process_oob(uint32_t timeout, bool all)
{
    _smutex.lock();
    _cbs_ble.Notified = 0;
    setTimeout(timeout);
    // Poll for inbound packets
    while (_parser.process_oob() && all) {
    }
    setTimeout();
    _smutex.unlock();
}

void ESP32::ble_attach_sigio(mbed::Callback<void()> cb_func)
{
    _smutex.lock();
    _cbs_ble.Notified = 0;
    _cbs_ble.callback = cb_func;
    _smutex.unlock();
}

void ESP32::ble_attach_conn(mbed::Callback<void(int, uint8_t *)> cb_func)
{
    _smutex.lock();
    _ble_conn_cb = cb_func;
    _smutex.unlock();
}

void ESP32::ble_attach_disconn(mbed::Callback<void(int)> cb_func)
{
    _smutex.lock();
    _ble_disconn_cb = cb_func;
    _smutex.unlock();
}

void ESP32::ble_attach_write(mbed::Callback<void(ble_packet_t *)> cb_func)
{
    _smutex.lock();
    _ble_write_cb = cb_func;
    _smutex.unlock();
}

void ESP32::ble_attach_scan(mbed::Callback<void(ble_scan_t *)> cb_func)
{
    _smutex.lock();
    _ble_scan_cb = cb_func;
    _smutex.unlock();
}

bool ESP32::_startup_ble()
{
    _startup_common();

    if (_init_end_ble) {
        return true;
    }

    if (_at_version < 0x01010300) {
        printf("Please update ESP32 FW \"AT version:1.1.3.0\" or later.\r\n");
        mbed_die();
    }

    setTimeout(ESP32_MISC_TIMEOUT);
    bool success = _parser.send("AT+BLEINIT=%d", _ble_role)
                && _parser.recv("OK");
    setTimeout();
    if (success) {
        _init_end_ble = true;
    }

    return success;
}

void ESP32::_ble_conn()
{
    int conn_index;
    uint8_t remote_addr[6+1]; /* It needs 1 byte extra. */

    _parser.recv("%d,\"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx\"", &conn_index,
                 &remote_addr[0], &remote_addr[1], &remote_addr[2], &remote_addr[3], &remote_addr[4], &remote_addr[5]);
    if(_ble_conn_cb) {
        _ble_conn_cb(conn_index, remote_addr);
    }
}

void ESP32::_ble_disconn()
{
    int conn_index;

    _parser.recv("%d", &conn_index);
    if(_ble_disconn_cb) {
        _ble_disconn_cb(conn_index);
    }
}

void ESP32::_ble_write()
{
    int conn_index;
    int srv_index;
    int char_index;
    int desc_index = -1;
    int amount;
    char c;
    uint32_t tmp_timeout;

    if(!_ble_write_cb) {
        return;
    }

    _parser.recv("%d,%d,%d,", &conn_index, &srv_index, &char_index);
    (void)conn_index;
    c = _parser.getc();
    if (c != ',') {
        desc_index = c;
        _parser.getc(); // to read ',' after desc_index.
    }
    _parser.recv("%d,", &amount);

    ble_packet_t *ble_packet = (ble_packet_t*)malloc(
            sizeof(ble_packet_t) + amount);
    if (!ble_packet) {
        return;
    }

    ble_packet->srv_index  = srv_index;
    ble_packet->char_index = char_index;
    ble_packet->desc_index = desc_index;
    ble_packet->len = amount;
    ble_packet->data = ble_packet + 1;

    tmp_timeout = last_timeout_ms;
    setTimeout(500);
    if (!(_parser.read((char*)(ble_packet + 1), amount))) {
        free(ble_packet);
        setTimeout(tmp_timeout);
        return;
    }
    if(_ble_write_cb) {
        _ble_write_cb(ble_packet);
    }
    free(ble_packet);
}

void ESP32::_ble_scan()
{
    char c;
    int idx;
    uint8_t work_buf[7];  /* It needs 1 byte extra. */
    int8_t  work_rssi[2]; /* It needs 1 byte extra. */

    if(!_ble_scan_cb) {
        return;
    }

    ble_scan_t *ble_scan = (ble_scan_t*)malloc(sizeof(ble_scan_t));
    if (!ble_scan) {
        return;
    }

    _parser.recv("%hhx:%hhx:%hhx:%hhx:%hhx:%hhx,%hhd,",
                 &work_buf[0], &work_buf[1], &work_buf[2],
                 &work_buf[3], &work_buf[4], &work_buf[5],
                 &work_rssi[0]);

    memcpy(ble_scan->addr, work_buf, 6);
    ble_scan->rssi = work_rssi[0];

    idx = 0;
    for (int i = 0; i < (31 * 2); i++) {
        c = _parser.getc();
        if (c == ',') {
            break;
        }
        if ((i & 0x01) == 0) {
            ble_scan->adv_data[idx] = (_char2int(c) << 4);
        } else {
            ble_scan->adv_data[idx] += _char2int(c);
            idx++;
        }
    }
    ble_scan->adv_data_len = idx;

    if (c != ',') {
        c = _parser.getc();
    }

    idx = 0;
    for (int i = 0; i < (31 * 2); i++) {
        c = _parser.getc();
        if (c == ',') {
            break;
        }
        if ((i & 0x01) == 0) {
            ble_scan->scan_rsp_data[idx] = (_char2int(c) << 4);
        } else {
            ble_scan->scan_rsp_data[idx] += _char2int(c);
            idx++;
        }
    }
    ble_scan->scan_rsp_data_len = idx;

    if (c != ',') {
        c = _parser.getc();
    }

    _parser.recv("%hhd\n", &work_buf[0]);
    ble_scan->addr_type = work_buf[0];

    if(_ble_scan_cb) {
        _ble_scan_cb(ble_scan);
    }
    free(ble_scan);
}

void ESP32::_ble_primsrv()
{
    // fix me (supports only short uuid)
    int conn_index;
    ble_primary_service_t * p_service = &_primary_service[_primary_service_idx];

    if (_primary_service_idx < PRIMARY_SERVICE_BUF_NUM) {
        if (_parser.recv("%d,%d,%hx,%d\n", &conn_index, &p_service->srv_index,
                          &p_service->srv_uuid, &p_service->srv_type)) {
            _primary_service_idx++;
        }
    }
}

void ESP32::_ble_discovers_char()
{
    // fix me (supports only short uuid)
    int conn_index;
    int srv_index;
    char type[4];
    uint8_t work_buf[2]; /* It needs 1 byte extra. */

    _parser.getc();  // skip '"'
    _parser.read(type, 4);
    _parser.getc();  // skip '"'

    if (_parser.recv(",%d,%d,", &conn_index, &srv_index)) {
        if (memcmp(type, "char", 4) == 0) {
            if (_discovers_char_idx < DISCOVERS_CHAR_BUF_NUM) {
                ble_discovers_char_t * p_char = &_discovers_char[_discovers_char_idx];
                if (_parser.recv("%d,%hx,%hhx\n", &p_char->char_index, &p_char->char_uuid, &work_buf[0])) {
                    p_char->char_prop = work_buf[0];
                    _discovers_char_idx++;
                }
            }
        } else if (memcmp(type, "desc", 4) == 0) {
            if (_discovers_desc_idx < DISCOVERS_DESC_BUF_NUM) {
                ble_discovers_desc_t * p_desc = &_discovers_desc[_discovers_desc_idx];
                if (_parser.recv("%d,%d,%hx\n", &p_desc->char_index, &p_desc->desc_index, &p_desc->desc_uuid)) {
                    _discovers_desc_idx++;
                }
            }
        } else {
            // do nothing
        }
    }
}

char ESP32::_int2char(int data)
{
    if ((data >= 0) && (data <= 9)) {
        return data + '0';
    } else if ((data >= 0xA) && (data <= 0xF)) {
        return data - 0xA + 'A';
    } else {
        return 0;
    }
}

int ESP32::_char2int(char c)
{
    if ((c >= '0') && (c <= '9')) {
        return c - '0';
    } else if ((c >= 'A') && (c <= 'F')) {
        return c - 'A' + 0xA;
    } else if ((c >= 'a') && (c <= 'f')) {
        return c - 'a' + 0xA;
    } else {
        return 0;
    }
}

int ESP32::_set_char(char * buf1, const uint8_t * buf2, int size)
{
    int idx = 0;

    for (int i = 0; i < size; i++) {
        buf1[idx++] = _int2char((buf2[i]>> 4) & 0x0F);
        buf1[idx++] = _int2char(buf2[i] & 0x0F);
    }
    buf1[idx] = '\0';

    return idx;
}
#endif /* TARGET_ESP32AT_BLE */

#endif

