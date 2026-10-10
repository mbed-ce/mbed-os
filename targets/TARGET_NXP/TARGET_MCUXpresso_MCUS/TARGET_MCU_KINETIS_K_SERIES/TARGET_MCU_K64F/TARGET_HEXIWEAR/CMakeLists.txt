# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-hexiwear INTERFACE)
target_link_libraries(mbed-hexiwear INTERFACE mbed-mcu-k64f)
target_include_directories(mbed-hexiwear INTERFACE .)
target_sources(mbed-hexiwear INTERFACE fsl_clock_config.c)