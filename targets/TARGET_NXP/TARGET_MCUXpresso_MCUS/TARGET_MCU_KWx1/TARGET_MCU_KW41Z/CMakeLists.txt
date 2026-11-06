# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-mcu-kw41z INTERFACE .)

target_sources(mbed-mcu-kw41z INTERFACE
    startup_MKW41Z4.S
)

add_subdirectory(TARGET_FRDM_KW41Z)
