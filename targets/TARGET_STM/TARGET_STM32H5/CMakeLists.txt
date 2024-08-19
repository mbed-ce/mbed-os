# Copyright (c) 2023 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H503xB EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H563xI EXCLUDE_FROM_ALL)

add_library(mbed-stm32h5 INTERFACE)

target_sources(mbed-stm32h5
    INTERFACE
    clock_cfg/system_clock.c
    analogin_device.c
    analogout_device.c
    gpio_irq_device.c
    i2c_device.c
    serial_device.c
    spi_api.c
    pwmout_device.c
)

target_include_directories(mbed-stm32h5
    INTERFACE
        .
)

target_link_libraries(mbed-stm32h5 INTERFACE mbed-stm mbed-stm32h5cube-fw)
