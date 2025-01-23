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

#ifndef EMAC_MEMBUF_H
#define EMAC_MEMBUF_H

/**
 * Read data from an Ethernet CTP frame received from the MAC.
 * The first ETH_FRAME_HEADER_LEN bytes are copied into \c eth_frame.
 * Bytes after there must be a fixed test pattern.
 *
 * @return Index (with 0 being the first byte of the Ethernet payload) of the first
 *    byte that did not match the test pattern, or 0 on success.
 */
int emac_if_memory_buffer_read_and_check(void *buf, unsigned char *eth_frame);

void emac_if_memory_buffer_write(void *buf, unsigned char *eth_frame, bool write_data);

#endif /* EMAC_MEMBUF_H */
