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
#ifndef MBED_PORTNAMES_H
#define MBED_PORTNAMES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    Gpio1 = 1, // GPIO_AD_Bx_xx
    Gpio2 = 2, // GPIO_Bx_xx
    Gpio3 = 3, // GPIO_SD_Bx_xx, GPIO_EMC_32 through GPIO_EMC_41
    Gpio4 = 4, // GPIO_EMC_00 through GPIO_EMC_31
    Gpio5 = 5  // WAKEUP, PMIC_ON_REQ, PMIC_STBY_REQ
} PortName;

#ifdef __cplusplus
}
#endif

#endif
