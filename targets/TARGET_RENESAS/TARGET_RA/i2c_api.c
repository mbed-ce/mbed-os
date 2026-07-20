/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#if DEVICE_I2C

#include "mbed_assert.h"
#include "mbed_error.h"
#include "i2c_api.h"
#include "pinmap.h"

#include "hal_data.h"
#include "r_ioport.h"

#include "i2c_device.h"

#define I2C_ERROR -1

extern const PinMap PinMap_I2C_SCL[];
extern const PinMap PinMap_I2C_SDA[];

extern const i2c_master_instance_t g_i2c_master0;
#if (BSP_FEATURE_IIC_VALID_CHANNEL_MASK & 2) != 0
extern const i2c_master_instance_t g_i2c_master1;
#endif
extern const ioport_instance_t g_ioport;

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
    if(!obj->i2c.p_ctrl->restart)
    {
        uint16_t addr = (uint16_t)(address >> 1);

        fsp_err_t err = R_IIC_MASTER_SlaveAddressSet(
            obj->i2c.p_ctrl,
            addr,
            ten_bit ? I2C_MASTER_ADDR_MODE_10BIT : I2C_MASTER_ADDR_MODE_7BIT
        );

        if (FSP_SUCCESS != err) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_I2C, MBED_ERROR_CODE_INVALID_ARGUMENT), "i2c_set_address");
        }
    }
}

/* --------------------------------------------------------------------------
 *  Init / Free
 * -------------------------------------------------------------------------- */

void i2c_init_direct(i2c_t *obj, const i2c_pinmap_t *pinmap)
{
    MBED_ASSERT(obj != NULL);
    MBED_ASSERT(pinmap != NULL);

    obj->i2c.pending_restart = false;
    obj->i2c.tx_active = false;
    obj->i2c.tx_len = 0;

    obj->i2c.i2c = (I2CName) pinmap->peripheral;

    ra_i2c_configure_pins(pinmap->sda_pin, pinmap->sda_function,
                          pinmap->scl_pin, pinmap->scl_function);

    const i2c_master_instance_t *inst = ra_i2c_instance_from_channel(obj->i2c.i2c);
    MBED_ASSERT(inst != NULL);

    obj->i2c.p_ctrl = (i2c_instance_ctrl_t *) inst->p_ctrl;

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

    fsp_err_t err = R_IIC_MASTER_Open(obj->i2c.p_ctrl, &obj->i2c.cfg);
    if (FSP_SUCCESS != err) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_I2C, MBED_ERROR_CODE_INITIALIZATION_FAILED), "i2c_init_direct");
    }
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    MBED_ASSERT(obj != NULL);

    obj->i2c.pending_restart = false;
    obj->i2c.tx_active = false;
    obj->i2c.tx_len = 0;

    int sda_periph = pinmap_peripheral(sda, PinMap_I2C_SDA);
    int scl_periph = pinmap_peripheral(scl, PinMap_I2C_SCL);
    int i2c_periph = pinmap_merge(sda_periph, scl_periph);

    if (i2c_periph == (int)NC) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_I2C, MBED_ERROR_CODE_INVALID_ARGUMENT), "i2c_init");
    }

    obj->i2c.i2c = (I2CName)i2c_periph;

    ra_i2c_configure_pins_from_pinmap(sda, scl);

    const i2c_master_instance_t *inst = ra_i2c_instance_from_channel(obj->i2c.i2c);
    MBED_ASSERT(inst != NULL);

    obj->i2c.p_ctrl = (i2c_instance_ctrl_t *) inst->p_ctrl;

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

    fsp_err_t err = R_IIC_MASTER_Open(obj->i2c.p_ctrl, &obj->i2c.cfg);
    if (FSP_SUCCESS != err) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_I2C, MBED_ERROR_CODE_INITIALIZATION_FAILED), "i2c_init");
    }
}

void i2c_free(i2c_t *obj)
{
    if (!obj) {
        return;
    }

    if (obj->i2c.p_ctrl) {
        (void) R_IIC_MASTER_Close(obj->i2c.p_ctrl);
    }
}

/* --------------------------------------------------------------------------
 *  Frequency
 * -------------------------------------------------------------------------- */

bool set_iic_master_clock_settings(int hz, iic_master_clock_settings_t *clock_settings)
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
        return;
    }

    if (hz <= 100000) {
        obj->i2c.cfg.rate = I2C_MASTER_RATE_STANDARD;
    } else if (hz <= 400000) {
        obj->i2c.cfg.rate = I2C_MASTER_RATE_FAST;
    } else {
#ifdef I2C_MASTER_RATE_FASTPLUS
        obj->cfg.rate = I2C_MASTER_RATE_FASTPLUS;
#endif
    }

    obj->i2c.hz = hz;

    R_IIC_MASTER_Close(obj->i2c.p_ctrl);
    fsp_err_t err = R_IIC_MASTER_Open(obj->i2c.p_ctrl, &obj->i2c.cfg);
    if (FSP_SUCCESS != err) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_I2C, MBED_ERROR_CODE_INITIALIZATION_FAILED), "i2c_frequency");
    }
}

