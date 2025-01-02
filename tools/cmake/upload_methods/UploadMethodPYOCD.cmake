# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

### pyOCD Upload Method
# This method needs the following parameters:
# PYOCD_TARGET_NAME - Name of your processor as passed to the -t option of pyOCD. This is usually the full or partial model number.
# PYOCD_CLOCK_SPEED - Clock speed of the JTAG or SWD connection. Default is in Hz, but can use k and M suffixes for MHz and GHz
# PYOCD_EXTRA_OPTIONS - Extra options to pass to the pyocd 'flash' and 'gdbserver' commands.

set(UPLOAD_SUPPORTS_DEBUG TRUE)

### Check if upload method can be enabled on this machine
include(CheckPythonPackage)
check_python_package(pyocd HAVE_PYOCD)
set(UPLOAD_PYOCD_FOUND ${HAVE_PYOCD})

### Function to generate upload target
set(PYOCD_PROBE_ARGS "" CACHE INTERNAL "" FORCE)
if(NOT "${MBED_UPLOAD_SERIAL_NUMBER}" STREQUAL "")
	set(PYOCD_PROBE_ARGS --probe ${MBED_UPLOAD_SERIAL_NUMBER} CACHE INTERNAL "" FORCE)
endif()

function(gen_upload_target TARGET_NAME BINARY_FILE)

	add_custom_target(flash-${TARGET_NAME}
		COMMENT "Flashing ${TARGET_NAME} with pyOCD..."
		COMMAND ${Python3_EXECUTABLE}
		-m pyocd
		flash
		-v
		--no-wait
		-f ${PYOCD_CLOCK_SPEED}
		-t ${PYOCD_TARGET_NAME}
		${PYOCD_PROBE_ARGS}
		--base-address ${MBED_UPLOAD_BASE_ADDR}
		${PYOCD_EXTRA_OPTIONS}
		${BINARY_FILE})

endfunction(gen_upload_target)

### Commands to run the debug server.
set(UPLOAD_GDBSERVER_DEBUG_COMMAND
	${Python3_EXECUTABLE}
	-m pyocd
	gdbserver
	--no-wait
	-t ${PYOCD_TARGET_NAME}
	${PYOCD_PROBE_ARGS}
	-f ${PYOCD_CLOCK_SPEED}
	-p ${MBED_GDB_PORT}
	${PYOCD_EXTRA_OPTIONS})

# Reference: https://github.com/Marus/cortex-debug/blob/056c03f01e008828e6527c571ef5c9adaf64083f/src/pyocd.ts#L40
set(UPLOAD_LAUNCH_COMMANDS
"monitor reset halt"
"load"
"tbreak main"

# It appears the device under debug must be halted after UPLOAD_LAUNCH_COMMANDS,
# or debugger will become abnormal.
"monitor reset halt"
)
set(UPLOAD_RESTART_COMMANDS
"monitor reset halt"
)