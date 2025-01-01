# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_RESET_REASON=1" IN_LIST MBED_TARGET_DEFINITIONS)
	set(TEST_SKIPPED "Reset Reason is not supported for this target")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-drivers-reset-reason
    TEST_SOURCES
        main.cpp
    HOST_TESTS_DIR
		../../host_tests
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
