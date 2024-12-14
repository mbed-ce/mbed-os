# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

### NXP LinkServer Upload Method
# This method needs the following parameters:
# LINKSERVER_DEVICE - Chip name and board to connect to, separated by a colon.

set(UPLOAD_SUPPORTS_DEBUG TRUE)

### Handle options
if("${MBED_UPLOAD_SERIAL_NUMBER}" STREQUAL "")
	# This argument causes Redlink to connect to the first available debug probe
	set(LINKSERVER_PROBE_ARGS "" CACHE INTERNAL "" FORCE)
else()
	set(LINKSERVER_PROBE_ARGS --probe ${MBED_UPLOAD_SERIAL_NUMBER} CACHE INTERNAL "" FORCE)
endif()

if("${LINKSERVER_DEVICE}" STREQUAL "")
	message(FATAL_ERROR "You must set LINKSERVER_DEVICE in your CMake scripts to use REDLINK")
endif()

### Check if upload method can be enabled on this machine
find_package(LinkServer)
set(UPLOAD_LINKSERVER_FOUND ${LinkServer_FOUND})

if(LinkServer_FOUND)
	if(${LinkServer_VERSION} VERSION_LESS 1.5.30 AND "${MBED_OUTPUT_EXT}" STREQUAL "hex")
		message(FATAL_ERROR "LinkServer <1.5.30 does not support flashing hex files! Please upgrade LinkServer and then clean and rebuild the project.")
	endif()
endif()

### Function to generate upload target

function(gen_upload_target TARGET_NAME BINARY_FILE)

	add_custom_target(flash-${TARGET_NAME}
		COMMENT "Flashing ${TARGET_NAME} with LinkServer..."
		COMMAND ${LinkServer_PATH}
			flash
			${LINKSERVER_PROBE_ARGS}
			${LINKSERVER_DEVICE}
			load
			--addr ${MBED_UPLOAD_BASE_ADDR}
			${BINARY_FILE})

endfunction(gen_upload_target)

### Commands to run the debug server.
set(UPLOAD_GDBSERVER_DEBUG_COMMAND
	${LinkServer_PATH}
	gdbserver
	${LINKSERVER_PROBE_ARGS}
	--gdb-port ${MBED_GDB_PORT}
	${LINKSERVER_DEVICE}
)

# request extended-remote GDB sessions
set(UPLOAD_WANTS_EXTENDED_REMOTE TRUE)

set(UPLOAD_LAUNCH_COMMANDS
	"monitor reset" # undocumented, but works
	"load"
	"break main"
	"monitor reset"

	# Workaround for LinkServer supplying incomplete memory information to GDB
	# (in particular, seems to be missing the peripheral memory space and external RAM).
	# Without this command, GDB will block the user from accessing values in any of these
	# memory spaces.
	"set mem inaccessible-by-default off"
)
set(UPLOAD_RESTART_COMMANDS
	"monitor reset"
)