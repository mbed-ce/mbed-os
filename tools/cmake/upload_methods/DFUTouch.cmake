# 1200 bps CDC touch: opens a serial port at 1200 baud and closes it,
# causing the running firmware to reset into the bootloader.
# If the port doesn't respond, the device may already be in DFU mode -
# this is not an error, we just print a note and continue.
#
# Expects: -DTOUCH_PORT=COM8

find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Use the mbed-os venv python if available (has pyserial installed)
set(DFU_TOUCH_PYTHON ${Python3_EXECUTABLE})
if(EXISTS ${CMAKE_SOURCE_DIR}/mbed-os/venv/Scripts/python.exe)
    set(DFU_TOUCH_PYTHON ${CMAKE_SOURCE_DIR}/mbed-os/venv/Scripts/python.exe)
endif()

execute_process(
    COMMAND ${DFU_TOUCH_PYTHON} -c
        "import serial,sys; s=serial.Serial('${TOUCH_PORT}',1200,timeout=0.5); s.close()"
    RESULT_VARIABLE TOUCH_RESULT
    ERROR_VARIABLE TOUCH_ERR
    OUTPUT_QUIET
)

if(TOUCH_RESULT EQUAL 0)
    message(STATUS "1200bps touch on ${TOUCH_PORT} OK")
else()
    message(STATUS "1200bps touch on ${TOUCH_PORT} skipped (device may already be in DFU mode)")
endif()

# Give the bootloader time to enumerate (typically <2s)
execute_process(COMMAND ${CMAKE_COMMAND} -E sleep 2)
