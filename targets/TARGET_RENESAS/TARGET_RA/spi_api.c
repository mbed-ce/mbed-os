/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed_assert.h"
#include "mbed_error.h"
#include "spi_api.h"
#include "pinmap.h"

#include "hal_data.h"        /* FSP-generated: g_spi0, g_spi1, g_ioport etc. */
#include "r_ioport.h"

#define SPI_TIMEOUT_DEFAULT_VALUE 500

extern const PinMap PinMap_SPI_MOSI[];
extern const PinMap PinMap_SPI_MISO[];
extern const PinMap PinMap_SPI_SCLK[];
extern const PinMap PinMap_SPI_SSEL[];

/* --------------------------------------------------------------------------
 *  Extern FSP instances
 * -------------------------------------------------------------------------- */

extern const spi_instance_t g_spi0;
#if BSP_FEATURE_SPI_NUM_CHANNELS >= 2
extern const spi_instance_t g_spi1;
#endif
extern const ioport_instance_t g_ioport;

/* --------------------------------------------------------------------------
 *  Helpers
 * -------------------------------------------------------------------------- */

static const spi_instance_t *ra_spi_instance_from_channel(SPIName ch)
{
    switch (ch) {
        case SPI_0: return &g_spi0;
#if BSP_FEATURE_SPI_NUM_CHANNELS >= 2
        case SPI_1: return &g_spi1;
#endif
        default:    return NULL;
    }
}

static void ra_spi_configure_pins(PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    /* Configure peripheral function for each pin using PinMap.function */
    uint32_t func;

    if (mosi != NC) {
        func = pinmap_function(mosi, PinMap_SPI_MOSI);
        if (func) {
            pin_function(mosi, func);
        }
    }

    if (miso != NC) {
        func = pinmap_function(miso, PinMap_SPI_MISO);
        if (func) {
            pin_function(miso, func);
        }
    }

    if (sclk != NC) {
        func = pinmap_function(sclk, PinMap_SPI_SCLK);
        if (func) {
            pin_function(sclk, func);
        }
    }

    if (ssel != NC) {
        func = pinmap_function(ssel, PinMap_SPI_SSEL);
        if (func) {
            pin_function(ssel, func);
        }
    }
}

static uint32_t spi_actual_frequency(rspck_div_setting_t *div)
{
#if BSP_FEATURE_SCI_HAS_SCISPI_CLOCK
    // SPI clock source must be selected as SCISPICLK
    uint32_t spi_source_clock = R_FSP_SciSpiClockHzGet();
#elif BSP_FEATURE_SPI_HAS_CLOCK
    // SPI clock source must be selected as SPICLK
    uint32_t spi_source_clock = R_FSP_SpiClockHzGet();
#else
    // SPI clock source is PCLKA on RA4 and RA6
    uint32_t spi_source_clock = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKA);
#endif
    uint32_t spbr = div->spbr;
    uint32_t brdv = div->brdv;

    uint32_t divisor = (2U * (spbr + 1U)) << brdv;

    return spi_source_clock / divisor;
}

/* --------------------------------------------------------------------------
 *  Mbed HAL API
 * -------------------------------------------------------------------------- */

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    MBED_ASSERT(obj != NULL);

    /* Resolve peripheral from pins */
    int mosi_periph = pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    int miso_periph = pinmap_peripheral(miso, PinMap_SPI_MISO);
    int sclk_periph = pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    int ssel_periph = pinmap_peripheral(ssel, PinMap_SPI_SSEL);

    int spi_periph = pinmap_merge(mosi_periph,
                        pinmap_merge(miso_periph,
                        pinmap_merge(sclk_periph, ssel_periph)));

    if (spi_periph == (int)NC) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SPI, MBED_ERROR_CODE_INVALID_ARGUMENT), "spi_init");
    }

    obj->channel = (SPIName)spi_periph;
    obj->has_miso = (miso != NC);
    obj->has_mosi = (mosi != NC);

    /* Configure pins to peripheral function */
    ra_spi_configure_pins(mosi, miso, sclk, ssel);

    /* Bind to FSP instance and copy configuration */
    const spi_instance_t *inst = ra_spi_instance_from_channel(obj->channel);
    MBED_ASSERT(inst != NULL);

    obj->p_ctrl = (spi_instance_ctrl_t *) inst->p_ctrl;
    const spi_cfg_t *cfg_src = inst->p_cfg;
    const spi_extended_cfg_t *ext_src = (const spi_extended_cfg_t *)cfg_src->p_extend;

    /* Shallow copy cfg and ext into local storage so we can tweak bitrate/mode later */
    obj->cfg = *cfg_src;
    if (ext_src) {
        obj->ext = *ext_src;
        obj->cfg.p_extend = &obj->ext;
    } else {
        obj->cfg.p_extend = NULL;
    }

    obj->cfg.p_context = obj;
    obj->bits = 8;
    obj->mode = 0;

    spi_extended_cfg_t *ext = (spi_extended_cfg_t *)obj->cfg.p_extend;
    obj->hz = spi_actual_frequency(&ext->spck_div);

