/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "crc_api.h"
#include "r_crc.h"
#include "hal_data.h"

static crc_instance_ctrl_t g_crc_ctrl;
static crc_cfg_t g_crc_cfg =
{
    .polynomial      = CRC_POLYNOMIAL_CRC_32,
    .bit_order       = CRC_BIT_ORDER_LMS_LSB,
    .snoop_address   = 0,
    .p_extend        = NULL,
};

static uint32_t g_crc_state = 0;
static uint32_t g_crc_initial = 0;
static uint32_t g_crc_final_xor = 0;
static bool g_crc_reflect_out = false;
static uint8_t g_crc_width = 32;

static uint32_t reflect_bits(uint32_t data, uint8_t width)
{
    uint32_t out = 0;
    for (uint8_t i = 0; i < width; i++)
    {
        out <<= 1;
        out |= (data & 1);
        data >>= 1;
    }
    return out;
}

void crc_init(const crc_mbed_config_t *config)
{
    g_crc_initial = config->initial_xor;
    g_crc_final_xor = config->final_xor;
    g_crc_reflect_out = config->reflect_out;
    g_crc_width = config->width;

    switch (config->polynomial)
    {
        case POLY_8BIT_CCITT:
            g_crc_cfg.polynomial = CRC_POLYNOMIAL_CRC_8;
            break;
        case POLY_16BIT_CCITT:
            g_crc_cfg.polynomial = CRC_POLYNOMIAL_CRC_16;
            break;
        case POLY_32BIT_ANSI:
        default:
            g_crc_cfg.polynomial = CRC_POLYNOMIAL_CRC_32;
            break;
    }

    g_crc_cfg.bit_order = config->reflect_in ?
                          CRC_BIT_ORDER_LMS_LSB :
                          CRC_BIT_ORDER_LMS_MSB;

    R_CRC_Open(&g_crc_ctrl, &g_crc_cfg);

    g_crc_state = g_crc_initial;
}

void crc_update(const void *data, size_t size)
{
    crc_input_t input;
    input.num_bytes = size;
    input.crc_seed = g_crc_state;
    input.p_input_buffer = data;

    uint32_t result = 0;
    R_CRC_Calculate(&g_crc_ctrl, &input, &result);

    g_crc_state = result;
}

uint32_t crc_get_result(void)
{
    uint32_t result = g_crc_state;

    if (g_crc_reflect_out)
        result = reflect_bits(result, g_crc_width);

    result ^= g_crc_final_xor;

    return result;
}
