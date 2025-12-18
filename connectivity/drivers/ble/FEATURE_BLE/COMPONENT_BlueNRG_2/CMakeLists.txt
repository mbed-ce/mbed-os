# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# This driver needs the RTOS
if(APPLICATION_PROFILE_CONFIG_BAREMETAL)
    return()
endif()

add_library(mbed-bluenrg2 STATIC EXCLUDE_FROM_ALL
    BlueNrg2HCIDriver.cpp)

target_link_libraries(mbed-bluenrg2 
    PUBLIC
        mbed-core-flags
        mbed-ble
)

# circular dependency between mbed-ble and the BlueNRG driver, because of the ble_cordio_get_hci_driver() implementation
target_link_libraries(mbed-ble PUBLIC mbed-bluenrg2)