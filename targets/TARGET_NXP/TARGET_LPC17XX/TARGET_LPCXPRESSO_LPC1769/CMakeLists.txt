# Copyright (c) 2025 Johannes Stratmann.
# SPDX-License-Identifier: Apache-2.0

include(mbed_set_post_build_nxp)

add_library(mbed-lpcxpresso-lpc1769 INTERFACE)

target_include_directories(mbed-lpcxpresso-lpc1769
    INTERFACE
        .
)

target_link_libraries(mbed-lpcxpresso-lpc1769 INTERFACE mbed-lpc17xx)

mbed_post_build_lpc_patch_vtable("LPC1768")
