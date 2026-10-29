# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-sdt64b INTERFACE)
target_link_libraries(mbed-sdt64b INTERFACE mbed-mcu-k64f)
target_include_directories(mbed-sdt64b INTERFACE .)
target_sources(mbed-sdt64b INTERFACE fsl_clock_config.c)