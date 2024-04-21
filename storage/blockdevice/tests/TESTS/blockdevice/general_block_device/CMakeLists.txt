# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("COMPONENT_DATAFLASH=1" IN_LIST MBED_TARGET_DEFINITIONS)
    list(APPEND mbed_blockdevice_libs mbed-storage-dataflash)
endif()

if("DEVICE_FLASH=1" IN_LIST MBED_TARGET_DEFINITIONS)
    list(APPEND mbed_blockdevice_libs mbed-storage-flashiap)
endif()

if("COMPONENT_I2CEE=1" IN_LIST MBED_TARGET_DEFINITIONS)
    list(APPEND mbed_blockdevice_libs mbed-storage-i2cee)
endif()

if("COMPONENT_OSPIF=1" IN_LIST MBED_TARGET_DEFINITIONS)
    list(APPEND mbed_blockdevice_libs mbed-storage-ospif)
endif()

if("COMPONENT_QSPIF=1" IN_LIST MBED_TARGET_DEFINITIONS)
    list(APPEND mbed_blockdevice_libs mbed-storage-qspif)
endif()

if("COMPONENT_SD=1" IN_LIST MBED_TARGET_DEFINITIONS)
    list(APPEND mbed_blockdevice_libs mbed-storage-sd)
endif()

if("COMPONENT_SPIF=1" IN_LIST MBED_TARGET_DEFINITIONS)
    list(APPEND mbed_blockdevice_libs mbed-storage-spif)
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-storage-blockdevice-general_block_device
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-storage-blockdevice
        mbed-storage
        ${mbed_blockdevice_libs}
)
