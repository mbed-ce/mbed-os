/*
 * Copyright (c) 2015-2016, Nuvoton Technology Corporation
 *
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

#include "spi_api.h"

#if DEVICE_SPI

#include <string.h>

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "gpio_api.h"
#include "nu_modutil.h"
#include "nu_miscutil.h"
#include "nu_bitutil.h"

#if DEVICE_SPI_ASYNCH
#include "dma_api.h"
#include "dma.h"
#endif

#define NU_SPI_FRAME_MIN    8
#define NU_SPI_FRAME_MAX    32

struct nu_spi_var {
#if DEVICE_SPI_ASYNCH
    uint8_t     pdma_perp_tx;
    uint8_t     pdma_perp_rx;
#endif
};

static struct nu_spi_var spi0_var = {
#if DEVICE_SPI_ASYNCH
    .pdma_perp_tx       =   PDMA_SPI0_TX,
    .pdma_perp_rx       =   PDMA_SPI0_RX
#endif
};
static struct nu_spi_var spi1_var = {
#if DEVICE_SPI_ASYNCH
    .pdma_perp_tx       =   PDMA_SPI1_TX,
    .pdma_perp_rx       =   PDMA_SPI1_RX
#endif
};
static struct nu_spi_var spi2_var = {
#if DEVICE_SPI_ASYNCH
    .pdma_perp_tx       =   PDMA_SPI2_TX,
    .pdma_perp_rx       =   PDMA_SPI2_RX
#endif
};
static struct nu_spi_var spi3_var = {
#if DEVICE_SPI_ASYNCH
    .pdma_perp_tx       =   PDMA_SPI3_TX,
    .pdma_perp_rx       =   PDMA_SPI3_RX
#endif
};
/* Degrade QSPI0/1 to SPI_4/5 for standard SPI usage */
static struct nu_spi_var spi4_var = {
#if DEVICE_SPI_ASYNCH
    .pdma_perp_tx       =   PDMA_QSPI0_TX,
    .pdma_perp_rx       =   PDMA_QSPI0_RX
#endif
};
static struct nu_spi_var spi5_var = {
#if DEVICE_SPI_ASYNCH
    .pdma_perp_tx       =   PDMA_QSPI1_TX,
    .pdma_perp_rx       =   PDMA_QSPI1_RX
#endif
};

/* Change to QSPI version functions
 *
 * In most cases, we can control degraded QSPI H/W to standard through BSP SPI driver
 * directly as if it is just SPI H/W. However, BSP SPI driver distinguishes among
 * SPI H/W instances in below functions:
 *
 * SPI_Open
 * SPI_Close
 * SPI_SetBusClock
 * SPI_GetBusClock
 *
 * In these cases, we must change to QSPI version instead for QSPI H/W.
 */
static int spi_is_qspi(spi_t *obj);

/* Synchronous version of SPI_ENABLE()/SPI_DISABLE() macros
 *
 * The SPI peripheral clock is asynchronous with the system clock. In order to make sure the SPI
 * control logic is enabled/disabled, this bit indicates the real status of SPI controller.
 *
 * NOTE: All configurations shall be ready before calling SPI_ENABLE_SYNC().
 * NOTE: Before changing the configurations of SPIx_CTL, SPIx_CLKDIV, SPIx_SSCTL and SPIx_FIFOCTL registers,
 *       user shall clear the SPIEN (SPIx_CTL[0]) and confirm the SPIENSTS (SPIx_STATUS[15]) is 0
 *       (by SPI_DISABLE_SYNC here).
 */
__STATIC_INLINE void SPI_ENABLE_SYNC(SPI_T *spi_base)
{
    if (! (spi_base->CTL & SPI_CTL_SPIEN_Msk)) {
        SPI_ENABLE(spi_base);
    }
    while (! (spi_base->STATUS & SPI_STATUS_SPIENSTS_Msk));
}
__STATIC_INLINE void SPI_DISABLE_SYNC(SPI_T *spi_base)
{
    if (spi_base->CTL & SPI_CTL_SPIEN_Msk) {
        // NOTE: SPI H/W may get out of state without the busy check.
        while (SPI_IS_BUSY(spi_base));
    
        SPI_DISABLE(spi_base);
    }
    while (spi_base->STATUS & SPI_STATUS_SPIENSTS_Msk);
}

/// Get the number of bytes of the Tx/Rx buffers that will be used to encode each word of data for the bus
static uint8_t nu_spi_get_bytes_per_word(struct spi_s const * const nu_spi)
{
    if(nu_spi->word_size_bits <= 8)
    {
        return 1;
    }
    else if(nu_spi->word_size_bits <= 16)
    {
        return 2;
    }
    else
    {
        return 4;
    }
}

