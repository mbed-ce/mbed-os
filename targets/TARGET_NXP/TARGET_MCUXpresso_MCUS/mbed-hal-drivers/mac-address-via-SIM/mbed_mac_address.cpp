/* mbed Microcontroller Library
 * Copyright (c) 2026 Jamie Smith
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

#include "fsl_sim.h"

#include "MbedCRC.h"

void mbed_mac_address(char *mac) {
    // Generate a random MAC address using the chip's 4 word unique ID.
    // This is not suitable for production use, but will ensure there are
    // no MAC conflicts.
    sim_uid_t uid;
    SIM_GetUniqueId(&uid);

    // The flash unique ID is 128 bits, but we need a unique ID no more
    // than 48 bits long. So, use a CRC.
    mbed::MbedCRC<POLY_32BIT_ANSI, 32, mbed::CrcMode::BITWISE> crcCalc;
    uint32_t crc;
    crcCalc.compute(&uid, sizeof(uid), &crc);

    // Build the MAC address
    mac[0] = 2; // Locally administered, unicast
    mac[1] = 0;
    mac[2] = crc >> 24;
    mac[3] = (crc >> 16) & 0xFF;
    mac[4] = (crc >> 8) & 0xFF;
    mac[5] = crc & 0xFF;
}