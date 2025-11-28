# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_RTC=1" IN_LIST MBED_TARGET_DEFINITIONS)
	set(TEST_SKIPPED "RTC is not supported for this target")
endif()

if(NOT "TARGET_K64F" IN_LIST MBED_TARGET_DEFINITIONS)
	# Currently this test works OK, but causes the next 5-10 programming operations with OpenOCD to fail
	set(TEST_SKIPPED "This test causes issues with this target's debug interface")
endif()

mbed_greentea_add_test(
	TEST_NAME
		mbed-hal-rtc-reset
	TEST_SOURCES
		main.cpp
	HOST_TESTS_DIR
		${CMAKE_CURRENT_SOURCE_DIR}/../../host_tests
	TEST_SKIPPED
		${TEST_SKIPPED}
)