// Set the DMA usage of this SPI instance.
// Allocates or deallocates channels as necessary.
// If no DMA channels are available, sets DMA usage to DMA_USAGE_NEVER
static void nu_spi_set_dma_usage(struct spi_s * const spi, DMAUsage new_dma_usage)
{
    if(new_dma_usage == DMA_USAGE_NEVER)
    {
        if(spi->dma_usage != DMA_USAGE_NEVER)
        {
            // Free channels
            dma_channel_free(spi->dma_chn_id_tx);
            spi->dma_chn_id_tx = DMA_ERROR_OUT_OF_CHANNELS;
            dma_channel_free(spi->dma_chn_id_rx);
            spi->dma_chn_id_rx = DMA_ERROR_OUT_OF_CHANNELS;
        }
    }
    else
    {
        // Temporary or permanent DMA usage
        if(spi->dma_usage == DMA_USAGE_NEVER)
        {
            // Need to allocate channels
            spi->dma_chn_id_tx = dma_channel_allocate(DMA_CAP_NONE);
            if(spi->dma_chn_id_tx == DMA_ERROR_OUT_OF_CHANNELS)
            {
                new_dma_usage = DMA_USAGE_NEVER;
            }
            else
            {
                spi->dma_chn_id_rx = dma_channel_allocate(DMA_CAP_NONE);
                if(spi->dma_chn_id_rx == DMA_ERROR_OUT_OF_CHANNELS)
                {
                    new_dma_usage = DMA_USAGE_NEVER;
                    dma_channel_free(spi->dma_chn_id_tx);
                }
            }
        }
    }

    spi->dma_usage = new_dma_usage;
}

#if DEVICE_SPI_ASYNCH
static void spi_enable_vector_interrupt(spi_t *obj, uint32_t handler, uint8_t enable);
static void spi_master_enable_interrupt(spi_t *obj, uint8_t enable);
static uint32_t spi_master_write_asynch(spi_t *obj, uint32_t tx_limit);
static uint32_t spi_master_read_asynch(spi_t *obj);
static uint32_t spi_event_check(spi_t *obj);
static void spi_enable_event(spi_t *obj, uint32_t event, uint8_t enable);
static void spi_buffer_set(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length);
static void nu_spi_set_dma_usage(struct spi_s * const spi, DMAUsage new_dma_usage);
static int spi_is_tx_complete(spi_t *obj);
static int spi_is_rx_complete(spi_t *obj);
static int spi_writeable(spi_t * obj);
static int spi_readable(spi_t * obj);
static void spi_dma_handler_tx(uint32_t id, uint32_t event_dma);
static void spi_dma_handler_rx(uint32_t id, uint32_t event_dma);
static uint32_t spi_fifo_depth(spi_t *obj);
#endif

static uint32_t spi_modinit_mask = 0;

static const struct nu_modinit_s spi_modinit_tab[] = {
    {SPI_0, SPI0_MODULE, CLK_CLKSEL2_SPI0SEL_PCLK1, MODULE_NoMsk, SPI0_RST, SPI0_IRQn, &spi0_var},
    {SPI_1, SPI1_MODULE, CLK_CLKSEL2_SPI1SEL_PCLK0, MODULE_NoMsk, SPI1_RST, SPI1_IRQn, &spi1_var},
    {SPI_2, SPI2_MODULE, CLK_CLKSEL2_SPI2SEL_PCLK1, MODULE_NoMsk, SPI2_RST, SPI2_IRQn, &spi2_var},
    {SPI_3, SPI3_MODULE, CLK_CLKSEL2_SPI3SEL_PCLK0, MODULE_NoMsk, SPI3_RST, SPI3_IRQn, &spi3_var},
    /* Degrade QSPI0/1 to SPI_4/5 for standard SPI usage */
    {SPI_4, QSPI0_MODULE, CLK_CLKSEL2_QSPI0SEL_PCLK0, MODULE_NoMsk, QSPI0_RST, QSPI0_IRQn, &spi4_var},
    {SPI_5, QSPI1_MODULE, CLK_CLKSEL3_QSPI1SEL_PCLK1, MODULE_NoMsk, QSPI1_RST, QSPI1_IRQn, &spi5_var},

    {NC, 0, 0, 0, 0, (IRQn_Type) 0, NULL}
};

SPIName spi_get_peripheral_name(PinName mosi, PinName miso, PinName sclk) {
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);

    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_per = (SPIName)pinmap_merge(spi_data, spi_sclk);

    return spi_per;
}

