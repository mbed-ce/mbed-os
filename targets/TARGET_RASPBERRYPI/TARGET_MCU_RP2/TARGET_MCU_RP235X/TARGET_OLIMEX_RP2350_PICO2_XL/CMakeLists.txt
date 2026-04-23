# Copyright (c) 2026 Jamie Smith. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-olimex-rp2350-pico2-xl INTERFACE)

target_include_directories(mbed-olimex-rp2350-pico2-xl
    INTERFACE
        .
)

target_link_libraries(mbed-olimex-rp2350-pico2-xl INTERFACE mbed-mcu-rp235x)