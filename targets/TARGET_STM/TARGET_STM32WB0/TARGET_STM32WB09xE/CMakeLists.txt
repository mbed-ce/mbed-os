# Copyright (c) 2026 MbedCE Community Contributors
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_WB09KE EXCLUDE_FROM_ALL)

add_library(mbed-stm32wb09xe INTERFACE)
target_include_directories(mbed-stm32wb09xe INTERFACE .)
target_link_libraries(mbed-stm32wb09xe INTERFACE mbed-stm32wb0)
