# Mbed OS upload method configuration file for target ARDUINO_UNO_R4_MINIMA.
# To change any of these parameters from their default values, set them in your build script between where you
# include mbed_toolchain_setup and where you add mbed os as a subdirectory.

# General config parameters
# -------------------------------------------------------------
# Default: direct dfu-util (works with the DFU bootloader shipped with the
# arduino:renesun_uno >= 1.1 core). Requires double-tap reset to enter DFU
# mode. J-Link and pyOCD work without the bootloader via SWD.
set(UPLOAD_METHOD_DEFAULT DFU_UTIL)

# Config options for DFU_UTIL
# -------------------------------------------------------------
set(DFU_UTIL_UPLOAD_ENABLED TRUE)

# Config options for JLINK (SWD, no bootloader required)
# -------------------------------------------------------------
set(JLINK_UPLOAD_ENABLED TRUE)
set(JLINK_CPU_NAME R7FA4M1AB)

# Config options for PYOCD (SWD, no bootloader required)
# -------------------------------------------------------------
set(PYOCD_UPLOAD_ENABLED TRUE)
