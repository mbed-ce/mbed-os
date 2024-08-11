# Mbed OS upload method configuration file for target NUCLEO_H503RB.
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# Notes:
# 1. If your target is not natively supported by the pyOCD, then you need install a keil package for family of your target by hands. Type "pyocd pack show" to console and you will see a list of already installed packages.
# - If any package for your family is not on the list, then you need install them via command "pyocd pack install stm32h5" (take long time).Then just type "pyocd pack find STM32h5" or "pyocd pack find STM32h503" and you will see the part name of your target.

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
set(PYOCD_TARGET_NAME stm32h503rbtx)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for STM32CubeIDE
# -------------------------------------------------------------

set(STM32CUBE_UPLOAD_ENABLED TRUE)
set(STM32CUBE_CONNECT_COMMAND -c port=SWD reset=HWrst)
set(STM32CUBE_GDBSERVER_ARGS --swd -m 1)
