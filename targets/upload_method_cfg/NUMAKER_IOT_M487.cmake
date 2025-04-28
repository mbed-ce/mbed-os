# Mbed OS upload method configuration file for target NUMAKER_IOT_M487.
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# Notes:
# 1. The Nuvoton fork of OpenOCD is required: https://github.com/OpenNuvoton/OpenOCD-Nuvoton/releases .
#    Point CMake to it via setting the OpenOCD option:
#    -DOpenOCD="C:/Program Files (x86)/OpenOCD-nuvoton/bin/openocd.exe"
# 2. Take note of the "MSG" DIP switch on the Nu-Link section of the board. If set to ON, the Nu-Link will run in
#    mass storage mode and can only work with the "MBED" upload method. If set to OFF, the Nu-Link will run in
#    Nu-Link mode and only works with Nuvoton OpenOCD.
# 3. On Windows you will need to install the Nu-Link Keil USB driver, which can be found near the bottom here:
#    https://www.nuvoton.com/tool-and-software/ide-and-compiler/
# 4. The onboard nu-link does not have a unique USB serial number configured, so the MBED_UPLOAD_SERIAL_NUMBER
#    option will not work (and it doesn't seem to work with OpenOCD 0.10.x anyway). This means, sadly, it's impossible
#    to work with more than one Nuvoton board at a time on a given machine.

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
    -f ${CMAKE_CURRENT_LIST_DIR}/openocd_cfgs/numicro_m4.cfg)

# Nuvoton NuMicro Cortex-M support for OpenOCD is forked, but has
# not yet upstreamed. Its version number is fixed to 0.10.022.
set(OPENOCD_VERSION_RANGE 0.10.022...0.10.22)
