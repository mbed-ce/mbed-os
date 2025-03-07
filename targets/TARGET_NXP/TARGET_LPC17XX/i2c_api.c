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
#include "i2c_api.h"

#if DEVICE_I2C

#include "cmsis.h"
#include "PeripheralPinMaps.h"

// Change to 1 to enable debug prints.
#define LPC1768_I2C_DEBUG 0

#if LPC1768_I2C_DEBUG
#include <stdio.h>
#include <inttypes.h>
#endif

#define I2C_CONSET(x)       (x->i2c->I2CONSET)
#define I2C_CONCLR(x)       (x->i2c->I2CONCLR)
#define I2C_STAT(x)         (x->i2c->I2STAT)
#define I2C_DAT(x)          (x->i2c->I2DAT)
#define I2C_SCLL(x, val)    (x->i2c->I2SCLL = val)
#define I2C_SCLH(x, val)    (x->i2c->I2SCLH = val)

static const uint32_t I2C_addr_offset[2][4] = {
    {0x0C, 0x20, 0x24, 0x28},
    {0x30, 0x34, 0x38, 0x3C}
};

static inline void i2c_conclr(i2c_t *obj, int start, int stop, int interrupt, int acknowledge) {
    I2C_CONCLR(obj) = (start << 5)
                    | (stop << 4)
                    | (interrupt << 3)
                    | (acknowledge << 2);
}

static inline void i2c_conset(i2c_t *obj, int start, int stop, int interrupt, int acknowledge) {
    I2C_CONSET(obj) = (start << 5)
                    | (stop << 4)
                    | (interrupt << 3)
                    | (acknowledge << 2);
}

// Clear the Serial Interrupt (SI)
static inline void i2c_clear_SI(i2c_t *obj) {
    i2c_conclr(obj, 0, 0, 1, 0);
}

static inline int i2c_status(i2c_t *obj) {
    return I2C_STAT(obj);
}

// Wait until the Serial Interrupt (SI) is set
static int i2c_wait_SI(i2c_t *obj) {
    int timeout = 0;
    while (!(I2C_CONSET(obj) & (1 << 3))) {
        timeout++;
        if (timeout > 100000) return -1;
    }
    return 0;
}

static inline void i2c_interface_disable(i2c_t *obj) {
    I2C_CONCLR(obj) = 0x40;
}

static inline void i2c_interface_enable(i2c_t *obj) {
    I2C_CONSET(obj) = 0x40;
}

static inline void i2c_power_enable(i2c_t *obj) {
    switch ((int)obj->i2c) {
        case I2C_0: LPC_SC->PCONP |= 1 << 7; break;
        case I2C_1: LPC_SC->PCONP |= 1 << 19; break;
        case I2C_2: LPC_SC->PCONP |= 1 << 26; break;
    }
}

void i2c_init_direct(i2c_t *obj, const i2c_pinmap_t *pinmap)
{
    obj->i2c = (LPC_I2C_TypeDef *)pinmap->peripheral;

    // enable power
    i2c_power_enable(obj);
    
    // set default frequency at 100k
    i2c_frequency(obj, 100000);

    // Reset the I2C peripheral by clearing all flags, including I2EN.
    // This does a software reset of sorts, which is important because the I2C::recover()
    // function, which is called before initializing the bus, seems to put the I2C 
    // peripheral in a weird state where the next transaction will fail.
    i2c_interface_disable(obj);
    i2c_conclr(obj, 1, 1, 1, 1);

    i2c_interface_enable(obj);
    
    // Map pins
    pin_function(pinmap->sda_pin, pinmap->sda_function);
    pin_mode(pinmap->sda_pin, OpenDrain);
    pin_function(pinmap->scl_pin, pinmap->scl_function);
    pin_mode(pinmap->scl_pin, OpenDrain);
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl) {

    i2c_pinmap_t pinmap;
    pinmap.sda_pin = sda;
    pinmap.scl_pin = scl;

    // Determine peripheral associated with these pins
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    pinmap.peripheral = pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT((int)pinmap.peripheral != NC);

    // Get pin functions
    pinmap.sda_function = pinmap_find_function(sda, PinMap_I2C_SDA);
    pinmap.scl_function = pinmap_find_function(scl, PinMap_I2C_SCL);

    i2c_init_direct(obj, &pinmap);
}


