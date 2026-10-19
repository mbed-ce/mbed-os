# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

mbed_set_linker_script(mbed-mcu-kwx1 ${CMAKE_CURRENT_SOURCE_DIR}/MKWx1.ld)

target_include_directories(mbed-mcu-kwx1 INTERFACE .)

target_sources(mbed-mcu-kwx1 INTERFACE
    PeripheralPins.c
    mbed_overrides.c
)

add_subdirectory(TARGET_MCU_KW41Z)