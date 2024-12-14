# Copyright (c) 2023 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

### Picotool upload method
### This method can be used for Raspberry Pi Pico chips with bootloaders that talk to the picotool program.
### The Pico must be manually rebooted into bootloader mode for the upload method to work.
# This method creates the following options:
# PICOTOOL_TARGET_BUS - If set, the `--bus` argument is passed to picotool to select the desired pico to program
# PICOTOOL_TARGET_ADDRESS - If set, the `--address` argument is passed to picotool to select the desired pico to program

set(UPLOAD_SUPPORTS_DEBUG FALSE)

### Check if upload method can be enabled on this machine
find_package(Picotool)
set(UPLOAD_PICOTOOL_FOUND ${Picotool_FOUND})

### Set up options
set(PICOTOOL_TARGET_BUS "" CACHE STRING "If set, the given bus number is passed to picotool to select the desired pico to program.  You can get the bus number from running `picotool list` with multiple picos plugged in.")
set(PICOTOOL_TARGET_ADDRESS "" CACHE STRING "If set, the given address is passed to picotool to select the desired pico to program.  You can get the address from running `picotool list` with multiple picos plugged in.")

### Function to generate upload target
function(gen_upload_target TARGET_NAME BINARY_FILE)

	set(PICOTOOL_TARGET_ARGS "")
	if(NOT "${PICOTOOL_TARGET_BUS}" STREQUAL "")
		list(APPEND PICOTOOL_TARGET_ARGS --bus ${PICOTOOL_TARGET_BUS})
	endif()
	if(NOT "${PICOTOOL_TARGET_ADDRESS}" STREQUAL "")
		list(APPEND PICOTOOL_TARGET_ARGS --address ${PICOTOOL_TARGET_ADDRESS})
	endif()

	if("${MBED_OUTPUT_EXT}" STREQUAL "hex")
        message(FATAL_ERROR "Bin file output must be enabled to use picotool.  Set MBED_OUTPUT_EXT to empty string or to 'bin' in your top level CMakeLists.txt!")
    endif()

	add_custom_target(flash-${TARGET_NAME}
		COMMAND ${Picotool}
			load
			--verify
		    --execute
			--offset ${MBED_UPLOAD_BASE_ADDR}
			${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_BASE_NAME:${TARGET_NAME}>.bin)

endfunction(gen_upload_target)
