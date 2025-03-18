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
#include <mbed_shared_queues.h>
#include <ThisThread.h>

#include <algorithm>

#define TRACE_GROUP "CEMAC"

// Flags for the MAC thread
static uint32_t THREAD_FLAG_TX_DESC_AVAILABLE = 1 << 0;
static uint32_t THREAD_FLAG_RX_DESC_AVAILABLE = 1 << 1;
static uint32_t THREAD_FLAG_RX_MEM_AVAILABLE = 1 << 2;
static uint32_t THREAD_FLAG_SHUTDOWN = 1 << 3;

namespace mbed {
    // Defined in PhyDrivers.cpp
    CompositeEMAC::PHYDriver * get_eth_phy_driver();

    void CompositeEMAC::rxISR() {
        // Note: Not locking mutex here as this is an ISR and should be able to run while the MAC thread is executing.
        if(rxDMA.rxHasPackets_ISR()) {
            // Reclaimable descriptor or complete packet detected.
            macThread->flags_set(THREAD_FLAG_RX_DESC_AVAILABLE);
        }
    }

    void CompositeEMAC::txISR() {
        // Reclaimable Tx descriptor detected
        macThread->flags_set(THREAD_FLAG_TX_DESC_AVAILABLE);
    }

    void CompositeEMAC::phyTask() {
        rtos::ScopedMutexLock lock(macOpsMutex);

        // If the MAC has been powered off, bail immediately (this event is about to be canceled)
        if(state == PowerState::OFF) {
            return;
        }

        bool phyLinkState = false;
        if(phy->checkLinkStatus(phyLinkState) != ErrCode::SUCCESS) {
            tr_error("phyTask(): Phy failed to check link status");
        }

        if(linkState == LinkState::UP) {
            if(!phyLinkState) {
                tr_info("Link down");
                linkState = LinkState::DOWN;

                if(mac.disable() != ErrCode::SUCCESS) {
                    tr_error("phyTask(): Mac failed to disable");
                }

                linkStateCallback(false);
            }
        }
        else { // LinkState::DOWN
            if(phyLinkState) {
                Duplex duplex;
                LinkSpeed speed;
                if(phy->checkLinkType(speed, duplex)!= ErrCode::SUCCESS) {
                    tr_error("phyTask(): Phy failed to check link type");
                    return;
                }

                char const * speedStr;
                if(speed == LinkSpeed::LINK_10MBIT) {
                    speedStr = "10Mbps";
                }
                else if(speed == LinkSpeed::LINK_100MBIT) {
                    speedStr = "100Mbps";
                }
                else {
                    speedStr = "1Gbps";
                }

                tr_info("Link up at %s %s duplex", speedStr, duplex == Duplex::FULL ? "full" : "half");

                linkState = LinkState::UP;
                if(mac.enable(speed, duplex) != ErrCode::SUCCESS) {
                    tr_error("phyTask(): Mac failed to enable");
                }

                linkStateCallback(true);
            }
        }
    }

    void CompositeEMAC::macTask() {
        while(true)
        {
            // Wait for something to happen
            uint32_t flags = rtos::ThisThread::flags_wait_any(THREAD_FLAG_TX_DESC_AVAILABLE | THREAD_FLAG_SHUTDOWN | THREAD_FLAG_RX_DESC_AVAILABLE | THREAD_FLAG_RX_MEM_AVAILABLE);
            if(flags & THREAD_FLAG_SHUTDOWN)
            {
                return;
            }

            // Now lock the mutex for the other cases
            rtos::ScopedMutexLock lock(macOpsMutex);

            if(flags & THREAD_FLAG_RX_DESC_AVAILABLE)
            {
                // Receive any available packets.
                // Note that if the ISR was delayed, we might get multiple packets per ISR, so we need to loop.
                while(true)
                {
                    auto * packet = rxDMA.dequeuePacket();
                    if(!packet) {
                        break;
                    }

                    linkInputCallback(packet);

                    // Rebuild descriptors if possible
                    rxDMA.rebuildDescriptors();
                }
            }
            if(flags & THREAD_FLAG_TX_DESC_AVAILABLE)
            {
                txDMA.reclaimTxDescs();
            }
            if(flags & THREAD_FLAG_RX_MEM_AVAILABLE) {
                rxDMA.rebuildDescriptors();
            }
        }
    }

    void CompositeEMAC::onRxPoolSpaceAvail() {
        rtos::ScopedMutexLock lock(macOpsMutex);

        if(state == PowerState::OFF) {
            // MAC is off, not interested in callbacks
            return;
        }

        macThread->flags_set(THREAD_FLAG_RX_MEM_AVAILABLE);
    }

