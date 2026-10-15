# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# ----------------------------------------------
# CMake finder for dfu-util
#
# Supports the following locations (checked in order):
#   1. dfu-util on PATH (e.g. installed via package manager)
#   2. Arduino board packages (bundled dfu-util)
#   3. DFUUtil_DIR hint (e.g. -DDFUUtil_DIR=C:/tools/dfu-util-0.11)
#
# This module defines:
# DFUUtil - full path to the dfu-util executable
# DFUUtil_FOUND - whether or not dfu-util was found

set(DFUUtil_PATHS "")

# try to figure out where dfu-util may be installed.
if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")

    # Check Arduino packages (highest priority - known to work with UNO R4)
    # e.g. C:\Users\<user>\AppData\Local\Arduino15\packages\arduino\tools\dfu-util\0.11.0-arduino5
    file(GLOB DFUUtil_PATHS
        "$ENV{LocalAppData}/Arduino*/packages/arduino/tools/dfu-util/*"
    )
else()
    # Linux / Mac
    file(GLOB DFUUtil_PATHS
        "$ENV{HOME}/.arduino*/packages/arduino/tools/dfu-util/*"
    )
endif()

# if we found multiple paths, check the one with the highest version number first
list(SORT DFUUtil_PATHS)
list(REVERSE DFUUtil_PATHS)

find_program(DFUUtil NAMES dfu-util
    HINTS
        ${DFUUtil_PATHS}
        ${DFUUtil_DIR}
    DOC "Path to the dfu-util executable")

find_package_handle_standard_args(DFUUtil REQUIRED_VARS DFUUtil)
