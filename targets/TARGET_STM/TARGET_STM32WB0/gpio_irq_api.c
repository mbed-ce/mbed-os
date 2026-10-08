/* Copyright (c) 2026 MbedCE Community Contributors (Jan Kamidra)
 * SPDX-License-Identifier: Apache-2.0
 */

#if DEVICE_INTERRUPTIN

#include "cmsis.h"
#include "mbed_error.h"
#include "gpio_irq_api.h"
#include "pinmap.h"
#include "platform/mbed_critical.h"

#define GPIO_PORT_COUNT 2U
#define GPIO_PIN_COUNT  16U

typedef struct {
    uintptr_t context;
    uint32_t events;
} gpio_irq_state_t;

static gpio_irq_handler irq_handler;
static gpio_irq_state_t irq_states[GPIO_PORT_COUNT][GPIO_PIN_COUNT];
static uint32_t active_pins[GPIO_PORT_COUNT];

static uint32_t syscfg_pin_bit(uint32_t port, uint32_t pin)
{
    // 32bits for 2 ports with 16 pins each, bit position = pin + (port * 16)
    return 1UL << (pin + (port * 16U));
}

static void gpio_irq_handler_port(uint32_t port)
{
    // Get the pending flags of active pins for the port
    uint32_t status = (SYSCFG->IO_ISCR >> (port * 16U)) & active_pins[port];
    
    // Go trought all pins of the GPIO port and call the callback
    // for each pin with a pending flag, skip rest
    for (uint32_t pin = 0U; pin < GPIO_PIN_COUNT; pin++) {
        uint32_t pin_bit = 1UL << pin;

        // Skip pins with no pending event
        if ((status & pin_bit) == 0U) {
            continue;
        }

        // Get pin's callback configuration
        uint32_t syscfg_bit = syscfg_pin_bit(port, pin);
        gpio_irq_state_t *state = &irq_states[port][pin];
        uint32_t event = state->events;

        // Acknowledge pin's pending flag
        SYSCFG->IO_ISCR = syscfg_bit;

        // Skip callbacks for masked pins
        if ((SYSCFG->IO_IER & syscfg_bit) == 0U) {
            continue;
        }

        // Skip callbacks without enabled edges
        if (event == IRQ_NONE) {
            continue;
        }

        // Get GPIO port base address
        GPIO_TypeDef *gpio = (port == PortA) ? GPIOA : GPIOB;
        // For both-edge mode, get the edge from the current pin level.
        // For single-edge mode, keep the configured edge.
        if (event == (IRQ_RISE | IRQ_FALL)) {
            event = (gpio->IDR & pin_bit) != 0U ? IRQ_RISE : IRQ_FALL;
        }

        irq_handler(state->context, (gpio_irq_event)event);
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
    uint32_t pin_bit = 1UL << pin_index;

    // Check if the GPIO port is valid
    if (port >= GPIO_PORT_COUNT) {
        error("InterruptIn error: unsupported gpio port.\n");
        return -1;
    }

    core_util_critical_section_enter();
    
    bool first_pin = (active_pins[port] == 0U);

    // Check if the pin is already in use
    if ((active_pins[port] & pin_bit) != 0U) {
        error("InterruptIn error: pin conflict\n");
        return -1;
    } else {
        active_pins[port] |= pin_bit;
    }

    // Enable SYSCFG Clock
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    // Enable GPIO clock, select irq handler and interrupt according to GPIO port
    uint32_t vector = 0;
    IRQn_Type irq_n;
    if(port == PortA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        vector  = (uint32_t)gpioa_irq_handler;
        irq_n = GPIOA_IRQn;
    } else if(port == PortB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        vector  = (uint32_t)gpiob_irq_handler;
        irq_n = GPIOB_IRQn;
    } else {
        error("InterruptIn error: unsupported gpio port.\n");
        return -1;
    }

    // Save informations for future use
    obj->pin = pin; // Store pin not pin index
    obj->irq_n = irq_n;
    obj->event = IRQ_NONE;
    obj->enabled = 1U;
    irq_handler = handler;
    irq_states[port][pin_index].context = context;
    irq_states[port][pin_index].events = IRQ_NONE;
    
    // Set and enable the interrupt vector if this is the first pin for the port
    if (first_pin) {
        NVIC_SetVector(obj->irq_n, vector);
        NVIC_EnableIRQ(obj->irq_n);
    }

    core_util_critical_section_exit();
    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    core_util_critical_section_enter();

    uint32_t port = STM_PORT(obj->pin);
    uint32_t pin_index = STM_PIN(obj->pin);
    uint32_t pin_bit = 1UL << pin_index;
    uint32_t syscfg_bit = syscfg_pin_bit(port, pin_index);

    // Reset registers for the pin and clear the pending flag
    SYSCFG->IO_IER &= ~syscfg_bit;
    SYSCFG->IO_IBER &= ~syscfg_bit;
    SYSCFG->IO_IEVR &= ~syscfg_bit;
    SYSCFG->IO_DTR &= ~syscfg_bit;
    SYSCFG->IO_ISCR = syscfg_bit;

    // Clear the pin from the active pins and reset the irq state
    active_pins[port] &= ~pin_bit;
    irq_states[port][pin_index].context = 0U;
    irq_states[port][pin_index].events = IRQ_NONE;
    obj->event = IRQ_NONE;
    obj->enabled = 0U;

    // Disable the interrupt if no more active pins for the port
    if (active_pins[port] == 0U) {
        NVIC_DisableIRQ(obj->irq_n);
        NVIC_ClearPendingIRQ(obj->irq_n);
    }

    core_util_critical_section_exit();
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    uint32_t port = STM_PORT(obj->pin);
    uint32_t pin_index = STM_PIN(obj->pin);
    uint32_t syscfg_bit = syscfg_pin_bit(port, pin_index);

    // Add or remove the requested edge while preserving the others
    if (enable != 0U) {
        obj->event |= event;
    } else {
        obj->event &= ~event;
    }
    // Update the enabled edges
    irq_states[port][pin_index].events = obj->event;

    // Set edge detection
    SYSCFG->IO_DTR &= ~syscfg_bit;
    if (obj->event == (IRQ_RISE | IRQ_FALL)) {
        // Enable detection of both rising and falling edges.
        SYSCFG->IO_IBER |= syscfg_bit;
    } else {
        // Select a single edge: bit set for rising, cleared for falling.
        SYSCFG->IO_IBER &= ~syscfg_bit;
        if (obj->event == IRQ_RISE) {
            SYSCFG->IO_IEVR |= syscfg_bit;
        } else {
            SYSCFG->IO_IEVR &= ~syscfg_bit;
        }
    }

    // Discard any pending event from the previous pin configuration.
    SYSCFG->IO_ISCR = syscfg_bit;
    // Enable the pin's interrupt only if enabled by the object and an edge is selected.
    if ((obj->enabled != 0U) && (obj->event != IRQ_NONE)) {
        SYSCFG->IO_IER |= syscfg_bit;
    } else {
        SYSCFG->IO_IER &= ~syscfg_bit;
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    uint32_t port = STM_PORT(obj->pin);
    uint32_t pin_index = STM_PIN(obj->pin);
    uint32_t syscfg_bit = syscfg_pin_bit(port, pin_index);

    obj->enabled = 1U;
    // Clear any pending event before enabling this pin's interrupt
    SYSCFG->IO_ISCR = syscfg_bit;
    // Enable the interrupt source only if at least one edge is configured.
    if (obj->event != IRQ_NONE) {
        SYSCFG->IO_IER |= syscfg_bit;
    }
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    uint32_t port = STM_PORT(obj->pin);
    uint32_t pin_index = STM_PIN(obj->pin);
    uint32_t syscfg_bit = syscfg_pin_bit(port, pin_index);

    obj->enabled = 0U;
    // Disable only this pin
    SYSCFG->IO_IER &= ~syscfg_bit;
    // Discard any pending event for this pin.
    SYSCFG->IO_ISCR = syscfg_bit;
}

#endif /* DEVICE_INTERRUPTIN */
