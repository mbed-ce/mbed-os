#ifndef I2C_DEVICE_H
#define I2C_DEVICE_H

#include "r_iic_master.h"

typedef struct {
    int hz;
    iic_master_clock_settings_t clk;
} iic_clock_entry_t;

static const iic_clock_entry_t g_iic_clock_table[] = {
    { 100000, { .cks_value = 3, .brh_value = 28, .brl_value = 28, .sddl_value = 0, .dlcs_value = 0 }},
    { 400000, { .cks_value = 1, .brh_value = 26, .brl_value = 25, .sddl_value = 0, .dlcs_value = 0 }},
    {1000000, { .cks_value = 0, .brh_value = 15, .brl_value = 15, .sddl_value = 0, .dlcs_value = 0 }},
};

#endif
