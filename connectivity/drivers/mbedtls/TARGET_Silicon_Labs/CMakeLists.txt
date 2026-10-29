# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-mbedtls
    PRIVATE
        ${MBED_PATH}/connectivity/mbedtls/source
    PUBLIC
        .
)

target_sources(mbed-mbedtls
    PRIVATE
        aes_aes.c
        crypto_aes.c
        crypto_ecp.c
        crypto_management.c
        crypto_sha.c
)
