# Copyright (c) 2026 MbedCE Community Contributors
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WB09xE EXCLUDE_FROM_ALL)

add_library(mbed-stm32wb0 INTERFACE)

target_sources(mbed-stm32wb0 INTERFACE
    clock_cfg/system_clock.c
    gpio_irq_api.c
    serial_device.c
    sleep.c
    wait_ns.c
)

target_include_directories(mbed-stm32wb0 INTERFACE .)
target_link_libraries(mbed-stm32wb0 INTERFACE mbed-stm mbed-stm32wb0cube-fw)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    get_startup_file(STARTUP_FILE "STM32Cube_FW/cmsis-device-wb0/Source/Templates/gcc")
    patch_startup_file(STARTUP_FILE_GEN "${STARTUP_FILE}" PRESERVE_SYSTEM_INIT_ORDER)

    target_sources(mbed-stm32wb0 INTERFACE ${STARTUP_FILE_GEN})
    mbed_set_linker_script(mbed-stm32wb0 linker_scripts/STM32WB0_COMMON.ld)
endif()
