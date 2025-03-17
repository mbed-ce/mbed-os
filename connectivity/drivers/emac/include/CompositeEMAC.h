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

#ifndef MBED_OS_COMPOSITEETHMAC_H
#define MBED_OS_COMPOSITEETHMAC_H

#include "EMAC.h"
#include "NonCopyable.h"
#include "Thread.h"

#include <atomic>

namespace mbed
{

/**
 * @brief Implementation of the EMAC API built up from several components implemented by device-specific classes.
 *
 * \par Motivation
 * Originally, Mbed interfaced with the Ethernet hardware on an MCU via a single class, an implementation of the
 * EMAC interface.  However, EMAC is a large interface, encompassing many responsibilities: setting up the pins,
 * managing the Ethernet peripheral, controlling the phy chip, and putting data into and out of DMA.  Many of these
 * pieces, such as PHY control and managing memory buffers, are common, but others are unique to each MCU family.
 * To better divide responsibility into common and target-specific parts, and to allow Ethernet drivers to
 * be organized more logically, the CompositeEMAC class was created.
 *
 * \par Division
 * CompositeEMAC divides Ethernet functionality up into several different classes, each with different
 * responsibilities:
 * <ul>
 * <li>\c MACDriver : Driver for the MAC peripheral itself.  Provides functionality to map MAC pins, start up and
 *   configure the MAC, configure interrupts, and communicate with the phy over MDIO.</li>
 * <li>\c PHYDriver : Communicates with the phy.  Responsible for configuring it and setting its status.</li>
 * <li>\c TxDMA : Driver for the Tx DMA ring.  Takes in Tx packets and queues them for transmission in the
 *   hardware.</li>
 * <li>\c RxDMA : Driver for the Rx DMA ring.  Sets up Rx descriptors and dequeues them once packets are received.</li>
 * </ul>
 *
 * \note CompositeEMAC itself does not use any global data and supports multiple instances for MCUs that have
 *    multiple EMACs.  However, the implementation for a specific MCU may or may not use global data -- if
 *    there's only one EMAC on the MCU, there isn't really a reason not to.
 */
class CompositeEMAC : public EMAC
{
public:
    enum class ErrCode
    {
        SUCCESS = 0,
        TIMEOUT = 1,
        HW_ERROR = 2,
        PHY_NOT_RESPONDING = 3,
        OUT_OF_MEMORY = 4,
        INVALID_ARGUMENT = 5,
        INVALID_USAGE = 6,
        NEGOTIATION_FAILED = 7,
    };

    /// Enumeration of possible Ethernet link speeds
    enum class LinkSpeed
    {
        LINK_10MBIT,
        LINK_100MBIT,
        LINK_1GBIT
    };

    /// Enumeration of possible Ethernet link duplexes
    enum class Duplex
    {
        HALF,
        FULL
    };

    /// Basic MAC address type
    static constexpr size_t MAC_ADDR_SIZE = 6;
    typedef std::array<uint8_t, MAC_ADDR_SIZE> MACAddress;

    /**
     * @brief Abstract interface for a driver for the low level ethernet MAC hardware.
     *
     * Thread safety: CompositeEMAC will guarantee only one thread is interacting with this class at a time.
     */
    class MACDriver : NonCopyable<MACDriver>
    {
    public:
        virtual ~MACDriver() = default;

        /**
         * @brief Initialize the MAC, map pins, and prepare it to send and receive packets.
         *    It should not be enabled yet.
         *
         * Implementations of this method should:
         * - Lock deep sleep (unless the MAC functions in deep sleep)
         * - Enable clock to the MAC
         * - Mux all MAC and PHY pins appropriately
         * - Configure MAC registers for operation (but NOT enable tx or rx, yet)
         *
         * @return Error code or SUCCESS
         */
        virtual ErrCode init() = 0;

        /**
         * @brief Deinit the MAC so that it's not using any clock/power. Should prepare for init() to be called
         *    again.
         *
         * After this function is called (from EMAC::power_down()), the MAC will not be used again
         * until power_up() is called and the process starts again.
         *
         * @return Error code or SUCCESS
         */
        virtual ErrCode deinit() = 0;

