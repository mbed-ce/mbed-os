# Mbed OS upload method configuration file for target FPB_RA8E1
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# Notes:
# 1. Using this device with PyOCD requires installing a pack:
#   pyocd pack install R7FA8E1AF
# 2. PyOCD is able to flash and debug, but it appears that it dies when attempting to load code at the
#   start of a debugging session. This means that, when using it, you must manually flash first, then
#   start debugging. This requires editing the generated IDE config files / gdbinit.

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT PYOCD)

# Config options for MBED
# -------------------------------------------------------------

set(MBED_UPLOAD_ENABLED FALSE)

# Config options for PYOCD
# -------------------------------------------------------------

set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME R7FA8E1AF)
set(PYOCD_CLOCK_SPEED 8000k)

# Config options for JLINK
# -------------------------------------------------------------

set(JLINK_UPLOAD_ENABLED TRUE)
set(JLINK_CPU_NAME R7FA8E1AF)
set(JLINK_CLOCK_SPEED 8000)
set(JLINK_UPLOAD_INTERFACE SWD)
