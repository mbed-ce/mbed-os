# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32F303x8 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F303xE EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(mbed-stm32f3 INTERFACE)

target_include_directories(mbed-stm32f3
    INTERFACE
        .
)

target_sources(mbed-stm32f3
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        i2c_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)

target_link_libraries(mbed-stm32f3 INTERFACE mbed-stm mbed-stm32f3cube-fw)

# Add global clock file
target_sources(mbed-stm32f3 INTERFACE clock_cfg/system_clock.c)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    get_startup_file(STARTUP_FILE "STM32Cube_FW/cmsis-device-f3/Source/Templates/gcc")

    patch_startup_file(STARTUP_FILE_GEN "${STARTUP_FILE}")

    target_sources(mbed-stm32f3
        INTERFACE
        ${STARTUP_FILE_GEN}
    )

    mbed_set_linker_script(mbed-stm32f3 linker_scripts/STM32F3_COMMON.ld)
endif()
