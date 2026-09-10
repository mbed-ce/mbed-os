/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "PeripheralNames.h"
#include "hal_data.h"
#include "device.h"

const can_instance_t * const g_can_instances[CAN_COUNT] = {
    &g_can0,
};
