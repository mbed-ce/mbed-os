# Copyright (c) 2025 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

# ----------------------------------------------
# CMake finder for the dfu_util tool
#
#
# This module defines:
# dfu_util_PATH - full path to dfu-util executable
# dfu_util_FOUND - whether or not the ArduinoBossac executable was found

set(dfu_util_PATHS "")

# try to figure out where dfu_util may be installed.
# We will look for it both from the Arduino IDE, and from a manual install.
if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")

    # on Windows, assume that the user extracted the binaries to Program Files
    # if the host is 64 bit, there will be a Program Files (x86) folder, this covers both
    file(GLOB dfu_util_PATHS "C:/Program Files*/dfu-util*/")

    # On my computer the path is C:\Users\jamie\AppData\Local\Arduino15\packages\arduino\tools\dfu-util\0.10.0-arduino1\dfu-util.exe
    file(GLOB dfu_util_arduino_PATHS "$ENV{LocalAppData}/Arduino*/packages/arduino/tools/dfu-util/0*")
    list(APPEND dfu_util_PATHS ${dfu_util_arduino_PATHS})
else()

    # Linux / Mac
    # a possible path would be $HOME/.arduino15/packages/arduino/tools/dfu-util/0.10.0-arduino1/dfu-util
    file(GLOB dfu_util_PATHS "$ENV{HOME}/.arduino*/packages/arduino/tools/dfu-util/0*")

endif()

find_program(dfu_util_PATH NAMES dfu-util HINTS ${dfu_util_PATHS} DOC "Path to the dfu-util executable")

if(EXISTS "${dfu_util_PATH}")
    # Detect version
    execute_process(COMMAND ${dfu_util_PATH} --version
            OUTPUT_VARIABLE dfu_util_VERSION_OUTPUT)

    # The output looks like "dfu-util 0.11\n", so use a regex to grab the version
    string(REGEX REPLACE "dfu-util ([^\n]+).+" "\\1" dfu_util_VERSION ${dfu_util_VERSION_OUTPUT})
endif()

find_package_handle_standard_args(dfu_util REQUIRED_VARS dfu_util_PATH VERSION_VAR dfu_util_VERSION)



