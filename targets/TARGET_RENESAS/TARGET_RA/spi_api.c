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

/* The FSP drivers apply the frame width on every write()/read() call, so the
 * width can be changed between transfers at no cost. Master mode only.
 * One FSP transfer per chunk; sized to the 16-bit DTC count limit (the RA8
 * boards here use the DMAC, which counts 32-bit). */
#define SPI_MAX_CHUNK_BYTES 65504

extern const PinMap PinMap_SPI_MOSI[];
extern const PinMap PinMap_SPI_MISO[];
extern const PinMap PinMap_SPI_SCLK[];
extern const PinMap PinMap_SPI_SSEL[];

/* --------------------------------------------------------------------------
 * Extern FSP instances
 * -------------------------------------------------------------------------- */

extern const spi_instance_t g_spi0;
#if BSP_FEATURE_SPI_NUM_CHANNELS >= 2
extern const spi_instance_t g_spi1;
#endif
extern const ioport_instance_t g_ioport;

/* With DEVICE_SPI_ASYNCH the generic spi_t wraps the target struct spi_s. */
#if DEVICE_SPI_ASYNCH
#define SPI_OBJ(obj) (&(obj)->spi)
#else
#define SPI_OBJ(obj) (obj)
#endif

/* --------------------------------------------------------------------------
 * Helpers
 * -------------------------------------------------------------------------- */

