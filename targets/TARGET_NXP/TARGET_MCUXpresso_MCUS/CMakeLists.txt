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
include(${CMAKE_CURRENT_SOURCE_DIR}/mimxrt116x_7x-sdk.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/lpc-54xxx-sdk.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/kwx1-sdk.cmake)

# Target dirs
add_subdirectory(TARGET_MCU_KL4x)
add_subdirectory(TARGET_MIMXRT105x_6x)
add_subdirectory(TARGET_MCU_KINETIS_K_SERIES)
add_subdirectory(TARGET_MCU_LPC54xxx)
add_subdirectory(TARGET_MIMXRT116x_7x)
add_subdirectory(TARGET_MCU_KWx1)
