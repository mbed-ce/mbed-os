/* Copyright (c) 2026 MbedCE Community Contributors (Jan Kamidra)
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MBED_LOW_SPEED_CLOCK_H
#define MBED_LOW_SPEED_CLOCK_H

#include "cmsis.h"
#include "mbed_error.h"

/* See LSI oscillator characteristics in Data Sheet */
static const uint32_t STM32_LSI_MAX_FREQUENCY =
#if defined(STM32F1)
    60000U
#elif defined(STM32L0) || defined(STM32L1)
    56000U
#elif defined(STM32F2) || defined(STM32F4) || defined(STM32F7)
    47000U
#elif defined(STM32F0) || defined(STM32F3)
    50000U
#elif defined(STM32H7) || defined(STM32L4) || \
      defined(STM32U5) || defined(STM32H5)
    33600U
#elif defined(STM32G0) || defined(STM32L5) || \
      defined(STM32G4) || defined(STM32WB) || \
      defined(STM32WL) || defined(STM32U0)
    34000U
#elif defined(STM32WB0)
    49000U
#else
#error "Unsupported STM32 target"
#endif
;

/**
  * @brief  Starts and checks the low-speed clock.
  *  First call starts, second call checks if the clock is ready.
  *  LSE is selected only when enabled over MBED_CONF_TARGET_LSE_AVAILABLE;
  *  otherwise LSI is selected.
  *  A failure triggers an Mbed OS fatal error.
  */
void lsc_start(void);

#endif // MBED_LOW_SPEED_CLOCK_H
