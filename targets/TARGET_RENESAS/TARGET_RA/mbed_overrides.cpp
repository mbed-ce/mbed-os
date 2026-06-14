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