#if MBED_CONF_RTOS_PRESENT
    if(obj->semaphoreId == NULL)
    {
        osSemaphoreAttr_t attr = { 0 };
        attr.cb_mem = &obj->semaphoreMem;
        attr.cb_size = sizeof(osRtxSemaphore_t);
        obj->semaphoreId = osSemaphoreNew(1, 0, &attr);
    }
#else
    obj->xfer_done = true;
#endif
    /* Open SPI */
    fsp_err_t err = R_SPI_Open(obj->p_ctrl, &obj->cfg);
    if (FSP_SUCCESS != err) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SPI, MBED_ERROR_CODE_INITIALIZATION_FAILED), "spi_init");
    }
}

void spi_init_direct(spi_t *obj, const spi_pinmap_t *pinmap)
{
    spi_init(obj, pinmap->mosi_pin, pinmap->miso_pin, pinmap->sclk_pin, pinmap->ssel_pin);
}

void spi_free(spi_t *obj)
{
    if (!obj) {
        return;
    }
    R_SPI_Close(obj->p_ctrl);
#if MBED_CONF_RTOS_PRESENT
    osSemaphoreDelete(obj->semaphoreId);
    obj->semaphoreId = NULL;
#endif
}

/* bits: 4..16, mode: 0..3, slave: 0=master, 1=slave (we only support 8-bit master transfer here) */
void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    MBED_ASSERT(obj != NULL);

    if (bits != 8) {
        bits = 8;
    }

    obj->bits = bits;

    int cpol = (mode & 0x2) ? 1 : 0;
    int cpha = (mode & 0x1) ? 1 : 0;

    obj->cfg.clk_polarity = cpol ? SPI_CLK_POLARITY_HIGH : SPI_CLK_POLARITY_LOW;
    obj->cfg.clk_phase    = cpha ? SPI_CLK_PHASE_EDGE_EVEN : SPI_CLK_PHASE_EDGE_ODD;

    obj->cfg.operating_mode = SPI_MODE_MASTER;

    R_SPI_Close(obj->p_ctrl);
    R_SPI_Open(obj->p_ctrl, &obj->cfg);
}

static fsp_err_t spi_calculate_bitrate(int hz, rspck_div_setting_t *div)
{
#if BSP_PERIPHERAL_SPI_B_PRESENT
    fsp_err_t err = R_SPI_B_CalculateBitrate(hz, SPI_B_CLOCK_SOURCE_SCISPICLK, div);
#else
    fsp_err_t err = R_SPI_CalculateBitrate(hz, div);
#endif
    return err;
}

static void spi_switch_tx_only_mode(spi_t *obj, bool enable_tx_only)
{
    if(enable_tx_only)
    {
        obj->ext.spi_comm = SPI_COMMUNICATION_TRANSMIT_ONLY;
#if BSP_PERIPHERAL_SPI_B_PRESENT
        obj->p_ctrl->p_regs->SPCR &= ~R_SPI_B0_SPCR_SPRIE_Msk;
        obj->p_ctrl->p_regs->SPCR |= (uint32_t) ((uint32_t)1 << R_SPI_B0_SPCR_TXMD_Pos);
#else
        obj->p_ctrl->p_regs->SPCR &= ~R_SPI0_SPCR_SPRIE_Msk;
        obj->p_ctrl->p_regs->SPCR |= (R_SPI0_SPCR_TXMD_Msk | R_SPI0_SPCR_SPTIE_Msk);
#endif
    }
    else
    {
        obj->ext.spi_comm = SPI_COMMUNICATION_FULL_DUPLEX;
#if BSP_PERIPHERAL_SPI_B_PRESENT
        obj->p_ctrl->p_regs->SPCR |= R_SPI_B0_SPCR_SPRIE_Msk;
        obj->p_ctrl->p_regs->SPCR &= ~R_SPI_B0_SPCR_TXMD_Msk;
#else
        obj->p_ctrl->p_regs->SPCR |= R_SPI0_SPCR_SPRIE_Msk;
        obj->p_ctrl->p_regs->SPCR &= ~(R_SPI0_SPCR_TXMD_Msk | R_SPI0_SPCR_SPTIE_Msk);
#endif
    }
}

