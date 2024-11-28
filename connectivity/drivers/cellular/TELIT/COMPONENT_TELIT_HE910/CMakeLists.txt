# Copyright (c) 2024 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-cellular
    PUBLIC
        .
)

target_sources(mbed-cellular
    PRIVATE
        TELIT_HE910.cpp
)

if("TARGET_MTS_DRAGONFLY_F411RE" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(TARGET_MTS_DRAGONFLY_F411RE)
endif()

if("TARGET_MTS_DRAGONFLY_F413RH" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(TARGET_MTS_DRAGONFLY_F413RH)
endif()

# Link override object file coming from static library anyway
#
# NOTE: This linker option is to pretend undefined symbol and won't cause
#       undefined symbol error even though the override object file actually
#       doesn't provide such symbol definition.
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    target_link_options(mbed-cellular
        INTERFACE
            LINKER:--undefined=LINK_TELIT_HE910_CPP
    )
endif()
