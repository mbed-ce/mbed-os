/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 Arm Limited and affiliates.
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
#include "sleep_api.h"
#include "am_hal_sysctrl.h"

void hal_sleep(void)
{
  am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_NORMAL);
}

void hal_deepsleep(void)
{
  // To pass Mbed unit tests, the us ticker is required to be off when in deep sleep.
  // Ideally all of the high speed clocks should be off, but the datasheet is not very helpful
  // about how to accomplish this. I *think* the HFRC oscillator can only be turned off by
  // finding every peripheral using it and disabling the peripheral or changing it to a different
  // clock source. Implementing this, though, might be a bit tough. For now we at least turn off
  // the STIMER clock and freeze the value.
  am_hal_stimer_config(AM_HAL_STIMER_CFG_FREEZE);

  am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);

  am_hal_stimer_config(US_TICKER_FREQ);
}
