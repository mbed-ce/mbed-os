# Mbed OS upload method configuration file for target B_U585_IOT02A.
# To change any of these parameters from their default values, set them in your build script between where you
# include mbed_toolchain_setup and where you add mbed os as a subdirectory.

# Notes:
# 1. The KitProg programmer on this board boots up in KitProg mode. To enable mass storage device mode, press the
#    button labeled "MODE" near the USB port to switch to DAPLink mode. The MBED and PYOCD upload methods need
#    the board to be in DAPLink mode (which is deprecated by Infineon).
# 2. Mbed upload method seems VERY slow on this board (takes like 1 minute to flash!).
# 3. PyOCD is tested working with no issues noted.

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
set(PYOCD_TARGET_NAME cy8c6xx5)
set(PYOCD_CLOCK_SPEED 4000k)
set(PYOCD_GDB_CLIENT_CORE_INDEX 1)
