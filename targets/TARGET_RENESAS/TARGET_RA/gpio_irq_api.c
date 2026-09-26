/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "gpio_irq_api.h"
#include "pinmap.h"
#include "mbed_critical.h"
#include "mbed_assert.h"
#include "common_data.h"
#include "mbed_atomic.h"
#include "mbed_error.h"
#include "PeripheralPins.h"

#ifdef __cplusplus
extern "C" {
#endif

static volatile gpio_irq_handler s_handler = NULL;
static volatile core_util_atomic_flag s_irq_used[IRQ_CHANNELS_COUNT] = {0};
static gpio_irq_t * volatile     s_irq_obj[IRQ_CHANNELS_COUNT] = {0};

extern icu_instance_ctrl_t * const g_icu_ctrl[IRQ_CHANNELS_COUNT];
extern external_irq_cfg_t * const g_icu_cfg[IRQ_CHANNELS_COUNT];

void external_irq_callback(external_irq_callback_args_t *p_args)
{
    uint32_t ch = p_args->channel;

    if (ch < IRQ_CHANNELS_COUNT && s_irq_used[ch]._flag && s_handler)
    {
        gpio_irq_t *obj = s_irq_obj[ch];
        int level = R_BSP_PinRead((bsp_io_port_pin_t) obj->pin);

        gpio_irq_event evt = level ? IRQ_RISE : IRQ_FALL;

        s_handler(obj->context, evt);
    }
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uintptr_t context)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(pin != NC);

    int irq_channel = pinmap_peripheral(pin, PinMap_IRQ);
    MBED_ASSERT(irq_channel != (int)NC);

    const bool already_used = core_util_atomic_flag_test_and_set(&s_irq_used[irq_channel]);
    if(already_used) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_GPIO, MBED_ERROR_CODE_ALREADY_IN_USE), "IRQ channel already in use!", irq_channel);
    }

    int irq_function = pinmap_function(pin, PinMap_IRQ);
    pin_function(pin, irq_function);

    core_util_critical_section_enter();
    s_handler = handler;
    core_util_critical_section_exit();

    obj->pin = pin;
    obj->channel = irq_channel;
    obj->context = context;
    obj->is_active = false;
    obj->edge_select = IRQ_NONE;

    s_irq_obj[irq_channel] = obj;

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    MBED_ASSERT(obj);

    uint32_t ch = obj->channel;
    if (ch >= IRQ_CHANNELS_COUNT)
        return;

    R_ICU_ExternalIrqDisable(g_icu_ctrl[ch]);
    R_ICU_ExternalIrqClose(g_icu_ctrl[ch]);

    core_util_atomic_flag_clear(&s_irq_used[ch]);

    obj->pin = NC;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    // Must disable first, even if we are reconfiguring
    if(obj->is_active) {
        R_ICU_ExternalIrqDisable(g_icu_ctrl[obj->channel]);
        R_ICU_ExternalIrqClose(g_icu_ctrl[obj->channel]);
        obj->is_active = false;
    }

    // Modify edge select accordingly
    if(enable) {
        obj->edge_select |= event;
    }
    else { // disable
        obj->edge_select &= ~event;
    }

    // set edge select and enable
    if(obj->edge_select != IRQ_NONE) {
        switch(obj->edge_select) {
            case IRQ_RISE:
                g_icu_cfg[obj->channel]->trigger = EXTERNAL_IRQ_TRIGGER_RISING;
                break;
            case IRQ_FALL:
                g_icu_cfg[obj->channel]->trigger = EXTERNAL_IRQ_TRIGGER_FALLING;
                break;
            case (IRQ_RISE | IRQ_FALL):
                g_icu_cfg[obj->channel]->trigger = EXTERNAL_IRQ_TRIGGER_BOTH_EDGE;
                break;
        }

        fsp_err_t err = R_ICU_ExternalIrqOpen(g_icu_ctrl[obj->channel], g_icu_cfg[obj->channel]);
        MBED_ASSERT(err == FSP_SUCCESS);
        R_ICU_ExternalIrqEnable(g_icu_ctrl[obj->channel]);

        obj->is_active = true;
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    R_ICU_ExternalIrqEnable(g_icu_ctrl[obj->channel]);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    R_ICU_ExternalIrqDisable(g_icu_ctrl[obj->channel]);
}

#ifdef __cplusplus
}
#endif
