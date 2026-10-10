# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-frdm-k64f INTERFACE)
target_link_libraries(mbed-frdm-k64f INTERFACE mbed-mcu-k64f)
target_include_directories(mbed-frdm-k64f INTERFACE .)
target_sources(mbed-frdm-k64f INTERFACE fsl_clock_config.c)