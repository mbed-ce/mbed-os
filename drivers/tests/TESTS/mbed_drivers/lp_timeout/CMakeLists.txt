# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_LPTICKER=1" IN_LIST MBED_TARGET_DEFINITIONS)
	set(TEST_SKIPPED "Low power ticker not supported for this target")
endif()

if(APPLICATION_PROFILE_CONFIG_BAREMETAL)
    set(TEST_SKIPPED "Test not supported on baremetal")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-drivers-lp-timeout
    TEST_SOURCES
        main.cpp
    HOST_TESTS_DIR
		../../host_tests
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
