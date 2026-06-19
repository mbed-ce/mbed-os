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

#pragma once

#include <mstd_atomic>
#include <FileHandle.h>

#include "SEGGER_RTT_Conf.h"

/**
 * \file
 * FileHandle implementation for RTT
 */

namespace mbed
{

/**
 * @brief Initialize the RTT library.
 *
 * This must be called before any code using SEGGER RTT is run. As the RTT library does not
 * handle double-initialization, Mbed contains a guard to make sure it will not be reinitialized
 * if this function is called again.
 *
 * This function will be called at boot via global constructor, and whenever an RTTHandle object is constructed
 * (to handle the case where an RTTHandle is used inside another global constructor).
 */
void initialize_rtt();

/**
 * @brief Class which wraps the RTT library as an Mbed FileHandle.
 */
class RTTHandle : public FileHandle {
    // Buffer index within RTT that this handle points to
    const unsigned int buffer_index;

    // Are wr in blocking mode?
    bool blocking_mode = true;

public:

    /**
     * @brief Instantiate an RTT instance pointing to RTT buffer zero (used for stdout/stdin)
     *
     * @note Buffer zero is special and always uses buffers allocated at compile time. The size of these buffers
     *     is controlled by the \c rtt.ch0-up-buffer-size and \c rtt.ch0-down-buffer-size settings.
     */
    RTTHandle();

    /**
     * @brief Instantiate an RTT instance pointing to the desired RTT buffer.
     *
     * @param buffer_index Index of buffer to use. This may not be 0 (use the no-arg constructor) and must be less than
     *     the value of the \c rtt.num-buffers setting.
     * @param up_buffer Pointer to memory to use for the up buffer (data printed from the MCU). May be nullptr
     *    if this channel is only going to be used for down (input) data.
     * @param up_buffer_size Size of the up buffer in bytes.
     * @param down_buffer Pointer to memory to use for the down buffer (data sent to the MCU). May be nullptr if
     *    this channel is only going to be used for up (output) data.
     * @param down_buffer_size Size of the down buffer in bytes.
     * @param name Optional name to give this channel.
     *
     * @warning On targets with a data cache, the up and down buffer memory needs to be declared in the section
     *    defined by \c SEGGER_RTT_SECTION, or otherwise placed into non-cachable memory. The same is true for
     *    \c name if it is not a string constant in flash.
     */
    RTTHandle(unsigned int buffer_index, char * up_buffer, size_t up_buffer_size, char * down_buffer, size_t down_buffer_size, char const * name = nullptr);

    ssize_t read(void *buffer, size_t size) override;

    ssize_t write(const void *buffer, size_t size) override;

    virtual off_t seek(off_t offset, int whence = SEEK_SET)
    {
        return -ESPIPE;
    }
    virtual off_t size()
    {
        return -EINVAL;
    }

    int close() override {
        // No-op: RTT streams cannot be "closed", we just stop using them
        return 0;
    }

    int isatty() override {
        return true;
    }

    int set_blocking(bool blocking) override {
        blocking_mode = blocking;
        return 0;
    }

    bool is_blocking() const override {
        return blocking_mode;
    }
};

}
