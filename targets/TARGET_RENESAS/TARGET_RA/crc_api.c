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
static bool g_crc_reflect_in = false;
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

void hal_crc_compute_partial_start(const crc_mbed_config_t *config)
{
    g_crc_initial = config->initial_xor;
    g_crc_final_xor = config->final_xor;
    g_crc_reflect_out = config->reflect_out;
    g_crc_reflect_in = config->reflect_in;
    g_crc_width = config->width;

    uint32_t mask = (g_crc_width == 32) ? 0xFFFFFFFFUL : ((1UL << g_crc_width) - 1);
    g_crc_initial &= mask;

    switch (config->polynomial)
    {
        case POLY_8BIT_CCITT:
            g_crc_cfg.polynomial = CRC_POLYNOMIAL_CRC_8;
            break;
        case POLY_16BIT_IBM:
            g_crc_cfg.polynomial = CRC_POLYNOMIAL_CRC_16;
            break;
        case POLY_16BIT_CCITT:
            g_crc_cfg.polynomial = CRC_POLYNOMIAL_CRC_CCITT;
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

    if (g_crc_reflect_in) {
        g_crc_state = reflect_bits(g_crc_initial, g_crc_width);
    } else {
        g_crc_state = g_crc_initial;
    }
}

void hal_crc_compute_partial(const uint8_t *data, size_t size)
{
    if (data == NULL || size == 0) return;

    size_t hw_size = size;
    if (g_crc_width == 32) {
        hw_size = size & ~0x03U;
    }

    if (hw_size > 0) {
        if (g_crc_width == 32 && g_crc_cfg.bit_order == CRC_BIT_ORDER_LMS_MSB) {
            uint32_t swapped;
            for (size_t offset = 0; offset < hw_size; offset += 4) {
                swapped = ((uint32_t)data[offset]     << 24) |
                          ((uint32_t)data[offset + 1] << 16) |
                          ((uint32_t)data[offset + 2] << 8)  |
                          ((uint32_t)data[offset + 3]);

                crc_input_t input;
                input.num_bytes = 4;
                input.p_input_buffer = &swapped;
                input.crc_seed = g_crc_state;
                uint32_t result = 0;
                R_CRC_Calculate(&g_crc_ctrl, &input, &result);
                g_crc_state = result;
            }
        } else {
            crc_input_t input;
            input.num_bytes = hw_size;
            input.p_input_buffer = (void *)data;
            input.crc_seed = g_crc_state;
            uint32_t result = 0;
            R_CRC_Calculate(&g_crc_ctrl, &input, &result);
            g_crc_state = result;
        }
        data += hw_size;
    }

    size_t rem_size = size - hw_size;
    if (rem_size > 0 && g_crc_width == 32) {
        uint32_t crc = g_crc_state;
        if (g_crc_cfg.bit_order == CRC_BIT_ORDER_LMS_LSB) {
            uint32_t poly = (g_crc_cfg.polynomial == CRC_POLYNOMIAL_CRC_32)
                            ? 0xEDB88320U : 0x82F63B78U;
            for (size_t i = 0; i < rem_size; i++) {
                crc ^= data[i];
                for (int j = 0; j < 8; j++) {
                    if (crc & 1) crc = (crc >> 1) ^ poly;
                    else         crc >>= 1;
                }
            }
        } else {
            uint32_t poly = (g_crc_cfg.polynomial == CRC_POLYNOMIAL_CRC_32)
                            ? 0x04C11DB7U : 0x1EDC6F41U;
            for (size_t i = 0; i < rem_size; i++) {
                crc ^= ((uint32_t)data[i] << 24);
                for (int j = 0; j < 8; j++) {
                    if (crc & 0x80000000U) crc = (crc << 1) ^ poly;
                    else                   crc <<= 1;
                }
            }
        }
        g_crc_state = crc;
    }
}

uint32_t hal_crc_get_result(void)
{
    uint32_t mask = (g_crc_width == 32) ? 0xFFFFFFFFUL : ((1UL << g_crc_width) - 1);
    uint32_t result = g_crc_state & mask;

    if (g_crc_reflect_out != g_crc_reflect_in) {
        result = reflect_bits(result, g_crc_width);
    }

    result ^= g_crc_final_xor;
    return result & mask;
}
