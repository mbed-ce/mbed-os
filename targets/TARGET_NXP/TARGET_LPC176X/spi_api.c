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
#include "mbed_assert.h"
#include <math.h>

#include "spi_api.h"
#include "cmsis.h"
#include "PeripheralPinMaps.h"
#include "mbed_error.h"

static inline int ssp_disable(spi_t *obj);
static inline int ssp_enable(spi_t *obj);

void spi_init_direct(spi_t *obj, const spi_pinmap_t *pinmap) {
    obj->spi = (LPC_SSP_TypeDef*)pinmap->peripheral;
    
    // enable power and clocking
    switch ((int)obj->spi) {
        case SPI_0: LPC_SC->PCONP |= 1 << 21; break;
        case SPI_1: LPC_SC->PCONP |= 1 << 10; break;
    }

    // pin out the spi pins
    pin_function(pinmap->mosi_pin, pinmap->mosi_function);
    pin_mode(pinmap->mosi_pin, PullNone);
    pin_function(pinmap->miso_pin, pinmap->miso_function);
    pin_mode(pinmap->miso_pin, PullNone);
    pin_function(pinmap->sclk_pin, pinmap->sclk_function);
    pin_mode(pinmap->sclk_pin, PullNone);

    if (pinmap->ssel_pin != NC) {
        pin_function(pinmap->ssel_pin, pinmap->ssel_function);
        pin_mode(pinmap->ssel_pin, PullNone);
    }
}

SPIName spi_get_peripheral_name(PinName mosi, PinName miso, PinName sclk)
{
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_periph = (SPIName)pinmap_merge(spi_sclk, spi_data);
    return spi_periph;
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel) {
    spi_pinmap_t pinmap;
    pinmap.mosi_pin = mosi;
    pinmap.miso_pin = miso;
    pinmap.sclk_pin = sclk;
    pinmap.ssel_pin = ssel;

    // determine the SPI to use
    SPIName spi_mosi_miso_sclk_periph = spi_get_peripheral_name(mosi, miso, sclk);
    SPIName spi_ssel_periph = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    pinmap.peripheral = pinmap_merge(spi_mosi_miso_sclk_periph, spi_ssel_periph);
    MBED_ASSERT(pinmap.peripheral != NC);

    // Get pin functions
    pinmap.mosi_function = pinmap_find_function(mosi, PinMap_SPI_MOSI);
    pinmap.miso_function = pinmap_find_function(miso, PinMap_SPI_MISO);
    pinmap.sclk_function = pinmap_find_function(sclk, PinMap_SPI_SCLK);
    pinmap.ssel_function = pinmap_find_function(ssel, PinMap_SPI_SSEL);

    spi_init_direct(obj, &pinmap);
}

void spi_free(spi_t *obj) {}

void spi_format(spi_t *obj, int bits, int mode, int slave) {
    ssp_disable(obj);
    MBED_ASSERT(((bits >= 4) && (bits <= 16)) && (mode >= 0 && mode <= 3));
    
    obj->bits_per_word = bits;
    int polarity = (mode & 0x2) ? 1 : 0;
    int phase = (mode & 0x1) ? 1 : 0;
    
    // set it up
    int DSS = bits - 1;            // DSS (data select size)
    int SPO = (polarity) ? 1 : 0;  // SPO - clock out polarity
    int SPH = (phase) ? 1 : 0;     // SPH - clock out phase
    
    int FRF = 0;                   // FRF (frame format) = SPI
    uint32_t tmp = obj->spi->CR0;
    tmp &= ~(0x00FF);              // Clear DSS, FRF, CPOL and CPHA [7:0]
    tmp |= DSS << 0
        | FRF << 4
        | SPO << 6
        | SPH << 7;
    obj->spi->CR0 = tmp;
    
    tmp = obj->spi->CR1;
    tmp &= ~(0xD);
    tmp |= 0 << 0                   // LBM - loop back mode - off
        | ((slave) ? 1 : 0) << 2   // MS - master slave mode, 1 = slave
        | 0 << 3;                  // SOD - slave output disable - na
    obj->spi->CR1 = tmp;
    
    ssp_enable(obj);
}

