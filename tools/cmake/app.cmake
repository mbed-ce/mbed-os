# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

message(WARNING "Mbed: Deprecated: Instead of including app.cmake, include mbed_toolchain_setup.cmake, then call project(), then include mbed_project_setup.cmake. For more info, see https://github.com/mbed-ce/mbed-os/pull/421")

include(${CMAKE_CURRENT_LIST_DIR}/mbed_toolchain_setup.cmake)

enable_language(C CXX ASM)

include(${CMAKE_CURRENT_LIST_DIR}/mbed_project_setup.cmake)