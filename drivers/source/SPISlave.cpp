/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "drivers/SPISlave.h"
#include "mbed_assert.h"

#if DEVICE_SPISLAVE

namespace mbed {

SPISlave::SPISlave(PinName mosi, PinName miso, PinName sclk, PinName ssel) :
    _spi(),
    _bits(8),
    _mode(0)
{
    spi_init(&_spi, mosi, miso, sclk, ssel);
    spi_format(&_spi, _bits, _mode, 1);

    // For legacy compatibility, tell the HAL to set the frequency to 1MHz.
    // This is done even though it does not make sense to set the frequency of a slave device;
    // it has to run at whatever SCLK frequency the master supplies.
    spi_frequency(&_spi, 1000000);
}

SPISlave::SPISlave(const spi_pinmap_t &pinmap) :
    _spi(),
    _bits(8),
    _mode(0)
{
    spi_init_direct(&_spi, &pinmap);
    spi_format(&_spi, _bits, _mode, 1);

    // For legacy compatibility, tell the HAL to set the frequency to 1MHz.
    // This is done even though it does not make sense to set the frequency of a slave device;
    // it has to run at whatever SCLK frequency the master supplies.
    spi_frequency(&_spi, 1000000);
}

SPISlave::~SPISlave()
{
    spi_free(&_spi);
}

void SPISlave::format(int bits, int mode)
{
    _bits = bits;
    _mode = mode;
    spi_format(&_spi, _bits, _mode, 1);
}

int SPISlave::receive(void)
{
    return (spi_slave_receive(&_spi));
}

int SPISlave::read(void)
{
    return (spi_slave_read(&_spi));
}

void SPISlave::reply(int value)
{
    spi_slave_write(&_spi, value);
}

} // namespace mbed

#endif
