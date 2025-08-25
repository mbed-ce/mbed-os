/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
// SPDX-License-Identifier: Apache-2.0
#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"

#include "objects_flash.h"
#include "objects_gpio.h"
#include "objects_uart.h"
#include "objects_iom.h"
#include "objects_spi.h"
#include "objects_i2c.h"
#include "objects_adc.h"
#include "objects_pwm.h"

#include "mbed_error.h"

// Macro to check the result of calling am am_hal function and trigger an error if it fails
#define MBED_CHECK_AM_HAL_CALL(call) \
    if((call) != AM_HAL_STATUS_SUCCESS) { \
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_HAL, MBED_ERROR_CODE_INVALID_OPERATION), "AM HAL Call Failed!"); \
    }

#endif
