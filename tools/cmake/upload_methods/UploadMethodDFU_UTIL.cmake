### DFU-util upload method
### This method can be used with boards that expose a native DFU bootloader,
### e.g. the Arduino UNO R4 with its stock DFU bootloader.
#
# If DFU_UTIL_SERIAL_PORT is set to a serial port (e.g. "COM8"), a
# "1200 bps touch" is performed on that port first, making the running
# application reset into the bootloader, then dfu-util uploads via DFU.
# Without DFU_UTIL_SERIAL_PORT, the device must be manually put into
# DFU mode (double-tap reset on the UNO R4).
#
# This method creates the following options:
# DFU_UTIL_DEVICE - USB vendor:product IDs of the device in DFU mode,
#                   e.g. "2341:0369". May list runtime and DFU IDs
#                   separated by a comma.
# DFU_UTIL_SERIAL_PORT - Serial port of the running application (CDC),
#                        e.g. "COM8". When set, a 1200 bps touch is
#                        performed before uploading.

set(UPLOAD_SUPPORTS_DEBUG FALSE)

### Check if upload method can be enabled on this machine
find_package(DFUUtil)
set(UPLOAD_DFU_UTIL_FOUND ${DFUUtil_FOUND})

### Set up options
set(DFU_UTIL_DEVICE "2341:0069,2341:0369" CACHE STRING "USB device IDs (vendor:product) of the target in runtime and DFU mode")
set(DFU_UTIL_SERIAL_PORT "" CACHE STRING "Serial port for 1200bps touch (e.g. COM8). Empty = manual DFU entry required")

# Save the script directory (CACHE INTERNAL so it's visible when
# gen_upload_target is called from the target's CMakeLists.txt)
set(DFU_UTIL_SCRIPT_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "DFU util script directory")

### Function to generate upload target
function(gen_upload_target TARGET_NAME BINARY_FILE)

	set(DFU_UTIL_UPLOAD_CMDS "")

	# 1200 bps CDC touch if DFU_UTIL_SERIAL_PORT is set
	if(NOT "${DFU_UTIL_SERIAL_PORT}" STREQUAL "")
		list(APPEND DFU_UTIL_UPLOAD_CMDS
			COMMAND ${CMAKE_COMMAND}
				-DTOUCH_PORT=${DFU_UTIL_SERIAL_PORT}
				-P ${DFU_UTIL_SCRIPT_DIR}/DFUTouch.cmake
		)
	endif()

	list(APPEND DFU_UTIL_UPLOAD_CMDS
		COMMAND ${DFUUtil}
			--device=${DFU_UTIL_DEVICE}
			--alt=0
			--download ${BINARY_FILE}
			-Q
	)

	add_custom_target(flash-${TARGET_NAME}
		${DFU_UTIL_UPLOAD_CMDS}
		VERBATIM)

endfunction(gen_upload_target)