void spi_get_capabilities(PinName ssel, bool slave, spi_capabilities_t *cap) {
    if (slave) {
        cap->minimum_frequency = 1;
        cap->maximum_frequency = 48000000; // Per the datasheet, max slave SCLK freq is 48MHz
        cap->word_length = 0xFFFFFF80; // Word lengths 32 bits through 8 bits
        cap->support_slave_mode = false; // to be determined later based on ssel
        cap->hw_cs_handle = false; // irrelevant in slave mode
        cap->slave_delay_between_symbols_ns = 2500; // 2.5 us - TODO update, this is currently not used for anything
        cap->clk_modes = 0x0f; // all clock modes
        cap->tx_rx_buffers_equal_length = false; // rx/tx buffers can have different sizes
        cap->async_mode = false;
    } else {
        cap->minimum_frequency = 375000; // Slowest clock is PCLK0/1 / 256
        cap->maximum_frequency = 96000000; // With clock divider 1, SCLK = PCLK0/1 clock, which is 96MHz
        cap->word_length = 0xFFFFFF80; // Word lengths 32 bits through 8 bits
        cap->support_slave_mode = false; // to be determined later based on ssel
        cap->hw_cs_handle = false; // to be determined later based on ssel
        cap->slave_delay_between_symbols_ns = 0; // irrelevant in master mode
        cap->clk_modes = 0x0f;  // all clock modes
        cap->tx_rx_buffers_equal_length = false; // rx/tx buffers can have different sizes
        cap->async_mode = true;
    }

    // check if given ssel pin is in the cs pinmap
    const PinMap *cs_pins = spi_master_cs_pinmap();
    while (cs_pins->pin != NC) {
        if (cs_pins->pin == ssel) {
#if DEVICE_SPISLAVE
            cap->support_slave_mode = true;
#endif
            cap->hw_cs_handle = true;
            break;
        }
        cs_pins++;
    }
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    // Determine which SPI_x the pins are used for
    uint32_t spi_mosi = pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    uint32_t spi_miso = pinmap_peripheral(miso, PinMap_SPI_MISO);
    uint32_t spi_sclk = pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    uint32_t spi_ssel = pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    uint32_t spi_data = pinmap_merge(spi_mosi, spi_miso);
    uint32_t spi_cntl = pinmap_merge(spi_sclk, spi_ssel);
    obj->spi.spi = (SPIName) pinmap_merge(spi_data, spi_cntl);
    MBED_ASSERT((int)obj->spi.spi != NC);

    const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->spi.spi);

    obj->spi.pin_mosi = mosi;
    obj->spi.pin_miso = miso;
    obj->spi.pin_sclk = sclk;
    obj->spi.pin_ssel = ssel;

    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    pinmap_pinout(ssel, PinMap_SPI_SSEL);

    // Select IP clock source
    CLK_SetModuleClock(modinit->clkidx, modinit->clksrc, modinit->clkdiv);

    // Enable IP clock
    CLK_EnableModuleClock(modinit->clkidx);

    // Reset this module
    SYS_ResetModule(modinit->rsetidx);

#if DEVICE_SPI_ASYNCH
    // Note: We don't want to touch the DMA usage here, because either this is a completely new SPI and the DMA usage is already set to 0 (NEVER),
    // or it's a re-initialization of an existing SPI and we can allow it to keep its existing DMA settings.
    
    /* NOTE: We use vector to judge if asynchronous transfer is on-going (spi_active).
     *       At initial time, asynchronous transfer is not on-going and so vector must
     *       be cleared to zero for correct judgement. */
    NVIC_SetVector(modinit->irq_n, 0);
#endif

    // Mark this module to be inited.
    int i = modinit - spi_modinit_tab;
    spi_modinit_mask |= 1 << i;
}

void spi_free(spi_t *obj)
{
#if DEVICE_SPI_ASYNCH
    // Free DMA channels
    nu_spi_set_dma_usage(&obj->spi, DMA_USAGE_NEVER);
#endif

    if (spi_is_qspi(obj)) {
        QSPI_Close((QSPI_T *) NU_MODBASE(obj->spi.spi));
    } else {
        SPI_Close((SPI_T *) NU_MODBASE(obj->spi.spi));
    }

    const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->spi.spi);

    SPI_DisableInt(((SPI_T *) NU_MODBASE(obj->spi.spi)), (SPI_FIFO_RXOV_INT_MASK | SPI_FIFO_RXTH_INT_MASK | SPI_FIFO_TXTH_INT_MASK));
    NVIC_DisableIRQ(modinit->irq_n);

    // Disable IP clock
    CLK_DisableModuleClock(modinit->clkidx);

    // Mark this module to be deinited.
    int i = modinit - spi_modinit_tab;
    spi_modinit_mask &= ~(1 << i);

    // Free up pins
    gpio_set(obj->spi.pin_mosi);
    gpio_set(obj->spi.pin_miso);
    gpio_set(obj->spi.pin_sclk);
    gpio_set(obj->spi.pin_ssel);
    obj->spi.pin_mosi = NC;
    obj->spi.pin_miso = NC;
    obj->spi.pin_sclk = NC;
    obj->spi.pin_ssel = NC;
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    MBED_ASSERT(bits >= NU_SPI_FRAME_MIN && bits <= NU_SPI_FRAME_MAX);

    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);

    SPI_DISABLE_SYNC(spi_base);

    obj->spi.word_size_bits = bits;

    if (spi_is_qspi(obj)) {
        QSPI_Open((QSPI_T *) spi_base,
                  slave ? QSPI_SLAVE : QSPI_MASTER,
                  (mode == 0) ? QSPI_MODE_0 : (mode == 1) ? QSPI_MODE_1 : (mode == 2) ? QSPI_MODE_2 : QSPI_MODE_3,
                  bits,
                  QSPI_GetBusClock((QSPI_T *)spi_base));
    } else {
        SPI_Open(spi_base,
                 slave ? SPI_SLAVE : SPI_MASTER,
                 (mode == 0) ? SPI_MODE_0 : (mode == 1) ? SPI_MODE_1 : (mode == 2) ? SPI_MODE_2 : SPI_MODE_3,
                 bits,
                 SPI_GetBusClock(spi_base));
    }
    // NOTE: Hardcode to be MSB first.
    SPI_SET_MSB_FIRST(spi_base);

    if (! slave) {
        // Master
        if (obj->spi.pin_ssel != NC) {
            // Configure SS as low active.
            SPI_EnableAutoSS(spi_base, SPI_SS, SPI_SS_ACTIVE_LOW);
        } else {
            SPI_DisableAutoSS(spi_base);
        }
    } else {
        // Slave
        // Configure SS as low active.
        spi_base->SSCTL &= ~SPI_SSCTL_SSACTPOL_Msk;
    }

    /* NOTE: M451's/M480's/M2351's SPI_Open() will enable SPI transfer (SPI_CTL_SPIEN_Msk).
     *       We cannot use SPI_CTL_SPIEN_Msk for judgement of spi_active().
     *       Judge with vector instead. */
}

