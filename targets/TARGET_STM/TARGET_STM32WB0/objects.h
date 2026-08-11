/* Copyright (c) 2026 MbedCE Community Contributors (Jan Kamidra)
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "cmsis.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "PinNames.h"
#include "stm32wb0x_ll_tim.h"
#include "stm32wb0x_ll_pwr.h"
#include "stm32wb0x_ll_system.h"
#include "stm32wb0x_ll_usart.h"

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
};

struct gpio_irq_s {
    PinName pin;
    uint32_t event;
    uint32_t enabled;
};

#include "gpio_object.h"

#endif
