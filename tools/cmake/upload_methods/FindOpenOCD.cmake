# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# ----------------------------------------------
# CMake finder for OpenOCD
#
#
# This module defines:
# OpenOCD - full path to OpenOCD executable
# OpenOCD_FOUND - whether or not the OpenOCD executable was found

set(OpenOCD_PATHS "")

# try to figure out where OpenOCD may be installed.
if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")

    # on Windows, assume that the user extracted the binaries to Program Files

    # if the host is 64 bit, there will be a Program Files (x86) folder, this covers both
    file(GLOB OpenOCD_PATHS "C:/Program Files*/openocd*/bin*")

    # if we found multiple paths, check the one with the highest version number first
    list(SORT OpenOCD_PATHS)
    list(REVERSE OpenOCD_PATHS)

endif()

find_program(OpenOCD NAMES openocd PATHS ${OpenOCD_PATHS} DOC "Path to the openocd executable")

if(OpenOCD AND EXISTS "${OpenOCD}")
    # Detect version. Most versions write to stderr but some (Infineon OpenOCD) write to stdout
    execute_process(COMMAND ${OpenOCD} --version
        ERROR_VARIABLE OpenOCD_VERSION_ERROR_OUTPUT
        OUTPUT_VARIABLE OpenOCD_VERSION_OUTPUT
        COMMAND_ERROR_IS_FATAL ANY)

    # Use a regex to grab the version number
    if("${OpenOCD_VERSION_ERROR_OUTPUT}" MATCHES "Open On-Chip Debugger ([^ ]+)")
        set(OpenOCD_VERSION ${CMAKE_MATCH_1})
    elseif("${OpenOCD_VERSION_OUTPUT}" MATCHES "Open On-Chip Debugger ([^ ]+)")
        set(OpenOCD_VERSION ${CMAKE_MATCH_1})
    else()
        message(WARNING "Unable to determine OpenOCD version")
    endif()

endif()

find_package_handle_standard_args(OpenOCD
    HANDLE_VERSION_RANGE
    FOUND_VAR OpenOCD_FOUND
    VERSION_VAR OpenOCD_VERSION
    REQUIRED_VARS OpenOCD)


