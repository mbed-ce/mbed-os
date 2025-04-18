# Mbed OS upload method configuration file for target FF_LPC546XX.
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.
#
# Notes:
# 1. PyOCD support for this device requires installing a pack:
#     source the Mbed OS venv, then run
#     $ pyocd pack install LPC54606J512BD100
# 2. PyOCD support for this device is glitchy. Flashing and debugging work, but trying to flash
#     or reset while in a debug session appears to crash the debug session. This means you have to
#     edit the generated Mbed debug scripts to disable loading the code when starting a debug session.

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT MBED)

# Config options for MBED
# -------------------------------------------------------------

set(MBED_UPLOAD_ENABLED TRUE)
set(MBED_RESET_BAUDRATE 115200)

# Config options for PYOCD
# -------------------------------------------------------------
set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME LPC54606J512BD100)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for LINKSERVER
# -------------------------------------------------------------
set(LINKSERVER_UPLOAD_ENABLED TRUE)
set(LINKSERVER_DEVICE ${CMAKE_CURRENT_LIST_DIR}/linkserver_devices/FF_LPC546XX.json)
