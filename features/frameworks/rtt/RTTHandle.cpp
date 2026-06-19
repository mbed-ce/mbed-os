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

#include "RTTHandle.h"

#include <SEGGER_RTT.h>
#include <ThisThread.h>

namespace mbed {

static mstd::atomic<bool> rtt_initialized = false;
void initialize_rtt() {
    // Initialize RTT, using a critical section to ensure thread safety
    core_util_critical_section_enter();
    bool expected_value = false;
    if(rtt_initialized.compare_exchange_strong(expected_value, true)) {
        SEGGER_RTT_Init();
    }
    core_util_critical_section_exit();
}

// Class used to call initialize_rtt() via global constructor.
// This is needed so that the debug
class RTTInitializer {
public:
    RTTInitializer() {
        initialize_rtt();
    }
};
static RTTInitializer rtt_initializer [[maybe_unused]];

RTTHandle::RTTHandle():
buffer_index(0)
{
    initialize_rtt();

    // Ensure up buffer is in nonblocking-with-trim mode
    SEGGER_RTT_SetFlagsUpBuffer(buffer_index, SEGGER_RTT_MODE_NO_BLOCK_TRIM);
}

RTTHandle::RTTHandle(unsigned int buffer_index, char *up_buffer, size_t up_buffer_size, char *down_buffer,
    size_t down_buffer_size, char const *name):
buffer_index(buffer_index)
{
    MBED_ASSERT(buffer_index != 0);
    MBED_ASSERT(buffer_index < MBED_CONF_RTT_MAX_BUFFERS);

    initialize_rtt();

    // Configure up and down buffers
    SEGGER_RTT_ConfigUpBuffer(buffer_index, name, up_buffer, up_buffer_size, SEGGER_RTT_MODE_NO_BLOCK_TRIM);
    SEGGER_RTT_ConfigDownBuffer(buffer_index, name, down_buffer, down_buffer_size, 0); // flags appear currently unused
}

ssize_t RTTHandle::read(void *buffer, size_t size) {
    if(!SEGGER_RTT_HasData(buffer_index)) {
        if(blocking_mode) {
            // Busy wait until data is available. Unfortunately RTT does not have a way to do this any better.
            while(!SEGGER_RTT_HasData(buffer_index)) {}
        }
        else {
            return -EAGAIN;
        }
    }

    return static_cast<ssize_t>(SEGGER_RTT_Read(buffer_index, buffer, size));
}

ssize_t RTTHandle::write(const void *buffer, size_t size) {
    size_t bytes_written = 0;
    do {
        // Try to write as much data as we can. We set the mode to NO_BLOCK_TRIM,
        // so only as much data as fits will be written
        bytes_written += static_cast<ssize_t>(SEGGER_RTT_Write(buffer_index, reinterpret_cast<uint8_t const *>(buffer) + bytes_written, size));
    }
    while(blocking_mode && bytes_written < size);
    return bytes_written;
}
}
