/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed_assert.h"
#include "port_api.h"
#include "r_ioport.h"
#include "hal_data.h"

void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    obj->port = port;
    obj->mask = mask;
    obj->direction = dir;

    R_IOPORT_PortDirectionSet(&g_ioport_ctrl, (bsp_io_port_t) obj->port, obj->mask,
                              (dir == PIN_OUTPUT) ? 0xFFFFFFFFF : 0x00000000);
}

void port_write(port_t *obj, int value)
{
    R_IOPORT_PortWrite(&g_ioport_ctrl, (bsp_io_port_t) obj->port, value, obj->mask);
}

int port_read(port_t *obj)
{
    uint16_t value;
    R_IOPORT_PortRead(&g_ioport_ctrl, (bsp_io_port_t) obj->port, &value);
    return value & obj->mask;
}

void port_dir(port_t *obj, PinDirection dir)
{
    R_IOPORT_PortDirectionSet(&g_ioport_ctrl, (bsp_io_port_t) obj->port, obj->mask,
                              (dir == PIN_OUTPUT) ? 0xFFFFFFFFF : 0x00000000);
}
