# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-kv-config STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-storage-kv-config
    PUBLIC
        .
        include
        include/kv_config
)

target_sources(mbed-storage-kv-config
    PRIVATE
        source/kv_config.cpp
)

target_link_libraries(mbed-storage-kv-config
    PUBLIC
        mbed-storage-kvstore
        mbed-storage-blockdevice
        mbed-storage-tdbstore
        mbed-storage-filesystemstore
        mbed-storage-securestore
        mbed-storage-littlefs
        mbed-storage-littlefs-v2
        mbed-storage-fat
)

if("DEVICE_FLASH=1" IN_LIST MBED_TARGET_DEFINITIONS)
    target_link_libraries(mbed-storage-kv-config PUBLIC mbed-storage-flashiap)
endif()

if("TARGET_CY_EXTERNAL_WIFI_FW" IN_LIST MBED_TARGET_DEFINITIONS)
    # On some Cypress PSOC6 targets, the external wifi fw library provides the definition of get_other_blockdevice()
    target_link_libraries(mbed-storage-kv-config PUBLIC mbed-cy-external-wifi-fw)
endif()