        /**
         * @brief Enable the MAC so that it can send and receive packets
         *
         * @param speed Speed of the link
         * @param duplex Duplex of the link
         *
         * @return Error code or SUCCESS
         */
        virtual ErrCode enable(LinkSpeed speed, Duplex duplex) = 0;

        /**
         * @brief Disable the MAC so that it will not send or receive packets
         *
         * @return Error code or SUCCESS
         */
        virtual ErrCode disable() = 0;

        /**
         * @brief Set the own address of this MAC.
         *
         * \note This shall be called by CompositeEMAC after init but before enable.
         *
         * @param ownAddress Address this MAC will use for itself on the network.
         */
        virtual void setOwnMACAddr(MACAddress const & ownAddress) = 0;

        /**
         * @brief Read a register from the PHY over the MDIO bus.
         *
         * @param devAddr PHY device address to read. This will usually be set via the phy strapping pins.
         * @param regAddr Register address from 0-31 to read.
         * @param result Result is returned here. Note that because MDIO is an open drain bus, a result of
         *     0xFFFF usually means the phy didn't respond at all.
         *
         * @return Error code or success.
         */
        virtual ErrCode mdioRead(uint8_t devAddr, uint8_t regAddr, uint16_t & result) = 0;

        /**
         * @brief Write a register to the PHY over the MDIO bus.
         *
         * @param devAddr PHY device address to write. This will usually be set via the phy strapping pins.
         * @param regAddr Register address from 0-31 to write.
         * @param data Data to write
         *
         * @return Error code or success.
         */
        virtual ErrCode mdioWrite(uint8_t devAddr, uint8_t regAddr, uint16_t data) = 0;

        /**
         * @brief Get the reset pin for the Ethernet PHY.
         *
         * @return Reset pin, or NC if the reset pin is not mapped
         */
        virtual PinName getPhyResetPin() = 0;

        /**
         * @brief Add a multicast MAC address that should be accepted by the MAC.
         *
         * \note Only a maximum of \c nsapi.emac-max-mcast-subscribes multicast addresses will be subscribed to
         * at once by the upper layer. If the application tried to subscribe to more than that, CEMAC will fall
         * back to enabling pass all mcast.
         *
         * @param mac MAC address to accept
         *
         * @return Error code or success
         */
        virtual ErrCode addMcastMAC(MACAddress mac) = 0;

        /**
         * @brief Clear the MAC multicast filter, removing all multicast subscriptions
         *
         * @return Error code or success
         */
        virtual ErrCode clearMcastFilter() = 0;

        /**
         * @brief Set whether the MAC passes all multicast traffic up to the application.
         *
         * CompositeEMAC will ensure this is called only after init(). It will call this either if
         * the network stack requests it, or if it can no longer track the mcast addresses that have
         * been added and wants to fall back to enabling all multicasts.
         *
         * @param pass True to pass all mcasts, false otherwise
         */
        virtual void setPassAllMcast(bool pass) = 0;

        /**
         * @brief Set promiscuous mode (where the Eth MAC passes all traffic up to the application, regardless
         *   of its destination address).
         *
         * CompositeEMAC will ensure this is called only after init().
         *
         * @param enable True to pass all traffic, false otherwise
         */
        virtual void setPromiscuous(bool enable) = 0;
    };

    /**
     * @brief Interface for a driver for the Ethernet PHY.
     *
     * Thread safety: CompositeEMAC will guarantee only one thread is utilizing this class at a time.
     */
    class PHYDriver : NonCopyable<PHYDriver>
    {
    protected:
        /// MAC driver. Shall be set in init().
        MACDriver * mac = nullptr;

    public:
        virtual ~PHYDriver() = default;

        /// Set the MAC driver of this PHY. Will be called by CompositeEMAC before init().
        void setMAC(MACDriver * mac) { this->mac = mac; }

        /**
         * @brief Initialize the PHY and set it up for Ethernet operation.
         *
         * @return Error code or success
         */
        virtual ErrCode init() = 0;

