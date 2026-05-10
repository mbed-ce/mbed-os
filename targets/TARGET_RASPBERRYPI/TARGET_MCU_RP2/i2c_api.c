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

#if DEVICE_I2C

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include "mbed_assert.h"
#include "i2c_api.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "objects.h"
#include "stdio.h"

/******************************************************************************
 * DEFINE
 ******************************************************************************/

#if 0
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif

#define NoData         0 // the slave has not been addressed
#define ReadAddressed  1 // the master has requested a read from this slave (slave = transmitter)
#define WriteGeneral   2 // the master is writing to all slave
#define WriteAddressed 3 // the master is writing to this slave (slave = receiver)

/******************************************************************************
 * CONST
 ******************************************************************************/

static unsigned int const DEFAULT_I2C_BAUDRATE = 100 * 1000; /* 100 kHz */

/******************************************************************************
 * FUNCTION DEFINITION
 ******************************************************************************/

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    /* Verify if both pins belong to the same I2C peripheral. */
    uint32_t const i2c_sda = pinmap_peripheral(sda, PinMap_I2C_SDA);
    uint32_t const i2c_scl = pinmap_peripheral(scl, PinMap_I2C_SCL);
    const uint32_t i2c_peripheral = pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT(i2c_peripheral != ((uint32_t)NC));

#if DEVICE_I2CSLAVE
    /** was_slave is used to decide which driver call we need
     * to use when uninitializing a given instance
     */
    obj->i2c.was_slave = false;
    obj->i2c.is_slave = false;
    obj->i2c.slave_addr = 0;
#endif

    /* Obtain the pointer to the I2C hardware instance. */
    obj->i2c.dev = (i2c_inst_t *)pinmap_function(sda, PinMap_I2C_SDA);
    //obj->i2c.baudrate = DEFAULT_I2C_BAUDRATE;
    //Call this function because if we are configuring a slave, we don't have to set the frequency
    //i2c_frequency(obj->i2c.dev, DEFAULT_I2C_BAUDRATE);

    /* Initialize the I2C module. */
    pico_sdk_i2c_init(obj->i2c.dev, DEFAULT_I2C_BAUDRATE);

    /* Configure GPIO for I2C as alternate function. */
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);

    /* Enable pull-ups for I2C pins. */
    gpio_pull_up(sda);
    gpio_pull_up(scl);
}

void i2c_free(i2c_t *obj) {
    i2c_deinit(obj->i2c.dev);
}

void i2c_frequency(i2c_t *obj, int hz)
{
    DEBUG_PRINTF("obj->i2c.is_slave: %d\r\n", obj->i2c.is_slave);

#if DEVICE_I2CSLAVE
    /* Slaves automatically get frequency from master */
    if(obj->i2c.is_slave) {
    		return;
    }
#endif
    obj->i2c.baudrate = i2c_set_baudrate(obj->i2c.dev, hz);
}

int i2c_start(i2c_t *obj) {
    (void)obj;
    // No-op, we need to wait until we get the address to start
    return 0;
}

int i2c_byte_write(i2c_t *obj, int data) {
    int ret = 1;

    if(obj->state == MBED_HAL_I2C_STATE_STARTED) {
        // First byte (address). Save for now in TAR register (won't actually get sent until later)
        obj->i2c.dev->hw->enable = 0;
        obj->i2c.dev->hw->tar = data >> 1;
        obj->i2c.dev->hw->enable = 1;
    }
    else {
        // Write byte. Force transmission of a start condition if this is the first data byte.
        obj->i2c.dev->hw->data_cmd = data | (obj->state == MBED_HAL_I2C_STATE_ADDRESSED ? I2C_IC_DATA_CMD_RESTART_BITS : 0);

        // Wait until the transmission of the address/data from the internal
        // shift register has completed. For this to function correctly, the
        // TX_EMPTY_CTRL flag in IC_CON must be set. The TX_EMPTY_CTRL flag
        // was set in pico_sdk_i2c_init.
        do {
            tight_loop_contents();
        }
        while (!(obj->i2c.dev->hw->raw_intr_stat & I2C_IC_RAW_INTR_STAT_TX_EMPTY_BITS));

        // Did we encounter an error?
        if(obj->i2c.dev->hw->raw_intr_stat & I2C_IC_RAW_INTR_STAT_TX_ABRT_BITS) {
            // Get the abort reason
            uint32_t abort_reason = obj->i2c.dev->hw->tx_abrt_source;
            if(abort_reason & (I2C_IC_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_BITS | I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_BITS | I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_BITS | I2C_IC_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_BITS)) {
                ret = 0; // no ACK
            }
            else {
                ret = 3; // other error
            }

            // Read register to clear abort
            obj->i2c.dev->hw->clr_tx_abrt;
        }
    }

    return ret;
}

