# Mbed OS upload method configuration file for target RA8P1_TITAN_MINI_CM33
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# The Cortex-M33 (CPU1) image lives in the upper Code MRAM partition at
# 0x020C0000. It is programmed through the CPU0 debug port (same probe and
# pyOCD/JLink device as the RA8P1_TITAN_MINI target), or merged into the
# M85 image with tools/merge_hex.py and flashed in one go.

# General config parameters
# -------------------------------------------------------------

set(UPLOAD_METHOD_DEFAULT PYOCD)

# Config options for MBED
# -------------------------------------------------------------

set(MBED_UPLOAD_ENABLED FALSE)

# Config options for PYOCD
# -------------------------------------------------------------

set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME R7KA8P1KF)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for JLINK
# -------------------------------------------------------------

set(JLINK_UPLOAD_ENABLED TRUE)
set(JLINK_CPU_NAME R7KA8P1KF)
set(JLINK_CLOCK_SPEED 4000)
set(JLINK_UPLOAD_INTERFACE SWD)
