/* SPDX-License-Identifier: Apache-2.0 */

#if DEVICE_INTERRUPTIN

#include "cmsis.h"
#include "gpio_irq_api.h"
#include "pinmap.h"
#include "platform/mbed_critical.h"

#define GPIO_PORT_COUNT 2U
#define GPIO_PIN_COUNT  16U

static gpio_irq_handler irq_handler;
static uintptr_t contexts[GPIO_PORT_COUNT][GPIO_PIN_COUNT];
static uint32_t events[GPIO_PORT_COUNT][GPIO_PIN_COUNT];
static uint16_t active_pins[GPIO_PORT_COUNT];

extern GPIO_TypeDef *Set_GPIO_Clock(uint32_t port_idx);

static GPIO_TypeDef *gpio_port(uint32_t port)
{
    return port == 0U ? GPIOA : GPIOB;
}

static IRQn_Type gpio_irq_number(uint32_t port)
{
    return port == 0U ? GPIOA_IRQn : GPIOB_IRQn;
}

static uint32_t syscfg_pin_mask(uint32_t port, uint32_t pin)
{
    return 1UL << (pin + (port * 16U));
}

static void gpio_irq_handler_port(uint32_t port)
{
    GPIO_TypeDef *gpio = gpio_port(port);
    uint32_t status = (SYSCFG->IO_ISCR >> (port * 16U)) & active_pins[port];

    for (uint32_t pin = 0U; pin < GPIO_PIN_COUNT; pin++) {
        uint32_t pin_mask = 1UL << pin;

        if ((status & pin_mask) == 0U) {
            continue;
        }

        uint32_t register_mask = syscfg_pin_mask(port, pin);
        uint32_t event = events[port][pin];
        uintptr_t context = contexts[port][pin];

        SYSCFG->IO_ISCR = register_mask;
        if ((context == 0U) || (event == IRQ_NONE)) {
            continue;
        }

        if (event == (IRQ_RISE | IRQ_FALL)) {
            event = (gpio->IDR & pin_mask) != 0U ? IRQ_RISE : IRQ_FALL;
        }

        irq_handler(context, (gpio_irq_event)event);
    }
}

static void gpioa_irq_handler(void)
{
    gpio_irq_handler_port(0U);
}

static void gpiob_irq_handler(void)
{
    gpio_irq_handler_port(1U);
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uintptr_t context)
{
    if (pin == NC) {
        return -1;
    }

    uint32_t port = STM_PORT(pin);
    uint32_t pin_index = STM_PIN(pin);

    if ((port >= GPIO_PORT_COUNT) || (pin_index >= GPIO_PIN_COUNT)) {
        return -1;
    }

    core_util_critical_section_enter();

    uint16_t pin_mask = (uint16_t)(1UL << pin_index);
    if ((active_pins[port] & pin_mask) != 0U) {
        core_util_critical_section_exit();
        return -1;
    }

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    Set_GPIO_Clock(port);

    obj->pin = pin;
    obj->event = IRQ_NONE;
    obj->enabled = 1U;
    irq_handler = handler;
    contexts[port][pin_index] = context;
    events[port][pin_index] = IRQ_NONE;
    active_pins[port] |= pin_mask;

    IRQn_Type irq_n = gpio_irq_number(port);
    NVIC_SetVector(irq_n, port == 0U ? (uint32_t)gpioa_irq_handler : (uint32_t)gpiob_irq_handler);
    NVIC_ClearPendingIRQ(irq_n);
    NVIC_EnableIRQ(irq_n);

    core_util_critical_section_exit();
    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    core_util_critical_section_enter();

    uint32_t port = STM_PORT(obj->pin);
    uint32_t pin = STM_PIN(obj->pin);
    uint16_t pin_mask = (uint16_t)(1UL << pin);
    uint32_t register_mask = syscfg_pin_mask(port, pin);

    SYSCFG->IO_IER &= ~register_mask;
    SYSCFG->IO_IBER &= ~register_mask;
    SYSCFG->IO_IEVR &= ~register_mask;
    SYSCFG->IO_DTR &= ~register_mask;
    SYSCFG->IO_ISCR = register_mask;

    active_pins[port] &= (uint16_t)~pin_mask;
    contexts[port][pin] = 0U;
    events[port][pin] = IRQ_NONE;
    obj->event = IRQ_NONE;
    obj->enabled = 0U;

    if (active_pins[port] == 0U) {
        IRQn_Type irq_n = gpio_irq_number(port);
        NVIC_DisableIRQ(irq_n);
        NVIC_ClearPendingIRQ(irq_n);
    }

    core_util_critical_section_exit();
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    uint32_t port = STM_PORT(obj->pin);
    uint32_t pin = STM_PIN(obj->pin);
    uint32_t register_mask = syscfg_pin_mask(port, pin);

    if (enable != 0U) {
        obj->event |= event;
    } else {
        obj->event &= ~event;
    }
    events[port][pin] = obj->event;

    SYSCFG->IO_DTR &= ~register_mask;
    if (obj->event == (IRQ_RISE | IRQ_FALL)) {
        SYSCFG->IO_IBER |= register_mask;
    } else {
        SYSCFG->IO_IBER &= ~register_mask;
        if (obj->event == IRQ_RISE) {
            SYSCFG->IO_IEVR |= register_mask;
        } else {
            SYSCFG->IO_IEVR &= ~register_mask;
        }
    }

    SYSCFG->IO_ISCR = register_mask;
    if ((obj->enabled != 0U) && (obj->event != IRQ_NONE)) {
        SYSCFG->IO_IER |= register_mask;
    } else {
        SYSCFG->IO_IER &= ~register_mask;
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    uint32_t port = STM_PORT(obj->pin);
    uint32_t pin = STM_PIN(obj->pin);
    uint32_t register_mask = syscfg_pin_mask(port, pin);

    obj->enabled = 1U;
    SYSCFG->IO_ISCR = register_mask;
    if (obj->event != IRQ_NONE) {
        SYSCFG->IO_IER |= register_mask;
    }
    NVIC_EnableIRQ(gpio_irq_number(port));
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    uint32_t port = STM_PORT(obj->pin);
    uint32_t pin = STM_PIN(obj->pin);
    uint32_t register_mask = syscfg_pin_mask(port, pin);

    obj->enabled = 0U;
    SYSCFG->IO_IER &= ~register_mask;
    SYSCFG->IO_ISCR = register_mask;
}

#endif /* DEVICE_INTERRUPTIN */
