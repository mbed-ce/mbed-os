/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __US_TICKER_DATA_H
#define __US_TICKER_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#define US_TICKER_CLOCK_HZ (3125000UL)
#define US_TICKER_CLOCK_BITS (16)
#define US_TICKER_VAR g_timer_gpt1
#define US_TICKER_INSTANCE R_GPT1
#define US_TICKER_IRQ  GPT1_CAPTURE_COMPARE_A_IRQn

/* RA6E2 devices have 3.125MHz us_ticker */
#define US_TICKER_PERIOD_NUM    8
#define US_TICKER_PERIOD_DEN    25

#ifdef __cplusplus
}
#endif

#endif // __US_TICKER_DATA_H
