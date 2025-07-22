# Mbed OS upload method configuration file for SparkFun Artemis DK
# To change any of these parameters from their default values, set them in your build script between where you
# include mbed_toolchain_setup and where you add mbed-os as a subdirectory.

# Notes:
# 1. Unlike all other SparkFun Artemis boards, this board has a CMSIS-DAP interface MCU on it, so it
#    should be debuggable & flashable out of the box via PyOCD.

set(UPLOAD_METHOD_DEFAULT MBED)

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

# Config options for MBED
# -------------------------------------------------------------
set(MBED_UPLOAD_ENABLED TRUE)