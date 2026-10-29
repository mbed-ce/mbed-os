# Mbed OS upload method configuration file for target FRDM_KL43Z.
# To change any of these parameters from their default values, set them in your build script between where you
# include mbed_toolchain_setup and where you add mbed os as a subdirectory.
#
# Notes:
# 1. Be sure to update the DAPLink firmware on the board via these instructions: https://www.nxp.com/design/design-center/development-boards-and-designs/OPENSDA?tid=vanOpenSDA#FRDM-KW41Z
# 2. The board comes with J-Link firmware loaded by default, so only the JLINK and MBED upload methods will work.
#

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT MBED)

# Config options for MBED
# -------------------------------------------------------------

set(MBED_UPLOAD_ENABLED TRUE)
set(MBED_RESET_BAUDRATE 115200)

# Config options for JLINK
# -------------------------------------------------------------
set(JLINK_UPLOAD_ENABLED TRUE)
set(JLINK_CPU_NAME MKW41Z512XXX4)
set(JLINK_UPLOAD_INTERFACE SWD)
set(JLINK_CLOCK_SPEED 4000)

# Config options for PYOCD
# -------------------------------------------------------------
set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME kw41z4)
set(PYOCD_CLOCK_SPEED 4000k)


# Config options for LINKSERVER
# -------------------------------------------------------------
set(LINKSERVER_UPLOAD_ENABLED TRUE)
set(LINKSERVER_DEVICE MKW41Z4:FRDM-KW41Z)
