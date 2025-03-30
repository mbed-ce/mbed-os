# Copyright (c) 2025 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

### STM32Cube DFU Upload Method
# This method needs the following parameters:
# STM32CUBE_DFU_CONNECT_COMMAND - "Connect" (-c) command to pass to the programmer

### Check if upload method can be enabled on this machine
find_package(STLINKTools COMPONENTS STM32CubeProg)
set(UPLOAD_STM32CUBE_DFU_FOUND ${STLINKTools_FOUND})

set(UPLOAD_SUPPORTS_DEBUG FALSE)

### Function to generate upload target

function(gen_upload_target TARGET_NAME BINARY_FILE)

	set(STM32CUBE_UPLOAD_PROBE_ARGS sn=${MBED_UPLOAD_SERIAL_NUMBER})

	add_custom_target(flash-${TARGET_NAME}
		COMMENT "Flashing ${TARGET_NAME} with STM32CubeProg..."
		COMMAND ${STM32CubeProg_COMMAND}
		-c ${STM32CUBE_DFU_CONNECT_COMMAND}
		${STM32CUBE_UPLOAD_PROBE_ARGS} # probe arg must be immediately after -c command as it gets appended to -c
		-w ${BINARY_FILE} ${MBED_UPLOAD_BASE_ADDR}
		VERBATIM
		USES_TERMINAL)

endfunction(gen_upload_target)