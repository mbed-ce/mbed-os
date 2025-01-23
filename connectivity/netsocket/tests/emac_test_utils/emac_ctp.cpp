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
#include "mbed.h"

#include "EMAC.h"
#include "EMACMemoryManager.h"
#include "EmacTestMemoryManager.h"
#include "EmacTestNetworkStack.h"

#include "emac_ctp.h"

#include "emac_util.h"
#include "emac_membuf.h"

// Unique identifier for message
static int receipt_number = 0;

static int emac_if_ctp_header_build(unsigned char *eth_frame, const unsigned char *dest_addr, const unsigned char *origin_addr, const unsigned char *forward_addr)
{
    auto *ctpFrame = reinterpret_cast<EthernetCTPFrame *>(eth_frame);

    memcpy(ctpFrame->destMAC, dest_addr, 6);
    memcpy(ctpFrame->srcMAC, origin_addr, 6);
    ctpFrame->etherType = __builtin_bswap16(CTP_ETHERTYPE);
    ctpFrame->skipCount = 0;

    auto *forwardCommand = reinterpret_cast<CTPForwardCommand *>(ctpFrame->nextCommand);

    forwardCommand->function = ctp_function::FORWARD;
    memcpy(forwardCommand->forwardMAC, forward_addr, 6);

    auto *replyCommand = reinterpret_cast<CTPReplyCommand *>(forwardCommand->nextCommand);
    replyCommand->function = ctp_function::REPLY;

    receipt_number++;
    replyCommand->receiptNumber = receipt_number;

    return receipt_number;
}

ctp_function emac_if_ctp_header_handle(unsigned char const *eth_input_frame, unsigned char *eth_output_frame, unsigned char const *origin_addr, int *receipt_number)
{
    auto *ctpFrame = reinterpret_cast<EthernetCTPFrame const *>(eth_input_frame);

    if (__builtin_bswap16(ctpFrame->etherType) != CTP_ETHERTYPE) {
        return ctp_function::INVALID;
    }

    unsigned char const *command_ptr = ctpFrame->nextCommand + ctpFrame->skipCount;
    ctp_function command_function = *reinterpret_cast<ctp_function const *>(command_ptr);

    // Forward
    if (command_function == ctp_function::FORWARD) {
        auto const *forwardCommandPtr = reinterpret_cast<const CTPForwardCommand *>(command_ptr);

        memcpy(eth_output_frame, eth_input_frame, ETH_FRAME_HEADER_LEN);
        auto *ctpFrameOut = reinterpret_cast<EthernetCTPFrame *>(eth_output_frame);

        // Update skip count so that the receiver will process the command after this one
        ctpFrameOut->skipCount = ctpFrame->skipCount + sizeof(CTPForwardCommand);

        // Set forward address to destination address
        memcpy(ctpFrameOut->destMAC, forwardCommandPtr->forwardMAC, 6);
        // Copy own address to origin
        memcpy(ctpFrameOut->srcMAC, origin_addr, 6);

        return ctp_function::FORWARD;
    }
    // Reply
    else if (command_function == ctp_function::REPLY) {
        auto const *replyCommandPtr = reinterpret_cast<const CTPReplyCommand *>(command_ptr);
        *receipt_number = replyCommandPtr->receiptNumber;
        return ctp_function::REPLY;
    }

    return ctp_function::INVALID;
}

void emac_if_ctp_msg_build(int eth_frame_len, const unsigned char *dest_addr, const unsigned char *origin_addr, const unsigned char *forward_addr, int options)
{
    if (eth_frame_len < ETH_FRAME_HEADER_LEN) {
        eth_frame_len = ETH_FRAME_HEADER_LEN;
    }

    if (emac_if_get_trace_level() & TRACE_SEND) {
        printf("message sent to %x:%x:%x:%x:%x:%x, fwd to %x:%x:%x:%x:%x:%x \r\n\r\n",
               dest_addr[0], dest_addr[1], dest_addr[2], dest_addr[3], dest_addr[4], dest_addr[5],
               forward_addr[0], forward_addr[1], forward_addr[2], forward_addr[3], forward_addr[4], forward_addr[5]);
    }

    int outgoing_msg_index = emac_if_add_outgoing_msg(eth_frame_len);

    if (outgoing_msg_index < 0) {
        SET_ERROR_FLAGS(OUT_OF_MSG_DATA);
        return;
    }

    int alloc_opt = 0;
    int align = 0;
    if (options & CTP_OPT_NON_ALIGNED) {
        alloc_opt |= MEM_NO_ALIGN; // Force align to odd address
        align = 1;                 // Reserve memory overhead to align to odd address
    }

    emac_mem_buf_t *buf = EmacTestMemoryManager::get_instance().alloc_heap(eth_frame_len, align, alloc_opt);

    if (!buf) {
        SET_ERROR_FLAGS(NO_FREE_MEM_BUF);
        emac_if_free_outgoing_msg(outgoing_msg_index);
        return;
    }

    if (memcmp(dest_addr, eth_mac_broadcast_addr, 6) == 0) {
        emac_if_set_outgoing_msg_flags(outgoing_msg_index, BROADCAST);
    }

    unsigned char eth_output_frame_data[ETH_FRAME_HEADER_LEN];
    int receipt_number = emac_if_ctp_header_build(eth_output_frame_data, dest_addr, origin_addr, forward_addr);
    emac_if_set_outgoing_msg_receipt_num(outgoing_msg_index, receipt_number);

    emac_if_memory_buffer_write(buf, eth_output_frame_data, true);

    emac_if_check_memory(true);
    EmacTestNetworkStack::get_instance().get_emac()->link_out(buf);
    emac_if_check_memory(false);
}
