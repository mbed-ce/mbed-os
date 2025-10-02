# Mbed OS upload method configuration file for target CY8CPROTO_062S3_4343W.
# To change any of these parameters from their default values, set them in your build script between where you
# include mbed_toolchain_setup and where you add mbed os as a subdirectory.

# Notes:
# 1. The KitProg3 programmer on this board boots up in KitProg mode. To enable mass storage device mode, press the
#    button labeled "MODE" near the USB port to switch to DAPLink mode. The MBED and PYOCD upload methods need
#    the board to be in DAPLink mode (which is deprecated by Infineon).
# 2. Mbed upload method seems VERY slow on this board (takes like 1 minute to flash!).
# 3. PyOCD is tested working correctly in DAPLink mode. In KitProg mode it can flash but debugging does
#    not appear to work correctly.
# 4. Infineon's fork of OpenOCD must be used to program this device as they have not yet upstreamed their
#    changes. This can be downloaded from here: https://github.com/Infineon/openocd/releases
#    Then configure CMake to use it with `-DOpenOCD=/path/to/infineon/openocd`
# 5. If using the wi-fi module on this board, you need to flash the wi-fi module firmware to the QSPI flash.
#    This is done automatically via Infineon's OpenOCD scripts when flashing any project that uses wifi with OpenOCD.
#    Flashing via other methods will likely not work for wifi projects.

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT MBED)

# Generally we are only interested in the CM4 core which is index 1
set(MBED_DEBUG_CORE_INDEX 1)

# Config options for MBED
# -------------------------------------------------------------

set(MBED_UPLOAD_ENABLED TRUE)
set(MBED_RESET_BAUDRATE 115200)

# Config options for PYOCD
# -------------------------------------------------------------

set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME cy8c6xx5)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for OpenOCD
# -------------------------------------------------------------

set(OPENOCD_UPLOAD_ENABLED TRUE)
set(OPENOCD_CHIP_CONFIG_COMMANDS
        -s ${CMAKE_CURRENT_LIST_DIR}/openocd_cfgs/CY8CPROTO_062S3_4343W
        -f ${CMAKE_CURRENT_LIST_DIR}/openocd_cfgs/infineon_psoc62_c8cxx5.cfg)

# Seems like a regular 'monitor reset' does not work correctly for this chip. This is the command sequence
# used by ModusToolbox IDE.
set(OPENOCD_GDB_RESET_SEQUENCE
    "monitor reset run"
    "monitor psoc6 reset_halt sysresetreq")