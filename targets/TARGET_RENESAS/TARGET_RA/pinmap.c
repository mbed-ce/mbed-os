#include "pinmap.h"
#include "common_data.h"

void pin_mode(PinName pin, PinMode mode)
{
    if (pin == NC) {
        return;
    }

    uint32_t cfg = 0;

    switch (mode) {
        case PullNone:
            cfg |= IOPORT_CFG_PORT_DIRECTION_INPUT;
            break;

        case PullUp:
            cfg |= IOPORT_CFG_PORT_DIRECTION_INPUT |
                   IOPORT_CFG_PULLUP_ENABLE;
            break;

        case PullDown:
            // PullDown not supported on RA
            cfg |= IOPORT_CFG_PORT_DIRECTION_INPUT;
            break;

        case OpenDrain:
            cfg |= IOPORT_CFG_PORT_DIRECTION_OUTPUT |
                   IOPORT_CFG_NMOS_ENABLE;
            break;

        default:
            cfg |= IOPORT_CFG_PORT_DIRECTION_INPUT;
            break;
    }

    R_IOPORT_PinCfg(g_ioport.p_ctrl, (bsp_io_port_pin_t) pin, cfg);
}

void pin_function(PinName pin, int function)
{
    uint32_t mode = RA_PIN_MODE(function);
    uint32_t pull = RA_PIN_PULL(function);
    uint32_t alt  = RA_PIN_ALT(function);

    uint32_t cfg = 0;

    switch (mode) {
        case RA_PIN_MODE_ANALOG:
            cfg |= IOPORT_CFG_ANALOG_ENABLE;
            break;

        case RA_PIN_MODE_PERIPHERAL_PP:
            cfg |= IOPORT_CFG_PERIPHERAL_PIN;
            cfg |= ((alt & 0x1F) << IOPORT_PRV_PFS_PSEL_OFFSET);
            break;

        case RA_PIN_MODE_PERIPHERAL_OD:
            cfg |= IOPORT_CFG_PERIPHERAL_PIN;
            cfg |= IOPORT_CFG_NMOS_ENABLE;
            cfg |= ((alt & 0x1F) << IOPORT_PRV_PFS_PSEL_OFFSET);
            break;

        case RA_PIN_MODE_IRQ:
            cfg |= IOPORT_CFG_IRQ_ENABLE;
            break;
    }

    // pull-up/down
    if (pull == RA_PIN_PULL_UP) {
        cfg |= IOPORT_CFG_PULLUP_ENABLE;
    } else if (pull == RA_PIN_PULL_DOWN) {
        // not supported by RA
    }

    R_IOPORT_PinCfg(g_ioport.p_ctrl, (bsp_io_port_pin_t) pin, cfg);
}
