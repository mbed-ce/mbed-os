# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-emac
    PUBLIC
        .
)

target_sources(mbed-emac
    PRIVATE
        m460_eth.c
        synopGMAC_Dev.c
        synopGMAC_network_interface.c
)