void spi_frequency(spi_t *obj, int hz) {
    ssp_disable(obj);
    
    // setup the spi clock diveder to /1
    switch ((int)obj->spi) {
        case SPI_0:
            LPC_SC->PCLKSEL1 &= ~(3 << 10);
            LPC_SC->PCLKSEL1 |=  (1 << 10);
            break;
        case SPI_1:
            LPC_SC->PCLKSEL0 &= ~(3 << 20);
            LPC_SC->PCLKSEL0 |=  (1 << 20);
            break;
    }
    
    uint32_t PCLK = SystemCoreClock;
    
    int prescaler;
    
    for (prescaler = 2; prescaler <= 254; prescaler += 2) {
        int prescale_hz = PCLK / prescaler;
        
        // calculate the divider
        int divider = floor(((float)prescale_hz / (float)hz) + 0.5f);
        
        // check we can support the divider
        if (divider < 256) {
            // prescaler
            obj->spi->CPSR = prescaler;
            
            // divider
            obj->spi->CR0 &= ~(0xFF00);  // Clear SCR: Serial clock rate [15:8]
            obj->spi->CR0 |= (divider - 1) << 8;
            ssp_enable(obj);
            return;
        }
    }
    error("Couldn't setup requested SPI frequency");
}

static inline int ssp_disable(spi_t *obj) {
    return obj->spi->CR1 &= ~(1 << 1);
}

static inline int ssp_enable(spi_t *obj) {
    return obj->spi->CR1 |= (1 << 1);
}

static inline int ssp_readable(spi_t *obj) {
    return obj->spi->SR & (1 << 2);
}

static inline int ssp_writeable(spi_t *obj) {
    return obj->spi->SR & (1 << 1);
}

static inline void ssp_write(spi_t *obj, int value) {
    while (!ssp_writeable(obj));
    obj->spi->DR = value;
}

static inline int ssp_read(spi_t *obj) {
    while (!ssp_readable(obj));
    return obj->spi->DR;
}

static inline int ssp_busy(spi_t *obj) {
    return (obj->spi->SR & (1 << 4)) ? (1) : (0);
}

int spi_master_write(spi_t *obj, int value) {
    ssp_write(obj, value);
    return ssp_read(obj);
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill) {
    int total = (tx_length > rx_length) ? tx_length : rx_length;

    if(obj->bits_per_word > 8) {
        // 2 bytes per write/read operation
        MBED_ASSERT(tx_length % 2 == 0);
        MBED_ASSERT(rx_length % 2 == 0);

        // Extend write fill value to 16 bits
        const uint16_t write_fill_u16 = (((uint16_t)write_fill) << 8) | write_fill;

        // Access input and output arrays as 16-bit words.
        // This might do unaligned access, but that's OK for integers on Cortex-M3
        uint16_t const * const tx_buffer_u16 = (uint16_t const *)tx_buffer;
        uint16_t * const rx_buffer_u16 = (uint16_t *)rx_buffer;

        const int tx_length_u16 = tx_length / 2;
        const int rx_length_u16 = rx_length / 2;

        for (int i = 0; i < total / 2; i++) {
            uint16_t out = (i < tx_length_u16) ? tx_buffer_u16[i] : write_fill_u16;
            uint16_t in = spi_master_write(obj, out);
            if (i < rx_length_u16) {
                rx_buffer_u16[i] = in;
                printf("rx_buffer_u16[%d] <= 0x%hx\n", i, in);
            }
        }
    }
    else {
        // 1 byte per read/write operation
        for (int i = 0; i < total; i++) {
            uint16_t out = (i < tx_length) ? tx_buffer[i] : write_fill;
            uint16_t in = spi_master_write(obj, out);
            if (i < rx_length) {
                rx_buffer[i] = in;
            }
        }
    }

    return total;
}

int spi_slave_receive(spi_t *obj) {
    return (ssp_readable(obj) && !ssp_busy(obj)) ? (1) : (0);
}

int spi_slave_read(spi_t *obj) {
    return obj->spi->DR;
}

void spi_slave_write(spi_t *obj, int value) {
    while (ssp_writeable(obj) == 0) ;
    obj->spi->DR = value;
}

int spi_busy(spi_t *obj) {
    return ssp_busy(obj);
}

const PinMap *spi_master_mosi_pinmap()
{
    return PinMap_SPI_MOSI;
}

const PinMap *spi_master_miso_pinmap()
{
    return PinMap_SPI_MISO;
}

const PinMap *spi_master_clk_pinmap()
{
    return PinMap_SPI_SCLK;
}

const PinMap *spi_master_cs_pinmap()
{
    return PinMap_SPI_SSEL;
}

const PinMap *spi_slave_mosi_pinmap()
{
    return PinMap_SPI_MOSI;
}

const PinMap *spi_slave_miso_pinmap()
{
    return PinMap_SPI_MISO;
}

const PinMap *spi_slave_clk_pinmap()
{
    return PinMap_SPI_SCLK;
}

const PinMap *spi_slave_cs_pinmap()
{
    return PinMap_SPI_SSEL;
}
