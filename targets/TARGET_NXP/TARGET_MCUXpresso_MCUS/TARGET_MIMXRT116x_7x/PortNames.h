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
    Gpio1 = 1, // GPIO_EMC_B1_00 through GPIO_EMC_B1_31
    Gpio2 = 2, // GPIO_EMC_B1_32 through GPIO_EMC_B1_41, GPIO_EMC_B2_xx, GPIO_AD_00
    Gpio3 = 3, // GPIO_AD_01 through GPIO_AD_32
    Gpio4 = 4, // GPIO_AD_33 through GPIO_AD_35, GPIO_SD_Bx_xx, GPIO_DISP_B1_00 through GPIO_DISP_B1_10
    Gpio5 = 5, // GPIO_DISP_B1_11, GPIO_DISP_B2_xx
    Gpio6 = 6, // GPIO_LPSR_xx
    // Logical GPIOs 7 through 12 are mirrors of GPIOs 1-6
    Gpio13 = 13, // WAKEUP, PMIC_ON_REQ, PMIC_STBY_REQ, GPIO_SNVS_xx
} PortName;

#ifdef __cplusplus
}
#endif

#endif