    void CompositeEMAC::get_ifname(char *name, uint8_t size) const {
        // Note that LwIP only supports a two character interface name prefix.
        // So, no point in going longer than that.
        // Also note that we don't want to copy the terminating null if it doesn't fit.
        const char * const ifPrefix = "en";
        memcpy(name, ifPrefix, (size < strlen(ifPrefix) + 1) ? size : strlen(ifPrefix) + 1);
    }

    void CompositeEMAC::set_hwaddr(const uint8_t *addr) {
        rtos::ScopedMutexLock lock(macOpsMutex);

        if(state != PowerState::ON_NO_LINK) {
            tr_err("MAC address can only be set after power up, before link up!");
            return;
        }

        MACAddress macAddr;
        memcpy(macAddr.data(), addr, MAC_ADDR_SIZE);
        mac.setOwnMACAddr(macAddr);
    }

    bool CompositeEMAC::link_out(emac_mem_buf_t *buf)
    {
        rtos::ScopedMutexLock lock(macOpsMutex);
        const auto ret = txDMA.txPacket(buf);

        if(ret != ErrCode::SUCCESS) {
            tr_warn("link_out(): Tx failed.");
        }
        return ret == ErrCode::SUCCESS;
    }

    bool CompositeEMAC::power_up()
    {
        rtos::ScopedMutexLock lock(macOpsMutex);

        if(state != PowerState::OFF) {
            tr_err("power_up(): Already powered up!");
            return false;
        }

        // Get phy
        phy = get_eth_phy_driver();
        if(phy == nullptr) {
            tr_err("power_up(): No Ethernet PHY driver configured! Either set nsapi.emac-phy-model or override mbed::get_eth_phy_driver().");
            return false;
        }

        // Set memory manager everywhere
        if(memory_manager == nullptr) {
            tr_err("power_up(): Memory manager not set yet!");
            return false;
        }
        txDMA.setMemoryManager(memory_manager);
        rxDMA.setMemoryManager(memory_manager);

        // Register memory available callback
        memory_manager->set_on_pool_space_avail_cb(callback(this, &CompositeEMAC::onRxPoolSpaceAvail));

        // Power up the MAC
        if(mac.init() != ErrCode::SUCCESS) {
            tr_err("power_up(): Failed to init MAC!");
            return false;
        }

        // Init DMA rings
        if(txDMA.init() != ErrCode::SUCCESS || rxDMA.init() != ErrCode::SUCCESS) {
            tr_err("power_up(): Failed to init DMA!");
            return false;
        }

        // Initialize the PHY
        phy->setMAC(&mac);
        if(phy->init() != ErrCode::SUCCESS) {
            tr_err("power_up(): Failed to init PHY!");
            return false;
        }

        state = PowerState::ON_NO_LINK;

        // Start phy task
        phyTaskHandle = mbed_event_queue()->call_every(std::chrono::milliseconds(MBED_CONF_NSAPI_EMAC_PHY_POLL_PERIOD),
                callback(this, &CompositeEMAC::phyTask));

        // Start MAC thread.
        // We want to run this thread at high priority since reclaiming descriptors generally needs to happen quickly
        // for the application to use the network at full speed.
        macThread = new rtos::Thread(osPriorityHigh, 2048, nullptr, "EMAC Thread");
        macThread->start(callback(this, &CompositeEMAC::macTask));

        return true;
    }

    void CompositeEMAC::power_down() {
        // Stop MAC thread (don't need to lock mutex for this)
        macThread->flags_set(THREAD_FLAG_SHUTDOWN);
        macThread->join();
        delete macThread;

        rtos::ScopedMutexLock lock(macOpsMutex);

        mbed_event_queue()->cancel(phyTaskHandle);

        state = PowerState::OFF;
        linkState = LinkState::DOWN;

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

    void CompositeEMAC::set_link_input_cb(emac_link_input_cb_t input_cb) {
        if(state != PowerState::OFF) {
            tr_err("Not available while MAC is on!");
            return;
        }
        linkInputCallback = input_cb;
    }

    void CompositeEMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb) {
        if(state != PowerState::OFF) {
            tr_err("Not available while MAC is on!");
            return;
        }
        linkStateCallback = state_cb;
    }

    void CompositeEMAC::add_multicast_group(const uint8_t *address)
    {
        rtos::ScopedMutexLock lock(macOpsMutex);

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

    void CompositeEMAC::remove_multicast_group(const uint8_t *address) {
        rtos::ScopedMutexLock lock(macOpsMutex);

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

    void CompositeEMAC::set_all_multicast(bool all) {
        rtos::ScopedMutexLock lock(macOpsMutex);

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
}