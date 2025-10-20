# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if((NOT "COMPONENT_SPIF=1" IN_LIST MBED_TARGET_DEFINITIONS) AND
    (NOT "COMPONENT_QSPIF=1" IN_LIST MBED_TARGET_DEFINITIONS) AND
    (NOT "COMPONENT_OSPIF=1" IN_LIST MBED_TARGET_DEFINITIONS) AND
    (NOT "COMPONENT_SD=1" IN_LIST MBED_TARGET_DEFINITIONS))
    set(SKIP_REASON "External block device needed.")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-storage-filesystem-general_filesystem
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-storage-littlefs
        mbed-storage
        mbed-storage-sd
    TEST_SKIPPED
        ${SKIP_REASON}
)
