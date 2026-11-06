# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ff-lpc546xx INTERFACE)
target_link_libraries(mbed-ff-lpc546xx INTERFACE mbed-mcu-lpc546xx)

target_include_directories(mbed-ff-lpc546xx INTERFACE .)
target_sources(mbed-ff-lpc546xx INTERFACE
    clock_config.c
)