/* --------------------------------------------------------------------------
 *  Blocking read / write
 * -------------------------------------------------------------------------- */

static int i2c_flush_tx_buf(i2c_t *obj, bool restart)
{
    if (!obj->i2c.tx_active || obj->i2c.tx_len == 0) {
        return 0;
    }

    int addr8 = obj->i2c.tx_buf[0] & 0xFE;
    ra_i2c_set_address(obj, addr8, false);

    obj->i2c.xfer_done = false;
    obj->i2c.xfer_aborted = false;

    fsp_err_t err = R_IIC_MASTER_Write(
        obj->i2c.p_ctrl,
        &obj->i2c.tx_buf[1],
        obj->i2c.tx_len - 1,
        restart
    );

    if (err != FSP_SUCCESS) {
        return I2C_ERROR;
    }

    while (!obj->i2c.xfer_done) {}
    if (obj->i2c.xfer_aborted) {
        return I2C_ERROR;
    }

    obj->i2c.tx_active = false;
    obj->i2c.tx_len = 0;

    return 0;
}


int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    MBED_ASSERT(obj != NULL);

    /* flush pending address bytes */
    if (i2c_flush_tx_buf(obj, true) != 0) {
        return I2C_ERROR;
    }

    if (!data || length <= 0) {
        return 0;
    }

    obj->i2c.xfer_done    = false;
    obj->i2c.xfer_aborted = false;

    ra_i2c_set_address(obj, address, false);

    bool restart = obj->i2c.pending_restart;

    fsp_err_t err = R_IIC_MASTER_Read(
        obj->i2c.p_ctrl,
        (uint8_t *) data,
        (uint32_t) length,
        restart
    );
    if (FSP_SUCCESS != err) {
        obj->i2c.pending_restart = false;
        return I2C_ERROR;
    }

    while (!obj->i2c.xfer_done) {}

    if (obj->i2c.xfer_aborted) {
        obj->i2c.pending_restart = false;
        return I2C_ERROR;
    }

    obj->i2c.pending_restart = (stop == 0);

    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    MBED_ASSERT(obj != NULL);

    /* flush pending address bytes */
    if (i2c_flush_tx_buf(obj, true) != 0) {
        return I2C_ERROR;
    }

    if (!data || length <= 0) {
        /* Zero-length write = send only address */
        obj->i2c.xfer_done = false;
        obj->i2c.xfer_aborted = false;

        ra_i2c_set_address(obj, address, false);

        uint8_t dummy = 0;

        fsp_err_t err = R_IIC_MASTER_Write(
            obj->i2c.p_ctrl,
            &dummy,
            1,
            false
        );

        if (err != FSP_SUCCESS) return I2C_ERROR;
        while (!obj->i2c.xfer_done) {}
        return obj->i2c.xfer_aborted ? I2C_ERROR : 0;
    }

    obj->i2c.xfer_done    = false;
    obj->i2c.xfer_aborted = false;

    ra_i2c_set_address(obj, address, false);

    bool restart = obj->i2c.pending_restart;

    fsp_err_t err = R_IIC_MASTER_Write(
        obj->i2c.p_ctrl,
        (uint8_t *) data,
        (uint32_t) length,
        restart
    );
    if (FSP_SUCCESS != err) {
        obj->i2c.pending_restart = false;
        return I2C_ERROR;
    }

    while (!obj->i2c.xfer_done) {}

    if (obj->i2c.xfer_aborted) {
        obj->i2c.pending_restart = false;
        return I2C_ERROR;
    }

    obj->i2c.pending_restart = (stop == 0);

    return length;
}

/* --------------------------------------------------------------------------
 *  Byte read / write
 * -------------------------------------------------------------------------- */

int i2c_byte_read(i2c_t *obj, int last)
{
    uint8_t b = 0;

    int ret = i2c_read(obj, obj->i2c.last_address | 1, (char *)&b, 1, last);
    if (ret != 1) {
        return I2C_ERROR;
    }
    return b;
}

int i2c_byte_write(i2c_t *obj, int data)
{
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
    obj->i2c.pending_restart = true;
    obj->i2c.tx_active = true;
    obj->i2c.tx_len = 0;
    return 0;
}

int i2c_stop(i2c_t *obj)
{
    /* flush pending bytes with restart = false */
    i2c_flush_tx_buf(obj, false);

    obj->i2c.tx_active = false;
    obj->i2c.tx_len = 0;
    obj->i2c.pending_restart = false;

    return 0;
}

void i2c_reset(i2c_t *obj)
{
    (void) i2c_stop(obj);
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
