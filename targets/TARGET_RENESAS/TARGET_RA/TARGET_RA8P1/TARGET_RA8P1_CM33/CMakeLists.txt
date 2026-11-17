# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# RA8P1 Cortex-M33 (CPU1) additions.
#
# The CPU1 image reuses the mbed-ra8p1 sources; everything core-specific is
# selected by the _RA_CORE=1 macro from the target definition (FSP BSP
# secondary-core build) and by the CPU1-local ICU vector table supplied by
# the board folder (e.g. TARGET_RA8P1_TITAN_MINI_CM33). The CPU1 linker
# script is selected conditionally in the parent CMakeLists.txt.

add_library(mbed-ra8p1-cm33 INTERFACE)

target_link_libraries(mbed-ra8p1-cm33 INTERFACE mbed-ra8p1)
