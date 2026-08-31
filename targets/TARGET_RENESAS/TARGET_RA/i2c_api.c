/* mbed Microcontroller Library
 * Copyright (c) 2024-2026 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#if DEVICE_I2C

#include "mbed_assert.h"
#include "mbed_error.h"
#include "mbed_toolchain.h"
#include "mbed_wait_api.h"
#include "i2c_api.h"
#include "pinmap.h"

#include "hal_data.h"
#include "r_ioport.h"

#include "i2c_device.h"

extern const PinMap PinMap_I2C_SCL[];
extern const PinMap PinMap_I2C_SDA[];

extern const i2c_master_instance_t g_i2c_master0;
#if (BSP_FEATURE_IIC_VALID_CHANNEL_MASK & 2) != 0
extern const i2c_master_instance_t g_i2c_master1;
#endif
extern const ioport_instance_t g_ioport;

/* Timeout for one byte transferred on the bus, in microseconds.  A byte takes about 90 us
 * at 100 kHz, so this is very generous and mainly guards against a stuck bus. */
#define RA_I2C_BYTE_TIMEOUT_US 10000

/* --------------------------------------------------------------------------
 *  Helpers
 * -------------------------------------------------------------------------- */

static const i2c_master_instance_t *ra_i2c_instance_from_channel(I2CName ch)
{
    switch (ch) {
        case I2C_0: return &g_i2c_master0;
#if (BSP_FEATURE_IIC_VALID_CHANNEL_MASK & 2) != 0
        case I2C_1: return &g_i2c_master1;
#endif
        default:    return NULL;
    }
}

static void ra_i2c_configure_pins(PinName sda, int sda_func, PinName scl, int scl_func)
{
    if (sda != NC && sda_func) {
        pin_function(sda, sda_func);
    }
    if (scl != NC && scl_func) {
        pin_function(scl, scl_func);
    }
}

static void ra_i2c_configure_pins_from_pinmap(PinName sda, PinName scl)
{
    uint32_t func;

    if (sda != NC) {
        func = pinmap_function(sda, PinMap_I2C_SDA);
        if (func) {
            pin_function(sda, func);
        }
    }

    if (scl != NC) {
        func = pinmap_function(scl, PinMap_I2C_SCL);
        if (func) {
            pin_function(scl, func);
        }
    }
}

static int ra_i2c_rate_to_hz(i2c_master_rate_t rate)
{
    switch (rate) {
        case I2C_MASTER_RATE_STANDARD:  return 100000;
        case I2C_MASTER_RATE_FAST:      return 400000;
        case I2C_MASTER_RATE_FASTPLUS:  return 1000000;
        default:                        return 100000;
    }
}

static void ra_i2c_set_address(i2c_t *obj, int address, bool ten_bit)
{
    /* Note: while the bus is being held after a restart-ending transfer, the FSP driver
     * refuses to change the slave address (FSP_ERR_IN_USE), because the next transfer
     * must continue with a repeated start to the same device.  In that case, skip the
     * call and keep the address latched from the previous transfer. */
    if (obj->i2c.p_ctrl->restart) {
        return;
    }

    uint16_t addr = (uint16_t)(address >> 1);

    fsp_err_t err = obj->i2c.p_api->slaveAddressSet(
        obj->i2c.p_ctrl,
        addr,
        ten_bit ? I2C_MASTER_ADDR_MODE_10BIT : I2C_MASTER_ADDR_MODE_7BIT
    );

    if (FSP_SUCCESS != err) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_I2C, MBED_ERROR_CODE_INVALID_ARGUMENT), "i2c_set_address");
    }
}

/**
 * Wait until the current FSP transfer finishes, with a timeout based on the transfer length.
 *
 * @return 0 on success, a negative I2C_ERROR_xxx code on failure.
 */
static int ra_i2c_wait_complete(i2c_t *obj, int length)
{
    uint32_t timeout = RA_I2C_BYTE_TIMEOUT_US * (uint32_t)(length + 1);

    while (!obj->i2c.xfer_done) {
        if (--timeout == 0) {
            /* Timed out.  Reset the peripheral so that the bus has a chance to recover. */
            obj->i2c.p_api->abort(obj->i2c.p_ctrl);
            return I2C_ERROR_BUS_BUSY;
        }
        wait_us(1);
    }

    /* Most aborts are caused by a NACK of the address (missing device) */
    return obj->i2c.xfer_aborted ? I2C_ERROR_NO_SLAVE : 0;
}

/**
 * Check that the HAL state machine (managed by the I2C C++ class, see i2c_api.h) allows
 * starting a new transaction from the given state.
 */
