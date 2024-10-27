/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
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

#ifndef EMAC_TEST_NETWORK_STACK_H
#define EMAC_TEST_NETWORK_STACK_H

#include "netsocket/nsapi_types.h"
#include "netsocket/EMAC.h"
#include "netsocket/OnboardNetworkStack.h"

#include "EmacTestMemoryManager.h"
#include "emac_util.h"

class EmacTestNetworkStack : public OnboardNetworkStack, private mbed::NonCopyable<EmacTestNetworkStack> {

    unsigned char eth_mac_addr[ETH_MAC_ADDR_LEN];

    bool emac_if_init(EMAC *emac);
public:

    static EmacTestNetworkStack &get_instance();

    EmacTestNetworkStack();
    virtual ~EmacTestNetworkStack() {}

    class Interface : public OnboardNetworkStack::Interface {
    public:

        /** Set MAC address on the network interface
         *
         *  @param          mac_addr Buffer containing the MAC address in hexadecimal format.
         *  @param          addr_len Length of provided buffer in bytes (6 or 8)
         *  @retval         NSAPI_ERROR_OK on success
         *  @retval         NSAPI_ERROR_UNSUPPORTED if this feature is not supported
         *  @retval         NSAPI_ERROR_PARAMETER if address is not valid
         *  @retval         NSAPI_ERROR_BUSY if address can't be set.
         */
        nsapi_error_t set_mac_address(uint8_t *mac_addr, nsapi_size_t addr_len);

        nsapi_error_t bringup(bool dhcp, const char *ip,
                              const char *netmask, const char *gw,
                              nsapi_ip_stack_t stack,
                              bool blocking
                             ) override;

        nsapi_error_t bringdown() override;

        void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb) override;

        nsapi_connection_status_t get_connection_status() const override;

        char *get_mac_address(char *buf, nsapi_size_t buflen) override;

        nsapi_error_t get_ip_address(SocketAddress *address) override;

        nsapi_error_t get_netmask(SocketAddress *address) override;

        nsapi_error_t get_gateway(SocketAddress *address) override;
    private:
        friend EmacTestNetworkStack;

        explicit Interface(EmacTestNetworkStack &netStack);

        EmacTestNetworkStack &m_netStack;
        EMAC *m_emac;
    };

    nsapi_error_t add_ethernet_interface(EMAC &emac, bool default_if, OnboardNetworkStack::Interface **interface_out, NetworkInterface *user_network_interface) override;

    /// Get a pointer to the EMAC driver.  Will return nullptr if no interface has been added yet.
    EMAC *get_emac();

    /// Get the MAC address being used for the ethernet port
    unsigned char const *get_mac_addr() const;

protected:

    nsapi_error_t socket_open(nsapi_socket_t *handle, nsapi_protocol_t proto) override;

    nsapi_error_t socket_close(nsapi_socket_t handle) override;

    nsapi_error_t socket_bind(nsapi_socket_t handle, const SocketAddress &address) override;

    nsapi_error_t socket_listen(nsapi_socket_t handle, int backlog) override;

    nsapi_error_t socket_connect(nsapi_socket_t handle, const SocketAddress &address) override;

    nsapi_error_t socket_accept(nsapi_socket_t server,
                                nsapi_socket_t *handle, SocketAddress *address = 0) override;

    nsapi_size_or_error_t socket_send(nsapi_socket_t handle,
                                      const void *data, nsapi_size_t size) override;

    nsapi_size_or_error_t socket_recv(nsapi_socket_t handle,
                                      void *data, nsapi_size_t size) override;

    nsapi_size_or_error_t socket_sendto(nsapi_socket_t handle, const SocketAddress &address,
                                        const void *data, nsapi_size_t size) override;

    nsapi_size_or_error_t socket_recvfrom(nsapi_socket_t handle, SocketAddress *address,
                                          void *buffer, nsapi_size_t size) override;

    void socket_attach(nsapi_socket_t handle, void (*callback)(void *), void *data) override;

    nsapi_error_t setsockopt(nsapi_socket_t handle, int level,
                             int optname, const void *optval, unsigned optlen) override;

    nsapi_error_t getsockopt(nsapi_socket_t handle, int level,
                             int optname, void *optval, unsigned *optlen) override;

private:
    call_in_callback_cb_t get_call_in_callback() override;

    nsapi_error_t call_in(int delay, mbed::Callback<void()> func) override;

    Interface m_interface;
};

#endif /* EMAC_TEST_NETWORK_STACK_H */
