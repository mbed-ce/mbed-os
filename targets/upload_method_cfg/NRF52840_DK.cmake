# Mbed OS upload method configuration file for target NRF52840_DK.
# To change any of these parameters from their default values, set them in your build script between where you
# include mbed_toolchain_setup and where you add mbed os as a subdirectory.

# Notes:
# 1. If you have multiple Mbed devices plugged in to your machine, the MBED upload method only
#    works for this device if you set the serial number (e.g. -DMBED_UPLOAD_SERIAL_NUMBER=001050278063).
#    This is because for some reason the USB disk mode doesn't make the target name available to the host PC.
# 2. In my testing this device may need a manual reset via the reset button after
#    flashing code via the MBED upload method.
# 3. To use this device with PyOCD you need to install the J-Link software package on your system.
# 4. As of Sep 2025, PyOCD can flash but cannot reliably debug. Getting 'cannot read register
#    ipsr because core #0 is not halted'.
# 5. This upload method configuration reflects version 3 of the DK board, which uses an nRF5340 MCU and does
#    not have DAPLink firmware available, only J-Link OB. The older version would have had better support for OpenOCD,
#    Mbed, and PyOCD upload methods but does not seem to be available any more.


# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT JLINK)

# Config options for MBED
# -------------------------------------------------------------
set(MBED_UPLOAD_ENABLED TRUE)

# Config options for JLINK
# -------------------------------------------------------------
set(JLINK_UPLOAD_ENABLED TRUE)
set(JLINK_CPU_NAME nRF52840_xxAA)
set(JLINK_UPLOAD_INTERFACE SWD)
set(JLINK_CLOCK_SPEED 4000)

# Config options for PYOCD
# -------------------------------------------------------------

set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME nrf52840)
set(PYOCD_CLOCK_SPEED 4000k)
