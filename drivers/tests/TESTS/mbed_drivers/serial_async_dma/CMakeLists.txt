# Copyright (c) 2026 Isabella Wu
# SPDX-License-Identifier: Apache-2.0

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