static const spi_instance_t *ra_spi_instance_from_channel(SPIName ch)
{
    switch (ch) {
        case SPI_0: return &g_spi0;
#if BSP_FEATURE_SPI_NUM_CHANNELS >= 2
        case SPI_1: return &g_spi1;
#endif
        default: break;
    }
#ifdef RA_SCI_SPI_INSTANCE
    /* Board-specific SCI-in-SPI-mode instance (e.g. SPI on Arduino header) */
    if (ch >= SPI_SCI_BASE) {
        return RA_SCI_SPI_INSTANCE;
    }
#endif
    return NULL;
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

/* Actual frequency of a SCI channel in simple SPI mode (mddr disabled). */
static uint32_t sci_spi_actual_frequency(const sci_spi_div_setting_t *div)
{
    uint32_t sci_source_clock = R_FSP_SystemClockHzGet(BSP_FEATURE_SCI_CLOCK);
    uint32_t divisor = (1U << (2U * (div->cks + 1U))) * (uint32_t) (div->brr + 1U);

    return sci_source_clock / divisor;
}

/* FSP frame width matching the configured bits-per-frame (SCI is always 8). */
static spi_bit_width_t spi_frame_width_get(const struct spi_s *s)
{
    switch (s->bits) {
        case 16: return SPI_BIT_WIDTH_16_BITS;
        case 32: return SPI_BIT_WIDTH_32_BITS;
        default: break;
    }
    return SPI_BIT_WIDTH_8_BITS;
}

/* Bytes per frame for the configured bits-per-frame. */
static int spi_frame_size_get(const struct spi_s *s)
{
    return (int) ((s->bits + 7U) / 8U);
}

/* --------------------------------------------------------------------------
 * Mbed HAL API
 * -------------------------------------------------------------------------- */

 void spi_get_capabilities(PinName ssel, bool slave, spi_capabilities_t *cap)
{
    if (slave) {
        // unsupported
    } else {
        cap->minimum_frequency = 200000;          // 200 kHz
        cap->maximum_frequency = 2000000;         // 2 MHz
        cap->word_length = 0x80008080;            // 8bit, 16bit and 32bit (bit n-1 => n-bit frames)
        cap->support_slave_mode = false;          // not supported
        cap->hw_cs_handle = false;                // to be determined later based on ssel
        cap->slave_delay_between_symbols_ns = 0;  // irrelevant in master mode
        cap->clk_modes = 0x0f;                    // all clock modes
        cap->tx_rx_buffers_equal_length = false;  // rx/tx buffers can have different sizes
        cap->async_mode = true;
    }

    // check if given ssel pin is in the cs pinmap
    const PinMap *cs_pins = spi_master_cs_pinmap();
    while (cs_pins->pin != NC) {
        if (cs_pins->pin == ssel) {
            cap->hw_cs_handle = true;
            break;
        }
        cs_pins++;
    }
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    MBED_ASSERT(obj != NULL);
    struct spi_s *s = SPI_OBJ(obj);

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

    s->channel = (SPIName)spi_periph;
    s->has_miso = (miso != NC);
    s->has_mosi = (mosi != NC);

    /* Configure pins to peripheral function */
    ra_spi_configure_pins(mosi, miso, sclk, ssel);

    /* Bind to FSP instance and copy configuration */
    const spi_instance_t *inst = ra_spi_instance_from_channel(s->channel);
    MBED_ASSERT(inst != NULL);

    s->p_ctrl = (spi_instance_ctrl_t *) inst->p_ctrl;
    s->p_api = inst->p_api;
    s->is_sci = (inst->p_api == &g_spi_on_sci);

    const spi_cfg_t *cfg_src = inst->p_cfg;

    /* Shallow copy cfg and ext into local storage so we can tweak bitrate/mode later.
     * The extended configuration layout depends on the driver (R_SPI vs R_SCI_SPI). */
    s->cfg = *cfg_src;
    if (s->is_sci) {
        s->ext.sci = *(const sci_spi_extended_cfg_t *) cfg_src->p_extend;
        s->cfg.p_extend = &s->ext.sci;
        s->hz = sci_spi_actual_frequency(&s->ext.sci.clk_div);
    } else {
        const spi_extended_cfg_t *ext_src = (const spi_extended_cfg_t *) cfg_src->p_extend;
        if (ext_src) {
            s->ext.spi = *ext_src;
            s->cfg.p_extend = &s->ext.spi;
        } else {
            s->cfg.p_extend = NULL;
        }
        const spi_extended_cfg_t *ext = (const spi_extended_cfg_t *) s->cfg.p_extend;
        s->hz = (ext != NULL) ? spi_actual_frequency((rspck_div_setting_t *) &ext->spck_div) : 0;
    }

    s->cfg.p_context = obj;
    s->bits = 8;
    s->mode = 0;
    s->sync_active = false;
#if DEVICE_SPI_ASYNCH
    s->async_active = false;
    s->async_result = SPI_EVENT_ERROR;
    s->async_handler = 0;
#endif

#if MBED_CONF_RTOS_PRESENT
    if(s->semaphoreId == NULL)
    {
        osSemaphoreAttr_t attr = { 0 };
        attr.cb_mem = &s->semaphoreMem;
        attr.cb_size = sizeof(osRtxSemaphore_t);
        s->semaphoreId = osSemaphoreNew(1, 0, &attr);
    }
#else
    s->xfer_done = true;
#endif
    /* Open SPI */
    fsp_err_t err = s->p_api->open(s->p_ctrl, &s->cfg);
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
    struct spi_s *s = SPI_OBJ(obj);
    s->p_api->close(s->p_ctrl);
#if MBED_CONF_RTOS_PRESENT
    osSemaphoreDelete(s->semaphoreId);
    s->semaphoreId = NULL;
#endif
}

/* bits: 8/16/32, mode: 0..3, slave: 0=master, 1=slave (master only is supported).
 * The FSP drivers take the frame width as a per-transfer argument and program
 * the width register on every transfer, so switching only the frame width is a
 * pure software operation (no close/open). A mode change reconfigures the
 * peripheral. SCI channels in simple SPI mode are limited to 8-bit frames. */
void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    MBED_ASSERT(obj != NULL);
    struct spi_s *s = SPI_OBJ(obj);
    (void) slave;

    if ((bits != 8) && (bits != 16) && (bits != 32)) {
        bits = 8;
    }
    if (s->is_sci) {
        bits = 8;
    }

    mode &= 0x3;

    if ((uint8_t) mode == s->mode) {
        /* Width-only change (or no-op): no hardware reconfiguration needed. */
        s->bits = (uint8_t) bits;
        return;
    }

    s->bits = (uint8_t) bits;
    s->mode = (uint8_t) mode;

    int cpol = (mode & 0x2) ? 1 : 0;
    int cpha = (mode & 0x1) ? 1 : 0;

    s->cfg.clk_polarity = cpol ? SPI_CLK_POLARITY_HIGH : SPI_CLK_POLARITY_LOW;
    s->cfg.clk_phase    = cpha ? SPI_CLK_PHASE_EDGE_EVEN : SPI_CLK_PHASE_EDGE_ODD;

    s->cfg.operating_mode = SPI_MODE_MASTER;

    s->p_api->close(s->p_ctrl);
    s->p_api->open(s->p_ctrl, &s->cfg);
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

/* Calculate the divider for the requested bit rate and store it in the local
 * extended configuration copy. Returns the actual frequency achieved. */
static uint32_t spi_update_bitrate(struct spi_s *s, int hz)
{
    if (s->is_sci) {
        sci_spi_div_setting_t div;
        if (FSP_SUCCESS != R_SCI_SPI_CalculateBitrate((uint32_t) hz, &div, false)) {
            return s->hz;
        }
        s->ext.sci.clk_div = div;
        return sci_spi_actual_frequency(&div);
    }

    rspck_div_setting_t div;
    if (FSP_SUCCESS != spi_calculate_bitrate(hz, &div)) {
        return s->hz;
    }
    s->ext.spi.spck_div = div;
    return spi_actual_frequency(&div);
}

void spi_frequency(spi_t *obj, int hz)
{
    MBED_ASSERT(obj != NULL);
    struct spi_s *s = SPI_OBJ(obj);

    if (hz <= 0) {
        hz = 1000000;
    }

    s->hz = spi_update_bitrate(s, hz);

    s->p_api->close(s->p_ctrl);
    s->p_api->open(s->p_ctrl, &s->cfg);
}

static void spi_switch_tx_only_mode(struct spi_s *s, bool enable_tx_only)
{
    /* The R_SCI_SPI driver discards received data on write(), no register
     * switch is needed for SCI channels. */
    if (s->is_sci) {
        return;
    }

    if(enable_tx_only)
    {
        s->ext.spi.spi_comm = SPI_COMMUNICATION_TRANSMIT_ONLY;
#if BSP_PERIPHERAL_SPI_B_PRESENT
        /* TXMD is a 2-bit field: 01 selects transmit-only. Do not use
         * R_SPI_B0_SPCR_TXMD_Msk here, it covers both bits (0b11). */
        s->p_ctrl->p_regs->SPCR &= ~R_SPI_B0_SPCR_SPRIE_Msk;
        s->p_ctrl->p_regs->SPCR |= (uint32_t) (1U << R_SPI_B0_SPCR_TXMD_Pos);
#else
        s->p_ctrl->p_regs->SPCR &= ~R_SPI0_SPCR_SPRIE_Msk;
        s->p_ctrl->p_regs->SPCR |= (R_SPI0_SPCR_TXMD_Msk | R_SPI0_SPCR_SPTIE_Msk);
#endif
    }
    else
    {
        s->ext.spi.spi_comm = SPI_COMMUNICATION_FULL_DUPLEX;
#if BSP_PERIPHERAL_SPI_B_PRESENT
        s->p_ctrl->p_regs->SPCR |= R_SPI_B0_SPCR_SPRIE_Msk;
        s->p_ctrl->p_regs->SPCR &= ~R_SPI_B0_SPCR_TXMD_Msk;
#else
        s->p_ctrl->p_regs->SPCR |= R_SPI0_SPCR_SPRIE_Msk;
        s->p_ctrl->p_regs->SPCR &= ~(R_SPI0_SPCR_TXMD_Msk | R_SPI0_SPCR_SPTIE_Msk);
#endif
    }
}

/* Mark a synchronous transfer as started (routes the FSP callback). */
static void spi_sync_transfer_begin(struct spi_s *s)
{
    s->sync_active = true;
#if !MBED_CONF_RTOS_PRESENT
    s->xfer_done = false;
#endif
}

/* Waiting for SPI transmission to complete */
static void spi_wait(spi_t *obj)
{
    struct spi_s *s = SPI_OBJ(obj);
#if MBED_CONF_RTOS_PRESENT
    osSemaphoreAcquire(s->semaphoreId, SPI_TIMEOUT_DEFAULT_VALUE);
#else
    while (!s->xfer_done);
#endif
}

/* Blocking single-symbol transfer: write one frame, return the received frame. */
int spi_master_write(spi_t *obj, int value)
{
    MBED_ASSERT(obj != NULL);
    struct spi_s *s = SPI_OBJ(obj);

    uint32_t tx = (uint32_t) value;
    uint32_t rx = 0;

#if BSP_CFG_DCACHE_ENABLED
    SCB_CleanDCache_by_Addr((void *) &tx, sizeof(tx));
#endif

    spi_bit_width_t width = spi_frame_width_get(s);
    fsp_err_t err = 0;

    spi_sync_transfer_begin(s);
    spi_switch_tx_only_mode(s, !s->has_miso);

    if (s->has_miso) {
        err = s->p_api->writeRead(s->p_ctrl, &tx, &rx, 1, width);
    }
    else {
        err = s->p_api->write(s->p_ctrl, &tx, 1, width);
    }
    if (FSP_SUCCESS != err) {
        s->sync_active = false;
#if !MBED_CONF_RTOS_PRESENT
        s->xfer_done = true;
#endif
        /* On error, return -1 to signal failure */
        return -1;
    }
    spi_wait(obj);

    return (int) rx;
}

int spi_master_block_write(spi_t *obj,
                           const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length,
                           char write_fill)
{
    MBED_ASSERT(obj != NULL);
    struct spi_s *s = SPI_OBJ(obj);

    int total = tx_length > rx_length ? tx_length : rx_length;
    if (total <= 0) {
        return 0;
    }

    /* Buffer lengths are in bytes and must be a multiple of the frame size. */
    const int frame = spi_frame_size_get(s);
    if ((total % frame) != 0) {
        MBED_ASSERT((total % frame) == 0);
        return -1;
    }

    spi_bit_width_t width = spi_frame_width_get(s);
    int offset = 0;

    /* Fill / dummy buffers widened to the largest supported frame. */
    uint8_t fill_byte = (uint8_t) write_fill;
    uint32_t fill = ((uint32_t) fill_byte << 24) | ((uint32_t) fill_byte << 16) |
                    ((uint32_t) fill_byte << 8)  |  (uint32_t) fill_byte;
    uint32_t dummy_rx;

#if BSP_CFG_DCACHE_ENABLED
    if(tx_length != 0)
    {
        SCB_CleanDCache_by_Addr((volatile void *)tx_buffer, tx_length);
    }
#endif
    bool is_tx_only = !s->has_miso || rx_buffer == NULL || rx_length == 0;
    spi_switch_tx_only_mode(s, is_tx_only);

    while (offset < total) {

        int chunk = total - offset;
        if (chunk > SPI_MAX_CHUNK_BYTES) {
            chunk = SPI_MAX_CHUNK_BYTES;
        }

        const void *tx_ptr;
        void *rx_ptr;

        if (offset < tx_length) {
            tx_ptr = (const void *) (tx_buffer + offset);
        } else {
            tx_ptr = &fill;
        }

        if (offset < rx_length) {
            rx_ptr = (void *) (rx_buffer + offset);
        } else {
            rx_ptr = &dummy_rx;
        }

        fsp_err_t err = 0;
        spi_sync_transfer_begin(s);

        if(!is_tx_only) {
            if(tx_length == 0) {
                err = s->p_api->read(s->p_ctrl, rx_ptr, (uint32_t) (chunk / frame), width);
            }
            else {
                err = s->p_api->writeRead(s->p_ctrl, tx_ptr, rx_ptr, (uint32_t) (chunk / frame), width);
            }
        }
        else {
            err = s->p_api->write(s->p_ctrl, tx_ptr, (uint32_t) (chunk / frame), width);
        }

        if (FSP_SUCCESS != err) {
            s->sync_active = false;
#if !MBED_CONF_RTOS_PRESENT
            s->xfer_done = true;
#endif
            return -1;
        }

        spi_wait(obj);

        offset += chunk;
    }

    return total;
}


int spi_busy(spi_t *obj)
{
    struct spi_s *s = SPI_OBJ(obj);
#if DEVICE_SPI_ASYNCH
    if (s->async_active || s->sync_active) {
        return 1;
    }
#else
    if (s->sync_active) {
        return 1;
    }
#endif
#if MBED_CONF_RTOS_PRESENT
    return osSemaphoreGetCount(s->semaphoreId) == 0;
#else
    return !s->xfer_done;
#endif
}

#if DEVICE_SPI_ASYNCH

bool spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    MBED_ASSERT(obj != NULL);
    struct spi_s *s = SPI_OBJ(obj);
    (void) bit_width; /* Always matches the configured frame width (see spi_api.h). */
    (void) event;     /* Completion is always reported through the callback. */
    (void) hint;      /* Transfer acceleration is fixed by the FSP instance config. */

    size_t total = (tx_length > rx_length) ? tx_length : rx_length;
    if (total == 0) {
        return false;
    }

    /* Buffer lengths are in bytes and must be a multiple of the frame size. */
    const int frame = spi_frame_size_get(s);
    if ((total % (size_t) frame) != 0) {
        MBED_ASSERT((total % (size_t) frame) == 0);
        return false;
    }

    spi_bit_width_t width = spi_frame_width_get(s);

    s->async_handler = handler;
    s->async_result = SPI_EVENT_ERROR;
    s->async_active = true;

    bool is_tx_only = !s->has_miso || (rx == NULL) || (rx_length == 0);
    spi_switch_tx_only_mode(s, is_tx_only);

    fsp_err_t err;
    if (is_tx_only) {
        err = s->p_api->write(s->p_ctrl, tx, (uint32_t) (total / frame), width);
    }
    else if (tx_length == 0) {
        err = s->p_api->read(s->p_ctrl, rx, (uint32_t) (total / frame), width);
    }
    else {
        err = s->p_api->writeRead(s->p_ctrl, tx, rx, (uint32_t) (total / frame), width);
    }

    if (FSP_SUCCESS != err) {
        s->async_active = false;
        return false;
    }

    /* Always report non-DMA: the FSP instances here accelerate TX with DMAC
     * but receive through the CPU, and the return value only controls whether
     * the driver layer invalidates the Rx buffer cache after completion. */
    return false;
}

uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    struct spi_s *s = SPI_OBJ(obj);
    return s->async_result;
}

uint8_t spi_active(spi_t *obj)
{
    struct spi_s *s = SPI_OBJ(obj);
    return (s->async_active || s->sync_active) ? 1 : 0;
}

void spi_abort_asynch(spi_t *obj)
{
    struct spi_s *s = SPI_OBJ(obj);
    if (s->async_active) {
        /* Clear first so a late FSP callback cannot fire the CThunk for an
         * already-aborted transfer (mbed expects no callback after abort). */
        s->async_active = false;

        if (!s->is_sci) {
            /* The FSP SPI API has no abort call, so stop the peripheral
             * directly: disable operation and all interrupt enables and clear
             * pending status. The next write()/read() call fully reprograms
             * the transfer state, so no further cleanup is needed. */
#if BSP_PERIPHERAL_SPI_B_PRESENT
            s->p_ctrl->p_regs->SPCR &= ~(R_SPI_B0_SPCR_SPTIE_Msk | R_SPI_B0_SPCR_SPRIE_Msk |
                                         R_SPI_B0_SPCR_CENDIE_Msk | R_SPI_B0_SPCR_SPEIE_Msk |
                                         R_SPI_B0_SPCR_SPE_Msk);
            s->p_ctrl->p_regs->SPSRC = R_SPI_B0_SPSRC_OVRFC_Msk | R_SPI_B0_SPSRC_PERFC_Msk |
                                       R_SPI_B0_SPSRC_MODFC_Msk | R_SPI_B0_SPSRC_UDRFC_Msk |
                                       R_SPI_B0_SPSRC_SPTEFC_Msk | R_SPI_B0_SPSRC_CENDFC_Msk |
                                       R_SPI_B0_SPSRC_SPRFC_Msk;
#else
            s->p_ctrl->p_regs->SPCR &= ~(R_SPI0_SPCR_SPTIE_Msk | R_SPI0_SPCR_SPRIE_Msk |
                                         R_SPI0_SPCR_CENDIE_Msk | R_SPI0_SPCR_SPEIE_Msk |
                                         R_SPI0_SPCR_SPE_Msk);
#endif
        }
        /* SCI simple-SPI has no clean register-level abort; the transfer is
         * left to finish in the background. The completion event is routed to
         * the ignore path because async_active is already false. */
    }
}

#endif /* DEVICE_SPI_ASYNCH */

/* ---------------- SPI callback ---------------- */

void spi_callback(spi_callback_args_t * p_args)
{
    spi_t *obj = (spi_t *) p_args->p_context;
    struct spi_s *s = SPI_OBJ(obj);

#if DEVICE_SPI_ASYNCH
    if (s->async_active) {
        s->async_result = (SPI_EVENT_TRANSFER_COMPLETE == p_args->event)
                          ? (uint32_t) SPI_EVENT_COMPLETE : (uint32_t) SPI_EVENT_ERROR;
        s->async_active = false;
        if (s->async_handler != 0) {
            ((void (*)(void)) s->async_handler)();
        }
        return;
    }
#endif

    /* Synchronous transfer completion (anything else, e.g. a late event after
     * an abort, is ignored). */
    if (s->sync_active) {
        s->sync_active = false;
#if MBED_CONF_RTOS_PRESENT
        osSemaphoreRelease(s->semaphoreId);
#else
        s->xfer_done = true;
#endif
    }
}

/* ---------------- PinMap getters ---------------- */

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