int i2c_byte_read(i2c_t *obj, int last) {

    // This I2C peripheral waits to generate the ACK/NACK after a read until either we try to read the
    // next byte or we generate a stop/repeated start. This means it automatically uses the correct
    // type of ACK or NACK and doesn't need (and can't accept) a flag on whether to do so.
    (void)last;

    // Trigger reading of a byte. Force generation of a start condition if this is the first byte.
    obj->i2c.dev->hw->data_cmd = I2C_IC_DATA_CMD_CMD_BITS |
        (obj->state == MBED_HAL_I2C_STATE_ADDRESSED ? I2C_IC_DATA_CMD_RESTART_BITS : 0);

    // Wait until the transmission of the address/data from the internal
    // shift register has completed. For this to function correctly, the
    // TX_EMPTY_CTRL flag in IC_CON must be set. The TX_EMPTY_CTRL flag
    // was set in pico_sdk_i2c_init.
    do {
        tight_loop_contents();
    }
    while (!(obj->i2c.dev->hw->raw_intr_stat & I2C_IC_RAW_INTR_STAT_TX_ABRT_BITS) && !i2c_get_read_available(obj->i2c.dev));

    // Did we encounter an error?
    if(obj->i2c.dev->hw->raw_intr_stat & I2C_IC_RAW_INTR_STAT_TX_ABRT_BITS) {
        // Sadly no way to pass up the abort reason so no reason to read tx_abrt_source
        // Also don't clear the abort yet!

        return 0;
    }

    // Return data
    return (uint8_t) obj->i2c.dev->hw->data_cmd;
}

int i2c_stop(i2c_t *obj)
{
    // If we didn't already generate a stop due to an error earlier in the transaction...
    if(!(obj->i2c.dev->hw->raw_intr_stat & I2C_IC_RAW_INTR_STAT_TX_ABRT_BITS)) {
        // ...then generate one now.
        obj->i2c.dev->hw->enable |= I2C_IC_ENABLE_ABORT_BITS;
        do {
            tight_loop_contents();
        }
        while (!(obj->i2c.dev->hw->raw_intr_stat & I2C_IC_RAW_INTR_STAT_TX_ABRT_BITS));
    }

    // Clear stop detection and abort reason flags (clear on read registers, ick!).
    // If we don't clear these the SDK read/write functions will get stuck.
    obj->i2c.dev->hw->clr_tx_abrt;
    obj->i2c.dev->hw->clr_stop_det;

    return 0;
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    // Make sure a repeated start is correctly generated if we currently have the bus locked
    obj->i2c.dev->restart_on_next = true;

    // If we were previously doing a single byte transaction which aborted for any reason,
    // ensure we clear that abort so that the Pico SDK function doesn't get stuck
    if(obj->i2c.dev->hw->raw_intr_stat & I2C_IC_RAW_INTR_STAT_TX_ABRT_BITS) {
        i2c_stop(obj);
    }

    int const bytes_read = i2c_read_blocking(obj->i2c.dev,
                                             (uint8_t)(address >> 1),
                                             (uint8_t *)data,
                                             (size_t)length,
                                             /* nostop = */(stop == 0));
    if (bytes_read < 0)
        return I2C_ERROR_NO_SLAVE;
    else
        return bytes_read;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    // Make sure a repeated start is correctly generated if we currently have the bus locked
    obj->i2c.dev->restart_on_next = true;

    // If we were previously doing a single byte transaction which aborted for any reason,
    // ensure we clear that abort so that the Pico SDK function doesn't get stuck
    if(obj->i2c.dev->hw->raw_intr_stat & I2C_IC_RAW_INTR_STAT_TX_ABRT_BITS) {
        i2c_stop(obj);
    }

    int const bytes_written = i2c_write_blocking(obj->i2c.dev,
                                                 address >> 1,
                                                 (const uint8_t *)data,
                                                 (size_t)length,
                                                 /* nostop = */(stop == 0));
    if (bytes_written < 0)
        return I2C_ERROR_NO_SLAVE;
    else
        return bytes_written;
}

