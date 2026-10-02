# Mbed OS upload method configuration file for target FPB_RA4E1
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT PYOCD)

# Config options for MBED
# -------------------------------------------------------------

set(MBED_UPLOAD_ENABLED FALSE)

# Config options for PYOCD
# -------------------------------------------------------------

set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME R7FA4E10D)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for JLINK
# -------------------------------------------------------------

set(JLINK_UPLOAD_ENABLED TRUE)
set(JLINK_CPU_NAME R7FA4E10D)
set(JLINK_CLOCK_SPEED 4000)
set(JLINK_UPLOAD_INTERFACE SWD)
