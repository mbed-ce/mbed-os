# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-cellular
    PUBLIC
        .
)

target_sources(mbed-cellular
    PRIVATE
        ALT1250_PPP.cpp
        ALT1250_PPP_CellularContext.cpp
        ALT1250_PPP_CellularNetwork.cpp
)

# Link override object file coming from static library anyway
#
# NOTE: This linker option is to pretend undefined symbol and won't cause
#       undefined symbol error even though the override object file actually
#       doesn't provide such symbol definition.
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    target_link_options(mbed-cellular
        INTERFACE
            LINKER:--undefined=LINK_ALT1250_PPP_CPP
    )
endif()
