# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

mbed_set_linker_script(mbed-mcu-lpc546xx ${CMAKE_CURRENT_SOURCE_DIR}/LPC546xx.ld)

target_include_directories(mbed-mcu-lpc546xx INTERFACE .)
target_sources(mbed-mcu-lpc546xx INTERFACE
    PeripheralPins.c
    mbed_overrides.cpp
    mbed_mpu_lpc546xx.c
)

add_subdirectory(TARGET_FF_LPC546XX)
add_subdirectory(TARGET_LPCXPRESSO54628)