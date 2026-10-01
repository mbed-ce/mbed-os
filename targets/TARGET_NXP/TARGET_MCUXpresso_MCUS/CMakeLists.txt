# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Add targets for each MCUX SDK MCU
add_library(mbed-mcu-k2x INTERFACE)
add_library(mbed-mcu-k22f INTERFACE)
add_library(mbed-frdm-k22f INTERFACE)

add_library(mbed-mcu-kl4x INTERFACE)
add_library(mbed-mcu-kl43z INTERFACE)
target_link_libraries(mbed-mcu-kl43z INTERFACE mbed-mcu-kl4x)
add_library(mbed-frdm-kl43z INTERFACE)
target_link_libraries(mbed-frdm-kl43z INTERFACE mbed-mcu-kl43z)

add_library(mbed-mcu-kw41z INTERFACE)
add_library(mbed-frdm-kw41z INTERFACE)
add_library(mbed-mcu-k6x INTERFACE)
add_library(mbed-mcu-k64f INTERFACE)
add_library(mbed-mcu-k66f INTERFACE)
add_library(mbed-hexiwear INTERFACE)
add_library(mbed-sdt64b INTERFACE)
add_library(mbed-frdm-k64f INTERFACE)
add_library(mbed-frdm-k66f INTERFACE)
add_library(mbed-mcu-k82f INTERFACE)
add_library(mbed-frdm-k82f INTERFACE)

# Add SDK files to the above targets
include(${CMAKE_CURRENT_SOURCE_DIR}/kl4x-sdk.cmake)

add_subdirectory(TARGET_KL4x)
add_subdirectory(TARGET_IMX EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_LPC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_LPC54114 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MCU_LPC546XX EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MIMXRT105x EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MIMXRT1170 EXCLUDE_FROM_ALL)
add_subdirectory(middleware/TARGET_USB EXCLUDE_FROM_ALL)

add_library(mbed-mcuxpresso-mcus INTERFACE)

target_include_directories(mbed-mcuxpresso-mcus
    INTERFACE
        .
        middleware/osa
)

target_sources(mbed-mcuxpresso-mcus
    INTERFACE
        USBPhy_MCUXpresso.cpp

        middleware/osa/fsl_os_abstraction_mbed.c
)

target_link_libraries(mbed-mcuxpresso-mcus INTERFACE mbed-nxp)
