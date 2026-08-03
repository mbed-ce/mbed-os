# Copyright (c) 2026 MbedCE Community Contributors
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_WB09KE EXCLUDE_FROM_ALL)

get_startup_file(STARTUP_SOURCE ../STM32Cube_FW/cmsis-device-wb0/Source/Templates/gcc)
patch_startup_file(STARTUP_FILE ${STARTUP_SOURCE})

add_library(mbed-stm32wb09xe INTERFACE)
target_sources(mbed-stm32wb09xe INTERFACE ${STARTUP_FILE})
target_include_directories(mbed-stm32wb09xe INTERFACE .)
mbed_set_linker_script(mbed-stm32wb09xe ${CMAKE_CURRENT_SOURCE_DIR}/TOOLCHAIN_GCC_ARM/STM32WB09xE.ld)
target_link_libraries(mbed-stm32wb09xe INTERFACE mbed-stm32wb0)
