# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT "DEVICE_LPTICKER=1" IN_LIST MBED_TARGET_DEFINITIONS OR NOT "DEVICE_SLEEP=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "Stats cpu test not supported.")
endif()

if((NOT "MBED_CPU_STATS_ENABLED=1" IN_LIST MBED_CONFIG_DEFINITIONS) AND (NOT "MBED_ALL_STATS_ENABLED=1" IN_LIST MBED_CONFIG_DEFINITIONS))
	set(TEST_SKIPPED "CPU stats not enabled")
endif()

if("MBED_CONF_TARGET_APPLICATION_PROFILE=bare-metal" IN_LIST MBED_CONFIG_DEFINITIONS)
    set(TEST_SKIPPED "This test requires the RTOS")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-platform-stats-cpu
    TEST_SOURCES
        main.cpp
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
