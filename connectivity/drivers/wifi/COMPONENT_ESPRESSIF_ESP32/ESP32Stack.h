/* ESP32 implementation of NetworkInterfaceAPI
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

#ifndef ESP32_STACK_H
#define ESP32_STACK_H


#include "ESP32.h"
#include "netsocket/NetworkInterface.h"
#include "netsocket/NetworkStack.h"
#include "netsocket/nsapi_types.h"
#include "netsocket/WiFiInterface.h"

/** ESP32Stack class
 *  Implementation of the NetworkStack for the ESP32
 */
class ESP32Stack : public NetworkStack {
protected:
    /** ESP32Stack lifetime
     * @param en        EN pin
     * @param io0       IO0 pin
     * @param tx        TX pin
     * @param rx        RX pin
     * @param debug     Enable debugging
     * @param rts       RTS pin
     * @param cts       CTS pin
     * @param baudrate  The baudrate of the serial port.
     * @param is_ap     0:Terminal 1:Access point.
     */
    ESP32Stack(PinName en, PinName io0, PinName tx, PinName rx, bool debug,
               PinName rts, PinName cts, int baudrate, int is_ap);

protected:
    int socket_open(void **handle, nsapi_protocol_t proto) override;

    int socket_close(void *handle) override;

    int socket_bind(void *handle, const SocketAddress &address) override;

    int socket_listen(void *handle, int backlog) override;

    int socket_connect(void *handle, const SocketAddress &address) override;

    int socket_accept(void *handle, void **socket, SocketAddress *address) override;

    int socket_send(void *handle, const void *data, unsigned size) override;

    int socket_recv(void *handle, void *data, unsigned size) override;

    int socket_sendto(void *handle, const SocketAddress &address, const void *data, unsigned size) override;

    int socket_recvfrom(void *handle, SocketAddress *address, void *buffer, unsigned size) override;

    void socket_attach(void *handle, void (*callback)(void *), void *data) override;

    nsapi_error_t setsockopt(nsapi_socket_t handle, int level,
                             int optname, const void *optval, unsigned optlen) override;

    nsapi_error_t getsockopt(nsapi_socket_t handle, int level,
                             int optname, void *optval, unsigned *optlen) override;

protected:
    ESP32 *_esp;
    uint16_t _local_ports[ESP32::SOCKET_COUNT];
    int _is_ap;
};

#endif
