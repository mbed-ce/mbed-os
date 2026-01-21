# Copyright (c) 2026 Jamie Smith. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_EMAC=1" IN_LIST MBED_TARGET_DEFINITIONS)
    return()
endif()

# EMAC needs the RTOS
if(APPLICATION_PROFILE_CONFIG_BAREMETAL)
    return()
endif()

target_include_directories(mbed-emac
    PUBLIC
        .
)

target_sources(mbed-emac
    PRIVATE
        GenericEthPhy.cpp
        PhyDrivers.cpp
)
