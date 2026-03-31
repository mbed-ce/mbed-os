# Copyright (c) 2025 MbedCE Community Contributors (Jan Kamidra)
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32F401xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F411xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F412xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F413xH EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F429xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F439xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F446xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F469xI EXCLUDE_FROM_ALL)

add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(mbed-stm32f4 INTERFACE)

target_include_directories(mbed-stm32f4
    INTERFACE
        .
)

target_sources(mbed-stm32f4
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)

target_link_libraries(mbed-stm32f4 INTERFACE mbed-stm mbed-stm32f4cube-fw)

# Add clock file depending on the frequency
get_system_clock_file(SYSTEM_CLOCK_FILE ${_stm32_family})
target_sources(mbed-stm32f4 INTERFACE ${SYSTEM_CLOCK_FILE})

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    get_startup_file(STARTUP_FILE "STM32Cube_FW/cmsis-device-f4/Source/Templates/gcc")

    patch_startup_file(STARTUP_FILE_GEN "${STARTUP_FILE}")

    target_sources(mbed-stm32f4
        INTERFACE
        ${STARTUP_FILE_GEN}
    )

    mbed_set_linker_script(mbed-stm32f4 linker_scripts/STM32F4_COMMON.ld)
endif()
