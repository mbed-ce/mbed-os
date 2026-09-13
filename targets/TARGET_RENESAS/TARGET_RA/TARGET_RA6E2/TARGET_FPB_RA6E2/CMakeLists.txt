# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-fpb-ra6e2 INTERFACE)
target_include_directories(mbed-fpb-ra6e2
    INTERFACE
        .
)
target_link_libraries(mbed-fpb-ra6e2 INTERFACE mbed-ra6e2)
