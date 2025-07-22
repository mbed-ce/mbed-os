/* mbed Microcontroller Library
 * Copyright (c) 2025 Jamie Smith
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

#ifndef OBJECTS_ADC_H
#define OBJECTS_ADC_H

#include "am_hal_adc.h"

#ifdef __cplusplus
extern "C" {
#endif

// ADC input names
typedef enum {
    ADC0_0 = AM_HAL_ADC_SLOT_CHSEL_SE0,
    ADC0_1 = AM_HAL_ADC_SLOT_CHSEL_SE1,
    ADC0_2 = AM_HAL_ADC_SLOT_CHSEL_SE2,
    ADC0_3 = AM_HAL_ADC_SLOT_CHSEL_SE3,
    ADC0_4 = AM_HAL_ADC_SLOT_CHSEL_SE4,
    ADC0_5 = AM_HAL_ADC_SLOT_CHSEL_SE5,
    ADC0_6 = AM_HAL_ADC_SLOT_CHSEL_SE6,
    ADC0_7 = AM_HAL_ADC_SLOT_CHSEL_SE7,
    ADC0_8 = AM_HAL_ADC_SLOT_CHSEL_SE8,
    ADC0_9 = AM_HAL_ADC_SLOT_CHSEL_SE9,
    ADC0_TEMP = AM_HAL_ADC_SLOT_CHSEL_TEMP
} ADCName;

struct analogin_s
{
    // Slot that this ADC input connects to on the ADC
    am_hal_adc_slot_chan_e slot;
};

// Opaque handle to the ADC peripheral. This is needed to
// use certain AM HAL calls from user code. This will be initialized
// the first time that any AnalogIn is initialized.
extern void* am_adc_handle;

#ifdef __cplusplus
}
#endif

#endif