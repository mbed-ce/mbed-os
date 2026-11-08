/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stddef.h>
#include "us_ticker_api.h"
#include "us_ticker_defines.h"
#include "PeripheralNames.h"
#include "fsl_pit.h"
#include "fsl_tpm.h"
#include "fsl_clock_config.h"
#include "mbed_interface.h"

// Defined in mbed_overrides.c
extern uint32_t tpm_get_clock();

const ticker_info_t* us_ticker_get_info()
{
    static const ticker_info_t info = {
        1000000,
        32
    };
    return &info;
}

static bool us_ticker_inited = false;

static volatile uint32_t us_ticker_int_counter = 0;
static volatile uint16_t us_ticker_int_remainder = 0;

// Selected prescaler for the TPM clock. With the bus clock in the range
// of ~20-50MHz, this gets TPM ticks at about 1-3 us.
const tpm_clock_prescale_t TPM_PRESCALER_SELECT = kTPM_Prescale_Divide_16;
const uint32_t TPM_PRESCALER_VAL = 16;

static void tpm_isr(void)
{
    // Clear the TPM timer overflow flag
    TPM_ClearStatusFlags(TPM2, kTPM_TimeOverflowFlag);
    TPM_StopTimer(TPM2);

    if (us_ticker_int_counter > 0) {
        TPM2->MOD = 0xFFFF;
        TPM_StartTimer(TPM2, kTPM_SystemClock);
        us_ticker_int_counter--;
    } else {
        if (us_ticker_int_remainder > 0) {
            TPM2->MOD = us_ticker_int_remainder;
            TPM_StartTimer(TPM2, kTPM_SystemClock);
            us_ticker_int_remainder = 0;
        } else {
            // This function is going to disable the interrupts if there are
            // no other events in the queue
            us_ticker_irq_handler();
        }
    }
}

static uint32_t tpmClock;

void us_ticker_init(void)
{

    /* Structure to initialize PIT. */
    pit_config_t pitConfig;

    if (us_ticker_inited) {
        /* calling init again should cancel current interrupt */
        TPM_DisableInterrupts(TPM2, kTPM_TimeOverflowInterruptEnable);
        return;
    }

    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);

    uint32_t busClock = CLOCK_GetFreq(kCLOCK_BusClk);
    tpmClock = tpm_get_clock();

    PIT->CHANNEL[kPIT_Chnl_0].LDVAL = (busClock / 1000000) - 1;
    PIT->CHANNEL[kPIT_Chnl_1].LDVAL = 0xFFFFFFFF;
    PIT_SetTimerChainMode(PIT, kPIT_Chnl_1, true);
    PIT_StartTimer(PIT, kPIT_Chnl_0);
    PIT_StartTimer(PIT, kPIT_Chnl_1);

    /* Configure interrupt generation counters and disable ticker interrupts. */
    tpm_config_t tpmConfig;

    TPM_GetDefaultConfig(&tpmConfig);
    /* Set to Div 16 to get needed clock source for TPM */
    tpmConfig.prescale = TPM_PRESCALER_SELECT;
    TPM_Init(TPM2, &tpmConfig);
    NVIC_SetVector(TPM2_IRQn, (uint32_t)tpm_isr);
    NVIC_EnableIRQ(TPM2_IRQn);

    us_ticker_inited = true;
}


uint32_t (us_ticker_read)()
{
    return us_ticker_read();
}

void us_ticker_disable_interrupt(void)
{
    TPM_DisableInterrupts(TPM2, kTPM_TimeOverflowInterruptEnable);
}

void us_ticker_clear_interrupt(void)
{
    TPM_ClearStatusFlags(TPM2, kTPM_TimeOverflowFlag);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    /* We get here absolute interrupt time which takes into account counter overflow.
     * Since we use additional count-down timer to generate interrupt we need to calculate
     * load value based on time-stamp.
     */
    const uint32_t now_ticks = us_ticker_read();
    uint32_t delta_ticks =
            timestamp >= now_ticks ? timestamp - now_ticks : (uint32_t)((uint64_t) timestamp + 0xFFFFFFFF - now_ticks);

    const uint32_t delta_us = (delta_ticks * US_TICKER_PERIOD_NUM) / US_TICKER_PERIOD_DEN;
    uint64_t delta_tpm_counts = (((uint64_t)delta_us) * (tpmClock / TPM_PRESCALER_VAL) / 1000000);

    if (delta_tpm_counts == 0) {
        /* The requested delay is less than the minimum resolution of this counter. */
        delta_tpm_counts = 1;
    }

    TPM_StopTimer(TPM2);
    TPM2->CNT = 0;

    us_ticker_int_counter   = (uint32_t)(delta_tpm_counts >> 16);
    us_ticker_int_remainder = (uint16_t)(0xFFFF & delta_tpm_counts);

    if (us_ticker_int_counter > 0) {
        TPM2->MOD = 0xFFFF;
        us_ticker_int_counter--;
    } else {
        TPM2->MOD = us_ticker_int_remainder;
        us_ticker_int_remainder = 0;
    }

    /* Clear the count and set match value */
    TPM_ClearStatusFlags(TPM2, kTPM_TimeOverflowFlag);
    TPM_EnableInterrupts(TPM2, kTPM_TimeOverflowInterruptEnable);
    TPM_StartTimer(TPM2, kTPM_SystemClock);
}

void us_ticker_fire_interrupt(void)
{
    us_ticker_int_counter = 0;
    us_ticker_int_remainder = 0;

    NVIC_SetPendingIRQ(TPM2_IRQn);
}

void us_ticker_free(void)
{
    TPM_StopTimer(TPM2);
}