void spi_frequency(spi_t *obj, int hz)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);

    SPI_DISABLE_SYNC(spi_base);

    if (spi_is_qspi(obj)) {
        QSPI_SetBusClock((QSPI_T *) NU_MODBASE(obj->spi.spi), hz);
    } else {
        SPI_SetBusClock((SPI_T *) NU_MODBASE(obj->spi.spi), hz);
    }
}


int spi_master_write(spi_t *obj, int value)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    PinName spi_miso = obj->spi.pin_miso;

    SPI_ENABLE_SYNC(spi_base);

    /* Wait for TX FIFO not full */
    while(! spi_writeable(obj));
    SPI_WRITE_TX(spi_base, value);

    /* Make inter-frame (SPI data frame) delay match configured suspend interval
     * in no MISO case
     *
     * This API requires data write/read simultaneously. However, it can enlarge
     * the inter-frame delay. The data flow for one call of this API would be:
     * 1. Write data to TX FIFO when it is not full
     * 2. Write delay consisting of TX FIFO to TX Shift Register...
     * 3. Actual data transfer on SPI bus
     * 4. Read delay consisting of RX FIFO from RX Shift Register...
     * 5. Read data from RX FIFO when it is not empty
     * Among above, S2&S4 contribute to the inter-frame delay.
     *
     * To favor no MISO case, we skip S4&S5. Thus, S2 can overlap with S3 and doesn't
     * contribute to the inter-frame delay when data is written successively. The solution
     * can cause RX FIFO overrun. Ignore it.
     */
    int value2 = -1;
    if (spi_miso != NC) {
        /* Wait for RX FIFO not empty */
        while (! spi_readable(obj));
        value2 = SPI_READ_RX(spi_base);
    }

    /* We don't call SPI_DISABLE_SYNC here for performance. */

    return value2;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill) {

    // Length is passed in bytes so we need to convert to words
    const uint8_t word_size_bytes = nu_spi_get_bytes_per_word(&obj->spi);
    MBED_ASSERT(tx_length % word_size_bytes == 0);
    MBED_ASSERT(rx_length % word_size_bytes == 0);

    const int tx_words = tx_length / word_size_bytes;
    const int rx_words = rx_length / word_size_bytes;
    const int total_words = (tx_words > rx_words) ? tx_words : rx_words;
    
    for (int word_idx = 0; word_idx < total_words; word_idx++) {

        int out = 0;
        if(word_idx >= tx_length){
            // Use fill char
            memset(&out, write_fill, word_size_bytes);
        }
        else{
            memcpy(&out, tx_buffer + (word_idx * word_size_bytes), word_size_bytes);
        }
        
        int in = spi_master_write(obj, out);
        if (word_idx < rx_words) {
            memcpy(rx_buffer + (word_idx * word_size_bytes), &in, word_size_bytes);
        }
    }

    return total_words * word_size_bytes;
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

#if DEVICE_SPISLAVE
int spi_slave_receive(spi_t *obj)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);

    SPI_ENABLE_SYNC(spi_base);

    return spi_readable(obj);
};

int spi_slave_read(spi_t *obj)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);

    SPI_ENABLE_SYNC(spi_base);

    // Wait for rx buffer full
    while (! spi_readable(obj));
    int value = SPI_READ_RX(spi_base);
    return value;
}

void spi_slave_write(spi_t *obj, int value)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);

    SPI_ENABLE_SYNC(spi_base);

    // Wait for tx buffer empty
    while(! spi_writeable(obj));
    SPI_WRITE_TX(spi_base, value);
}
#endif