        /**
         * @brief Check whether the link is up or down
         *
         * @param[out] status Set to true or false depending on whether the link is up or down
         *
         * @return Error code or success
         */
        virtual ErrCode checkLinkStatus(bool & status) = 0;

        /**
         * @brief Get the negotiated (or preset) Ethernet speed and duplex, given that the link is up
         *
         * \note Result speed and duplex undefined if the link is not up.
         *
         * @param[out] speed Link speed
         * @param[out] duplex Link duplex
         *
         * @return Error code or success
         */
        virtual ErrCode checkLinkType(LinkSpeed & speed, Duplex & duplex) = 0;
    };

    /**
     * @brief Abstract interface for a driver for the Tx DMA ring in the Ethernet MAC.
     *
     * Thread safety: CompositeEMAC will guarantee only one thread is utilizing this class at a time.
     */
    class TxDMA {
    protected:
        /// Pointer to memory manager for the EMAC
        EMACMemoryManager * memory_manager = nullptr;

    public:
        virtual ~TxDMA() = default;

        /// Set the mem manager of this DMA ring. Will be called by CompositeEMAC before init().
        void setMemoryManager(EMACMemoryManager * memory_manager) { this->memory_manager = memory_manager; }

        /// Initialize this Tx DMA ring.
        virtual ErrCode init() = 0;

        /// Stop the DMA running.
        /// init() should be able to be called again after this function completes to restart DMA.
        /// This shall be called after the MAC is disabled but before it is powered down.
        virtual ErrCode deinit() = 0;

        /// Reclaims the Tx buffers for any transmitted packets and frees their memory.
        /// Invoked by the CompositeEMAC internal thread after a Tx interrupt happens.
        /// Returns true if any descriptors became available, false otherwise
        virtual bool reclaimTxDescs() = 0;

        /// Transmit a packet out of the Tx DMA ring.  Note that this function
        /// *takes ownership of* the passed packet and must free it either now or after
        /// it's been transmitted.
        /// Should block until there is space to transmit the packet.
        virtual ErrCode txPacket(net_stack_mem_buf_t * buf) = 0;
    };

    /**
     * @brief Abstract interface for a driver for the Rx DMA ring in the Ethernet MAC.
     *
     * Thread safety: CompositeEMAC will guarantee only one thread is utilizing this class at a time.
     */
    class RxDMA {
    protected:
        /// Pointer to memory manager for the EMAC
        EMACMemoryManager * memory_manager = nullptr;

    public:
        virtual ~RxDMA() = default;

        /// Set the mem manager of this DMA ring. Will be called by CompositeEMAC before init().
        void setMemoryManager(EMACMemoryManager * memory_manager) { this->memory_manager = memory_manager; }

        /// Initialize this Rx DMA ring.
        virtual ErrCode init() = 0;

        /// Stop the DMA running. init() should be able to be called again after this function completes to restart DMA.
        /// This shall be called after the MAC is disabled but before it is powered down.
        virtual ErrCode deinit() = 0;

        /**
         * @brief Check if the MAC may have a packet to receive. Called from the Rx ISR.
         *
         * This function is called to provide a hint to CompositeEMAC about whether it needs to call
         * dequeuePacket() after an Rx ISR is received.
         *
         * @return True if the MAC might have a descriptor to receive. False if there is definitely no complete packet yet.
         */
        virtual bool rxHasPackets_ISR() = 0;

        /**
         * @brief Dequeue a packet, if one is ready to be received.
         *
         * This function should also dequeue and dispose of any error or incomplete DMA descriptors.
         * After rxHasPackets_ISR() returns true, the MAC thread will call this it over and over again
         * until it returns nullptr.
         *
         * @return Packet pointer, or nullptr if there were no packets.
         */
        virtual net_stack_mem_buf_t * dequeuePacket() = 0;

        /**
         * @brief Rebuild DMA descriptors, if there are descriptors that need building and there is free pool memory.
         *
         * This function is called by the MAC thread after a packet has been dequeued, and also when memory in the Rx
         * pool becomes free.
         */
        virtual void rebuildDescriptors() = 0;
    };

protected:

