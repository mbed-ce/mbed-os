# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if((NOT "MBED_THREAD_STATS_ENABLED=1" IN_LIST MBED_CONFIG_DEFINITIONS) AND (NOT "MBED_ALL_STATS_ENABLED=1" IN_LIST MBED_CONFIG_DEFINITIONS))
	set(TEST_SKIPPED "Thread stats not enabled")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-platform-stats-thread
    TEST_SOURCES
        main.cpp
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