#if DEVICE_SPI_ASYNCH
bool spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);

    // Make sure Tx and Rx lengths are sane
    const uint8_t word_size_bytes = nu_spi_get_bytes_per_word(&obj->spi);
    MBED_ASSERT(tx_length % word_size_bytes == 0);
    MBED_ASSERT(rx_length % word_size_bytes == 0);

    // Conditions to go DMA way:
    // (1) No DMA support for non-8 multiple data width.
    // (2) tx length >= rx length. Otherwise, as tx DMA is done, no bus activity for remaining rx.
    if (((obj->spi.word_size_bits % 8) != 0) ||
            (tx_length < rx_length)) {
        hint = DMA_USAGE_NEVER;
    }

    // Set DMA usage, allocating or releasing DMA channels
    nu_spi_set_dma_usage(&obj->spi, hint);

    // SPI IRQ is necessary for both interrupt way and DMA way.
    // However, if we are using DMA then overflows can happen if Tx length > Rx length, so ignore them
    if(obj->spi.dma_usage != DMA_USAGE_NEVER)
    {
        event &= ~(SPI_EVENT_RX_OVERFLOW);
    }
    spi_enable_event(obj, event, 1);
    spi_buffer_set(obj, tx, tx_length, rx, rx_length);

    SPI_ENABLE_SYNC(spi_base);

    // Initialize total SPI transfer frames
    obj->spi.txrx_rmn = NU_MAX(tx_length, rx_length);

    if (obj->spi.dma_usage == DMA_USAGE_NEVER) {
        // Interrupt way
        spi_master_write_asynch(obj, spi_fifo_depth(obj) / 2);
        spi_enable_vector_interrupt(obj, handler, 1);
        spi_master_enable_interrupt(obj, 1);
    } else {
        // DMA way
        const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
        MBED_ASSERT(modinit != NULL);
        MBED_ASSERT(modinit->modname == (int) obj->spi.spi);

        PDMA_T *pdma_base = dma_modbase();

        // Configure tx DMA
        pdma_base->CHCTL |= 1 << obj->spi.dma_chn_id_tx;  // Enable this DMA channel
        PDMA_SetTransferMode(pdma_base,
                             obj->spi.dma_chn_id_tx,
                             ((struct nu_spi_var *) modinit->var)->pdma_perp_tx,    // Peripheral connected to this PDMA
                             0,  // Scatter-gather disabled
                             0); // Scatter-gather descriptor address
        PDMA_SetTransferCnt(pdma_base,
                            obj->spi.dma_chn_id_tx,
                            (word_size_bytes == 1) ? PDMA_WIDTH_8 : (word_size_bytes == 2) ? PDMA_WIDTH_16 : PDMA_WIDTH_32,
                            tx_length / word_size_bytes);
        PDMA_SetTransferAddr(pdma_base,
                             obj->spi.dma_chn_id_tx,
                             (uint32_t) tx,  // NOTE:
                             // NUC472: End of source address
                             // M451/M480: Start of source address
                             PDMA_SAR_INC,   // Source address incremental
                             (uint32_t) &spi_base->TX,   // Destination address
                             PDMA_DAR_FIX);  // Destination address fixed
        PDMA_SetBurstType(pdma_base,
                          obj->spi.dma_chn_id_tx,
                          PDMA_REQ_SINGLE,    // Single mode
                          0); // Burst size
        PDMA_EnableInt(pdma_base,
                       obj->spi.dma_chn_id_tx,
                       PDMA_INT_TRANS_DONE);   // Interrupt type
        // Register DMA event handler
        dma_set_handler(obj->spi.dma_chn_id_tx, (uint32_t) spi_dma_handler_tx, (uint32_t) obj, DMA_EVENT_ALL);

        // Configure rx DMA
        pdma_base->CHCTL |= 1 << obj->spi.dma_chn_id_rx;  // Enable this DMA channel
        PDMA_SetTransferMode(pdma_base,
                             obj->spi.dma_chn_id_rx,
                             ((struct nu_spi_var *) modinit->var)->pdma_perp_rx,    // Peripheral connected to this PDMA
                             0,  // Scatter-gather disabled
                             0); // Scatter-gather descriptor address
        PDMA_SetTransferCnt(pdma_base,
                            obj->spi.dma_chn_id_rx,
                            (word_size_bytes == 1) ? PDMA_WIDTH_8 : (word_size_bytes == 2) ? PDMA_WIDTH_16 : PDMA_WIDTH_32,
                            rx_length / word_size_bytes);
        PDMA_SetTransferAddr(pdma_base,
                             obj->spi.dma_chn_id_rx,
                             (uint32_t) &spi_base->RX,   // Source address
                             PDMA_SAR_FIX,   // Source address fixed
                             (uint32_t) rx,  // NOTE:
                             // NUC472: End of destination address
                             // M451/M480: Start of destination address
                             PDMA_DAR_INC);  // Destination address incremental
        PDMA_SetBurstType(pdma_base,
                          obj->spi.dma_chn_id_rx,
                          PDMA_REQ_SINGLE,    // Single mode
                          0); // Burst size
        PDMA_EnableInt(pdma_base,
                       obj->spi.dma_chn_id_rx,
                       PDMA_INT_TRANS_DONE);   // Interrupt type
        // Register DMA event handler
        dma_set_handler(obj->spi.dma_chn_id_rx, (uint32_t) spi_dma_handler_rx, (uint32_t) obj, DMA_EVENT_ALL);

        /* Start tx/rx DMA transfer
         *
         * If we have both PDMA and SPI interrupts enabled and PDMA priority is lower than SPI priority,
         * we would trap in SPI interrupt handler endlessly with the sequence:
         *
         * 1. PDMA TX transfer done interrupt occurs and is well handled.
         * 2. SPI RX FIFO threshold interrupt occurs. Trap here because PDMA RX transfer done interrupt doesn't get handled.
         * 3. PDMA RX transfer done interrupt occurs but it cannot be handled due to above.
         *
         * To fix it, we don't enable SPI TX/RX threshold interrupts but keep SPI vector handler set to be called
         * in PDMA TX/RX transfer done interrupt handlers (spi_dma_handler_tx/spi_dma_handler_rx).
         */
        NVIC_SetVector(modinit->irq_n, handler);

        /* Order to enable PDMA TX/RX functions
         *
         * H/W spec: In SPI Master mode with full duplex transfer, if both TX and RX PDMA functions are
         *           enabled, RX PDMA function cannot be enabled prior to TX PDMA function. User can enable
         *           TX PDMA function firstly or enable both functions simultaneously.
         * Per real test, it is safer to start RX PDMA first and then TX PDMA. Otherwise, receive FIFO is 
         * subject to overflow by TX DMA.
         *
         * With the above conflicts, we enable PDMA TX/RX functions simultaneously.
         */
        spi_base->PDMACTL |= (SPI_PDMACTL_TXPDMAEN_Msk | SPI_PDMACTL_RXPDMAEN_Msk);

        /* Don't enable SPI TX/RX threshold interrupts as commented above */
    }

    return obj->spi.dma_usage != DMA_USAGE_NEVER;
}

