
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
#ifndef MBED_CAN_API_H
#define MBED_CAN_API_H

#include "device.h"
#include "pinmap.h"

#if DEVICE_CAN

#include "PinNames.h"
#include "PeripheralNames.h"
#include "hal/can_helper.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    IRQ_RX,
    IRQ_TX,
    IRQ_ERROR,
    IRQ_OVERRUN,
    IRQ_WAKEUP,
    IRQ_PASSIVE,
    IRQ_ARB,
    IRQ_BUS,
    IRQ_READY
} CanIrqType;


typedef enum {
    MODE_RESET,
    MODE_NORMAL,
    MODE_SILENT,
    MODE_TEST_LOCAL,
    MODE_TEST_GLOBAL,
    MODE_TEST_SILENT
} CanMode;

typedef struct {
    int peripheral;
    PinName rd_pin;
    int rd_function;
    PinName td_pin;
    int td_function;
} can_pinmap_t;

typedef void (*can_irq_handler)(uintptr_t context, CanIrqType type);

typedef struct can_s can_t;

/** Initialize the CAN peripheral. It sets the default parameters for CAN
 *  peripheral, and configures its specifieds pins.
 *
 * @param obj CAN object
 * @param rd The CAN RD pin name
 * @param td The CAN TD pin name
 */
void can_init(can_t *obj, PinName rd, PinName td);

/** Initialize the CAN peripheral. It sets the default parameters for CAN
 *  peripheral, and configures its specifieds pins.
 *
 * @param obj The CAN object
 * @param pinmap pointer to structure which holds static pinmap
 */
void can_init_direct(can_t *obj, const can_pinmap_t *pinmap);

/** Initialize the CAN peripheral. It sets the default parameters for CAN
 *  peripheral, and configures its specifieds pins.
 *
 * @param obj CAN object
 * @param rd The CAN RD pin name
 * @param td The CAN TD pin name
 * @param hz The bus frequency in classical CAN mode, or nominal phase frequency in CAN FD mode
 * @param data_hz The data phase frequency in CAN FD mode, the CAN object is put into Classical CAN mode if this parameter is zero
 */
void can_init_freq(can_t *obj, PinName rd, PinName td, int hz
#ifdef DEVICE_CAN_FD
                   , int data_hz
#endif
                  );

/** Initialize the CAN peripheral. It sets the default parameters for CAN
 *  peripheral, and configures its specifieds pins.
 *
 * @param obj CAN object
 * @param pinmap pointer to structure which holds static pinmap
 * @param hz The bus frequency in classical CAN mode, or nominal phase frequency in CAN FD mode
 * @param data_hz The data phase frequency in CAN FD mode, the CAN object is put into Classical CAN mode if this parameter is zero
 */
void can_init_freq_direct(can_t *obj, const can_pinmap_t *pinmap, int hz
#ifdef DEVICE_CAN_FD
                          , int data_hz
#endif
                         );

/** Release the CAN peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param obj The CAN object
 */
void can_free(can_t *obj);

/** Configure the CAN bus frequency
 *
 * @param obj The CAN object
 * @param hz The bus frequency in classical CAN mode, or nominal phase frequency in CAN FD mode
 * @param data_hz The data phase frequency in CAN FD mode, the CAN object is put into Classical CAN mode if this parameter is zero
 */
int can_frequency(can_t *obj, int hz
#ifdef DEVICE_CAN_FD
                  , int data_hz
#endif
                 );

/** Initialize the CAN IRQ handler
 *
 * @param obj     The CAN object
 * @param handler The handler to be attached to CAN IRQ
 * @param context The context to be passed back to the handler (context != 0, 0 is reserved)
 */
void can_irq_init(can_t *obj, can_irq_handler handler, uintptr_t context);

/** Remove the CAN IRQ handler
 *
 * @param obj The CAN object
 */
void can_irq_free(can_t *obj);

/** Enable/disable the CAN IRQ event
 *
 * @param obj    The CAN object
 * @param irq    The CAN IRQ event
 * @param enable The enable flag
 */
void can_irq_set(can_t *obj, CanIrqType irq, uint32_t enable);

/** Write a CAN message to the bus.
 *
 * @param obj    The CAN object
 * @param msg    The CAN message to write.
 *
 * @return 0 if write failed,
 *    1 if write was successful
 */
int can_write(can_t *obj, CAN_Message msg);

/** Read a CAN message from the bus.
 *
 * @param obj    CAN object
 * @param msg    A CAN message to read to.
 * @param handle message filter handle (0 for any message).
 *
 * @return 0 if no message arrived,
 *         1 if message arrived
 */
int can_read(can_t *obj, CAN_Message *msg, int handle);

/** Change CAN operation to the specified mode.
 *
 * @param obj The CAN object
 * @param mode The new operation mode (MODE_NORMAL, MODE_SILENT, MODE_TEST_LOCAL, MODE_TEST_GLOBAL, MODE_TEST_SILENT).
 *
 * @return 0 if mode change failed or unsupported,
 *   1 if mode change was successful
 */
int can_mode(can_t *obj, CanMode mode);

/** Filter out incomming messages.
 *
 * @param obj The CAN object
 * @param id the id to filter on.
 * @param mask the mask applied to the id.
 * @param format format to filter on (Default CANAny).
 * @param handle message filter handle (Optional).
 *
 * @return 0 if filter change failed or unsupported,
 *    new filter handle if successful
 */
int can_filter(can_t *obj, uint32_t id, uint32_t mask, CANFormat format, int32_t handle);

/** Reset CAN interface.
 *
 * @param obj    CAN object
 *
 * To use after error overflow.
 */
void can_reset(can_t *obj);

/**  Detects read errors - Used to detect read overflow errors.
 *
 * @param obj CAN object
 * @return number of read errors
 */
unsigned char can_rderror(can_t *obj);

/** Detects write errors - Used to detect write overflow errors.
 *
 * @param obj CAN object
 * @return number of write errors
 */
unsigned char can_tderror(can_t *obj);

/** Puts or removes the CAN interface into silent monitoring mode.
 *
 * @param obj CAN object
 * @param silent boolean indicating whether to go into silent mode or not.
 */
void can_monitor(can_t *obj, int silent);

#if DEVICE_CAN_FD
/** Write a CAN FD Message to the bus.
 *
 * @param obj    The CAN object
 * @param msg    The CAN FD Message to write.
 *
 * @return 0 if write failed,
 *    1 if write was successful
 */
int canfd_write(can_t *obj, CANFD_Message msg);

/** Read a Classical CAN or CAN FD Message from the bus.
 *
 * @param obj    CAN object
 * @param msg    A Classical CAN or CAN FD Message to read to.
 * @param handle message filter handle (0 for any message).
 *
 * @return 0 if no message arrived,
 *         1 if message arrived
 */
int canfd_read(can_t *obj, CANFD_Message *msg, int handle);
#endif

/** Get the pins that support CAN RD
 *
 * Return a PinMap array of pins that support CAN RD. The
 * array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *can_rd_pinmap(void);

/** Get the pins that support CAN TD
 *
 * Return a PinMap array of pins that support CAN TD. The
 * array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *can_td_pinmap(void);

#ifdef __cplusplus
}
#endif

#endif    // MBED_CAN_API_H

#endif

/** @}*/
