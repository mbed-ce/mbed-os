# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_QSPI=1" IN_LIST MBED_TARGET_DEFINITIONS)
	set(TEST_SKIPPED "QSPI is not supported for this target")
endif()

include_directories(flash_configs)

mbed_greentea_add_test(
	TEST_NAME
		mbed-hal-qspi
	TEST_SOURCES
		main.cpp
        qspi_test_utils.cpp
    TEST_INCLUDE_DIRS
        ${TEST_HEADER_DIRS}
	HOST_TESTS_DIR
		${CMAKE_CURRENT_SOURCE_DIR}/../../host_tests
	TEST_SKIPPED
		${TEST_SKIPPED}
)
