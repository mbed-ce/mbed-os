# Copyright (c) 2026 Jamie Smith.
# SPDX-License-Identifier: Apache-2.0

# Build file for SEGGER RTT

mbed_setup_submodule(RTT CHECK_FILE RTT/SEGGER_RTT.c)

add_library(mbed-rtt STATIC
    RTT/RTT/SEGGER_RTT.c
    RTT/RTT/SEGGER_RTT_ASM_ARMv7M.S
    RTTHandle.cpp)

target_include_directories(mbed-rtt PUBLIC
    .
    RTT/RTT)

target_link_libraries(mbed-rtt mbed-core-flags)