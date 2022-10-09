# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_library(mbed-randlib EXCLUDE_FROM_ALL)

target_include_directories(mbed-randlib
    PUBLIC
        include
        include/mbed-client-randlib
        include/mbed-client-randlib/platform
)

target_sources(mbed-randlib
    PRIVATE
        source/randLIB.c
        source/arm_hal_random.c
)

target_link_libraries(mbed-randlib
    PUBLIC
        mbed-mbedtls
        mbed-core-flags
)