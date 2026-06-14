#include "analogout_api.h"
#include "pinmap.h"
#include "mbed_assert.h"
#include "r_dac.h"
#include "r_dac_api.h"
#include "PeripheralPins.h"

static dac_instance_ctrl_t dac_ctrls[2];

void dac_init(dac_t *obj, PinName pin)
{
    MBED_ASSERT(obj);

    int ch = pinmap_peripheral(pin, PinMap_DAC);
    MBED_ASSERT(ch != NC);

    obj->channel = ch;
    obj->ctrl = &dac_ctrls[ch];

    pinmap_pinout(pin, PinMap_DAC);

    obj->ext_cfg.enable_charge_pump = true;
    obj->ext_cfg.output_amplifier_enabled = true;
    obj->ext_cfg.internal_output_enabled = false;
    obj->ext_cfg.data_format = DAC_DATA_FORMAT_FLUSH_RIGHT;
    obj->ext_cfg.ref_volt_sel = DAC_VREF_VREFH_VREFL;

    obj->cfg.channel = ch;
    obj->cfg.ad_da_synchronized = false;
    obj->cfg.p_extend = &obj->ext_cfg;

    R_DAC_Open(obj->ctrl, &obj->cfg);

    obj->last_value = 0;
    R_DAC_Write(obj->ctrl, 0);

    R_DAC_Start(obj->ctrl);
}

void dac_free(dac_t *obj)
{
}

void dac_write_u16(dac_t *obj, uint16_t value)
{
    obj->last_value = value;

    uint16_t dac_val = value >> 4;

    R_DAC_Write(obj->ctrl, dac_val);
}

uint16_t dac_read_u16(dac_t *obj)
{
    return obj->last_value;
}

void dac_write(dac_t *obj, float value)
{
    if (value < 0.0f) value = 0.0f;
    if (value > 1.0f) value = 1.0f;

    uint16_t v = (uint16_t)(value * 65535.0f);
    dac_write_u16(obj, v);
}

float dac_read(dac_t *obj)
{
    return dac_read_u16(obj) / 65535.0f;
}
