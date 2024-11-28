# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-cellular
    PUBLIC
        .
)

target_sources(mbed-cellular
    PRIVATE
        TELIT_ME910.cpp
        TELIT_ME910_CellularContext.cpp
        TELIT_ME910_CellularNetwork.cpp
)

if("TARGET_EP_AGORA" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(TARGET_EP_AGORA)
endif()

# Link override object file coming from static library anyway
#
# NOTE: This linker option is to pretend undefined symbol and won't cause
#       undefined symbol error even though the override object file actually
#       doesn't provide such symbol definition.
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    target_link_options(mbed-cellular
        INTERFACE
            LINKER:--undefined=LINK_TELIT_ME910_CPP
    )
endif()
