# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("M480" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_M480)
elseif("NUC472" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUC472)
elseif("M460" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_M460)
endif()

if(NOT "M480" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-emac
        PUBLIC
            .
    )

    target_sources(mbed-emac
        PRIVATE
            numaker_emac.cpp
    )
endif()