# Copyright (c) 2026 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

### E2-Lite Upload Method
# Flashes Renesas RA-family MCUs through an E2 emulator Lite (or E2 emulator) debug probe,
# using the Renesas Flash Programmer command line tool (rfp-cli).
#
# This method supports the following parameters (set them in the target's upload_method_cfg
# file, in your top-level CMakeLists.txt before add_subdirectory(mbed-os), or on the CMake
# command line):
# - E2LITE_RFP_DEVICE: Device name passed to RFP with the -device option.
#   Defaults to "RA", which works for any RA-family MCU.  Setting the exact device name
#   (e.g. R7FA8E1AF) allows RFP to skip device auto-detection, which is a bit faster.
# - E2LITE_RFP_TOOL: RFP tool type.  Defaults to "e2l" (E2 emulator Lite).  "e2" (E2 emulator)
#   also works, this method name is kept for consistency with the other probe-based methods.
# - E2LITE_INTERFACE: Debug interface to use.  Defaults to "swd".
# - E2LITE_SPEED: Communication speed for the probe.  Defaults to auto.
# - E2LITE_RFP_ARGS: Extra arguments to pass to rfp-cli when flashing.
#
# The serial number of the probe can be selected with the standard MBED_UPLOAD_SERIAL_NUMBER
# option (this is convenient if multiple Renesas probes are connected).
#
# Debugging is provided by the Renesas GDB server (e2-server-gdb), which is part of the
# Renesas debug support files used by e2 studio and the Renesas VS Code extensions.  If it
# cannot be found, this upload method operates in flash-only mode.  The following options
# control the debug setup:
# - E2LITE_GDBSERVER_PATH: Full path to the e2-server-gdb executable.
# - E2LITE_DEBUG_SUPPORT_DIR: Root of the Renesas debug support files (the folder that
#   contains e2-server-gdb and the ra/ folder with device data).  Defaults to the folder
#   containing the detected e2-server-gdb.
# - E2LITE_GDBSERVER_ARGS: Extra arguments appended to the e2-server-gdb command line.

### Check if upload method can be enabled on this machine
find_package(RFP)
set(UPLOAD_E2LITE_FOUND ${RFP_FOUND})

### Set up flashing parameters
# NOTE: variables used by gen_upload_target() must be CACHE INTERNAL, because that function
# runs outside of the mbed-os subdirectory scope (see the warning in UploadMethodManager.cmake).
if(NOT DEFINED E2LITE_RFP_DEVICE)
	set(E2LITE_RFP_DEVICE RA)
endif()

if(NOT DEFINED E2LITE_RFP_TOOL)
	set(E2LITE_RFP_TOOL e2l)
endif()

if(NOT DEFINED E2LITE_INTERFACE)
	set(E2LITE_INTERFACE swd)
endif()

set(E2LITE_RFP_DEVICE "${E2LITE_RFP_DEVICE}" CACHE INTERNAL "" FORCE)
set(E2LITE_RFP_TOOL "${E2LITE_RFP_TOOL}" CACHE INTERNAL "" FORCE)
set(E2LITE_INTERFACE "${E2LITE_INTERFACE}" CACHE INTERNAL "" FORCE)

# Serial number of the probe is appended to the tool name, see rfp-cli -tool option
if(NOT "${MBED_UPLOAD_SERIAL_NUMBER}" STREQUAL "")
	set(E2LITE_TOOL_SPEC "${E2LITE_RFP_TOOL}:${MBED_UPLOAD_SERIAL_NUMBER}" CACHE INTERNAL "" FORCE)
else()
	set(E2LITE_TOOL_SPEC "${E2LITE_RFP_TOOL}" CACHE INTERNAL "" FORCE)
endif()

if(NOT "${E2LITE_SPEED}" STREQUAL "")
	set(E2LITE_SPEED_ARGS -speed ${E2LITE_SPEED} CACHE INTERNAL "" FORCE)
else()
	set(E2LITE_SPEED_ARGS "" CACHE INTERNAL "" FORCE)
endif()

