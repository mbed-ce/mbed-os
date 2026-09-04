# Mbed OS upload method configuration file for target EK_RA6M3G
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# General config parameters
# -------------------------------------------------------------
# The on-board debugger is a SEGGER J-Link OB whose virtual COM port is not
# wired to the RA6M3 MCU, so J-Link (SWD, with RTT for the console) is the
# default upload method for this board.
set(UPLOAD_METHOD_DEFAULT JLINK)

# Config options for MBED
# -------------------------------------------------------------

set(MBED_UPLOAD_ENABLED FALSE)

# Config options for PYOCD
# -------------------------------------------------------------

set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME R7FA6M3AF)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for JLINK
# -------------------------------------------------------------

set(JLINK_UPLOAD_ENABLED TRUE)
set(JLINK_CPU_NAME R7FA6M3AF)
set(JLINK_CLOCK_SPEED 4000)
set(JLINK_UPLOAD_INTERFACE SWD)

# Config options for E2LITE
# ------------------------------------------------------------

set(E2LITE_UPLOAD_ENABLED TRUE)
set(E2LITE_RFP_DEVICE RA) # RFP only knows the generic "RA" device name (auto-detect), not exact part numbers
