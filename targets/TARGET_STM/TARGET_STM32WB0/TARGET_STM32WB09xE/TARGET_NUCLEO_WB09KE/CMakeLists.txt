# Copyright (c) 2026 MbedCE Community Contributors (Jan Kamidra)
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-wb09ke INTERFACE)
target_sources(mbed-nucleo-wb09ke INTERFACE PeripheralPins.c)
target_include_directories(mbed-nucleo-wb09ke INTERFACE .)
target_link_libraries(mbed-nucleo-wb09ke INTERFACE mbed-stm32wb09xe)
