# Copyright (c) 2026 Jan Gerrit Gers
# SPDX-License-Identifier: Apache-2.0

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-mbedtls-aes-cbc
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-mbedtls
)
