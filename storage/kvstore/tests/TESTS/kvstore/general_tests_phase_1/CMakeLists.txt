# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_TRNG=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "This test requires TRNG support!")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-storage-kvstore-general_tests_phase_1
    TEST_SKIPPED
        ${TEST_SKIPPED}
    MIN_FLASH_BYTES 140000
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-storage
        mbed-storage-filesystemstore
)
