/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
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

#ifndef EMAC_CTP_H
#define EMAC_CTP_H

// CTP types --------------------------------------------------------------------

enum class ctp_function : uint16_t {

    INVALID = 0,
    FORWARD = 0x02,
    REPLY = 0x01
};

const uint16_t CTP_ETHERTYPE = 0x9000;

// CTP structures ---------------------------------------------------------------

/**
 * @brief Structure for a CTP reply message.
 */
struct __attribute__((packed)) CTPReplyCommand {
    ctp_function function; ///< Function. Should be set to REPLY
    uint16_t receiptNumber; ///< Receipt number.  Set arbitrarily by the originating station
    uint8_t payload[0]; ///< Payload.  Arbitrary, set by the originating station.
};

/**
 * @brief Structure for a CTP forward message.
 */
struct CTPForwardCommand;
struct __attribute__((packed)) CTPForwardCommand {
    ctp_function function; ///< Function. Should be set to FORWARD.
    uint8_t forwardMAC[6]; ///< MAC to forward this message to.

    /// Contained command, as a generic pointer
    uint8_t nextCommand[0];
};

/**
 * @brief Packed structure representing an Ethernet frame with a CTP packet
 */
struct __attribute__((packed)) EthernetCTPFrame {

    // Ethernet II header
    uint8_t destMAC[6];
    uint8_t srcMAC[6];
    uint16_t etherType;

    /// CTP skip count.
    /// Indicates the offset where the receiving station should find its command in this packet.
    /// This will be 0 when a packet is sent from the originating station.  When a station processes
    /// a CTP REPLY command, it updates this field in the reply packet to point to the next command.
    uint16_t skipCount;

    /// Contained command, as a generic pointer
    uint8_t nextCommand[0];
};

// CTP functions ---------------------------------------------------------------

// Test memory manager options
#define CTP_OPT_NON_ALIGNED    0x02   // Force memory buffers to be non-aligned

/* Builds and sends CTP message. Forward to address is the address where echo server sends the reply.
   Default is own Ethernet MAC address. Options can be used to specify test memory manager options.
 */
#define CTP_MSG_SEND(length, send_to_address, own_address, forward_to_address, mem_mngr_options) \
    emac_if_ctp_msg_build(length, send_to_address, own_address, forward_to_address, mem_mngr_options)

/**
 * @brief Handle an incoming Ethernet frame.
 *
 * If the frame is a CTP forward command, a response frame is written to \c eth_output_frame and
 * FORWARD is returned.
 *
 * If the frame is a CTP reply (meaning, this packet reached its final destination at this station),
 * REPLY is returned and the receipt number is saved to \c receipt_number .
 *
 * Otherwise (invalid or non CTP packet), INVALID is returned.
 */
ctp_function emac_if_ctp_header_handle(unsigned char const *eth_input_frame, unsigned char *eth_output_frame, unsigned char const *origin_addr, int *receipt_number);
void emac_if_ctp_msg_build(int eth_frame_len, const unsigned char *dest_addr, const unsigned char *origin_addr, const unsigned char *forward_addr, int options);
void emac_if_ctp_reply_handle(int lenght, int invalid_data_index);

#endif /* EMAC_CTP_H */
