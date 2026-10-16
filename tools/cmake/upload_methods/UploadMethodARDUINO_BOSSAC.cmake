### Arduino Bossac upload method
### This method can be used for boards with Arduino bootloaders that talk to the bossac flash program.
# The Arduino IDE performs a "1200 bps touch" (open/close the CDC serial port
# at 1200 baud) before calling bossac, which triggers the running firmware or
# the ESP32-S3 bridge to reset the MCU into the bootloader. We replicate that
# here using the same DFUTouch.cmake helper.
#
# This method creates the following options:
# ARDUINO_BOSSAC_SERIAL_PORT - Serial port to talk to the device bootloader on, e.g. "COM7"
# ARDUINO_BOSSAC_EXTRA_ARGS  - Extra arguments to pass to bossac (e.g. "--offset=0x4000")
# ARDUINO_BOSSAC_DO_TOUCH    - Whether to do the 1200 bps touch first (default TRUE)

set(UPLOAD_SUPPORTS_DEBUG FALSE)

### Check if upload method can be enabled on this machine
find_package(ArduinoBossac)
set(UPLOAD_ARDUINO_BOSSAC_FOUND ${ArduinoBossac_FOUND})

# Save script directory for the touch helper
set(ARDUINO_BOSSAC_SCRIPT_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "Bossac script dir")

### Set up options
set(ARDUINO_BOSSAC_SERIAL_PORT "" CACHE STRING "Serial port to talk to the device bootloader on, e.g. 'COM7'")
set(ARDUINO_BOSSAC_EXTRA_ARGS "" CACHE STRING "Extra arguments to pass to bossac (e.g. '--offset=0x4000')")
set(ARDUINO_BOSSAC_DO_TOUCH TRUE CACHE BOOL "Perform 1200 bps touch before uploading")
if("${ARDUINO_BOSSAC_SERIAL_PORT}" STREQUAL "")
	message(WARNING "Please set ARDUINO_BOSSAC_SERIAL_PORT to the serial port to communicate with the target bootloader on.  Until this is set, flashing will not work.")
endif()

### Function to generate upload target
function(gen_upload_target TARGET_NAME BINARY_FILE)

	set(BOSSAC_UPLOAD_CMDS "")

	# 1200 bps touch (same as Arduino IDE does before calling bossac)
	if(ARDUINO_BOSSAC_DO_TOUCH)
		list(APPEND BOSSAC_UPLOAD_CMDS
			COMMAND ${CMAKE_COMMAND}
				-DTOUCH_PORT=${ARDUINO_BOSSAC_SERIAL_PORT}
				-P ${ARDUINO_BOSSAC_SCRIPT_DIR}/DFUTouch.cmake
		)
	endif()

	list(APPEND BOSSAC_UPLOAD_CMDS
		COMMAND ${ArduinoBossac}
			-d
			--port=${ARDUINO_BOSSAC_SERIAL_PORT}
			-U
			-e
			-w ${BINARY_FILE}
			-R
			${ARDUINO_BOSSAC_EXTRA_ARGS}
	)

	add_custom_target(flash-${TARGET_NAME}
		${BOSSAC_UPLOAD_CMDS}
		VERBATIM)

endfunction(gen_upload_target)
