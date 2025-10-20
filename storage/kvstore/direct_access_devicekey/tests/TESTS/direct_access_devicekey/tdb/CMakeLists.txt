# Copyright (c) 2025 Jamie Smith.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_FLASH=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "This test requires flash support!")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-storage-kvstore-direct-access-devicekey
    TEST_SKIPPED
        ${TEST_SKIPPED}
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-storage-direct-access-devicekey
)
