# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# List of all features libraries available.
add_library(mbed-fpga-ci-test-shield INTERFACE)
add_library(mbed-client-cli INTERFACE)
mbed_add_library(mbed-unity EXCLUDE_FROM_ALL)
mbed_add_library(mbed-utest EXCLUDE_FROM_ALL)

add_subdirectory(frameworks/COMPONENT_FPGA_CI_TEST_SHIELD)
add_subdirectory(frameworks/mbed-client-cli)
add_subdirectory(frameworks/unity)
add_subdirectory(frameworks/utest)
add_subdirectory(frameworks/mbed-greentea-io)