inline int i2c_start(i2c_t *obj) {
    int status = 0;
    int isInterrupted = I2C_CONSET(obj) & (1 << 3);

#if LPC1768_I2C_DEBUG
    printf("i2c_start(): status was originally 0x%x\n", i2c_status(obj));
#endif

    // 8.1 Before master mode can be entered, I2CON must be initialised to:
    //  - I2EN STA STO SI AA - -
    //  -  1    0   0   x  x - -
    // if AA = 0, it can't enter slave mode
    i2c_conclr(obj, 1, 1, 0, 1);

    // The master mode may now be entered by setting the STA bit
    // this will generate a start condition when the bus becomes free
    i2c_conset(obj, 1, 0, 0, 1);
    // Clearing SI bit when it wasn't set on entry can jump past state
    // 0x10 or 0x08 and erroneously send uninitialized slave address.
    if (isInterrupted)
        i2c_clear_SI(obj);

    i2c_wait_SI(obj);
    status = i2c_status(obj);

#if LPC1768_I2C_DEBUG
    printf("i2c_start(): status is now 0x%x\n", status);
#endif

    // Clear start bit now that it's transmitted
    i2c_conclr(obj, 1, 0, 0, 0);
    return status;
}

inline int i2c_stop(i2c_t *obj) {
    int timeout = 0;

    // write the stop bit
    i2c_conset(obj, 0, 1, 0, 0);
    i2c_clear_SI(obj);
    
    // wait for STO bit to reset
    while(I2C_CONSET(obj) & (1 << 4)) {
        timeout ++;
        if (timeout > 100000) return 1;
    }

    return 0;
}

static inline int i2c_do_write(i2c_t *obj, int value, uint8_t addr) {
    // write the data
    I2C_DAT(obj) = value;
    
    // clear SI to init a send
    i2c_clear_SI(obj);
    
    // wait and return status
    i2c_wait_SI(obj);
    return i2c_status(obj);
}

static inline int i2c_do_read(i2c_t *obj, int last) {
    // we are in state 0x40 (SLA+R tx'd) or 0x50 (data rx'd and ack)
    if(last) {
        i2c_conclr(obj, 0, 0, 0, 1); // send a NOT ACK
    } else {
        i2c_conset(obj, 0, 0, 0, 1); // send a ACK
    }
    
    // accept byte
    i2c_clear_SI(obj);
    
    // wait for it to arrive
    i2c_wait_SI(obj);
    
    // return the data
    return (I2C_DAT(obj) & 0xFF);
}

void i2c_frequency(i2c_t *obj, int hz) {
    // [TODO] set pclk to /4
    uint32_t PCLK = SystemCoreClock / 4;
    
    uint32_t pulse = PCLK / (hz * 2);
    
    // I2C Rate
    I2C_SCLL(obj, pulse);
    I2C_SCLH(obj, pulse);
}

