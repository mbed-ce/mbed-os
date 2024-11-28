# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-cellular
    PUBLIC
        .
)

target_sources(mbed-cellular
    PRIVATE
        SARA4_PPP.cpp
        SARA4_PPP_CellularNetwork.cpp
)

if("MTS_DRAGONFLY_L471QG" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(TARGET_MTS_DRAGONFLY_L471QG)
endif()

# Link override object file coming from static library anyway
#
# NOTE: This linker option is to pretend undefined symbol and won't cause
#       undefined symbol error even though the override object file actually
#       doesn't provide such symbol definition.
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    target_link_options(mbed-cellular
        INTERFACE
            LINKER:--undefined=LINK_SARA4_PPP_CPP
    )
endif()
