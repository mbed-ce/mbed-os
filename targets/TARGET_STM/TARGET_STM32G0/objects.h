/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015 STMicroelectronics.
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
#include "stm32g0xx_ll_usart.h"
#include "stm32g0xx_ll_tim.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_rtc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RTC_WKUP_IRQn RTC_TAMP_IRQn

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
    int index; // Used by irq
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

struct flash_s {
    /*  nothing to be stored for now */
    uint32_t dummy;
};

struct analogin_s {
    ADC_HandleTypeDef handle;
    PinName pin;
    uint8_t channel;
};

#if DEVICE_CAN
struct can_s {
    FDCAN_HandleTypeDef CanHandle;
    int index;
    int hz;
    int data_hz;
};
#endif

#include "gpio_object.h"

#if DEVICE_ANALOGOUT
struct dac_s {
    DACName dac;
    PinName pin;
    uint32_t channel;
    DAC_HandleTypeDef handle;
};
#endif

#define HAL_CRC_IS_SUPPORTED(polynomial, width) ((width) == 7 || (width) == 8 || (width) == 16 || (width) == 32)


#ifdef __cplusplus
}
#endif

#endif
