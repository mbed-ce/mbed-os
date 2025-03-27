# Copyright (c) 2025 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

### dfu_util upload method
### This method can be used for chips with DFU bootloaders that talk to the dfu-util program.
### The chip must be rebooted into bootloader mode for the upload method to work.
# This method creates the following parameters:
# DFU_UTIL_TARGET_VID_PID - VID:PID pair of the target device being programmed.
# DFU_UTIL_TARGET_INTERFACE - Interface number of the interface on the target that should be programmed.

set(UPLOAD_SUPPORTS_DEBUG FALSE)

### Check if upload method can be enabled on this machine
find_package(dfu_util)
set(UPLOAD_DFU_UTIL_FOUND ${dfu_util_FOUND})

### Function to generate upload target
function(gen_upload_target TARGET_NAME BINARY_FILE)

	set(DFU_UTIL_SERIAL_ARGS "")
	if(NOT "${MBED_UPLOAD_SERIAL_NUMBER}" STREQUAL "")
		list(APPEND DFU_UTIL_SERIAL_ARGS --serial ${MBED_UPLOAD_SERIAL_NUMBER})
	endif()

	add_custom_target(flash-${TARGET_NAME}
		COMMAND ${dfu_util_PATH}
			--device ${DFU_UTIL_TARGET_VID_PID}
			--download ${BINARY_FILE}
			--alt ${DFU_UTIL_TARGET_INTERFACE}
			${DFU_UTIL_SERIAL_ARGS}
			--dfuse-address ${MBED_UPLOAD_BASE_ADDR}:leave
		VERBATIM
		USES_TERMINAL)

endfunction(gen_upload_target)
