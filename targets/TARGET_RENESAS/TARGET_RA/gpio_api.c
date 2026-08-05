/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "gpio_api.h"
#include "r_ioport.h"
#include "common_data.h"

static inline bsp_io_port_pin_t pin_to_bsp(PinName pin)
{
    return (bsp_io_port_pin_t)pin;
}

static inline bsp_io_port_t pin_to_port(PinName pin)
{
    return (bsp_io_port_t)(pin >> 8);
}

static inline uint32_t pin_to_mask(PinName pin)
{
    return (1U << (pin & 0xFF));
}

void gpio_init(gpio_t *obj, PinName pin)
{
    obj->pin = pin;
    obj->port = pin_to_port(pin);
    obj->pin_mask = pin_to_mask(pin);

    // Keep IRQ flag
#if (3U == BSP_FEATURE_IOPORT_VERSION)
    uint32_t cfg = R_PFS->PORT[pin >> 8].PIN[pin & BSP_IO_PRV_8BIT_MASK].PmnPFS;
#else
    uint32_t cfg = R_PFS->PORT[pin >> 8].PIN[pin & BSP_IO_PRV_8BIT_MASK].PmnPFS;
#endif
    R_BSP_PinCfg(pin_to_bsp(pin), (cfg & IOPORT_CFG_IRQ_ENABLE) ? IOPORT_CFG_IRQ_ENABLE: IOPORT_CFG_PORT_DIRECTION_INPUT);
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    PinName pin = obj->pin;
#if (3U == BSP_FEATURE_IOPORT_VERSION)
    uint32_t cfg = R_PFS->PORT[pin >> 8].PIN[pin & BSP_IO_PRV_8BIT_MASK].PmnPFS;
#else
    uint32_t cfg = R_PFS->PORT[pin >> 8].PIN[pin & BSP_IO_PRV_8BIT_MASK].PmnPFS;
#endif

    switch (mode) {
        case PullUp:
            cfg |= IOPORT_CFG_PULLUP_ENABLE;
            cfg &= ~IOPORT_CFG_NMOS_ENABLE;
            break;
        case OpenDrain:
            cfg &= ~IOPORT_CFG_PULLUP_ENABLE;
            cfg |= IOPORT_CFG_NMOS_ENABLE;
            break;
        case PullDown:
        case PullNone:
        default:
            cfg &= ~(IOPORT_CFG_PULLUP_ENABLE|IOPORT_CFG_NMOS_ENABLE);
            break;
    }

    R_BSP_PinCfg(pin_to_bsp(pin), cfg);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    PinName pin = obj->pin;
#if (3U == BSP_FEATURE_IOPORT_VERSION)
    uint32_t cfg = R_PFS->PORT[pin >> 8].PIN[pin & BSP_IO_PRV_8BIT_MASK].PmnPFS;
#else
    uint32_t cfg = R_PFS->PORT[pin >> 8].PIN[pin & BSP_IO_PRV_8BIT_MASK].PmnPFS;
#endif

    if (direction == PIN_OUTPUT) {
        cfg |= IOPORT_CFG_PORT_DIRECTION_OUTPUT;
    } else {
        cfg &= ~IOPORT_CFG_PORT_DIRECTION_OUTPUT;
    }

    R_BSP_PinCfg(pin_to_bsp(obj->pin), cfg);
}

void gpio_write(gpio_t *obj, int value)
{
    R_BSP_PinWrite(pin_to_bsp(obj->pin), value ? BSP_IO_LEVEL_HIGH : BSP_IO_LEVEL_LOW);
}

int gpio_read(gpio_t *obj)
{
    return (R_BSP_PinRead(pin_to_bsp(obj->pin)) != 0) ? 1 : 0;
}

int gpio_is_connected(const gpio_t *obj)
{
    if (obj->pin == NC) {
        return 0;
    } else {
        return 1;
    }
}
