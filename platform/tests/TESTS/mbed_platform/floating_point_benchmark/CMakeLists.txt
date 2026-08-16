# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

mbed_greentea_add_test(
    TEST_NAME
        mbed-platform-floating-point-benchmark
    TEST_SOURCES
        test_mbed_platform_fp_benchmark.cpp
)
