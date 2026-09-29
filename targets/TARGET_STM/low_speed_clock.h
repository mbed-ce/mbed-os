/* Copyright (c) 2026 MbedCE Community Contributors (Jan Kamidra)
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MBED_LOW_SPEED_CLOCK_H
#define MBED_LOW_SPEED_CLOCK_H

#include "cmsis.h"
#include "mbed_error.h"

/**
  * @brief  Starts and checks the low-speed clock.
  *  First call starts, second call checks if the clock is ready.
  *  LSE is selected only when enabled over MBED_CONF_TARGET_LSE_AVAILABLE;
  *  otherwise LSI is selected.
  *  A failure triggers an Mbed OS fatal error.
  */
void lsc_start(void);

#endif // MBED_LOW_SPEED_CLOCK_H
