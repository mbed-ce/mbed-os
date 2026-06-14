#include "gpio_irq_api.h"
#include "pinmap.h"
#include "mbed_critical.h"
#include "mbed_assert.h"
#include "common_data.h"
#include "PeripheralPins.h"

#ifdef __cplusplus
extern "C" {
#endif

static gpio_irq_handler s_handler = NULL;
static uint8_t          s_irq_used[IRQ_CHANNELS_COUNT] = {0};
static gpio_irq_t *     s_irq_obj[IRQ_CHANNELS_COUNT] = {0};

extern icu_instance_ctrl_t * const g_icu_ctrl[IRQ_CHANNELS_COUNT];
extern const external_irq_cfg_t * const g_icu_cfg[IRQ_CHANNELS_COUNT];

void external_irq_callback(external_irq_callback_args_t *p_args)
{
    uint32_t ch = p_args->channel;

    if (ch < IRQ_CHANNELS_COUNT && s_irq_used[ch] && s_handler)
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

    core_util_critical_section_enter();
    s_handler = handler;
    core_util_critical_section_exit();

    int irq_channel = pinmap_peripheral(pin, PinMap_IRQ);
    MBED_ASSERT(irq_channel != (int)NC);

    int irq_function = pinmap_function(pin, PinMap_IRQ);
    pin_function(pin, irq_function);

    obj->pin = pin;
    obj->channel  = irq_channel;
    obj->context  = context;

    s_irq_obj[irq_channel] = obj;
    s_irq_used[irq_channel] = 1;

    fsp_err_t err = R_ICU_ExternalIrqOpen(g_icu_ctrl[irq_channel], g_icu_cfg[irq_channel]);
    MBED_ASSERT(err == FSP_SUCCESS);

    R_ICU_ExternalIrqEnable(g_icu_ctrl[irq_channel]);

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    MBED_ASSERT(obj);

    uint32_t ch = obj->channel;
    if (ch >= IRQ_CHANNELS_COUNT)
        return;

    s_irq_used[ch] = 0;

    R_ICU_ExternalIrqDisable(g_icu_ctrl[ch]);
    R_ICU_ExternalIrqClose(g_icu_ctrl[ch]);

    obj->pin = NC;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    /* RA4E1 �� FSP ��֧������ʱ�޸Ĵ�����ʽ����Ҫ���� Open��
       �������Ҫ֧�� RISE/FALL ��̬�л����������������� Open ͨ�� */

    (void)obj;
    (void)event;
    (void)enable;

    /* ���գ�Mbed �� InterruptIn Ĭ��ֻ��Ҫ˫�ش��� */
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
