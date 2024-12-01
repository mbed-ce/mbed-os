# Mbed OS upload method configuration file for target K64F.
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.
#
# Notes:
# 1. Be sure to update the DAPLink firmware on the board via these instructions: https://os.mbed.com/blog/entry/DAPLink-bootloader-update/
# 2. OpenOCD 0.12 flashes this device perfectly and can enter a debug session, but cannot hit breakpoints
# 3. LinkServer can both flash and debug, so it's the recommended upload method for this device.
# 4. LinkServer does appear to have a bug where it doesn't map the peripheral registers as valid memory, so you can't
#    inspect them.  I was able to work around this by inserting a block like this into <LinkServer install dir>/devices/FRDM-K64F.json:
#
#         "name": "MK64FN1M0xxx12",
#         "family": "K6x",
#         "memory": [
# +       {
# +          "location": "0x40000000",
# +          "size": "0x000f0000",
# +          "type": "RAM"
# +        },

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
set(PYOCD_TARGET_NAME k64f)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for OPENOCD
# -------------------------------------------------------------

set(OPENOCD_UPLOAD_ENABLED TRUE)
set(OPENOCD_CHIP_CONFIG_COMMANDS
    -f ${CMAKE_CURRENT_LIST_DIR}/openocd_cfgs/mk64f.cfg)

# Config options for LINKSERVER
# -------------------------------------------------------------
set(LINKSERVER_UPLOAD_ENABLED TRUE)
set(LINKSERVER_DEVICE MK64FN1M0xxx12:FRDM-K64F)
