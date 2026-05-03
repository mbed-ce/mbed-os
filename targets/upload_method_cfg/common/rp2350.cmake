# Mbed OS upload method configuration file for most RP2350-based boards
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
# 1. As of Apr 2026, RP2350 support has not been released in mainline OpenOCD. It will probably be included in the
#    1.0 release, whenever that happens. Until then, you will need to build OpenOCD from source to use this chip.
#
#    Alternately, you should be able to use the Raspberry Pi fork of OpenOCD from here:
#    https://github.com/raspberrypi/pico-sdk-tools/releases/latest
#    However, be aware that (at least the Windows version) has a bug which requires a manual re-org of the
#    file structure: https://github.com/raspberrypi/pico-sdk-tools/issues/22

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT PICOTOOL)

# Config options for PICOTOOL
# -------------------------------------------------------------
set(PICOTOOL_UPLOAD_ENABLED TRUE)

# Config options for OPENOCD
# -------------------------------------------------------------

set(OPENOCD_UPLOAD_ENABLED TRUE)
set(OPENOCD_CHIP_CONFIG_COMMANDS
        -f interface/cmsis-dap.cfg
        -f target/rp2350.cfg
        -c "set USE_CORE cm0" # Don't pause core 1 as that causes weird effects like keeping the TIMER stuck at 0: https://github.com/raspberrypi/picoprobe/issues/45
        -c "adapter speed 4000"
)