# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED=1" IN_LIST MBED_CONFIG_DEFINITIONS)
	set(TEST_SKIPPED "MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED is not enabled for this target")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-platform-crash-reporting
    TEST_SOURCES
        main.cpp
    HOST_TESTS_DIR
        "${CMAKE_CURRENT_LIST_DIR}/../../host_tests"
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
