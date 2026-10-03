# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Clone SDK submodule
mbed_setup_submodule(
    mbed-mcux-sdk
    CHECK_FILE import-mcux-sdk.py
    IF_LABEL TARGET_MCUXpresso_MCUS
)

# Generic MCUXpresso-based libraries
add_subdirectory(mcux-usb)

# Add targets for each MCUX SDK MCU
add_library(mbed-mcu-k2x INTERFACE)
add_library(mbed-mcu-k22f INTERFACE)
add_library(mbed-frdm-k22f INTERFACE)

# KL4x family
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

# MIMXRT105x/6x family
add_library(mbed-mcu-mimxrt105x_6x INTERFACE)
add_library(mbed-mcu-mimxrt105x INTERFACE)
target_link_libraries(mbed-mcu-mimxrt105x INTERFACE mbed-mcu-mimxrt105x_6x)
add_library(mbed-mcu-mimxrt106x INTERFACE)
target_link_libraries(mbed-mcu-mimxrt106x INTERFACE mbed-mcu-mimxrt105x_6x)
add_library(mbed-mimxrt1050-evk INTERFACE)
target_link_libraries(mbed-mimxrt1050-evk INTERFACE mbed-mcu-mimxrt105x)
add_library(mbed-mimxrt1060-evk INTERFACE)
target_link_libraries(mbed-mimxrt1060-evk INTERFACE mbed-mcu-mimxrt106x)
add_library(mbed-teensy-4x INTERFACE)
target_link_libraries(mbed-teensy-4x INTERFACE mbed-mcu-mimxrt106x)
add_library(mbed-teensy-40 INTERFACE)
target_link_libraries(mbed-teensy-40 INTERFACE mbed-teensy-4x)
add_library(mbed-teensy-41 INTERFACE)
target_link_libraries(mbed-teensy-41 INTERFACE mbed-teensy-4x)

# Add SDK files to the above targets
include(${CMAKE_CURRENT_SOURCE_DIR}/kl4x-sdk.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/mimxrt105x_6x-sdk.cmake)

add_subdirectory(TARGET_KL4x)
add_subdirectory(TARGET_MIMXRT105x_6x)

#add_subdirectory(TARGET_IMX EXCLUDE_FROM_ALL)
#add_subdirectory(TARGET_LPC EXCLUDE_FROM_ALL)
#add_subdirectory(TARGET_LPC54114 EXCLUDE_FROM_ALL)
#add_subdirectory(TARGET_MCU_LPC546XX EXCLUDE_FROM_ALL)
#add_subdirectory(TARGET_MIMXRT1170 EXCLUDE_FROM_ALL)