    /// Pointer to memory manager for the EMAC
    EMACMemoryManager * memory_manager = nullptr;

    // Callbacks to the MAC
    emac_link_state_change_cb_t linkStateCallback{};
    emac_link_input_cb_t linkInputCallback{};

    // Instances of each of the 4 component classes
    PHYDriver * phy = nullptr;
    RxDMA & rxDMA;
    TxDMA & txDMA;
    MACDriver & mac;

    // Event queue handle for phy task
    int phyTaskHandle;

    // Thread running inside the MAC. Processes interrupts (both Tx and Rx) and receives packets.
    rtos::Thread * macThread = nullptr;

    // State of the MAC
    enum class PowerState {
        OFF = 0,
        ON_NO_LINK,
        ON_LINK_UP
    };

    std::atomic<PowerState> state = PowerState::OFF;

    // State of the link
    enum class LinkState {
        DOWN,
        UP
    };

    std::atomic<LinkState> linkState = LinkState::DOWN;

    // Multicast subscribe information
    MACAddress mcastMacs[MBED_CONF_NSAPI_EMAC_MAX_MCAST_SUBSCRIBES];

    // Note: if this variable becomes >= MBED_CONF_NSAPI_EMAC_MAX_MCAST_SUBSCRIBES, we are in multicast
    // fallback mode and are accepting all multicasts. The only way to get out of this mode is to power down
    // and power up the MAC.
    size_t numSubscribedMcastMacs;

    // Mutex that must be acquired before interacting with the MAC. This is used to protect against, for example,
    // one thread calling power_down() while the phy task is still running.
    rtos::Mutex macOpsMutex;

    /// Subclass should call this when a receive interrupt is detected
    void rxISR();

    /// Subclass should call this when a transmit complete interrupt is detected
    void txISR();

    /// Called periodically to poll the phy and bring link up/down
    void phyTask();

    /// Run in its own thread to service the MAC.
    void macTask();

    /// Callback from memory manager when Rx pool space frees up
    void onRxPoolSpaceAvail();

    /// Constructor. Should be called by subclass.
    CompositeEMAC(TxDMA & txDMA, RxDMA & rxDMA, MACDriver & macDriver):
    rxDMA(rxDMA),
    txDMA(txDMA),
    mac(macDriver)
    {}

public:
    uint32_t get_mtu_size() const override {
        // Regular Ethernet has an MTU of 1500.
        // Some MACs support Jumbo Frames of up to 9000 bytes; this might be worth adding at some point
        return 1500;
    }

    uint32_t get_align_preference() const override {
        // Most DMAs require or work best with word-aligned buffers.
        // NOTE: As of Feb 2025, nothing in Mbed actually uses this value.
        return 4;
    }

    void get_ifname(char *name, uint8_t size) const override;

    uint8_t get_hwaddr_size() const override {
        return MAC_ADDR_SIZE;
    }

    bool get_hwaddr(uint8_t *addr) const override {
        // Return false to tell upper layer code to use mbed_mac_address() to get the MAC address.
        // TODO once we support more than 1 ethernet port per device, this will need to be updated
        // to make sure each interface has a different MAC address.
        return false;
    }

    void set_hwaddr(const uint8_t *addr) override;

    bool link_out(emac_mem_buf_t *buf) override;

    bool power_up() override;

    void power_down() override;

    void set_link_input_cb(emac_link_input_cb_t input_cb) override;

    void set_link_state_cb(emac_link_state_change_cb_t state_cb) override;

    void add_multicast_group(const uint8_t *address) override;

    void remove_multicast_group(const uint8_t *address) override;

    void set_all_multicast(bool all) override;

    void set_memory_manager(EMACMemoryManager &mem_mngr) override {
        MBED_ASSERT(state == PowerState::OFF);
        memory_manager = &mem_mngr;
    }
};

}
#endif //MBED_OS_COMPOSITEETHMAC_H
