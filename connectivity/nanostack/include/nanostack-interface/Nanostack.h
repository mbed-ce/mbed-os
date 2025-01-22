/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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

#ifndef NANOSTACK_H_
#define NANOSTACK_H_

#include "OnboardNetworkStack.h"
#include "NanostackMemoryManager.h"
#include "MeshInterface.h"
#include "mesh_interface_types.h"
#include "eventOS_event.h"

struct ns_address;

class Nanostack : public OnboardNetworkStack, private mbed::NonCopyable<Nanostack> {
public:
    static Nanostack &get_instance();

    // Our Nanostack::Interface etc are defined by mbed_mesh_api
    class Interface;
    class EthernetInterface;
    class MeshInterface;
    class LoWPANNDInterface;
    class ThreadInterface;
    class WisunInterface;
    class PPPInterface;

    /* Implement OnboardNetworkStack method */
    nsapi_error_t add_ethernet_interface(EMAC &emac, bool default_if, OnboardNetworkStack::Interface **interface_out, NetworkInterface *user_network_interface = NULL) override;
    nsapi_error_t add_ethernet_interface(EMAC &emac, bool default_if, OnboardNetworkStack::Interface **interface_out, const uint8_t *mac_addr, NetworkInterface *user_network_interface = NULL) override;

    /* Local variant with stronger typing and manual address specification */
    nsapi_error_t add_ethernet_interface(EMAC &emac, bool default_if, Nanostack::EthernetInterface **interface_out, const uint8_t *mac_addr = NULL, NetworkInterface *user_network_interface = NULL);

    nsapi_error_t add_ppp_interface(PPP &ppp, bool default_if, OnboardNetworkStack::Interface **interface_out) override;

    /* Local variant with stronger typing and manual address specification */
    nsapi_error_t add_ppp_interface(PPP &ppp, bool default_if, Nanostack::PPPInterface **interface_out);

    nsapi_error_t remove_ppp_interface(OnboardNetworkStack::Interface **interface_out) override;

    /// Get the memory manager for the Nanostack stack
    NanostackMemoryManager &get_memory_manager()
    {
        return memory_manager;
    }

protected:

    Nanostack();

    nsapi_error_t get_ip_address(SocketAddress *address) override;

    nsapi_error_t gethostbyname(const char *host, SocketAddress *address, nsapi_version_t version, const char *interface_name) override;

    nsapi_value_or_error_t gethostbyname_async(const char *host, hostbyname_cb_t callback, nsapi_version_t version, const char *interface_name) override;

    nsapi_error_t get_dns_server(int index, SocketAddress *address, const char *interface_name) override;

    nsapi_error_t socket_open(void **handle, nsapi_protocol_t proto) override;

    nsapi_error_t socket_close(void *handle) override;

    nsapi_error_t socket_bind(void *handle, const SocketAddress &address) override;

    nsapi_error_t socket_listen(void *handle, int backlog) override;

    nsapi_error_t socket_connect(void *handle, const SocketAddress &address) override;

    nsapi_error_t socket_accept(void *handle, void **server, SocketAddress *address) override;

    nsapi_size_or_error_t socket_send(void *handle, const void *data, nsapi_size_t size) override;

    nsapi_size_or_error_t socket_recv(void *handle, void *data, nsapi_size_t size) override;

    nsapi_size_or_error_t socket_sendto(void *handle, const SocketAddress &address, const void *data, nsapi_size_t size) override;

    nsapi_size_or_error_t socket_recvfrom(void *handle, SocketAddress *address, void *buffer, nsapi_size_t size) override;

    void socket_attach(void *handle, void (*callback)(void *), void *data) override;

    nsapi_error_t setsockopt(void *handle, int level, int optname, const void *optval, unsigned optlen) override;

    nsapi_error_t getsockopt(void *handle, int level, int optname, void *optval, unsigned *optlen) override;

private:

    call_in_callback_cb_t get_call_in_callback() override;

    nsapi_error_t call_in(int delay, mbed::Callback<void()> func) override;

    struct nanostack_callback {
        mbed::Callback<void()> callback;
    };

    nsapi_size_or_error_t do_sendto(void *handle, const struct ns_address *address, const void *data, nsapi_size_t size);
    static void call_event_tasklet_main(arm_event_s *event);
    char text_ip_address[40];
    NanostackMemoryManager memory_manager;
    int8_t call_event_tasklet;
};

nsapi_error_t map_mesh_error(mesh_error_t err);

#endif /* NANOSTACK_H_ */