/**
 * Abort an SPI transfer
 * This is a helper function for event handling. When any of the events listed occurs, the HAL will abort any ongoing
 * transfers
 * @param[in] obj The SPI peripheral to stop
 */
void spi_abort_asynch(spi_t *obj)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    PDMA_T *pdma_base = dma_modbase();

    if (obj->spi.dma_usage != DMA_USAGE_NEVER) {
        // Receive FIFO Overrun in case of tx length > rx length on DMA way
        if (spi_base->STATUS & SPI_STATUS_RXOVIF_Msk) {
            spi_base->STATUS = SPI_STATUS_RXOVIF_Msk;
        }

        if (obj->spi.dma_chn_id_tx != DMA_ERROR_OUT_OF_CHANNELS) {
            PDMA_DisableInt(pdma_base, obj->spi.dma_chn_id_tx, PDMA_INT_TRANS_DONE);
            // NOTE: On NUC472, next PDMA transfer will fail with PDMA_STOP() called. Cause is unknown.
            pdma_base->CHCTL &= ~(1 << obj->spi.dma_chn_id_tx);
        }
        SPI_DISABLE_TX_PDMA(((SPI_T *) NU_MODBASE(obj->spi.spi)));

        if (obj->spi.dma_chn_id_rx != DMA_ERROR_OUT_OF_CHANNELS) {
            PDMA_DisableInt(pdma_base, obj->spi.dma_chn_id_rx, PDMA_INT_TRANS_DONE);
            // NOTE: On NUC472, next PDMA transfer will fail with PDMA_STOP() called. Cause is unknown.
            pdma_base->CHCTL &= ~(1 << obj->spi.dma_chn_id_rx);
        }
        SPI_DISABLE_RX_PDMA(((SPI_T *) NU_MODBASE(obj->spi.spi)));

        // If DMA was temporary, free its channels
        if(obj->spi.dma_usage == DMA_USAGE_TEMPORARY_ALLOCATED || obj->spi.dma_usage == DMA_USAGE_OPPORTUNISTIC)
        {
            nu_spi_set_dma_usage(&obj->spi, DMA_USAGE_NEVER);
        }
    }

    // Necessary for both interrupt way and DMA way
    spi_enable_vector_interrupt(obj, 0, 0);
    spi_master_enable_interrupt(obj, 0);

    /* Necessary for accessing FIFOCTL below */
    SPI_DISABLE_SYNC(spi_base);

    SPI_ClearRxFIFO(spi_base);
    SPI_ClearTxFIFO(spi_base);

    // Clear any events which may have been triggered by the transfer or the abort
    if (spi_base->STATUS & SPI_STATUS_RXOVIF_Msk) {
        spi_base->STATUS = SPI_STATUS_RXOVIF_Msk;
    }

    // Receive Time-Out
    if (spi_base->STATUS & SPI_STATUS_RXTOIF_Msk) {
        spi_base->STATUS = SPI_STATUS_RXTOIF_Msk;
    }

    // Transmit FIFO Under-Run
    if (spi_base->STATUS & SPI_STATUS_TXUFIF_Msk) {
        spi_base->STATUS = SPI_STATUS_TXUFIF_Msk;
    }
}

