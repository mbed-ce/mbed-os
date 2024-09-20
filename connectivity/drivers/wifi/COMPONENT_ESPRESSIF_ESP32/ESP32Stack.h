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

#include "mbed.h"
#include "ESP32.h"

/** ESP32Stack class
 *  Implementation of the NetworkStack for the ESP32
 */
class ESP32Stack : public NetworkStack
{
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
    /** Open a socket
     *  @param handle       Handle in which to store new socket
     *  @param proto        Type of socket to open, NSAPI_TCP or NSAPI_UDP
     *  @return             0 on success, negative on failure
     */
    virtual int socket_open(void **handle, nsapi_protocol_t proto);

    /** Close the socket
     *  @param handle       Socket handle
     *  @return             0 on success, negative on failure
     *  @note On failure, any memory associated with the socket must still
     *        be cleaned up
     */
    virtual int socket_close(void *handle);

    /** Bind a server socket to a specific port
     *  @param handle       Socket handle
     *  @param address      Local address to listen for incoming connections on
     *  @return             0 on success, negative on failure.
     */
    virtual int socket_bind(void *handle, const SocketAddress &address);

    /** Start listening for incoming connections
     *  @param handle       Socket handle
     *  @param backlog      Number of pending connections that can be queued up at any
     *                      one time [Default: 1]
     *  @return             0 on success, negative on failure
     */
    virtual int socket_listen(void *handle, int backlog);

    /** Connects this TCP socket to the server
     *  @param handle       Socket handle
     *  @param address      SocketAddress to connect to
     *  @return             0 on success, negative on failure
     */
    virtual int socket_connect(void *handle, const SocketAddress &address);

    /** Accept a new connection.
     *  @param handle       Handle in which to store new socket
     *  @param server       Socket handle to server to accept from
     *  @return             0 on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_accept(void *handle, void **socket, SocketAddress *address);

    /** Send data to the remote host
     *  @param handle       Socket handle
     *  @param data         The buffer to send to the host
     *  @param size         The length of the buffer to send
     *  @return             Number of written bytes on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_send(void *handle, const void *data, unsigned size);

    /** Receive data from the remote host
     *  @param handle       Socket handle
     *  @param data         The buffer in which to store the data received from the host
     *  @param size         The maximum length of the buffer
     *  @return             Number of received bytes on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_recv(void *handle, void *data, unsigned size);

    /** Send a packet to a remote endpoint
     *  @param handle       Socket handle
     *  @param address      The remote SocketAddress
     *  @param data         The packet to be sent
     *  @param size         The length of the packet to be sent
     *  @return             The number of written bytes on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_sendto(void *handle, const SocketAddress &address, const void *data, unsigned size);

    /** Receive a packet from a remote endpoint
     *  @param handle       Socket handle
     *  @param address      Destination for the remote SocketAddress or null
     *  @param buffer       The buffer for storing the incoming packet data
     *                      If a packet is too long to fit in the supplied buffer,
     *                      excess bytes are discarded
     *  @param size         The length of the buffer
     *  @return             The number of received bytes on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_recvfrom(void *handle, SocketAddress *address, void *buffer, unsigned size);

    /** Register a callback on state change of the socket
     *  @param handle       Socket handle
     *  @param callback     Function to call on state change
     *  @param data         Argument to pass to callback
     *  @note Callback may be called in an interrupt context.
     */
    virtual void socket_attach(void *handle, void (*callback)(void *), void *data);

    /** Set stack-specific socket options
     *  The setsockopt allow an application to pass stack-specific hints
     *  to the underlying stack. For unsupported options,
     *  NSAPI_ERROR_UNSUPPORTED is returned and the socket is unmodified.
     *
     *  @param handle   Socket handle
     *  @param level    Stack-specific protocol level
     *  @param optname  Stack-specific option identifier
     *  @param optval   Option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t setsockopt(nsapi_socket_t handle, int level,
            int optname, const void *optval, unsigned optlen);

    /** Get stack-specific socket options
     *  The getstackopt allow an application to retrieve stack-specific hints
     *  from the underlying stack. For unsupported options,
     *  NSAPI_ERROR_UNSUPPORTED is returned and optval is unmodified.
     *
     *  @param handle   Socket handle
     *  @param level    Stack-specific protocol level
     *  @param optname  Stack-specific option identifier
     *  @param optval   Destination for option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t getsockopt(nsapi_socket_t handle, int level,
            int optname, void *optval, unsigned *optlen);

protected:
    ESP32 *_esp;
    uint16_t _local_ports[ESP32::SOCKET_COUNT];
    int _is_ap;
};

#endif