static bool ra_i2c_state_allows_transaction(const i2c_t *obj)
{
    return obj->state == MBED_HAL_I2C_STATE_IDLE
        || obj->state == MBED_HAL_I2C_STATE_HOLDING_BUS
        || obj->state == MBED_HAL_I2C_STATE_BYTE_READ
        || obj->state == MBED_HAL_I2C_STATE_BYTE_WRITE;
}

/* --------------------------------------------------------------------------
 *  Init / Free
 * -------------------------------------------------------------------------- */

static void ra_i2c_setup_instance(i2c_t *obj, I2CName channel)
{
    const i2c_master_instance_t *inst = ra_i2c_instance_from_channel(channel);
    MBED_ASSERT(inst != NULL);

    obj->i2c.p_ctrl = (i2c_instance_ctrl_t *) inst->p_ctrl;
    obj->i2c.p_api = inst->p_api;

    const i2c_master_cfg_t *cfg_src = inst->p_cfg;
    const i2c_extended_cfg_t *ext_src = (const i2c_extended_cfg_t *) cfg_src->p_extend;

    obj->i2c.cfg = *cfg_src;
    obj->i2c.cfg.p_context = (void *) obj;
    if (ext_src) {
        obj->i2c.ext = *ext_src;
        obj->i2c.cfg.p_extend = &obj->i2c.ext;
    } else {
        obj->i2c.cfg.p_extend = NULL;
    }

    obj->i2c.hz = ra_i2c_rate_to_hz(obj->i2c.cfg.rate);
    obj->i2c.last_address = 0;
    obj->i2c.tx_active = false;
    obj->i2c.tx_len = 0;

    fsp_err_t err = obj->i2c.p_api->open(obj->i2c.p_ctrl, &obj->i2c.cfg);
    if (FSP_SUCCESS != err) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_I2C, MBED_ERROR_CODE_INITIALIZATION_FAILED), "i2c_init");
    }
}

void i2c_init_direct(i2c_t *obj, const i2c_pinmap_t *pinmap)
{
    MBED_ASSERT(obj != NULL);
    MBED_ASSERT(pinmap != NULL);

    obj->i2c.i2c = (I2CName) pinmap->peripheral;

    ra_i2c_configure_pins(pinmap->sda_pin, pinmap->sda_function,
                          pinmap->scl_pin, pinmap->scl_function);

    ra_i2c_setup_instance(obj, obj->i2c.i2c);
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    MBED_ASSERT(obj != NULL);

    int sda_periph = pinmap_peripheral(sda, PinMap_I2C_SDA);
    int scl_periph = pinmap_peripheral(scl, PinMap_I2C_SCL);
    int i2c_periph = pinmap_merge(sda_periph, scl_periph);

    if (i2c_periph == (int)NC) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_I2C, MBED_ERROR_CODE_INVALID_ARGUMENT), "i2c_init");
    }

    obj->i2c.i2c = (I2CName)i2c_periph;

    ra_i2c_configure_pins_from_pinmap(sda, scl);

    ra_i2c_setup_instance(obj, obj->i2c.i2c);
}

void i2c_free(i2c_t *obj)
{
    if (!obj) {
        return;
    }

    if (obj->i2c.p_ctrl) {
        (void) obj->i2c.p_api->close(obj->i2c.p_ctrl);
    }
}

/* --------------------------------------------------------------------------
 *  Frequency
 * -------------------------------------------------------------------------- */

static bool set_iic_master_clock_settings(int hz, iic_master_clock_settings_t *clock_settings)
{
    for (size_t i = 0; i < sizeof(g_iic_clock_table)/sizeof(g_iic_clock_table[0]); i++) {
        if (g_iic_clock_table[i].hz == hz) {
            *clock_settings = g_iic_clock_table[i].clk;
            return true;
        }
    }
    return false;
}

void i2c_frequency(i2c_t *obj, int hz)
{
    MBED_ASSERT(obj != NULL);

    if (!set_iic_master_clock_settings(hz, &obj->i2c.ext.clock_settings)) {
        /* The requested frequency is not available in the FSP-generated clock table */
        MBED_ASSERT(false);
        return;
    }

    if (hz <= 100000) {
        obj->i2c.cfg.rate = I2C_MASTER_RATE_STANDARD;
    } else if (hz <= 400000) {
        obj->i2c.cfg.rate = I2C_MASTER_RATE_FAST;
    } else {
        obj->i2c.cfg.rate = I2C_MASTER_RATE_FASTPLUS;
    }

    obj->i2c.hz = hz;

    obj->i2c.p_api->close(obj->i2c.p_ctrl);
    fsp_err_t err = obj->i2c.p_api->open(obj->i2c.p_ctrl, &obj->i2c.cfg);
    if (FSP_SUCCESS != err) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_I2C, MBED_ERROR_CODE_INITIALIZATION_FAILED), "i2c_frequency");
    }
}

