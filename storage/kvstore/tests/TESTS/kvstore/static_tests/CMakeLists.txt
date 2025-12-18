# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("TFM_DUALCPU" IN_LIST MBED_TARGET_LABELS AND APPLICATION_PROFILE_CONFIG_BAREMETAL)
    set(TEST_SKIPPED "KVStore requires the RTOS on this platform")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-storage-kvstore-static_tests
    TEST_SKIPPED
        ${TEST_SKIPPED}
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-storage-kv-global-api
    NEEDED_FLASH_KIB 70
)
