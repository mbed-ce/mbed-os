# Mbed OS upload method configuration file for most RP2040-based boards
#
# Currently, only picotool is supported out of the box for uploading code, and it requires manually putting the board
# into bootloader mode whenever you wish to program it (unlike the pico SDK which has special handling for this).
#
# If you have an external SWD debugger such as a Picoprobe or a Pitaya-Link, PyOCD and OpenOCD can also be used.
# 
# To change any of these parameters from their default values, set them in your build script between where you
# include mbed_toolchain_setup and where you add mbed os as a subdirectory.

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