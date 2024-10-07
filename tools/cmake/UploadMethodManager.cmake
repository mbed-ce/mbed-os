# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Change GDB load command to flash post-build processed image in debug launch.
# With this adjustment, GDB load command changes to "load <app>.hex" from
# just "load":
# 1. "Load" will load <app>.elf and is inappropriate for targets like
#    bootloader or TF-M enabled which need to post-build process images.
# 2. "load <app>.bin" is not considered because GDB load command
#    doesn't support binary format.
#
# NOTE: Place at the very start so that it can override by the below loaded
#       upload method if need be.
function(mbed_adjust_upload_debug_commands target)
    # MBED_UPLOAD_LAUNCH_COMMANDS defined?
    if(NOT DEFINED MBED_UPLOAD_LAUNCH_COMMANDS)
        return()
    endif()

    # GDB load command in MBED_UPLOAD_LAUNCH_COMMANDS?
    list(FIND MBED_UPLOAD_LAUNCH_COMMANDS "load" LOAD_INDEX)
    if(${LOAD_INDEX} LESS "0")
        return()
    endif()

    # <app>.hex for debug launch load
    set(HEX_FILE ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_BASE_NAME:${target}>.hex)

    # "load" -> "load <app>.hex"
    #
    # GDB load command doesn't support binary format. Ignore OUTPUT_EXT
    # and fix to Intel Hex format.
    #
    # NOTE: The <app>.hex file name needs to be quoted (\\\") to pass along
    #       to gdb correctly.
    list(TRANSFORM MBED_UPLOAD_LAUNCH_COMMANDS APPEND " \\\"${HEX_FILE}\\\"" AT ${LOAD_INDEX})

    # Update MBED_UPLOAD_LAUNCH_COMMANDS in cache
    set(MBED_UPLOAD_LAUNCH_COMMANDS ${MBED_UPLOAD_LAUNCH_COMMANDS} CACHE INTERNAL "" FORCE)
endfunction()

# ----------------------------------------------
# Load the upload method that the user selects

# This variable should have been set in app.cmake or by the upload method cfg file, sanity check here
if(NOT DEFINED UPLOAD_METHOD_DEFAULT)
	message(FATAL_ERROR "UPLOAD_METHOD_DEFAULT not found.")
endif()

set(UPLOAD_METHOD "${UPLOAD_METHOD_DEFAULT}" CACHE STRING "Method for uploading programs to the mbed")

# use a higher numbered port to allow use without root on Linux/Mac
set(GDB_PORT 23331 CACHE STRING "Port that the GDB server will be started on.")

# Upload methods must be uppercase, guard against the user making a mistake (since Windows will allow opening
# an include file with the wrong case, the error message gets confusing)
string(TOUPPER "${UPLOAD_METHOD}" UPLOAD_METHOD_UCASE)
if(NOT "${UPLOAD_METHOD_UCASE}" STREQUAL "${UPLOAD_METHOD}")
	message(WARNING "UPLOAD_METHOD value should be uppercase.  It has been automatically changed to \"${UPLOAD_METHOD_UCASE}\".")
	set(UPLOAD_METHOD "${UPLOAD_METHOD_UCASE}" CACHE STRING "" FORCE)
endif()

# Load the upload method.  This is expected to set the following variables:
# UPLOAD_${UPLOAD_METHOD}_FOUND - True iff the dependencies for this upload method were found
# UPLOAD_SUPPORTS_DEBUG - True iff this upload method supports debugging
# UPLOAD_GDBSERVER_DEBUG_COMMAND - Command to start a new GDB server
# UPLOAD_WANTS_EXTENDED_REMOTE - True iff GDB should use "target extended-remote" to connect to the GDB server
# UPLOAD_LAUNCH_COMMANDS - List of GDB commands to run after launching GDB.
# UPLOAD_RESTART_COMMANDS - List of GDB commands to run when the "restart chip" function is used.
# See here for more info: https://github.com/mbed-ce/mbed-os/wiki/Debugger-Commands-and-State-in-Upload-Methods
include(UploadMethod${UPLOAD_METHOD})

if(NOT "${UPLOAD_${UPLOAD_METHOD}_FOUND}")
	message(FATAL_ERROR "The upload method ${UPLOAD_METHOD} does not have the dependencies needed to run on this machine.")
endif()

if(NOT (("${UPLOAD_METHOD}" STREQUAL NONE) OR ("${${UPLOAD_METHOD}_UPLOAD_ENABLED}")))
	message(FATAL_ERROR "The upload method ${UPLOAD_METHOD} is not enabled in the config code for this target -- set ${UPLOAD_METHOD}_UPLOAD_ENABLED to TRUE to enable it.")
endif()

if(UPLOAD_SUPPORTS_DEBUG)
	message(STATUS "Mbed: Code upload and debugging enabled via upload method ${UPLOAD_METHOD}")
elseif(NOT "${UPLOAD_METHOD}" STREQUAL "NONE")
	message(STATUS "Mbed: Code upload enabled via upload method ${UPLOAD_METHOD}")
endif()

# These variables need to be made into a cache variables so that they can
# be seen by higher level directories when they call mbed_generate_upload_debug_targets()
set(MBED_UPLOAD_SUPPORTS_DEBUG ${UPLOAD_SUPPORTS_DEBUG} CACHE INTERNAL "" FORCE)
set(MBED_UPLOAD_GDBSERVER_DEBUG_COMMAND ${UPLOAD_GDBSERVER_DEBUG_COMMAND} CACHE INTERNAL "" FORCE)
set(MBED_UPLOAD_WANTS_EXTENDED_REMOTE ${UPLOAD_WANTS_EXTENDED_REMOTE} CACHE INTERNAL "" FORCE)
set(MBED_UPLOAD_LAUNCH_COMMANDS ${UPLOAD_LAUNCH_COMMANDS} CACHE INTERNAL "" FORCE)
set(MBED_UPLOAD_RESTART_COMMANDS ${UPLOAD_RESTART_COMMANDS} CACHE INTERNAL "" FORCE)

# ----------------------------------------------
# Function for creating targets

function(mbed_generate_upload_target target)
	# add upload target
	if ("${MBED_OUTPUT_EXT}" STREQUAL "" OR MBED_OUTPUT_EXT STREQUAL "bin")
		gen_upload_target(${target} ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_BASE_NAME:${target}>.bin)
	else()
		gen_upload_target(${target} ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_BASE_NAME:${target}>.hex)
	endif()
endfunction()