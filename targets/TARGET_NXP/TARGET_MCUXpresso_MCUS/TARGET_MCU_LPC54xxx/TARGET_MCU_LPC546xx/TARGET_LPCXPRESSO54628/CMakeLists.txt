# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-lpcxpresso54628 INTERFACE)
target_link_libraries(mbed-lpcxpresso54628 INTERFACE mbed-mcu-lpc546xx)

target_include_directories(mbed-lpcxpresso54628 INTERFACE .)
target_sources(mbed-lpcxpresso54628 INTERFACE
        clock_config.c
        lpc546xx_sdram.c
)