/* Copyright (c) 2026 MbedCE Community Contributors (Jan Kamidra)
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MBED_LOW_SPEED_CLOCK_H
#define MBED_LOW_SPEED_CLOCK_H

#include "cmsis.h"
#include "mbed_error.h"

typedef enum {
    LSC_NONE = 0,
    LSC_LSE,
    LSC_LSI
} lsc_t;


/**
  * @brief  Starts and checks the low-speed clock.
  *  First call starts, second call checks if the clock is ready.
  *  LSE is selected only when enabled over MBED_CONF_TARGET_LSE_AVAILABLE;
  *  otherwise LSI is selected.
  * @return the status of the low-speed clock:
  *          - LSC_NONE: LSC failed to start (trigger mbed-os error)
  *          - LSC_LSE: LSC started or is running with LSE
  *          - LSC_LSI: LSC started or is running with LSI
  */
lsc_t lsc_start(void);

#endif // MBED_LOW_SPEED_CLOCK_H
