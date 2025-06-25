# Copyright (c) 2025 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

### Ambiq SVL upload method
### This upload method allows flashing code over the UART port to SparkFun boards using
### the Ambiq Apollo3 SoC and SVL bootloader.
### Unlike some other bootloader-based upload methods, the SparkFun setup is able to reset the
### MCU into bootloader without needing any buttons to be pressed on the board!
# This method creates the following options:
# AMBIQ_SVL_SERIAL_PORT - Serial port to connect to the SVL bootloader over, e.g. 'COM20' or '/dev/ttyUSB0'
# This method creates the following parameters:
# AMBIQ_SVL_UPLOAD_BAUD - Baudrate to upload at. Defaults to 115200 baud.

set(UPLOAD_SUPPORTS_DEBUG FALSE)

set(AMBIQ_SVL_SERIAL_PORT "" CACHE STRING "Serial port to connect to the SVL bootloader over, e.g. 'COM20' or '/dev/ttyACM0'")

# note: the ambiq_svl script is included under tools/python/ambiq_svl and is already
# found by mbed_python_interpreter.cmake
set(UPLOAD_AMBIQ_SVL_FOUND TRUE)

### Function to generate upload target
function(gen_upload_target TARGET_NAME BINARY_FILE)

	if("${AMBIQ_SVL_SERIAL_PORT}" STREQUAL "")
		message(FATAL_ERROR "Must specify AMBIQ_SVL_SERIAL_PORT to use the AMBIQ_SVL upload method!")
	endif()

	if("${AMBIQ_SVL_UPLOAD_BAUD}" STREQUAL "")
		set(AMBIQ_SVL_UPLOAD_BAUD 115200)
	endif()

	add_custom_target(flash-${TARGET_NAME}
		COMMAND ${ambiq_svl}
			-f ${BINARY_FILE}
			-b ${AMBIQ_SVL_UPLOAD_BAUD}
			${AMBIQ_SVL_SERIAL_PORT}
		VERBATIM
		USES_TERMINAL)

endfunction(gen_upload_target)
