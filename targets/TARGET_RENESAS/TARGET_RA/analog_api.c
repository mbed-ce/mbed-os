#include "analogin_api.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "hal_data.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#if (BSP_PERIPHERAL_ADC_PRESENT && BSP_PERIPHERAL_ADC_CHANNEL_MASK == 0x3U) || \
    (BSP_PERIPHERAL_ADC_B_PRESENT && BSP_PERIPHERAL_ADC_B_CHANNEL_MASK == 0x3U) || \
    (BSP_PERIPHERAL_ADC_D_PRESENT && BSP_PERIPHERAL_ADC_D_CHANNEL_MASK == 0x3U)
#define HAS_ADC1 1
#endif

static bool g_adc_initialized = false;

static void mbed_adc_init_once(void)
{
    if (g_adc_initialized) {
        return;
    }

    fsp_err_t err;

    err = R_ADC_Open(&g_adc0_ctrl, &g_adc0_cfg);
    MBED_ASSERT(err == FSP_SUCCESS);

    err = R_ADC_ScanCfg(&g_adc0_ctrl, &g_adc0_channel_cfg);
    MBED_ASSERT(err == FSP_SUCCESS);

#ifdef HAS_ADC1
    err = R_ADC_Open(&g_adc1_ctrl, &g_adc1_cfg);
    MBED_ASSERT(err == FSP_SUCCESS);

    err = R_ADC_ScanCfg(&g_adc1_ctrl, &g_adc1_channel_cfg);
    MBED_ASSERT(err == FSP_SUCCESS);
#endif

    g_adc_initialized = true;
}

void analogin_init(analogin_t *obj, PinName pin)
{
    MBED_ASSERT(obj);

    mbed_adc_init_once();

    uint32_t peripheral = pinmap_peripheral(pin, PinMap_ADC);

    obj->pin = pin;
    obj->peripheral = peripheral;
    uint32_t function = pinmap_function(pin, PinMap_ADC);
    pin_function(pin, function);
    obj->channel = RA_PIN_CHANNEL(function);
}

void analogin_free(analogin_t *obj)
{
    pin_mode(obj->pin, PullNone);

    obj->pin = NC;
    obj->peripheral = 0;
    obj->channel = 0;
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = analogin_read_u16(obj);
    return (float)value / 65535.0f;
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    adc_status_t status;
    uint16_t adc_value = 0;

    if(obj->peripheral == ADC_0)
    {
        fsp_err_t err = R_ADC_ScanStart(&g_adc0_ctrl);
        MBED_ASSERT(err == FSP_SUCCESS);

        do {
            R_ADC_StatusGet(&g_adc0_ctrl, &status);
        } while (status.state != ADC_STATE_IDLE);

        err = R_ADC_Read(&g_adc0_ctrl, (adc_channel_t) obj->channel, &adc_value);
        MBED_ASSERT(err == FSP_SUCCESS);
    }
#ifdef HAS_ADC1
    else if(obj->peripheral == ADC_1)
    {
        fsp_err_t err = R_ADC_ScanStart(&g_adc1_ctrl);
        MBED_ASSERT(err == FSP_SUCCESS);

        do {
            R_ADC_StatusGet(&g_adc1_ctrl, &status);
        } while (status.state != ADC_STATE_IDLE);

        err = R_ADC_Read(&g_adc1_ctrl, (adc_channel_t) obj->channel, &adc_value);
        MBED_ASSERT(err == FSP_SUCCESS);
    }
#endif
    // Extend from 12bits to 16bits
    return (adc_value << 4);
}

PinName analogin_pin(analogin_t *obj)
{
    return obj->pin;
}

const PinMap *analogin_pinmap(void)
{
    return PinMap_ADC;
}
