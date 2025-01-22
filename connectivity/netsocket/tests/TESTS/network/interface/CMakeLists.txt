# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

list(
    APPEND
        TEST_SOURCE_LIST
            main.cpp
            networkinterface_conn_disc_repeat.cpp
            networkinterface_status.cpp
)

if(APPLICATION_PROFILE_CONFIG_BAREMETAL)
    set(TEST_SKIPPED "This test requires an RTOS!")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-netsocket-lwipstack-network-interface
    TEST_SOURCES
        ${TEST_SOURCE_LIST}
    TEST_REQUIRED_LIBS
        mbed-netsocket
        mbed-netsocket-tests-common-inc
        mbed-netsocket-api
        mbed-netsocket-tests-use-lwipstack
    TEST_SKIPPED
        ${TEST_SKIPPED}
)

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-netsocket-nanostack-network-interface
    TEST_SOURCES
        ${TEST_SOURCE_LIST}
    TEST_REQUIRED_LIBS
        mbed-netsocket
        mbed-netsocket-tests-common-inc
        mbed-netsocket-api
        mbed-netsocket-tests-use-nanostack
    TEST_SKIPPED
        ${TEST_SKIPPED}
)

