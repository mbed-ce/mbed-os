/* Copyright (c) 2024 Jamie Smith
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


#include "CompositeEMAC.h"

#include <mbed_trace.h>

#include <algorithm>

#define TRACE_GROUP "CEMAC"

void mbed::CompositeEMAC::get_ifname(char *name, uint8_t size) const {
    // Note that LwIP only supports a two character interface name prefix.
    // So, no point in going longer than that.
    // Also note that we don't want to copy the terminating null if it doesn't fit.
    const char * const ifPrefix = "en";
    memcpy(name, ifPrefix, (size < strlen(ifPrefix) + 1) ? size : strlen(ifPrefix) + 1);
}

void mbed::CompositeEMAC::set_hwaddr(const uint8_t *addr) {
    if(state != PowerState::ON_NO_LINK) {
        tr_err("MAC address can only be set after power up, before link up!");
        return;
    }

    MACAddress macAddr;
    memcpy(macAddr.data(), addr, MAC_ADDR_SIZE);
    mac.setOwnMACAddr(macAddr);
}

bool mbed::CompositeEMAC::link_out(emac_mem_buf_t *buf)
{

}

bool mbed::CompositeEMAC::power_up()
{
    if(state != PowerState::OFF) {
        tr_err("power_up(): Already powered up!");
        return false;
    }

    // Set memory manager everywhere
    if(memory_manager == nullptr) {
        tr_err("power_up(): Memory manager not set yet!");
        return false;
    }
    txDMA.setMemoryManager(memory_manager);
    rxDMA.setMemoryManager(memory_manager);

    // Power up the MAC
    if(mac.init() != ErrCode::SUCCESS) {
        tr_err("power_up(): Failed to init MAC!");
        return false;
    }

    state = PowerState::ON_NO_LINK;
    return true;
}

void mbed::CompositeEMAC::power_down() {
    // TODO stop phy task

    state = PowerState::OFF;

    // TODO sync with other thread(s), ensure that no other threads are accessing the MAC
    // (lock mutex?)

    // Clear multicast filter, so that we start with a clean slate next time
    if(mac.clearMcastFilter() != ErrCode::SUCCESS) {
        tr_err("power_down(): Failed to clear mcast filter");
        return;
    }

    // Disable tx & rx
    if(mac.disable() != ErrCode::SUCCESS) {
        tr_err("power_down(): Failed to disable MAC");
        return;
    }

    // Disable DMA
    if(txDMA.deinit() != ErrCode::SUCCESS || rxDMA.deinit() != ErrCode::SUCCESS) {
        tr_err("power_down(): Failed to disable DMA");
        return;
    }

    // Finally, disable the MAC itself
    if(mac.deinit() != ErrCode::SUCCESS) {
        tr_err("power_down(): Failed to disable MAC");
    }
}

void mbed::CompositeEMAC::add_multicast_group(const uint8_t *address)
{
    if(state == PowerState::OFF) {
        tr_err("Not available while MAC is off!");
        return;
    }

    ++numSubscribedMcastMacs;

    if(numSubscribedMcastMacs >= MBED_CONF_NSAPI_EMAC_MAX_MCAST_SUBSCRIBES)
    {
        tr_warn("Out of multicast group entries (currently have %d). Increase the 'nsapi.emac-max-mcast-subscribes' JSON option!", MBED_CONF_NSAPI_EMAC_MAX_MCAST_SUBSCRIBES);
        // Fall back to accepting all multicasts
        set_all_multicast(true);
        return;
    }

    memcpy(mcastMacs[numSubscribedMcastMacs - 1].data(), address, 6);

    if(mac.addMcastMAC(mcastMacs[numSubscribedMcastMacs - 1]) != ErrCode::SUCCESS) {
        tr_err("addMcastMAC() failed!");
    }
}

void mbed::CompositeEMAC::remove_multicast_group(const uint8_t *address) {
    if(state == PowerState::OFF) {
        tr_err("Not available while MAC is off!");
        return;
    }

    if(numSubscribedMcastMacs >= MBED_CONF_NSAPI_EMAC_MAX_MCAST_SUBSCRIBES) {
        // We are in fallback mode, so we can no longer unsusbscribe at the MAC level because we don't know
        // which MACs are subscribed anymore.
        return;
    }

    // Find MAC address in the subscription list
    auto macsEndIter = std::begin(mcastMacs) + numSubscribedMcastMacs;
    auto toRemoveIter = std::find_if(std::begin(mcastMacs), macsEndIter, [&](auto element) {
        return memcmp(element.data(), address, 6) == 0;
    });

    if(toRemoveIter == macsEndIter)
    {
        tr_warning("Tried to remove mcast group that was not added");
        return;
    }

    // Swap the MAC addr to be removed to the end of the list, if it is not there already
    auto lastElementIter = macsEndIter - 1;
    if(toRemoveIter != std::begin(mcastMacs) && toRemoveIter != lastElementIter)
    {
        std::swap(*toRemoveIter, *lastElementIter);
    }

    // 'remove' the last element by changing the length
    numSubscribedMcastMacs--;

    // Clear the filter and re-add all the addresses except the desired one
    if(mac.clearMcastFilter() != ErrCode::SUCCESS) {
        tr_err("clearMcastFilter() failed!");
        return;
    }
    for(size_t macIdx = 0; macIdx < numSubscribedMcastMacs; ++macIdx) {
        if(mac.addMcastMAC(mcastMacs[macIdx]) != ErrCode::SUCCESS) {
            tr_err("addMcastMAC() failed!");
        }
    }
}

void mbed::CompositeEMAC::set_all_multicast(bool all) {
    if(state == PowerState::OFF) {
        tr_err("Not available while MAC is off!");
        return;
    }

    if(numSubscribedMcastMacs >= MBED_CONF_NSAPI_EMAC_MAX_MCAST_SUBSCRIBES && !all) {
        // Don't allow setting pass all multicast to off while we are in fallback mode
        return;
    }

    mac.setPassAllMcast(all);
}