if(NOT DEFINED E2LITE_RFP_ARGS)
	set(E2LITE_RFP_ARGS "" CACHE INTERNAL "" FORCE)
endif()

# rfp-cli wants plain hex addresses (no 0x prefix) for the -bin option
string(REGEX REPLACE "^0x" "" E2LITE_BIN_ADDR "${MBED_UPLOAD_BASE_ADDR}")
set(E2LITE_BIN_ADDR "${E2LITE_BIN_ADDR}" CACHE INTERNAL "" FORCE)

### Function to generate upload target
function(gen_upload_target TARGET_NAME BINARY_FILE)

	if(BINARY_FILE MATCHES "\\.bin$")
		# Raw binary files have no address information, so the base address must be given to RFP
		set(E2LITE_DATA_ARGS -bin ${E2LITE_BIN_ADDR} ${BINARY_FILE})
	else()
		# Hex files already contain their load addresses
		set(E2LITE_DATA_ARGS ${BINARY_FILE})
	endif()

	add_custom_target(flash-${TARGET_NAME}
		COMMENT "Flashing ${TARGET_NAME} with Renesas Flash Programmer (E2-Lite)..."
		COMMAND ${rfp_cli_PATH}
		-device ${E2LITE_RFP_DEVICE}
		-tool ${E2LITE_TOOL_SPEC}
		-if ${E2LITE_INTERFACE}
		${E2LITE_SPEED_ARGS}
		${E2LITE_RFP_ARGS}
		-auto # erase, program, and verify the flash
		-run # let the target run after the probe disconnects
		-noquery # never stop to prompt for authentication codes interactively
		-nologo
		${E2LITE_DATA_ARGS}
		VERBATIM
		USES_TERMINAL)

endfunction(gen_upload_target)

# Reset target: rfp-cli resets the target when it disconnects (the -reset option is enabled
# by default), so simply connecting and disconnecting is enough to reset the chip.
add_custom_target(reset
	COMMENT "Resetting target with Renesas Flash Programmer (E2-Lite)..."
	COMMAND ${rfp_cli_PATH}
		-device ${E2LITE_RFP_DEVICE}
		-tool ${E2LITE_TOOL_SPEC}
		-if ${E2LITE_INTERFACE}
		${E2LITE_SPEED_ARGS}
		${E2LITE_RFP_ARGS}
		-noquery
		-nologo
	VERBATIM
	USES_TERMINAL)

### Commands to run the debug server.
# The Renesas GDB server (e2-server-gdb) is shipped with e2 studio and with the debug
# support files of the Renesas VS Code extensions.  We search for it in:
# - the E2LITE_GDBSERVER_PATH / E2LITE_DEBUG_SUPPORT_DIR variables, if the user set them
# - the debug support files managed by the Renesas VS Code debug extension
# - the default e2 studio install locations
set(E2LITE_GDBSERVER_PATH "" CACHE FILEPATH "Path to the Renesas E2/E2-Lite GDB server (e2-server-gdb).  If empty, Mbed will try to locate it automatically.")

if("${E2LITE_GDBSERVER_PATH}" STREQUAL "" AND DEFINED E2LITE_DEBUG_SUPPORT_DIR AND EXISTS "${E2LITE_DEBUG_SUPPORT_DIR}")
	file(GLOB_RECURSE E2LITE_GDBSERVER_CANDIDATES LIST_DIRECTORIES FALSE
		"${E2LITE_DEBUG_SUPPORT_DIR}/e2-server-gdb.exe"
		"${E2LITE_DEBUG_SUPPORT_DIR}/e2-server-gdb")
	if(E2LITE_GDBSERVER_CANDIDATES)
		list(GET E2LITE_GDBSERVER_CANDIDATES 0 E2LITE_GDBSERVER_PATH)
	endif()
endif()

