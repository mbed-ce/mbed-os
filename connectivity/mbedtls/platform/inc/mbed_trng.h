/*
 * Copyright (c) 2026 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MBED_TRNG_H
#define MBED_TRNG_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen);

#ifdef __cplusplus
}
#endif

#endif /* MBED_TRNG_H */
