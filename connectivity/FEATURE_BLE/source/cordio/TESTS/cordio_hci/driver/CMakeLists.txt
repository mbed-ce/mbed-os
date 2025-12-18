# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("BlueNRG_2" IN_LIST MBED_TARGET_LABELS OR "BlueNRG_MS" IN_LIST MBED_TARGET_LABELS)
    if(APPLICATION_PROFILE_CONFIG_BAREMETAL)
        set(TEST_SKIPPED "BlueNRG bluetooth modules require an RTOS!")
    endif()
endif()


mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-ble-cordio-hci-driver
    TEST_SKIPPED
        ${TEST_SKIPPED}
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-ble
)
