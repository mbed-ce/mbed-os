/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "trng_api.h"
#include "common_data.h"
#include "mbed_error.h"

#ifdef TARGET_RA4E1

// In RA4E1, TRNG is part of SCE9

#include "r_sce.h"

void trng_init(trng_t *obj)
{
    (void)obj;
    fsp_err_t err = FSP_SUCCESS;
    err = R_SCE_Open(&sce_ctrl, &sce_cfg);
    if (err != FSP_SUCCESS)
    {
        mbed_error(MBED_ERROR_CODE_OPEN_FAILED, "Failed to open SCE for random number generator", 0, MBED_FILENAME, __LINE__);
    }
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *actual)
{
    (void)obj;

    uint32_t rnd[4];
    size_t generated = 0;

    while (generated < length)
    {
        fsp_err_t err = R_SCE_RandomNumberGenerate(rnd);

        if (err != FSP_SUCCESS)
        {
            *actual = generated;
            return -1;
        }

        size_t remain = length - generated;
        size_t copy_len = (remain >= 16) ? 16 : remain;

        memcpy(output + generated, rnd, copy_len);
        generated += copy_len;
    }

    *actual = generated;
    return 0;
}

void trng_free(trng_t *obj)
{
    (void)obj;
}

#elif defined(TARGET_RA6E2)

// In RA6E2, TRNG is standalone and only accessible via registers

void trng_init(trng_t *obj)
{
    (void)obj;

    /* Cancel Random Number Generator Module Stop */
    R_MSTP->MSTPCRC_b.MSTPC28 = 0;

    /* SEED Generation Circuit Enable */
    R_TRNG->TRNGSCR0_b.SGCEN = 1;
}


int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *actual)
{
    (void)obj;

    uint32_t previous_seed = 0xFFFFFFFF;
    size_t generated = 0;

    while (generated < length)
    {
        /* Start SEED generation */
        R_TRNG->TRNGSCR0_b.SGSTART = 1;

        while (R_TRNG->TRNGSCR0_b.RDRDY == 0)
        {
            /* wait */
        }

        /* Read 32-bit SEEDs */
        uint32_t seed = 0;
        seed |= ((uint32_t)R_TRNG->TRNGSDR) << 24;
        seed |= ((uint32_t)R_TRNG->TRNGSDR) << 16;
        seed |= ((uint32_t)R_TRNG->TRNGSDR) << 8;
        seed |= ((uint32_t)R_TRNG->TRNGSDR);

        /* FIPS 140-2 continuous test */
        if (seed == previous_seed)
        {
            return 0;
        }
        previous_seed = seed;

        /* output as bytes */
        for (int i = 0; i < 4 && generated < length; i++)
        {
            output[generated++] = (seed >> (24 - i * 8)) & 0xFF;
        }
    }

    *actual = generated;
    return 0;
}

void trng_free(trng_t *obj)
{
    (void)obj;

    /* SEED Generation Circuit Disable */
    R_TRNG->TRNGSCR0_b.SGCEN = 0;

    /* Random Number Generator Module Stop */
    R_MSTP->MSTPCRC_b.MSTPC28 = 1;
}

#elif defined(TARGET_RA8E1)

// In RA8E1, TRNG is part of RSIP

#include "r_rsip.h"

void trng_init(trng_t *obj)
{
    (void)obj;
    fsp_err_t err = FSP_SUCCESS;
    err = R_RSIP_Open(&g_rsip_ctrl, &g_rsip_cfg);
    if (err != FSP_SUCCESS)
    {
        mbed_error(MBED_ERROR_CODE_OPEN_FAILED, "Failed to open RSIP for random number generator", 0, MBED_FILENAME, __LINE__);
    }
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *actual)
{
    (void)obj;

    uint32_t rnd[4];
    size_t generated = 0;

    while (generated < length)
    {
        fsp_err_t err = R_RSIP_RandomNumberGenerate(&g_rsip_ctrl, (uint8_t*) rnd);

        if (err != FSP_SUCCESS)
        {
            *actual = generated;
            return -1;
        }

        size_t remain = length - generated;
        size_t copy_len = (remain >= 16) ? 16 : remain;

        memcpy(output + generated, rnd, copy_len);
        generated += copy_len;
    }

    *actual = generated;
    return 0;
}

void trng_free(trng_t *obj)
{
    (void)obj;
}

#else

void trng_init(trng_t *obj)
{
    (void)obj;
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *actual)
{
    (void)obj;

    return -1;
}

void trng_free(trng_t *obj)
{
    (void)obj;
}

#endif
