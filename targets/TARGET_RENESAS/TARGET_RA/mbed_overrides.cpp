/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "cmsis.h"
#include "objects.h"
#include "platform/mbed_error.h"

int mbed_sdk_inited = 0;

// This function is called after RAM initialization and before main.
extern "C" void mbed_sdk_init()
{
    /* Enable Pin access globally */
    R_BSP_PinAccessEnable();

    mbed_sdk_inited = 1;
}
