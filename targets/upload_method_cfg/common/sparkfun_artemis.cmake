# Mbed OS upload method configuration file for most SparkFun Artemis devices
# To change any of these parameters from their default values, set them in your build script between where you
# include mbed_toolchain_setup and where you add mbed-os as a subdirectory.

# Notes:
# 1. This board does not have an onboard debugger. You must use an external debugger, e.g. a PicoProbe
#    or J-Link, if you wish to debug code.

set(UPLOAD_METHOD_DEFAULT AMBIQ_SVL)

# Config options for PYOCD
# -------------------------------------------------------------
set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME ama3b1kk_kbr)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for JLINK
# -------------------------------------------------------------
set(JLINK_UPLOAD_ENABLED TRUE)
set(JLINK_CPU_NAME AMA3B1KK-KBR)
set(JLINK_CLOCK_SPEED 4000)
set(JLINK_UPLOAD_INTERFACE SWD)

# Config options for AMBIQ_SVL
# -------------------------------------------------------------
set(AMBIQ_SVL_UPLOAD_ENABLED TRUE)