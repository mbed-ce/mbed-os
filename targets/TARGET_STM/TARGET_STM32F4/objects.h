/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "cmsis.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "PinNames.h"

#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx_ll_rtc.h"
#include "stm32f4xx_ll_rcc.h"

#include "stm_dma_info.h"
#if MBED_CONF_RTOS_PRESENT
#include "cmsis_os.h"
#include "cmsis_os2.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct gpio_irq_s {
    IRQn_Type irq_n;
    uint32_t irq_index;
    uint32_t event;
    PinName pin;
};

struct port_s {
    PortName port;
    uint32_t mask;
    PinDirection direction;
    __IO uint32_t *reg_in;
    __IO uint32_t *reg_out;
};

struct serial_s {
    UARTName uart;
    int index;
    uint32_t baudrate;
    uint32_t databits;
    uint32_t stopbits;
    uint32_t parity;
    PinName pin_tx;
    PinName pin_rx;
#if DEVICE_SERIAL_ASYNCH
    uint32_t events;
#endif
#if DEVICE_SERIAL_FC
    uint32_t hw_flow_ctl;
    PinName pin_rts;
    PinName pin_cts;
#endif
};

struct i2c_s {
    /*  The 1st 2 members I2CName i2c
     *  and I2C_HandleTypeDef handle should
     *  be kept as the first members of this struct
     *  to have get_i2c_obj() function work as expected
     */
    I2CName  i2c;
    I2C_HandleTypeDef handle;
    uint8_t index;
    int hz;
    PinName sda;
    PinName scl;
    int sda_func;
    int scl_func;
    IRQn_Type event_i2cIRQ;
    IRQn_Type error_i2cIRQ;
    uint32_t XferOperation;
    volatile uint8_t event;
#if DEVICE_I2CSLAVE
    uint8_t slave;
    volatile uint8_t pending_slave_tx_master_rx;
    volatile uint8_t pending_slave_rx_maxter_tx;
    volatile uint8_t slave_tx_transfer_in_progress;
    volatile uint8_t slave_rx_transfer_in_progress;
    uint8_t *slave_rx_buffer;
    volatile uint16_t slave_rx_buffer_size;
    volatile uint16_t slave_rx_count;
#endif
#if DEVICE_I2C_ASYNCH
    uint32_t address;
    uint8_t stop;
    uint8_t available_events;
#endif
};

#if DEVICE_FLASH
struct flash_s {
    uint32_t dummy;
};
#endif

struct analogin_s {
    ADC_HandleTypeDef handle;
    PinName pin;
    uint8_t channel;
};

#define GPIO_IP_WITHOUT_BRR
#include "gpio_object.h"

#if DEVICE_ANALOGOUT
struct dac_s {
    DACName dac;
    PinName pin;
    uint32_t channel;
    DAC_HandleTypeDef handle;
};
#endif

#if DEVICE_TRNG
struct trng_s {
    RNG_HandleTypeDef handle;
};
#endif

#if DEVICE_CAN
struct can_s {
    CAN_HandleTypeDef CanHandle;
    int index;
    int hz;
    int rxIrqEnabled;
};
#endif

#if DEVICE_QSPI
struct qspi_s {
    QSPI_HandleTypeDef handle;
    QSPIName qspi;
    PinName io0;
    PinName io1;
    PinName io2;
    PinName io3;
    PinName sclk;
    PinName ssel;
    bool dmaInitialized;
#if MBED_CONF_RTOS_PRESENT
    osSemaphoreId_t semaphoreId;
    osRtxSemaphore_t semaphoreMem;
#endif
};
#endif

#ifdef __cplusplus
}
#endif

#endif
