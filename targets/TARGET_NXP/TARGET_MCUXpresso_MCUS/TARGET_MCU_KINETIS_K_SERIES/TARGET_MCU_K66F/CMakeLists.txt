# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-mcu-k66f INTERFACE .)
target_sources(mbed-mcu-k66f
    INTERFACE
        PeripheralPins.c
)

add_subdirectory(TARGET_FRDM_K66F)