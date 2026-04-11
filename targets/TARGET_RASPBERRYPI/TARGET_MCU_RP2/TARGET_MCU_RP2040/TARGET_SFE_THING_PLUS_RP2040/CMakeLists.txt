# Copyright (c) 2026 Jamie Smith. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-sfe-thing-plus-rp2040 INTERFACE)

target_include_directories(mbed-sfe-thing-plus-rp2040
    INTERFACE
        .
)

target_link_libraries(mbed-sfe-thing-plus-rp2040 INTERFACE mbed-mcu-rp2040)