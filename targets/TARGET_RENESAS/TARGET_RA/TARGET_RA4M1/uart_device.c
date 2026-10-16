/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "PeripheralNames.h"
#include "hal_data.h"
#include "device.h"

#if UART_COUNT > 2
/* Boards with a third UART instance (e.g. ARDUINO_UNO_R4_WIFI: SCI9 bridge console) */
const uart_instance_t * const g_uart_instances[UART_COUNT] = {
    &g_uart1,
    &g_uart2,
    &g_uart9
};
#else
const uart_instance_t * const g_uart_instances[UART_COUNT] = {
    &g_uart1,
    &g_uart2
};
#endif
