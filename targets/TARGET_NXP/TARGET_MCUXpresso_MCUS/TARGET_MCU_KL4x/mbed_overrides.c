/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "gpio_api.h"
#include "pinmap.h"
#include "fsl_rtc.h"
#include "fsl_clock_config.h"
#include "rtc_api.h"

// called before main - implement here if board needs it otherwise, let
//  the application override this if necessary
void mbed_sdk_init()
{
    BOARD_BootClockRUN();
    /* Set the TPM clock source to be IRC48M, do not change as TPM2 is used for the usticker */
    CLOCK_SetTpmClock(1U);

    // Configure clock sources for some other peripherals
    CLOCK_SetLpuart0Clock(1); // IRC48M
    CLOCK_SetLpuart1Clock(1); // IRC48M

    // Init RTC because it provides the clock for the LP ticker
    rtc_init();
}

// Change the NMI pin to an input. This allows NMI pin to
//  be used as a low power mode wakeup.  The application will
//  need to change the pin back to NMI_b or wakeup only occurs once!
void NMI_Handler(void)
{
    gpio_t gpio;
    gpio_init_in(&gpio, PTA4);
}

uint32_t tpm_get_clock() {
    /* TPM clock source is set to IRC48M during init */
    return CLOCK_GetFreq(kCLOCK_McgIrc48MClk);
}