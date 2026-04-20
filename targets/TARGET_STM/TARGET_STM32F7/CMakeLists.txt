# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32F722xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F746xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F756xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F767xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F769xI EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(mbed-stm32f7 INTERFACE)

target_sources(mbed-stm32f7
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


target_include_directories(mbed-stm32f7
    INTERFACE
        .
)

target_link_libraries(mbed-stm32f7 INTERFACE mbed-stm mbed-stm32f7cube-fw)

# Use shared F7 clock configuration for all F7 targets.
target_sources(mbed-stm32f7 INTERFACE clock_cfg/system_clock.c)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    get_startup_file(STARTUP_FILE "STM32Cube_FW/cmsis-device-f7/Source/Templates/gcc")

    patch_startup_file(STARTUP_FILE_GEN "${STARTUP_FILE}")

    target_sources(mbed-stm32f7
        INTERFACE
        ${STARTUP_FILE_GEN}
    )

    mbed_set_linker_script(mbed-stm32f7 linker_scripts/STM32F7_COMMON.ld)
endif()
