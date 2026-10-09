# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_RESET_REASON=1" IN_LIST MBED_TARGET_DEFINITIONS)
	set(TEST_SKIPPED "Reset Reason is not supported for this target")
endif()

# Both the watchdog and NVIC reset methods seem to prevent K6x boards from being debugged
# again after reset. Not sure what's going on here, may need to dig deeper at some point.
if("MCU_K6x" IN_LIST MBED_TARGET_LABELS)
	set(TEST_SKIPPED "Test would lock out dev board from debugging")
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
