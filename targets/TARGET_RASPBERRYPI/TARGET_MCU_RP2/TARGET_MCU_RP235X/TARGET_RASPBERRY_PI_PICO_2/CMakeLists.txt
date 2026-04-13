# Copyright (c) 2026 Jamie Smith. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-raspberry-pi-pico-2 INTERFACE)

target_include_directories(mbed-raspberry-pi-pico-2
    INTERFACE
        .
)

target_link_libraries(mbed-raspberry-pi-pico-2 INTERFACE mbed-mcu-rp235x)