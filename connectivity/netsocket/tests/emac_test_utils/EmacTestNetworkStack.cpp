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
#include "EMACMemoryManager.h"
#include "EmacTestNetworkStack.h"

#include "mbed_interface.h"

bool EmacTestNetworkStack::emac_if_init(EMAC *emac)
{
    emac->set_link_input_cb(emac_if_link_input_cb);
    emac->set_link_state_cb(emac_if_link_state_change_cb);

    if (!emac->power_up()) {
        printf("emac power up failed!");
        return false;
    }

    int hwaddr_len = emac->get_hwaddr_size();
    printf("emac hwaddr length %i\r\n\r\n", hwaddr_len);

    if (hwaddr_len != 6) {
        printf("invalid emac hwaddr length %d!\n", hwaddr_len);
        return false;
    }

    // If driver updates this, write it back, otherwise write default from mbed_mac_address
    mbed_mac_address(reinterpret_cast<char *>(&eth_mac_addr[0]));
    emac->get_hwaddr(eth_mac_addr);
    emac->set_hwaddr(eth_mac_addr);
    printf("emac hwaddr %x:%x:%x:%x:%x:%x\r\n\r\n", eth_mac_addr[0], eth_mac_addr[1], eth_mac_addr[2], eth_mac_addr[3], eth_mac_addr[4], eth_mac_addr[5]);

    int mtu_size = emac->get_mtu_size();
    printf("emac mtu %i\r\n\r\n", mtu_size);
    emac_if_set_mtu_size(mtu_size);

    char hw_name[11];
    emac->get_ifname(hw_name, 10);
    printf("emac if name %s\r\n\r\n", hw_name);

    return true;
}

EmacTestNetworkStack::EmacTestNetworkStack()
    : m_interface(*this)
{

}


nsapi_error_t EmacTestNetworkStack::call_in(int delay, mbed::Callback<void()> func)
{
    // Implemented as empty to save memory
    return NSAPI_ERROR_DEVICE_ERROR;
}

EmacTestNetworkStack::call_in_callback_cb_t EmacTestNetworkStack::get_call_in_callback()
{
    call_in_callback_cb_t cb(this, &EmacTestNetworkStack::call_in);
    return cb;
}

nsapi_error_t EmacTestNetworkStack::socket_open(nsapi_socket_t *handle, nsapi_protocol_t proto)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t EmacTestNetworkStack::socket_close(nsapi_socket_t handle)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t EmacTestNetworkStack::socket_bind(nsapi_socket_t handle, const SocketAddress &address)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t EmacTestNetworkStack::socket_listen(nsapi_socket_t handle, int backlog)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t EmacTestNetworkStack::socket_connect(nsapi_socket_t handle, const SocketAddress &address)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t EmacTestNetworkStack::socket_accept(nsapi_socket_t server, nsapi_socket_t *handle, SocketAddress *address)
{
    return NSAPI_ERROR_OK;
}

nsapi_size_or_error_t EmacTestNetworkStack::socket_send(nsapi_socket_t handle, const void *data, nsapi_size_t size)
{
    return NSAPI_ERROR_OK;
}

nsapi_size_or_error_t EmacTestNetworkStack::socket_recv(nsapi_socket_t handle, void *data, nsapi_size_t size)
{
    return NSAPI_ERROR_OK;
}

nsapi_size_or_error_t EmacTestNetworkStack::socket_sendto(nsapi_socket_t handle, const SocketAddress &address, const void *data, nsapi_size_t size)
{
    return size;
}

nsapi_size_or_error_t EmacTestNetworkStack::socket_recvfrom(nsapi_socket_t handle, SocketAddress *address, void *data, nsapi_size_t size)
{
    return NSAPI_ERROR_DEVICE_ERROR;
}

nsapi_error_t EmacTestNetworkStack::setsockopt(nsapi_socket_t handle, int level, int optname, const void *optval, unsigned optlen)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t EmacTestNetworkStack::getsockopt(nsapi_socket_t handle, int level, int optname, void *optval, unsigned *optlen)
{
    return NSAPI_ERROR_OK;
}

void EmacTestNetworkStack::socket_attach(nsapi_socket_t handle, void (*callback)(void *), void *data)
{

}

nsapi_error_t EmacTestNetworkStack::add_ethernet_interface(EMAC &emac, bool default_if, OnboardNetworkStack::Interface **interface_out, NetworkInterface *user_network_interface)
{
    // Only one interface is supported
    if (m_interface.m_emac != nullptr) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    m_interface.m_emac = &emac;

    EmacTestMemoryManager *memory_manager = &EmacTestMemoryManager::get_instance();
    emac.set_memory_manager(*memory_manager);

    *interface_out = &m_interface;

    return NSAPI_ERROR_OK;
}

EMAC *EmacTestNetworkStack::get_emac()
{
    return m_interface.m_emac;
}

unsigned char const *EmacTestNetworkStack::get_mac_addr() const
{
    return eth_mac_addr;
}

EmacTestNetworkStack::Interface::Interface(EmacTestNetworkStack &netStack):
    m_netStack(netStack),
    m_emac(nullptr)
{

}


void EmacTestNetworkStack::Interface::attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{

}

nsapi_connection_status_t EmacTestNetworkStack::Interface::get_connection_status() const
{
    return NSAPI_STATUS_ERROR_UNSUPPORTED;
}

char *EmacTestNetworkStack::Interface::get_mac_address(char *buf, nsapi_size_t buflen)
{
    return nullptr;
}

nsapi_error_t EmacTestNetworkStack::Interface::set_mac_address(uint8_t *buf, nsapi_size_t buflen)
{
    return NSAPI_STATUS_ERROR_UNSUPPORTED;
}

nsapi_error_t EmacTestNetworkStack::Interface::get_ip_address(SocketAddress *address)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t EmacTestNetworkStack::Interface::get_netmask(SocketAddress *address)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t EmacTestNetworkStack::Interface::get_gateway(SocketAddress *address)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t EmacTestNetworkStack::Interface::bringup(bool dhcp, const char *ip, const char *netmask, const char *gw, const nsapi_ip_stack_t stack, bool blocking)
{
    if (!m_netStack.emac_if_init(m_emac)) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t EmacTestNetworkStack::Interface::bringdown()
{
    return NSAPI_ERROR_OK;
}

EmacTestNetworkStack &EmacTestNetworkStack::get_instance()
{
    static EmacTestNetworkStack test_stack;
    return test_stack;
}
