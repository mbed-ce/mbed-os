# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# List of all features libraries available.
add_library(mbed-fpga-ci-test-shield INTERFACE)
add_library(mbed-client-cli INTERFACE)

add_subdirectory(frameworks/COMPONENT_FPGA_CI_TEST_SHIELD)
add_subdirectory(frameworks/mbed-client-cli)
add_subdirectory(frameworks/cy_rtos_rtx_adapter)

if(MBED_ENABLE_TESTING)
    add_library(mbed-unity STATIC EXCLUDE_FROM_ALL)
    add_library(mbed-utest STATIC EXCLUDE_FROM_ALL)
    add_subdirectory(frameworks/unity)
    add_subdirectory(frameworks/utest)
    add_subdirectory(frameworks/mbed-greentea-io)
endif()