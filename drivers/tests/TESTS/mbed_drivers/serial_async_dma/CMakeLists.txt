# Copyright (c) 2026 Isabella Wu
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_SERIAL_ASYNCH=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "Asynchronous serial communication not supported for this target")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-drivers-serial-async-dma
    TEST_SOURCES
        main.cpp
    HOST_TESTS_DIR
        ../../host_tests
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
