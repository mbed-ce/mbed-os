/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "cmsis.h"
#include "objects.h"
#include "platform/mbed_error.h"
#include "RTTConsole.h"

int mbed_sdk_inited = 0;

#if MBED_CONF_TARGET_CONSOLE_RTT
// Override printf to use RTT
static RTTConsole rtt_console;
mbed::FileHandle *mbed::mbed_override_console(int fd)
{
    return &rtt_console;
}
#endif

// This function is called after RAM initialization and before main.
extern "C" void mbed_sdk_init()
{
    /* Enable Pin access globally */
    R_BSP_PinAccessEnable();

    /* Initialize SEGGER RTT */
#if MBED_CONF_TARGET_CONSOLE_RTT
    SEGGER_RTT_Init();
#endif

    mbed_sdk_inited = 1;
}
