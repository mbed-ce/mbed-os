# Copyright (c) 2025 Jamie Smith.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_TRNG=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "This test requires TRNG support!")
endif()

if(NOT "DEVICE_FLASH=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "This test requires flash API support!")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-storage-kvstore-filesystemstore
    TEST_SKIPPED
        ${TEST_SKIPPED}
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-storage
        mbed-storage-filesystemstore
)
