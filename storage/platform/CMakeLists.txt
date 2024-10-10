# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-storage
    PRIVATE
        source/PlatformStorage.cpp
)

list(APPEND mbed-storage-libs
    mbed-storage-blockdevice
    mbed-storage-filesystem
    mbed-storage-fat
    mbed-storage-littlefs
    mbed-storage-littlefs-v2
)

if("COMPONENT_DATAFLASH=1" IN_LIST MBED_TARGET_DEFINITIONS)
    list(APPEND mbed-storage-libs mbed-storage-dataflash)
endif()

if("DEVICE_FLASH=1" IN_LIST MBED_TARGET_DEFINITIONS)
    list(APPEND mbed-storage-libs mbed-storage-flashiap)
endif()

if("COMPONENT_QSPIF=1" IN_LIST MBED_TARGET_DEFINITIONS)
    list(APPEND mbed-storage-libs mbed-storage-qspif)
endif()

if("COMPONENT_OSPIF=1" IN_LIST MBED_TARGET_DEFINITIONS)
    list(APPEND mbed-storage-libs mbed-storage-ospif)
endif()

if("COMPONENT_SD=1" IN_LIST MBED_TARGET_DEFINITIONS)
    list(APPEND mbed-storage-libs mbed-storage-sd)
endif()

if("COMPONENT_SPIF=1" IN_LIST MBED_TARGET_DEFINITIONS)
    list(APPEND mbed-storage-libs mbed-storage-spif)
endif()

target_link_libraries(mbed-storage
    PUBLIC
        ${mbed-storage-libs}
)