if("${E2LITE_GDBSERVER_PATH}" STREQUAL "" AND EXISTS "$ENV{APPDATA}/Code/User/globalStorage/renesaselectronicscorporation.renesas-debug")
	# Debug support files managed by the Renesas VS Code debug extension (downloaded on
	# demand by the extension's Support Files Manager)
	file(GLOB_RECURSE E2LITE_GDBSERVER_CANDIDATES LIST_DIRECTORIES FALSE
		"$ENV{APPDATA}/Code/User/globalStorage/renesaselectronicscorporation.renesas-debug/e2-server-gdb.exe")
	if(E2LITE_GDBSERVER_CANDIDATES)
		list(GET E2LITE_GDBSERVER_CANDIDATES 0 E2LITE_GDBSERVER_PATH)
	endif()
endif()

if("${E2LITE_GDBSERVER_PATH}" STREQUAL "" AND EXISTS "C:/Renesas")
	# e2 studio default install locations
	file(GLOB_RECURSE E2LITE_GDBSERVER_CANDIDATES LIST_DIRECTORIES FALSE
		"C:/Renesas/e2-server-gdb.exe")
	if(E2LITE_GDBSERVER_CANDIDATES)
		list(GET E2LITE_GDBSERVER_CANDIDATES 0 E2LITE_GDBSERVER_PATH)
	endif()
endif()

if(EXISTS "${E2LITE_GDBSERVER_PATH}")
	set(UPLOAD_SUPPORTS_DEBUG TRUE)

	if(NOT DEFINED E2LITE_DEBUG_SUPPORT_DIR)
		get_filename_component(E2LITE_DEBUG_SUPPORT_DIR "${E2LITE_GDBSERVER_PATH}" DIRECTORY)
	endif()

	# e2-server-gdb expects to be run from the folder it is installed in (it loads its
	# device data from there), so use cmake -E chdir to set the working directory.
	# The command line below mirrors the parameters used by the Renesas VS Code debug
	# extension for RA-family MCUs with the E2-Lite (ra-E2LITE.ts).
	set(UPLOAD_WANTS_EXTENDED_REMOTE TRUE)
	set(UPLOAD_GDBSERVER_DEBUG_COMMAND
		${CMAKE_COMMAND} -E chdir ${E2LITE_DEBUG_SUPPORT_DIR}
		${E2LITE_GDBSERVER_PATH}
		-g E2LITE
		-p ${MBED_GDB_PORT}
		-uConnectionTimeout=30
		-uClockSrc=0
		-uAllowClockSourceInternal=1
		-uInteface=${E2LITE_INTERFACE} # (the typo in this option name is intentional, it is what the server expects)
		-uIfSpeed=auto
		-w 1
		-z 33
		-uIdCodeBytes=FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
		-uResetCon=1
		-uLowPower=1
		-uresetOnReload=1
		-n 0
		-uFlashBp=1
		-uhookWorkRamSize=0x400
		-ueraseRomOnDownload=0
		-ueraseDataRomOnDownload=0
		-uTimeMeasurementEnable=1
		-uEnableSciBoot=0
		-l
		--english
		--gdbVersion=7.2
		${E2LITE_GDBSERVER_ARGS})

	set(UPLOAD_LAUNCH_COMMANDS
		"monitor reset"
		"load"

		# Tell GDB to allow reads to any region of memory, ignoring the memory map sent by the GDB server.
		# This is needed because often the GDB server's memory map doesn't include peripheral memory, so
		# the user can't inspect peripheral registers.
		"set mem inaccessible-by-default off"

		"tbreak main"
		"monitor reset"
	)
	set(UPLOAD_RESTART_COMMANDS
		"monitor reset"
	)
else()
	set(UPLOAD_SUPPORTS_DEBUG FALSE)
	message(STATUS "Mbed: Renesas GDB server (e2-server-gdb) not found, debugging with the E2LITE upload method is disabled "
		"(flashing still works).  e2-server-gdb is part of the Renesas debug support files, which can be obtained by "
		"installing e2 studio, or by downloading them with the Support Files Manager of the Renesas VS Code debug "
		"extension.  Once installed, set E2LITE_GDBSERVER_PATH or E2LITE_DEBUG_SUPPORT_DIR to point at it.")
endif()
