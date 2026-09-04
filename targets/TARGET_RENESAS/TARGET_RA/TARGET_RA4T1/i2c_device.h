/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef I2C_DEVICE_H
#define I2C_DEVICE_H

#include "r_iic_b_master.h"

typedef struct {
    int hz;
    iic_b_master_clock_settings_t clk;
} iic_clock_entry_t;

static const iic_clock_entry_t g_iic_clock_table[] = {
    { 100000, { .cks_value = 2, .brh_value = 243, .brl_value = 244, .sdod_value = 0, .sdodcs_value = 0 }},
    { 400000, { .cks_value = 0, .brh_value = 225, .brl_value = 226, .sdod_value = 0, .sdodcs_value = 0 }},
    {1000000, { .cks_value = 0, .brh_value = 75, .brl_value = 75, .sdod_value = 0, .sdodcs_value = 0 }},
};

#endif
