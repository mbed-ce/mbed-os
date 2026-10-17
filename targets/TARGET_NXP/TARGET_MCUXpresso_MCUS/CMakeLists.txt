# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Clone SDK submodule
mbed_setup_submodule(
    mbed-mcux-sdk
    CHECK_FILE import-mcux-sdk.py
    IF_LABEL MCUXpresso_MCUS
)

# Generic MCUXpresso-based libraries
add_subdirectory(mcux-usb)

# MCU targets
include(${CMAKE_CURRENT_SOURCE_DIR}/kl4x-sdk.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/k-series-sdk.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/mimxrt105x_6x-sdk.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/lpc-54xxx-sdk.cmake)

add_subdirectory(TARGET_MCU_KL4x)
add_subdirectory(TARGET_MIMXRT105x_6x)
add_subdirectory(TARGET_MCU_KINETIS_K_SERIES)
add_subdirectory(TARGET_MCU_LPC54xxx)

#add_subdirectory(TARGET_IMX EXCLUDE_FROM_ALL)
#add_subdirectory(TARGET_LPC EXCLUDE_FROM_ALL)
#add_subdirectory(TARGET_LPC54114 EXCLUDE_FROM_ALL)
#add_subdirectory(TARGET_MCU_LPC546XX EXCLUDE_FROM_ALL)
#add_subdirectory(TARGET_MIMXRT1170 EXCLUDE_FROM_ALL)

