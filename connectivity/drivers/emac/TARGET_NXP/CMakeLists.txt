# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("MCU_MIMXRT105X" IN_LIST MBED_TARGET_LABELS OR "MCU_K6x" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(eth-mac-via-ENET)
elseif("LPCTarget" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_LPCTarget)
elseif("MCU_LPC546XX" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MCU_LPC546XX)
endif()

