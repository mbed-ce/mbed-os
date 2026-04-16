# Mbed OS upload method configuration file for most RP2040-based boards
#
# Currently, only picotool is supported out of the box for uploading code, and it requires manually putting the board
# into bootloader mode whenever you wish to program it (unlike the pico SDK which has special handling for this).
#
# If you have an external SWD debugger such as a Picoprobe or a Pitaya-Link, PyOCD and OpenOCD can also be used.
# 
# To change any of these parameters from their default values, set them in your build script between where you
# include mbed_toolchain_setup and where you add mbed os as a subdirectory.
#
# Notes:
# 1. As of PyOCD 0.44, PyOCD can only flash this chip if it already has a valid program on it. Attempting
#   to flash while the RP2040 is in USB bootloader mode (either because the BOOTSEL button was pressed, or
#   because it has no valid code to boot up to) will result in incorrect data being written to flash.
#   I filed a bug about this: https://github.com/pyocd/pyOCD/issues/1961
# 2. OpenOCD will fail to flash with "Error: Unknown flash device (ID 0x00ffffff)" if the RP2040 has an invalid
#   program. To get around this, either flash a valid program with picotool first, or boot into the bootloader
#   using BOOTSEL.

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT PICOTOOL)

# Config options for PICOTOOL
# -------------------------------------------------------------
set(PICOTOOL_UPLOAD_ENABLED TRUE)

# Config options for PYOCD
# -------------------------------------------------------------

set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME rp2040_core0)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for OPENOCD
# -------------------------------------------------------------

set(OPENOCD_UPLOAD_ENABLED TRUE)
set(OPENOCD_CHIP_CONFIG_COMMANDS
        -f interface/cmsis-dap.cfg
        -f target/rp2040.cfg
        -c "set USE_CORE 0" # Don't pause core 1 as that causes weird effects like keeping the TIMER stuck at 0: https://github.com/raspberrypi/picoprobe/issues/45
        -c "adapter speed 4000"
)