# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-fpb-ra4e1 INTERFACE)
target_include_directories(mbed-fpb-ra4e1
    INTERFACE
        .
)
target_link_libraries(mbed-fpb-ra4e1 INTERFACE mbed-ra4e1)
