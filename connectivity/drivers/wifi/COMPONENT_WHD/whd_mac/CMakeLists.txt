# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-wifi
    INTERFACE
        .
        ./interface
        ./network
        ./utils
)

target_sources(mbed-wifi
    INTERFACE
        interface/CyDhcpServer.cpp
        interface/WhdAccessPoint.cpp
        interface/WhdSTAInterface.cpp
        interface/WhdSoftAPInterface.cpp
        interface/whd_emac.cpp
        interface/whd_interface.cpp

        utils/cydhcp_server_debug.cpp
        utils/cynetwork_utils.c
)

# Link override object file coming from static library anyway
#
# NOTE: This linker option is to pretend undefined symbol and won't cause
#       undefined symbol error even though the override object file actually
#       doesn't provide such symbol definition.
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    target_link_options(mbed-wifi
        INTERFACE
            LINKER:--undefined=LINK_WHD_INTERFACE_CPP
    )
endif()
