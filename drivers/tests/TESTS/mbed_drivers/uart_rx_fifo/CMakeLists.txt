# Copyright (c) 2026 Arm Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "NUCLEO_H753ZI" IN_LIST MBED_TARGET_LABELS)
    set(TEST_SKIPPED "Test requires an STM32 target with a hardware UART FIFO")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-drivers-uart-rx-fifo
    TEST_SOURCES
        main.cpp
    HOST_TESTS_DIR
        ../../host_tests
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
