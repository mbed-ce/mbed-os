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
        INVALID_USAGE = 6
    };

    enum class LinkSpeed
    {
        LINK_10MBIT,
        LINK_100MBIT,
        LINK_1GBIT
    };

    enum class Duplex
    {
        HALF,
        FULL
    };

    typedef std::array<uint8_t, 6> MACAddress;

    /**
     * @brief Abstract interface for a driver for the low level ethernet MAC hardware.
     *
     * Thread safety: CompositeEMAC will guarantee only one thread is utilizing this class at a time.
     */
    class MACDriver
    {
        /**
         * @brief Initialize the MAC, map pins, and prepare it to send and receive packets.
         *    It should not be enabled yet.
         *
         * @param ownAddress MAC address that this device should use
         *
         * @return Error code or SUCCESS
         */
        virtual ErrCode init(MACAddress const & ownAddress) = 0;

        /**
         * @brief Deinit the MAC so that it's not using any clock/power. Should prepare for init() to be called
         *    again.
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
         * @param pass True to pass all mcasts, false otherwise
         *
         * @return Error code or success
         */
        virtual ErrCode setPassAllMcast(bool pass);

        /**
         * @brief Set promiscuous mode (where the Eth MAC passes all traffic up to the application, regardless
         *   of its destination address).
         *
         * @param enable True to pass all traffic, false otherwise
         *
         * @return Error code or success
         */
        virtual ErrCode setPromiscuous(bool enable);
    };

    /**
     * @brief Interface for a driver for the Ethernet PHY.
     *
     * Thread safety: CompositeEMAC will guarantee only one thread is utilizing this class at a time.
     */
    class PhyDriver
    {
    protected:
        /// MAC driver. Shall be set in init().
        MACDriver * mac = nullptr;

    public:
        /// Set the MAC driver of this PHY. Will be called by CompositeEMAC before init().
        void setMAC(MACDriver * mac) { this->mac = mac; }

        /**
         * @brief Initialize the PHY and set it up for Ethernet operation.
         *
         * @return Error code or success
         */
        ErrCode init();

        /**
         * @brief Check whether the link is up or down
         *
         * @param[out] status Set to true or false depending on whether the link is up or down
         *
         * @return Error code or success
         */
        ErrCode checkLinkStatus(bool & status);

        /**
         * @brief Get the negotiated (or preset) Ethernet speed and duplex, given that the link is up
         *
         * @param[out] speed Link speed
         * @param[out] duplex Link duplex
         *
         * @return Error code or success
         */
        ErrCode checkLinkType(LinkSpeed & speed, Duplex & duplex);
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
        /// Set the mem manager of this DMA ring. Will be called by CompositeEMAC before init().
        void setMemoryManager(EMACMemoryManager * memory_manager) { this->memory_manager = memory_manager; }

        /// Initialize this Tx DMA ring.
        ErrCode init();

        /// Stop the DMA running. init() should be able to be called again after this function completes to restart DMA.
        ErrCode deinit();

        /// Called by CompositeEMAC when the MAC generates a transmit complete interrupt
        void txISR();

        /// Transmit a packet out of the Tx DMA ring.  Note that this function
        /// *takes ownership of* the passed packet and must free it either now or after
        /// it's been transmitted.
        /// Should block until there is space to transmit the packet.
        HAL_StatusTypeDef txPacket(net_stack_mem_buf_t * buf);
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
        /// Set the mem manager of this DMA ring. Will be called by CompositeEMAC before init().
        void setMemoryManager(EMACMemoryManager * memory_manager) { this->memory_manager = memory_manager; }

        /// Initialize this Rx DMA ring.
        ErrCode init();

        /// Stop the DMA running. init() should be able to be called again after this function completes to restart DMA.
        ErrCode deinit();

        /**
         * @brief Check if the MAC may have a packet to receive. Called from the Rx ISR.
         *
         * This function is called to provide a hint to CompositeEMAC about whether it needs to call
         * dequeuePacket() after an Rx ISR is received.
         *
         * @return True if the MAC might have a descriptor to receive. False if there is definitely no complete packet yet.
         */
        bool rxHasPackets_ISR();

        /**
         * @brief Dequeue a packet, if one is ready to be received.
         *
         * This function should also dequeue and dispose of any error or incomplete DMA descriptors.
         * The MAC thread calls it over and over again until it returns nullptr.
         *
         * @return Packet pointer, or nullptr if there were no packets.
         */
        net_stack_mem_buf_t * dequeuePacket();

        /**
         * @brief Rebuild DMA descriptors, if there are descriptors that need building and there is free pool memory.
         *
         * This function is called by the MAC thread after a packet has been dequeued and afte r
         */
        void rebuildDescriptors();
    };

protected:

    /// Subclass should call this when a receive interrupt is detected
    void rxISR();

    /// Subclass should call this when a transmit complete interrupt is detected
    void txISR();
};

}
#endif //MBED_OS_COMPOSITEETHMAC_H
