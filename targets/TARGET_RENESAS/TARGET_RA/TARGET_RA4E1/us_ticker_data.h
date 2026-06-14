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

#ifndef __US_TICKER_DATA_H
#define __US_TICKER_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#define US_TICKER_CLOCK_HZ (25000000UL)
#define US_TICKER_CLOCK_BITS (32)
#define US_TICKER_VAR g_timer_gpt1
#define US_TICKER_INSTANCE R_GPT1
#define US_TICKER_IRQ  GPT1_CAPTURE_COMPARE_A_IRQn

/* RA4E1 devices have 25MHz us_ticker */
#define US_TICKER_PERIOD_NUM    1
#define US_TICKER_PERIOD_DEN    25

#ifdef __cplusplus
}
#endif

#endif // __US_TICKER_DATA_H

