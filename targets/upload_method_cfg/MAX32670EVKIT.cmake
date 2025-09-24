# Mbed OS upload method configuration file for target MAX32670EVKIT.
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# Notes:
# 1. For the MBED upload method to work, you may need to update the DAPLink firmware as described here:
#    https://github.com/analogdevicesinc/max32625pico-firmware-images
#    My board shipped with firmware that claimed it was an MAX32630FTHR, causing
#    "ERROR: The target board you compiled for is not connected to your system"
# 2. MAX32670 support did not land in upstream OpenOCD until Aug 2025. As of this writing, you need to compile
#    it yourself from the git version (or use the OpenOCD that is distributed with the Maxim SDK).
#    However, this should be resolved with the release of OpenOCD 1.0, which is imminent.
# 3. PyOCD does not work as of Sep 2025:
#    0000691 I DAP is not accessible after reset; attempting reconnect [dap]
#    0000795 E Error during board uninit: [session]
#    0000805 C No ACK received [__main__]

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT MBED)

# Config options for MBED
# -------------------------------------------------------------

set(MBED_UPLOAD_ENABLED TRUE)
set(MBED_RESET_BAUDRATE 115200)

# Config options for OPENOCD
# -------------------------------------------------------------

set(OPENOCD_UPLOAD_ENABLED TRUE)
set(OPENOCD_CHIP_CONFIG_COMMANDS
        -f interface/cmsis-dap.cfg
        -s ${CMAKE_CURRENT_LIST_DIR}/openocd_cfgs/max32xxx
        -f target/max32670.cfg)

# Config options for PYOCD
# -------------------------------------------------------------
set(PYOCD_UPLOAD_ENABLED FALSE)
set(PYOCD_TARGET_NAME max32670)
set(PYOCD_CLOCK_SPEED 4000k)