// The I2C does a read or a write as a whole operation
// There are two types of error conditions it can encounter
//  1) it can not obtain the bus
//  2) it gets error responses at part of the transmission
//
// We tackle them as follows:
//  1) we retry until we get the bus. we could have a "timeout" if we can not get it
//      which basically turns it in to a 2)
//  2) on error, we use the standard error mechanisms to report/debug
//
// Therefore an I2C transaction should always complete. If it doesn't it is usually
// because something is setup wrong (e.g. wiring), and we don't need to programatically
// check for that

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop) {
    int count, status;
    
    status = i2c_start(obj);
    
    if ((status != 0x10) && (status != 0x08)) {
        i2c_stop(obj);
        return I2C_ERROR_BUS_BUSY;
    }
    
    status = i2c_do_write(obj, (address | 0x01), 1);
    if (status != 0x40) {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }
    
    // Read in all except last byte
    for (count = 0; count < (length - 1); count++) {
        int value = i2c_do_read(obj, 0);
        status = i2c_status(obj);
        if (status != 0x50) {
            i2c_stop(obj);
            return count;
        }
        data[count] = (char) value;
    }
    
    // read in last byte
    int value = i2c_do_read(obj, 1);
    status = i2c_status(obj);
    if (status != 0x58) {
        i2c_stop(obj);
        return length - 1;
    }
    
    data[count] = (char) value;
    
    // If not repeated start, send stop.
    if (stop) {
        i2c_stop(obj);
    }
    
    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop) {
    int i, status;
    
    status = i2c_start(obj);
    
    if ((status != 0x10) && (status != 0x08)) {
        i2c_stop(obj);
        return I2C_ERROR_BUS_BUSY;
    }
    
    status = i2c_do_write(obj, (address & 0xFE), 1);
    if (status != 0x18) {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }
    
    for (i=0; i<length; i++) {
        status = i2c_do_write(obj, data[i], 0);
        if(status != 0x28) {
            i2c_stop(obj);
            return i;
        }
    }
    
    // clearing the serial interrupt here might cause an unintended rewrite of the last byte
    // see also issue report https://mbed.org/users/mbed_official/code/mbed/issues/1
    // i2c_clear_SI(obj);
    
    // If not repeated start, send stop.
    if (stop) {
        i2c_stop(obj);
    }
    
    return length;
}

void i2c_reset(i2c_t *obj) {
    i2c_stop(obj);
}

int i2c_byte_read(i2c_t *obj, int last) {
    return (i2c_do_read(obj, last) & 0xFF);
}

int i2c_byte_write(i2c_t *obj, int data) {
    int ack;
    int status = i2c_do_write(obj, (data & 0xFF), 0);

#if LPC1768_I2C_DEBUG
    printf("i2c_do_write(0x%hhx) returned 0x%x\n", data & 0xFF, status);
#endif
    
    switch(status) {
        case 0x18: case 0x28:       // Master transmit ACKs
            ack = 1;
            break;
        case 0x40:                  // Master receive address transmitted ACK
            ack = 1;
            break;
        case 0xB8:                  // Slave transmit ACK
            ack = 1;
            break;
        default:
            ack = 0;
            break;
    }
    
    return ack;
}

void i2c_slave_mode(i2c_t *obj, int enable_slave) {
    if (enable_slave != 0) {
        i2c_conclr(obj, 1, 1, 1, 0);
        i2c_conset(obj, 0, 0, 0, 1);
    } else {
        i2c_conclr(obj, 1, 1, 1, 1);
    }
}

int i2c_slave_receive(i2c_t *obj) {
    int status;
    int retval;
    
    status = i2c_status(obj);
    switch(status) {
        case 0x60: retval = 3; break;
        case 0x70: retval = 2; break;
        case 0xA8: retval = 1; break;
        default  : retval = 0; break;
    }
    
    return(retval);
}

