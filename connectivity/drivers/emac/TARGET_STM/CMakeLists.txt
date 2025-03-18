# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("STM32F2" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F2)
elseif("STM32F4" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F4)
elseif("STM32F7" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F7)
elseif("STM32H7" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32H7)
elseif("STM32H5" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32H5)
endif()

target_include_directories(mbed-emac
    PUBLIC
        .
)

if("STM32H7" IN_LIST MBED_TARGET_LABELS OR "STM32H5" IN_LIST MBED_TARGET_LABELS)
    target_sources(mbed-emac
        PRIVATE
            STM32EthMACv2.cpp
    )
endif()

if("STM32F2" IN_LIST MBED_TARGET_LABELS OR "STM32F4" IN_LIST MBED_TARGET_LABELS OR "STM32F7" IN_LIST MBED_TARGET_LABELS)
    target_sources(mbed-emac
        PRIVATE
            STM32EthMACv1.cpp
    )
endif()


target_compile_options(mbed-emac PRIVATE -Wno-packed-bitfield-compat)
