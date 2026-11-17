/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __US_TICKER_DATA_H
#define __US_TICKER_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#define US_TICKER_CLOCK_HZ (15000000UL)
#define US_TICKER_CLOCK_BITS (32)

/* Each core drives its own GPT channel for the us ticker so that a dual-core
 * application can run both images at the same time: CPU0 (Cortex-M85) uses
 * GPT1, CPU1 (Cortex-M33) uses GPT12. A CM33 board must therefore provide
 * the GPT12 overflow / capture-compare-A vectors in its vector_data.h. */
#if defined(TARGET_RA8P1_CM33)
#define US_TICKER_VAR g_timer_gpt12
#define US_TICKER_INSTANCE R_GPT12
#define US_TICKER_IRQ  GPT12_CAPTURE_COMPARE_A_IRQn
#else
#define US_TICKER_VAR g_timer_gpt1
#define US_TICKER_INSTANCE R_GPT1
#define US_TICKER_IRQ  GPT1_CAPTURE_COMPARE_A_IRQn
#endif

/* RA8P1 devices have 15MHz us_ticker */
#define US_TICKER_PERIOD_NUM    1
#define US_TICKER_PERIOD_DEN    15

#ifdef __cplusplus
}
#endif

#endif // __US_TICKER_DATA_H