void spi_frequency(spi_t *obj, int hz)
{
    MBED_ASSERT(obj != NULL);

    if (hz <= 0) {
        hz = 1000000;
    }

    rspck_div_setting_t div;

    fsp_err_t err = spi_calculate_bitrate(hz, &div);
    if (err != FSP_SUCCESS) {
        hz = 1000000;
        spi_calculate_bitrate(hz, &div);
    }

    spi_extended_cfg_t *ext = (spi_extended_cfg_t *)obj->cfg.p_extend;
    ext->spck_div = div;

    obj->hz = spi_actual_frequency(&div);

    R_SPI_Close(obj->p_ctrl);
    R_SPI_Open(obj->p_ctrl, &obj->cfg);
}

/* Waiting for SPI transmission to complete */
static void spi_wait(spi_t *obj)
{
#if MBED_CONF_RTOS_PRESENT
    osSemaphoreAcquire(obj->semaphoreId, SPI_TIMEOUT_DEFAULT_VALUE);
#else
    while (!obj->xfer_done);
#endif
}

/* Blocking 8-bit transfer: write one byte, return received byte */
int spi_master_write(spi_t *obj, int value)
{
    MBED_ASSERT(obj != NULL);

    uint8_t tx = (uint8_t)value;
    uint8_t rx = 0;

#if BSP_CFG_DCACHE_ENABLED
    SCB_CleanDCache_by_Addr((volatile void *)&value, 1);
#endif

    fsp_err_t err = 0;
#if !MBED_CONF_RTOS_PRESENT
    obj->xfer_done = false;
#endif
    spi_switch_tx_only_mode(obj, !obj->has_miso);

    if (obj->has_miso) {
        err = R_SPI_WriteRead(obj->p_ctrl, &tx, &rx, 1, SPI_BIT_WIDTH_8_BITS);
    }
    else {
        err = R_SPI_Write(obj->p_ctrl, &tx, 1, SPI_BIT_WIDTH_8_BITS);
    }
    if (FSP_SUCCESS != err) {
        /* On error, return -1 to signal failure */
        return -1;
    }
    spi_wait(obj);

    return (int)rx;
}

int spi_master_block_write(spi_t *obj,
                           const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length,
                           char write_fill)
{
    MBED_ASSERT(obj != NULL);

    int total = tx_length > rx_length ? tx_length : rx_length;
    if (total <= 0) {
        return 0;
    }

    const int CHUNK = 1024;
    int offset = 0;

    uint8_t fill = write_fill;
    uint8_t dummy_rx;

#if BSP_CFG_DCACHE_ENABLED
    if(tx_length != 0)
    {
        SCB_CleanDCache_by_Addr((volatile void *)tx_buffer, tx_length);
    }
#endif
    bool is_tx_only = !obj->has_miso || rx_buffer == NULL || rx_length == 0;
    spi_switch_tx_only_mode(obj, is_tx_only);

    while (offset < total) {

        int chunk = total - offset;
        if (chunk > CHUNK) {
            chunk = CHUNK;
        }

        const uint8_t *tx_ptr;
        uint8_t *rx_ptr;

        if (offset < tx_length) {
            tx_ptr = (const uint8_t *)(tx_buffer + offset);
        } else {
            tx_ptr = &fill;
        }

        if (offset < rx_length) {
            rx_ptr = (uint8_t *)(rx_buffer + offset);
        } else {
            rx_ptr = &dummy_rx;
        }

        fsp_err_t err = 0;
#if !MBED_CONF_RTOS_PRESENT
        obj->xfer_done = false;
#endif

        if(!is_tx_only) {
            if(tx_length == 0) {
                err = R_SPI_Read(obj->p_ctrl, rx_ptr, chunk, SPI_BIT_WIDTH_8_BITS);
            }
            else {
                err = R_SPI_WriteRead(obj->p_ctrl, tx_ptr, rx_ptr, chunk, SPI_BIT_WIDTH_8_BITS);
            }
        }
        else {
            err = R_SPI_Write(obj->p_ctrl, tx_ptr, chunk, SPI_BIT_WIDTH_8_BITS);
        }

        if (FSP_SUCCESS != err) {
            return -1;
        }

        spi_wait(obj);

        offset += chunk;
    }

    return total;
}


int spi_busy(spi_t *obj)
{
#if MBED_CONF_RTOS_PRESENT
    return osSemaphoreGetCount(obj->semaphoreId) == 0;
#else
    return !obj->xfer_done;
#endif
}

/* ---------------- SPI callback ---------------- */

void spi_callback(spi_callback_args_t * p_args)
{
    spi_t *obj = (spi_t *) p_args->p_context;
#if MBED_CONF_RTOS_PRESENT
    osSemaphoreRelease(obj->semaphoreId);
#else
    obj->xfer_done = true;
#endif
}