/**
 * Handle the SPI interrupt
 * Read frames until the RX FIFO is empty.  Write at most as many frames as were read.  This way,
 * it is unlikely that the RX FIFO will overflow.
 * @param[in] obj The SPI peripheral that generated the interrupt
 * @return
 */
uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    // Check for SPI events
    uint32_t event = spi_event_check(obj);
    if (event) {
        spi_abort_asynch(obj);
    }

    return (obj->spi.event_mask & event) | ((event & SPI_EVENT_COMPLETE) ? SPI_EVENT_INTERNAL_TRANSFER_COMPLETE : 0);
}

uint8_t spi_active(spi_t *obj)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->spi.spi);

    /* Vector will be cleared when asynchronous transfer is finished or aborted.
       Use it to judge if asynchronous transfer is on-going. */
    uint32_t vec = NVIC_GetVector(modinit->irq_n);
    return vec ? 1 : 0;
}

static int spi_is_qspi(spi_t *obj)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);

    return (spi_base == ((SPI_T *) QSPI0) || spi_base == ((SPI_T *) QSPI1));
}

static int spi_writeable(spi_t * obj)
{
    // Receive FIFO must not be full to avoid receive FIFO overflow on next transmit/receive
    return (! SPI_GET_TX_FIFO_FULL_FLAG(((SPI_T *) NU_MODBASE(obj->spi.spi))));
}

static int spi_readable(spi_t * obj)
{
    return ! SPI_GET_RX_FIFO_EMPTY_FLAG(((SPI_T *) NU_MODBASE(obj->spi.spi)));
}

static void spi_enable_event(spi_t *obj, uint32_t event, uint8_t enable)
{
    obj->spi.event_mask &= ~SPI_EVENT_ALL;
    obj->spi.event_mask |= (event & SPI_EVENT_ALL);
    if (event & SPI_EVENT_RX_OVERFLOW) {
        SPI_EnableInt((SPI_T *) NU_MODBASE(obj->spi.spi), SPI_FIFO_RXOV_INT_MASK);
    }
}

static void spi_enable_vector_interrupt(spi_t *obj, uint32_t handler, uint8_t enable)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->spi.spi);

    if (enable) {
        NVIC_SetVector(modinit->irq_n, handler);
        NVIC_EnableIRQ(modinit->irq_n);
    } else {
        NVIC_DisableIRQ(modinit->irq_n);
        NVIC_SetVector(modinit->irq_n, 0);
    }
}

static void spi_master_enable_interrupt(spi_t *obj, uint8_t enable)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);

    if (enable) {
        uint32_t fifo_depth = spi_fifo_depth(obj);
        SPI_SetFIFO(spi_base, fifo_depth / 2, fifo_depth / 2);
        // Enable tx/rx FIFO threshold interrupt
        SPI_EnableInt(spi_base, SPI_FIFO_RXTH_INT_MASK | SPI_FIFO_TXTH_INT_MASK);
    } else {
        SPI_DisableInt(spi_base, SPI_FIFO_RXTH_INT_MASK | SPI_FIFO_TXTH_INT_MASK);
    }
}

static uint32_t spi_event_check(spi_t *obj)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    uint32_t event = 0;

    if (obj->spi.dma_usage == DMA_USAGE_NEVER) {
        uint32_t n_rec = spi_master_read_asynch(obj);
        spi_master_write_asynch(obj, n_rec);
    }

    if (spi_is_tx_complete(obj) && spi_is_rx_complete(obj)) {
        event |= SPI_EVENT_COMPLETE;
    }

    // Receive FIFO Overrun
    if (spi_base->STATUS & SPI_STATUS_RXOVIF_Msk) {
        event |= SPI_EVENT_RX_OVERFLOW;
    }

    // Transmit FIFO Under-Run
    if (spi_base->STATUS & SPI_STATUS_TXUFIF_Msk) {
        event |= SPI_EVENT_ERROR;
    }

    return event;
}

/**
 * Send words from the SPI TX buffer until the send limit is reached or the TX FIFO is full
 * tx_limit is provided to ensure that the number of SPI frames (words) in flight can be managed.
 * @param[in] obj       The SPI object on which to operate
 * @param[in] tx_limit  The maximum number of words to send
 * @return The number of SPI words that have been transfered
 */
