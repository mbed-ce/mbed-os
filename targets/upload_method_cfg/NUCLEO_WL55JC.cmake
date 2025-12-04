# Mbed OS upload method configuration file for target NUCLEO_WL55JC.
# To change any of these parameters from their default values, set them in your build script between where you
# include mbed_toolchain_setup and where you add mbed os as a subdirectory.

# Notes:
# 1. Using this device with PyOCD requires installing a pack:
#    $ pyocd pack install stm32wl55jcix
#    However, in my testing, PyOCD appeared to flash successfully but the code does not run correctly.


# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT MBED)

# Config options for MBED
# -------------------------------------------------------------

set(MBED_UPLOAD_ENABLED TRUE)
set(MBED_RESET_BAUDRATE 115200)

# Config options for PYOCD
# -------------------------------------------------------------

#set(PYOCD_UPLOAD_ENABLED TRUE)
#set(PYOCD_TARGET_NAME stm32wl55jcix)
#set(PYOCD_CLOCK_SPEED 4000k)

# Config options for OPENOCD
# -------------------------------------------------------------

set(OPENOCD_UPLOAD_ENABLED TRUE)
set(OPENOCD_CHIP_CONFIG_COMMANDS
    -f ${CMAKE_CURRENT_LIST_DIR}/openocd_cfgs/st_nucleo_wlx.cfg)

# Config options for STM32Cube
# -------------------------------------------------------------

set(STM32CUBE_UPLOAD_ENABLED TRUE)
set(STM32CUBE_CONNECT_COMMAND -c port=SWD reset=HWrst)
set(STM32CUBE_GDBSERVER_ARGS --swd)

# Config options for stlink
# -------------------------------------------------------------

set(STLINK_UPLOAD_ENABLED TRUE)
set(STLINK_ARGS --connect-under-reset)