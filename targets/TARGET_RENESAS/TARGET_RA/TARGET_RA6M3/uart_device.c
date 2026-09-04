/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "PeripheralNames.h"
#include "hal_data.h"
#include "device.h"

const uart_instance_t * const g_uart_instances[UART_COUNT] = {
    &g_uart0,
    &g_uart1,
    &g_uart2,
    /* SCI3 is used in simple SPI mode for the RW007 WiFi module, not as a UART. */
    &g_uart4,
    &g_uart5,
    &g_uart6,
    &g_uart7,
    &g_uart8,
    &g_uart9,
};
