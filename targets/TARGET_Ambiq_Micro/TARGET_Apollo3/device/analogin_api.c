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


#include "mbed_assert.h"
#include "analogin_api.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#include "am_hal_adc.h"

void* am_adc_handle = NULL;

// ADC constants
#define ADC_RESOLUTION_SEL AM_HAL_ADC_SLOT_14BIT
#define ADC_RESOLUTION_BITS 14
#define ADC_CONVERSION_FACTOR (1.0f / (1 << ADC_RESOLUTION_BITS))

#define ADC_REFERENCE_SEL AM_HAL_ADC_REFSEL_INT_2P0

static uint32_t powerControlADC(bool on){
    uint32_t status = AM_HAL_STATUS_SUCCESS;

    if(on){
        status = am_hal_adc_initialize(0, &am_adc_handle);
        if(status != AM_HAL_STATUS_SUCCESS){ return status; }

        status = am_hal_adc_power_control(am_adc_handle, AM_HAL_SYSCTRL_WAKE, false);
        if(status != AM_HAL_STATUS_SUCCESS){ return status; }
    }else{
        status = am_hal_adc_disable(am_adc_handle);
        if(status != AM_HAL_STATUS_SUCCESS){ return status; }

        status = am_hal_pwrctrl_periph_disable(AM_HAL_PWRCTRL_PERIPH_ADC);
        if(status != AM_HAL_STATUS_SUCCESS){ return status; }

        status = am_hal_adc_deinitialize(am_adc_handle);
        if(status != AM_HAL_STATUS_SUCCESS){ return status; }
    }

    return status;
}

static uint32_t initializeADC( void ){
    am_hal_adc_config_t ADCConfig;

    // Power on the ADC.
    powerControlADC(true);

    // Set up the ADC configuration parameters. These settings are reasonable
    // for accurate measurements at a low sample rate.
    ADCConfig.eClock = AM_HAL_ADC_CLKSEL_HFRC;
    ADCConfig.ePolarity = AM_HAL_ADC_TRIGPOL_RISING;
    ADCConfig.eTrigger = AM_HAL_ADC_TRIGSEL_SOFTWARE;
    ADCConfig.eReference = ADC_REFERENCE_SEL;
    ADCConfig.eClockMode = AM_HAL_ADC_CLKMODE_LOW_LATENCY;
    ADCConfig.ePowerMode = AM_HAL_ADC_LPMODE0;
    ADCConfig.eRepeat = AM_HAL_ADC_SINGLE_SCAN;

    return am_hal_adc_configure(am_adc_handle, &ADCConfig);
}


void analogin_init(analogin_t *obj, PinName pin)
{
    // Find ADC slot and pin from pinmap
    const am_hal_adc_slot_chan_e adcSlot = pinmap_peripheral(pin, PinMap_ADC);
    const uint32_t pinFunction = pinmap_function(pin, PinMap_ADC);

    // Configure pin as an analog pin
    am_hal_gpio_pincfg_t pincfg = g_AM_HAL_GPIO_INPUT;
    pincfg.uFuncSel = pinFunction;
    am_hal_gpio_pinconfig(pin, pincfg);

    /* Initialize the ADC the first time it is being used,
     * but don't reinitialize it again afterwards.
     */
    static bool is_adc_initialized = false;
    if (!is_adc_initialized) 
    {
        initializeADC();
        is_adc_initialized = true;
    }

    obj->slot = adcSlot;
}

// Reconfigure ADC slot 0 to target the given channel
static void ap3_config_channel(am_hal_adc_slot_chan_e channel){
    am_hal_adc_slot_config_t ADCSlotConfig;

    // Set up an ADC slot
    ADCSlotConfig.eMeasToAvg = AM_HAL_ADC_SLOT_AVG_1;
    ADCSlotConfig.ePrecisionMode = ADC_RESOLUTION_SEL;
    ADCSlotConfig.eChannel = channel;
    ADCSlotConfig.bWindowCompare = false;
    ADCSlotConfig.bEnabled = true;

    MBED_CHECK_AM_HAL_CALL(am_hal_adc_disable(am_adc_handle));

    MBED_CHECK_AM_HAL_CALL(am_hal_adc_configure_slot(am_adc_handle, 0, &ADCSlotConfig));

    MBED_CHECK_AM_HAL_CALL(am_hal_adc_enable(am_adc_handle));
}

// Read an analog in channel as a 14-bit number
static uint16_t readAnalogIn(analogin_t *obj)
{
    // Target this channel
    ap3_config_channel(obj->slot);

    // Clear any set interrupt flags
    uint32_t intStatus;
    am_hal_adc_interrupt_status(am_adc_handle, &intStatus, false);
    MBED_CHECK_AM_HAL_CALL(am_hal_adc_interrupt_clear(am_adc_handle, intStatus));

    // Issue SW trigger
    am_hal_adc_sw_trigger(am_adc_handle);

    do { // Wait for conversion complete interrupt
        MBED_CHECK_AM_HAL_CALL(am_hal_adc_interrupt_status(am_adc_handle, &intStatus, false));
    } while(!(intStatus & AM_HAL_ADC_INT_CNVCMP));
    MBED_CHECK_AM_HAL_CALL(am_hal_adc_interrupt_clear(am_adc_handle, intStatus));

    uint32_t numSamplesToRead = 1;
    am_hal_adc_sample_t sample;
    MBED_CHECK_AM_HAL_CALL(am_hal_adc_samples_read(am_adc_handle, false, NULL, &numSamplesToRead, &sample));

    return sample.ui32Sample;
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    uint16_t reading = readAnalogIn(obj);

    /* Return a 16-Bit ADC value. */
    return reading << (16 - ADC_RESOLUTION_BITS);
}

float analogin_read(analogin_t *obj)
{
    /* Read the raw 14-Bit value from the ADC. */
    uint16_t analog_in_raw = readAnalogIn(obj);

    /* Convert it to a voltage value. */
    return (analog_in_raw * ADC_CONVERSION_FACTOR);
}


const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}
