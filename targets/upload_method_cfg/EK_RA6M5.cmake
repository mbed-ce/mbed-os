# Mbed OS upload method configuration file for target EK_RA6M5
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# General config parameters
# -------------------------------------------------------------
# The on-board debugger is a SEGGER J-Link OB whose virtual COM port is not
# wired to the RA6M5 MCU, so J-Link (SWD, with RTT for the console) is the
# default upload method for this board.
set(UPLOAD_METHOD_DEFAULT JLINK)

# Config options for MBED
# -------------------------------------------------------------

set(MBED_UPLOAD_ENABLED FALSE)

# Config options for PYOCD
# -------------------------------------------------------------
# The board MCU is an R7FA6M5BH (2MB flash); R7FA6M5BF (1MB) is the closest
# device in the CMSIS pack used by pyOCD and links apps into the first 1MB.
set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME R7FA6M5BF)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for JLINK
# -------------------------------------------------------------
# Exact device fitted on the EK-RA6M5 (R7FA6M5BH3CFC, LQFP176, 2MB flash).
set(JLINK_UPLOAD_ENABLED TRUE)
set(JLINK_CPU_NAME R7FA6M5BH)
set(JLINK_CLOCK_SPEED 4000)
set(JLINK_UPLOAD_INTERFACE SWD)

# Config options for E2LITE
# ------------------------------------------------------------

set(E2LITE_UPLOAD_ENABLED TRUE)
set(E2LITE_RFP_DEVICE RA) # RFP only knows the generic "RA" device name (auto-detect), not exact part numbers
