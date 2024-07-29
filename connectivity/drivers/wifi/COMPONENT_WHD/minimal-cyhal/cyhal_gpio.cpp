/*
 * Copyright 2020 Arduino SA
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

/** @file
 *  Provides the porting layer for STM32 GPIOs on WHD driver
 */

#include <stdio.h>
#include "cyhal.h"
#include "cybsp.h"
#include "drivers/DigitalOut.h"
#include "drivers/InterruptIn.h"

#include <minimal_cyhal_config.h>

/*******************************************************************************
*       Internal
*******************************************************************************/

static mbed::InterruptIn * interruptIns[MINIMAL_CYHAL_NUM_PINS] = {};
static mbed::DigitalOut * digitalOuts[MINIMAL_CYHAL_NUM_PINS] = {};

static cyhal_gpio_event_t oob_event = CYHAL_GPIO_IRQ_FALL;
static cyhal_gpio_event_callback_t oob_handler;
static void *oob_handler_arg;

/*******************************************************************************
*       Internal Interrrupt Service Routine
*******************************************************************************/
static void cb()
{
    oob_handler(oob_handler_arg, oob_event);
}

/*******************************************************************************
*       HAL Implementation
*******************************************************************************/

cy_rslt_t cyhal_gpio_init(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drvMode, bool initVal)
{
    cy_rslt_t     ret = CY_RSLT_SUCCESS;

    // Find pin name
    PinName pinName;
    switch(pin)
    {
        case CYBSP_WIFI_WL_REG_ON:
            pinName = WIFI_WL_REG_ON;
            break;
        case CYBSP_LED1:
            pinName = BSP_LED1;
            break;
        case CYBSP_LED2:
            pinName = BSP_LED2;
            break;
        case CYBSP_SDIO_OOB_IRQ:
            pinName = WIFI_SDIO_OOB_IRQ;
            break;
        default:
            return CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_HAL, 1);
    }

    // Ignore the parameter and take the pin config directly from a static array definitions.
    // For the purposes of the wifi driver, pins are only initialized as digital out or interrupt in,
    // so we just need to handle those two use cases.
    if(direction == CYHAL_GPIO_DIR_OUTPUT)
    {
        digitalOuts[pin] = new mbed::DigitalOut(pinName, initVal);
    }
    else
    {
        interruptIns[pin] = new mbed::InterruptIn(pinName);
    }

    // Workaround to initialize sdio interface without cypress bsp init
    if (pin == CYBSP_WIFI_WL_REG_ON) {
        cyhal_sdio_t *sdio_p = cybsp_get_wifi_sdio_obj();
        ret = cyhal_sdio_init(sdio_p, CYBSP_WIFI_SDIO_CMD, CYBSP_WIFI_SDIO_CLK, CYBSP_WIFI_SDIO_D0, CYBSP_WIFI_SDIO_D1, CYBSP_WIFI_SDIO_D2, CYBSP_WIFI_SDIO_D3);
    }
    return ret;
}

void cyhal_gpio_write(cyhal_gpio_t pin, bool value)
{
    digitalOuts[pin]->write(value);
}

void cyhal_gpio_register_callback(cyhal_gpio_t pin, cyhal_gpio_event_callback_t handler, void *handler_arg)
{
    if (handler && handler_arg) {
        oob_handler = handler;
        oob_handler_arg = handler_arg;
    }
}

void cyhal_gpio_enable_event(cyhal_gpio_t pin, cyhal_gpio_event_t event, uint8_t intr_priority, bool enable)
{
    oob_event = event;
    if (enable) {
        if (event == CYHAL_GPIO_IRQ_RISE) {
            interruptIns[pin]->rise(cb);
        }
        if (event == CYHAL_GPIO_IRQ_FALL) {
            interruptIns[pin]->fall(cb);
        }
        interruptIns[pin]->enable_irq();
    } else {
        interruptIns[pin]->disable_irq();
    }
}

void cyhal_gpio_free(cyhal_gpio_t pin)
{
    // Delete any objects associated with the pins
    delete interruptIns[pin];
    interruptIns[pin] = nullptr;
    delete digitalOuts[pin];
    digitalOuts[pin] = nullptr;
}
