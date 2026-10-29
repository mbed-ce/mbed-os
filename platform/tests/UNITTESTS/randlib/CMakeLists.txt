# Copyright (c) 2026 Mbed CE Contributors
# SPDX-License-Identifier: Apache-2.0

include(GoogleTest)

foreach(HARDWARE_ENTROPY IN ITEMS 0 1)
    set(TEST_NAME randlib-seed-${HARDWARE_ENTROPY}-unittest)
    add_executable(${TEST_NAME}
        ${mbed-os_SOURCE_DIR}/platform/randlib/source/arm_hal_random.c
        test_seed.cpp
    )
    target_include_directories(${TEST_NAME} PRIVATE
        .
        ${mbed-os_SOURCE_DIR}/platform/randlib/include/mbed-client-randlib/platform
        ${mbed-os_SOURCE_DIR}/connectivity/libraries/nanostack-libservice/mbed-client-libservice
        ${mbed-os_SOURCE_DIR}/connectivity/mbedtls/include
        ${mbed-os_SOURCE_DIR}/connectivity/mbedtls/platform/inc
    )
    target_compile_definitions(${TEST_NAME} PRIVATE
        MBEDTLS_CONFIG_FILE="seed_config.h"
        TEST_HARDWARE_ENTROPY=${HARDWARE_ENTROPY}
    )
    target_link_libraries(${TEST_NAME} PRIVATE gmock_main)
    gtest_discover_tests(${TEST_NAME}
        TEST_PREFIX "entropy-${HARDWARE_ENTROPY}."
        PROPERTIES LABELS "platform"
    )
endforeach()