/* --------------------------------------------------------------------------
 *  Blocking read / write
 * -------------------------------------------------------------------------- */

/**
 * Send out the bytes accumulated by i2c_byte_write() as a single FSP transfer.
 *
 * Note about the restart parameter: it controls how the *flush transfer itself* ends
 * (holding the bus with a repeated start vs. releasing it with a stop).  Whether the next
 * transfer is issued as a repeated start is decided automatically by the FSP driver based
 * on whether the bus is still held.
 */
static int i2c_flush_tx_buf(i2c_t *obj, bool restart)
{
    if (!obj->i2c.tx_active || obj->i2c.tx_len == 0) {
        return 0;
    }

    int addr8 = obj->i2c.tx_buf[0] & 0xFE;
    ra_i2c_set_address(obj, addr8, false);
    obj->i2c.last_address = addr8;

    obj->i2c.xfer_done = false;
    obj->i2c.xfer_aborted = false;

    fsp_err_t err = obj->i2c.p_api->write(
        obj->i2c.p_ctrl,
        &obj->i2c.tx_buf[1],
        obj->i2c.tx_len - 1,
        restart
    );

    if (err != FSP_SUCCESS) {
        obj->i2c.tx_active = false;
        obj->i2c.tx_len = 0;
        return I2C_ERROR_OTHER;
    }

    int result = ra_i2c_wait_complete(obj, obj->i2c.tx_len);

    obj->i2c.tx_active = false;
    obj->i2c.tx_len = 0;

    return result;
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    MBED_ASSERT(obj != NULL);

    if (!ra_i2c_state_allows_transaction(obj)) {
        return I2C_ERROR_INVALID_USAGE;
    }

    if (length < 0) {
        return I2C_ERROR_INVALID_USAGE;
    }

    static uint8_t dummy; /* FSP requires a non-NULL buffer even for zero-length transfers */
    if (length == 0) {
        data = (char *)&dummy;
    } else if (!data) {
        return I2C_ERROR_INVALID_USAGE;
    }

    /* flush pending bytes from the single-byte API, keeping the bus */
    int result = i2c_flush_tx_buf(obj, true);
    if (result != 0) {
        return result;
    }

    obj->i2c.xfer_done    = false;
    obj->i2c.xfer_aborted = false;

    ra_i2c_set_address(obj, address, false);
    obj->i2c.last_address = address & 0xFE;

    fsp_err_t err = obj->i2c.p_api->read(
        obj->i2c.p_ctrl,
        (uint8_t *) data,
        (uint32_t) length,
        stop == 0
    );
    if (FSP_SUCCESS != err) {
        return I2C_ERROR_OTHER;
    }

    result = ra_i2c_wait_complete(obj, length);
    if (result != 0) {
        return result;
    }

    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    MBED_ASSERT(obj != NULL);

    if (!ra_i2c_state_allows_transaction(obj)) {
        return I2C_ERROR_INVALID_USAGE;
    }

    if (length < 0) {
        return I2C_ERROR_INVALID_USAGE;
    }

    static uint8_t dummy; /* FSP requires a non-NULL buffer even for zero-length transfers */
    if (length == 0) {
        data = (const char *)&dummy;
    } else if (!data) {
        return I2C_ERROR_INVALID_USAGE;
    }

    /* flush pending bytes from the single-byte API, keeping the bus */
    int result = i2c_flush_tx_buf(obj, true);
    if (result != 0) {
        return result;
    }

    obj->i2c.xfer_done    = false;
    obj->i2c.xfer_aborted = false;

    ra_i2c_set_address(obj, address, false);
    obj->i2c.last_address = address & 0xFE;

    /* A zero-length write is an address-only probe of the device (START + address + STOP),
     * which the FSP driver supports by passing 0 as the transfer length. */
    fsp_err_t err = obj->i2c.p_api->write(
        obj->i2c.p_ctrl,
        (uint8_t *) data,
        (uint32_t) length,
        stop == 0
    );
    if (FSP_SUCCESS != err) {
        return I2C_ERROR_OTHER;
    }

    result = ra_i2c_wait_complete(obj, length);
    if (result != 0) {
        return result;
    }

    return length;
}

/* --------------------------------------------------------------------------
 *  Byte read / write (single-byte API)
 *
 * The FSP driver can only do complete transfers, so the single-byte API is emulated in
 * software: address and data bytes passed to i2c_byte_write() are buffered and only sent
 * when the sequence is finished, i.e. when a transaction (i2c_read/i2c_write), another
 * single-byte read, or i2c_stop() flushes the buffer.  Consequently, both the start
 * condition and the address of a single-byte transaction are delayed until that point.
 * -------------------------------------------------------------------------- */

