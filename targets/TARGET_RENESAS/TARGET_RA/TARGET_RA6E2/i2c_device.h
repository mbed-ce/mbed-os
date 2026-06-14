#ifndef I2C_DEVICE_H
#define I2C_DEVICE_H

#include "r_iic_b_master.h"

typedef struct {
    int hz;
    iic_b_master_clock_settings_t clk;
} iic_clock_entry_t;

static const iic_clock_entry_t g_iic_clock_table[] = {
    { 100000, { .cks_value = 1, .brh_value = 243, .brl_value = 244, .sdod_value = 0, .sdodcs_value = 0 }},
    { 400000, { .cks_value = 0, .brh_value = 112, .brl_value = 113, .sdod_value = 0, .sdodcs_value = 0 }},
    {1000000, { .cks_value = 0, .brh_value = 37, .brl_value = 37, .sdod_value = 0, .sdodcs_value = 0 }},
};

#endif
