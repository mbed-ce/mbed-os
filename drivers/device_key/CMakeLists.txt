# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_library(mbed-device_key EXCLUDE_FROM_ALL)

target_include_directories(mbed-device_key
    PUBLIC
        include
        include/device_key
)

target_sources(mbed-device_key
    PRIVATE
        source/DeviceKey.cpp
)

target_link_libraries(mbed-device_key
    PUBLIC
        mbed-storage-kvstore
        mbed-storage-tdbstore
        mbed-storage-kv-global-api
)