int i2c_byte_read(i2c_t *obj, int last)
{
    if (obj->state != MBED_HAL_I2C_STATE_ADDRESSED
        && obj->state != MBED_HAL_I2C_STATE_BYTE_READ) {
        return -1;
    }

    uint8_t b = 0;

    int ret = i2c_read(obj, obj->i2c.last_address | 1, (char *)&b, 1, last);
    if (ret != 1) {
        return -1;
    }
    return b;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    if (obj->state != MBED_HAL_I2C_STATE_STARTED
        && obj->state != MBED_HAL_I2C_STATE_ADDRESSED
        && obj->state != MBED_HAL_I2C_STATE_BYTE_WRITE) {
        return 3;
    }

    if (!obj->i2c.tx_active) {
        return 3;
    }

    if (obj->i2c.tx_len >= sizeof(obj->i2c.tx_buf)) {
        return 3;
    }

    obj->i2c.tx_buf[obj->i2c.tx_len++] = (uint8_t)data;
    return 1;
}

/* --------------------------------------------------------------------------
 *  Start / Stop / Reset
 * -------------------------------------------------------------------------- */

int i2c_start(i2c_t *obj)
{
    /* If a single-byte transaction is in progress, finish it now (keeping the bus with
     * a repeated start) so that the buffer can be reused for the address of the new
     * transaction.  With only the address byte buffered, this sends the zero-length
     * transaction (START + address) that the old sequence asked for. */
    if (obj->i2c.tx_active && obj->i2c.tx_len > 0) {
        int result = i2c_flush_tx_buf(obj, true);
        if (result != 0) {
            return result;
        }
    }

    obj->i2c.tx_active = true;
    obj->i2c.tx_len = 0;
    return 0;
}

int i2c_stop(i2c_t *obj)
{
    if (obj->state != MBED_HAL_I2C_STATE_ADDRESSED
        && obj->state != MBED_HAL_I2C_STATE_BYTE_READ
        && obj->state != MBED_HAL_I2C_STATE_BYTE_WRITE) {
        return I2C_ERROR_INVALID_USAGE;
    }

    /* Flush any pending bytes, ending the transfer with a STOP condition.
     * With only the address byte buffered this sends a zero-length transfer
     * (START + address + STOP). */
    int result = i2c_flush_tx_buf(obj, false);

    obj->i2c.tx_active = false;
    obj->i2c.tx_len = 0;

    return result;
}

void i2c_reset(i2c_t *obj)
{
    obj->i2c.p_api->abort(obj->i2c.p_ctrl);

    obj->i2c.tx_active = false;
    obj->i2c.tx_len = 0;
}

/* --------------------------------------------------------------------------
 *  PinMap getters
 * -------------------------------------------------------------------------- */

const PinMap *i2c_master_sda_pinmap(void)
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_master_scl_pinmap(void)
{
    return PinMap_I2C_SCL;
}

const PinMap *i2c_slave_sda_pinmap(void)
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_slave_scl_pinmap(void)
{
    return PinMap_I2C_SCL;
}

/* --------------------------------------------------------------------------
 *  Capabilities
 * -------------------------------------------------------------------------- */

static const i2c_capabilities_t i2c_caps = {
    /* The start condition and the address of a single-byte transaction are only sent
     * when the byte buffer is flushed, i.e. together with (before) the first data byte. */
    .single_byte_address_delayed = true,
    .single_byte_start_cond_delayed = true,

    .supports_single_byte = true,

    /* start(); write_byte(address); stop() is sent as START + address + STOP
     * by flushing the single-address-byte buffer as a zero-length transfer. */
    .supports_zero_length_transfer_single_byte = true,

    /* Zero-length transactions are done with zero-length FSP transfers. */
    .supports_zero_length_transfer_transaction = true,
};

MBED_WEAK i2c_capabilities_t const *i2c_get_capabilities()
{
    return &i2c_caps;
}

/* ---------------- I2C callback ---------------- */

void i2c_callback(i2c_master_callback_args_t *p_args)
{
    i2c_t *obj = (i2c_t *) p_args->p_context;

    switch (p_args->event) {
        case I2C_MASTER_EVENT_TX_COMPLETE:
        case I2C_MASTER_EVENT_RX_COMPLETE:
            obj->i2c.xfer_done    = true;
            obj->i2c.xfer_aborted = false;
            break;

        case I2C_MASTER_EVENT_ABORTED:
            obj->i2c.xfer_done    = true;
            obj->i2c.xfer_aborted = true;
            break;

        default:
            break;
    }
}

#endif
