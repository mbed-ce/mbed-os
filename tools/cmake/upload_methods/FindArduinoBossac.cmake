# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# ----------------------------------------------
# CMake finder for the Arduino variant of bossac
#
# This module defines:
# ArduinoBossac - full path to bossac executable
# ArduinoBossac_FOUND - whether or not the ArduinoBossac executable was found

set(ArduinoBossac_PATHS "")

# try to figure out where ArduinoBossac may be installed.
if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")

    # on Windows, assume that the user extracted the binaries to Program Files

    # On my computer the path is C:\Users\jamie\AppData\Local\Arduino15\packages\arduino\tools\bossac\1.9.1-arduino2\bossac.exe
    file(GLOB ArduinoBossac_PATHS "$ENV{LocalAppData}/Arduino*/packages/arduino/tools/bossac/*")
else()

    # Linux / Mac
    # Per here: https://docs.zephyrproject.org/2.7.0/boards/arm/arduino_nano_33_ble/doc/index.html
    # a possible path would be $HOME/.arduino15/packages/arduino/tools/bossac/1.9.1-arduino2/bossac
    file(GLOB ArduinoBossac_PATHS "$ENV{HOME}/.arduino*/packages/arduino/tools/bossac/*")
endif()

# if we found multiple paths, check the one with the highest version number first
list(SORT ArduinoBossac_PATHS)
list(REVERSE ArduinoBossac_PATHS)

find_program(ArduinoBossac NAMES bossac HINTS ${ArduinoBossac_PATHS} DOC "Path to the Arduino variant of bossac")

# Make sure this is the Arduino fork of BOSSA, not the original BOSSA from
# shumatech. Bootloaders of Arduino boards like the UNO R4 need the extra
# options of the fork (notably --offset) which original BOSSA does not have.
if(ArduinoBossac)
    execute_process(
        COMMAND ${ArduinoBossac} --help
        OUTPUT_VARIABLE ArduinoBossac_HELP_OUTPUT
        ERROR_VARIABLE ArduinoBossac_HELP_OUTPUT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE ArduinoBossac_HELP_RESULT
    )
    if(NOT "${ArduinoBossac_HELP_OUTPUT}" MATCHES "--offset")
        message(WARNING
            "The bossac found at ${ArduinoBossac} does not support the --offset option. "
            "This looks like the original BOSSA instead of the Arduino fork, which cannot talk to "
            "Arduino bootloaders such as the UNO R4 one. Install the Arduino fork of bossac "
            "(e.g. by installing the UNO R4 board support with the Arduino IDE or arduino-cli) "
            "and make sure it is found on the PATH or in the Arduino15 directory.")
        unset(ArduinoBossac)
        unset(ArduinoBossac CACHE)
    endif()
endif()

find_package_handle_standard_args(ArduinoBossac REQUIRED_VARS ArduinoBossac)
