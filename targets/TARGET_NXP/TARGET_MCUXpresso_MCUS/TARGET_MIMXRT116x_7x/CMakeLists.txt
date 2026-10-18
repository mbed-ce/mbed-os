# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

mbed_set_linker_script(mbed-mcu-mimxrt116x_7x ${CMAKE_CURRENT_SOURCE_DIR}/MIMXRT116x_7x.ld)

target_include_directories(mbed-mcu-mimxrt116x_7x INTERFACE .)

add_subdirectory(TARGET_MIMXRT117x)