static uint32_t spi_master_write_asynch(spi_t *obj, uint32_t tx_limit)
{
    uint32_t n_words = 0;
    const uint8_t word_size_bytes = nu_spi_get_bytes_per_word(&obj->spi);
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);

    while (obj->spi.txrx_rmn && spi_writeable(obj)) {
        if (spi_is_tx_complete(obj)) {
            // Transmit dummy as transmit buffer is empty
            SPI_WRITE_TX(spi_base, 0);
        } else {
            uint8_t *tx = (uint8_t *)(obj->tx_buff.buffer) + obj->tx_buff.pos;
            switch (word_size_bytes) {
            case 4:
                SPI_WRITE_TX(spi_base, nu_get32_le(tx));
                break;
            case 2:
                SPI_WRITE_TX(spi_base, nu_get16_le(tx));
                break;
            case 1:
                SPI_WRITE_TX(spi_base, *tx);
                break;
            }

            obj->tx_buff.pos += word_size_bytes;
        }
        n_words ++;
        obj->spi.txrx_rmn -= word_size_bytes;
    }

    //Return the number of words that have been sent
    return n_words;
}

/**
 * Read SPI words out of the RX FIFO
 * Continues reading words out of the RX FIFO until the following condition is met:
 * o There are no more words in the FIFO
 * OR BOTH OF:
 * o At least as many words as the TX buffer have been received
 * o At least as many words as the RX buffer have been received
 * This way, RX overflows are not generated when the TX buffer size exceeds the RX buffer size
 * @param[in] obj The SPI object on which to operate
 * @return Returns the number of words extracted from the RX FIFO
 */
static uint32_t spi_master_read_asynch(spi_t *obj)
{
    uint32_t n_words = 0;
    const uint8_t word_size_bytes = nu_spi_get_bytes_per_word(&obj->spi);
    
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);

    while (spi_readable(obj)) {
        if (spi_is_rx_complete(obj)) {
            // Disregard as receive buffer is full
            SPI_READ_RX(spi_base);
        } else {
            uint8_t *rx = (uint8_t *)(obj->rx_buff.buffer) + obj->rx_buff.pos;
            switch (word_size_bytes) {
            case 4: {
                uint32_t val = SPI_READ_RX(spi_base);
                nu_set32_le(rx, val);
                break;
            }
            case 2: {
                uint16_t val = SPI_READ_RX(spi_base);
                nu_set16_le(rx, val);
                break;
            }
            case 1:
                *rx = SPI_READ_RX(spi_base);
                break;
            }

            obj->rx_buff.pos += word_size_bytes;
        }
        n_words ++;
    }

    // Return the number of words received
    return n_words;
}

static void spi_buffer_set(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length)
{
    obj->tx_buff.buffer = (void *) tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos = 0;
    obj->tx_buff.width = nu_spi_get_bytes_per_word(&obj->spi) * 8;
    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos = 0;
    obj->rx_buff.width = nu_spi_get_bytes_per_word(&obj->spi) * 8;
}

static int spi_is_tx_complete(spi_t *obj)
{
    return (obj->tx_buff.pos == obj->tx_buff.length);
}

static int spi_is_rx_complete(spi_t *obj)
{
    return (obj->rx_buff.pos == obj->rx_buff.length);
}

static void spi_dma_handler_tx(uint32_t id, uint32_t event_dma)
{
    spi_t *obj = (spi_t *) id;

    // TODO: Pass this error to caller
    if (event_dma & DMA_EVENT_ABORT) {
    }
    // Expect SPI IRQ will catch this transfer done event
    if (event_dma & DMA_EVENT_TRANSFER_DONE) {
        obj->tx_buff.pos = obj->tx_buff.length;
    }
    // TODO: Pass this error to caller
    if (event_dma & DMA_EVENT_TIMEOUT) {
    }

    const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->spi.spi);

    void (*vec)(void) = (void (*)(void)) NVIC_GetVector(modinit->irq_n);
    MBED_ASSERT(vec != NULL);
    vec();
}

static void spi_dma_handler_rx(uint32_t id, uint32_t event_dma)
{
    spi_t *obj = (spi_t *) id;

    // TODO: Pass this error to caller
    if (event_dma & DMA_EVENT_ABORT) {
    }
    // Expect SPI IRQ will catch this transfer done event
    if (event_dma & DMA_EVENT_TRANSFER_DONE) {
        obj->rx_buff.pos = obj->rx_buff.length;
    }
    // TODO: Pass this error to caller
    if (event_dma & DMA_EVENT_TIMEOUT) {
    }

    const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->spi.spi);

    void (*vec)(void) = (void (*)(void)) NVIC_GetVector(modinit->irq_n);
    MBED_ASSERT(vec != NULL);
    vec();
}

/* Return FIFO depth of the SPI peripheral
 *
 * M480
 *   QSPI0/1        8
 *   SPI0/1/2/3     8 if data width <=16; 4 otherwise
 */
static uint32_t spi_fifo_depth(spi_t *obj)
{
    if (spi_is_qspi(obj)) {
        return 8;
    }

    return (obj->spi.word_size_bits <= 16) ? 8 : 4;
}

#endif

#endif