int i2c_slave_read(i2c_t *obj, char *data, int length) {

    int count = 0;

    if(i2c_status(obj) != 0x60 && i2c_status(obj) != 0x70) {
        return -1; // I2C peripheral not in setup-write-to-slave mode
    }

    if(i2c_status(obj) == 0x70) {
        // When addressed with the general call address, per the manual we can only receive a max of 1 byte.
        if(length > 1) {
            length = 1;
        }
    }

    i2c_conset(obj, 0, 0, 0, length > 0); // Set AA flag to acknowledge write as long as we have buffer space to store a byte in
    i2c_clear_SI(obj);

    // This is implemented as a state machine according to section 19.10.8 in the reference manual.
    while(true) {

        // Wait until the I2C peripheral has an event for us
        i2c_wait_SI(obj);

#if LPC1768_I2C_DEBUG
        printf("i2c_slave_read(): in state 0x%hhx\n", i2c_status(obj));
#endif

        switch(i2c_status(obj)) {
        case 0x68:
        case 0x78:
            // Arbitration Lost event.  I'm a bit confused about how this can happen as a slave device but the manual says it can...
            i2c_conclr(obj, 1, 0, 0, 1); // Clear start and ack
            i2c_clear_SI(obj);
            
            // Reset the I2C state machine. This doesn't actually send a STOP condition in slave mode.
            i2c_stop(obj);
            return -2; // Arbitration lost

        case 0x80:
        case 0x90:
            // Received another data byte from master.  ACK has been returned.
            data[count++] = I2C_DAT(obj) & 0xFF;
            
            if(count >= length) {
                // Out of buffer space, NACK the next byte
                i2c_conclr(obj, 0, 0, 0, 1);
                i2c_clear_SI(obj);
            }
            else {
                // Ack the next byte
                i2c_conset(obj, 0, 0, 0, 1);
                i2c_clear_SI(obj);
            }
            break;

        case 0x88:
        case 0x98:
            // Master wrote us a byte and we NACKed it.  Slave receive mode has been exited.
            i2c_conset(obj, 0, 0, 0, 1); // Set AA flag so that we go back to idle slave state
            i2c_clear_SI(obj);
            return count;

        case 0xA0:
            // Stop condition received.  Slave receive mode has been exited.
            i2c_conset(obj, 0, 0, 0, 1); // Set AA flag so that we go back to idle slave state
            i2c_clear_SI(obj);
            return count;
        }
    }
}

int i2c_slave_write(i2c_t *obj, const char *data, int length) {

    int next_byte_idx = 0;

    if(i2c_status(obj) != 0xA8) {
        return -1; // I2C peripheral not in setup-read-from-slave mode
    }
    
    // Write first data byte.  Note that there's no way for the slave to NACK a read-from-slave event, so if we run out of bytes,
    // just send zeroes.
    I2C_DAT(obj) = (next_byte_idx < length) ? data[next_byte_idx] : 0;
    ++next_byte_idx;

    i2c_conset(obj, 0, 0, 0, 1); // Set AA flag to acknowledge read as long as we have something to transmit
    i2c_clear_SI(obj);

    // This is implemented as a state machine according to section 19.10.9 in the reference manual.
    while(true) {

        // Wait until the I2C peripheral has an event for us
        i2c_wait_SI(obj);

        switch(i2c_status(obj)) {
        case 0xB0:
            // Arbitration Lost event.  I'm a bit confused about how this can happen as a slave device but the manual says it can...
            i2c_conclr(obj, 1, 0, 0, 1); // Clear start and ack
            i2c_clear_SI(obj);
            
            // Reset the I2C state machine. This doesn't actually send a STOP condition in slave mode.
            i2c_stop(obj);
            return -2; // Arbitration lost

        case 0xB8:
            // Prev data byte has been transmitted.  ACK has been received.  Write the next data byte.
            I2C_DAT(obj) = (next_byte_idx < length) ? data[next_byte_idx] : 0;
            ++next_byte_idx;

            i2c_conset(obj, 0, 0, 0, 1); // Set AA flag to acknowledge read as long as we have something to transmit
            i2c_clear_SI(obj);
            break;

        case 0xC0:
        case 0xC8:
            // Last data byte transmitted (ended either by us not setting AA, or by the master NACKing)
            i2c_conset(obj, 0, 0, 0, 1); // Set AA flag so that we go back to idle slave state
            i2c_clear_SI(obj);

            return next_byte_idx;
        }
    }
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask) {
    uint32_t addr;
    
    if ((idx >= 0) && (idx <= 3)) {
        addr = ((uint32_t)obj->i2c) + I2C_addr_offset[0][idx];
        *((uint32_t *) addr) = address & 0xFF;
        addr = ((uint32_t)obj->i2c) + I2C_addr_offset[1][idx];
        *((uint32_t *) addr) = mask & 0xFE;
    }
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

#endif  // #if DEVICE_I2C