void i2c_reset(i2c_t *obj)
{
    i2c_deinit(obj->i2c.dev);
    pico_sdk_i2c_init(obj->i2c.dev, obj->i2c.baudrate);
}

const PinMap *i2c_master_sda_pinmap()
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_master_scl_pinmap()
{
    return PinMap_I2C_SCL;
}

const PinMap *i2c_slave_sda_pinmap()
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_slave_scl_pinmap()
{
    return PinMap_I2C_SCL;
}

// Report I2C capabilities
static const i2c_capabilities_t i2c_caps = {
    .single_byte_address_delayed = true,
    .single_byte_start_cond_delayed = true,
    .supports_single_byte = true,
    .supports_zero_length_transfer_single_byte = false,
    .supports_zero_length_transfer_transaction = false
};
MBED_WEAK i2c_capabilities_t const * i2c_get_capabilities()
{
    return &i2c_caps;
}

#if DEVICE_I2CSLAVE

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param enable_slave Enable i2c hardware so you can receive events with ::i2c_slave_receive
 *  @return non-zero if a value is available
 */
void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    DEBUG_PRINTF("i2c_slave_mode: %p, %d\r\n", obj, enable_slave);

    obj->i2c.is_slave = enable_slave;
}

/** Check to see if the I2C slave has been addressed.
 *  @param obj The I2C object
 *  @return The status - 1 - read addressed, 2 - write to all slaves,
 *         3 write addressed, 0 - the slave has not been addressed
 */
int i2c_slave_receive(i2c_t *obj)
{
    int retValue = NoData;

    int rd_req = (obj->i2c.dev->hw->raw_intr_stat & I2C_IC_RAW_INTR_STAT_RD_REQ_BITS) >> 5;

    if (rd_req == I2C_IC_RAW_INTR_STAT_RD_REQ_VALUE_ACTIVE) {
        DEBUG_PRINTF("Read addressed\r\n");
        return ReadAddressed;
    }

    int wr_req = (obj->i2c.dev->hw->status & I2C_IC_STATUS_RFNE_BITS) >> 3;

    if (wr_req == I2C_IC_STATUS_RFNE_VALUE_NOT_EMPTY) {
        DEBUG_PRINTF("Write addressed\r\n");
        return WriteAddressed;
    }

    return (retValue);
}

/** Configure I2C as slave or master.
 *  @param obj The I2C objecti2c_get_read_availableread
 *  @return non-zero if a value is available
 */
int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    int bytes_read = 0;
    for (size_t i = 0; i < (size_t)length; ++i) {
        while (!i2c_get_read_available(obj->i2c.dev)) {
            tight_loop_contents();
        }

        *data = obj->i2c.dev->hw->data_cmd;
        bytes_read++;

        // Check stop condition
        bool stop = (obj->i2c.dev->hw->raw_intr_stat & I2C_IC_RAW_INTR_STAT_STOP_DET_BITS) != 0;
        if (stop && !i2c_get_read_available(obj->i2c.dev)) {
            // Clear stop (by reading the register)
            int clear_stop = obj->i2c.dev->hw->clr_stop_det;
            (void)clear_stop;
            break;
        } else {
            data++;
        }
    }

    return bytes_read;
}

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to write
 *  @return non-zero if a value is available
 */
int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    DEBUG_PRINTF("i2c_slave_write\r\n");

    i2c_write_raw_blocking(obj->i2c.dev, (const uint8_t *)data, (size_t)length);

    // Clear interrupt (by reading the register)
    int clear_read_req = i2c_get_hw(obj->i2c.dev)->clr_rd_req;
    (void)clear_read_req;
    DEBUG_PRINTF("clear_read_req: %d\n", clear_read_req);

    return length;
}

/** Configure I2C address.
 *  @param obj     The I2C object
 *  @param idx     Currently not used
 *  @param address The address to be set
 *  @param mask    Currently not used
 */
void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    if (obj->i2c.is_slave) {
        DEBUG_PRINTF("i2c_slave_address: %p, %d, %d, %d\r\n", obj, idx, address, mask);

        obj->i2c.slave_addr = (uint8_t)(address >> 1);
        i2c_set_slave_mode(obj->i2c.dev, true, obj->i2c.slave_addr);
    }
}

#endif // DEVICE_I2CSLAVE

#endif