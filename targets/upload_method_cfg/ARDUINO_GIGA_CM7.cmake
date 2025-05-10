# Mbed OS upload method configuration file for target ARDUINO_GIGA_CM7.
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# Notes:
# 1. To use this target with PyOCD, you need to install a pack: `pyocd pack install STM32H747XIHx`.
#    You might also need to run `pyocd pack update` first.
# 2. To use the DFU upload methods, you need to double-tap the reset button first to put the
#    board in DFU mode.
# 3. On Linux, you will need to add a udev rule to be able to upload over DFU:
#    SUBSYSTEM=="usb", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="0366", MODE="664", GROUP="plugdev"

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT DFU_UTIL)

# Config options for JLINK
# -------------------------------------------------------------

set(JLINK_UPLOAD_ENABLED FALSE)
set(JLINK_CPU_NAME STM32H747XI)
set(JLINK_CLOCK_SPEED 4000)
set(JLINK_UPLOAD_INTERFACE SWD)

# Config options for PYOCD
# -------------------------------------------------------------

set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME STM32H747XIHx)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for OPENOCD
# -------------------------------------------------------------

set(OPENOCD_UPLOAD_ENABLED TRUE)
set(OPENOCD_CHIP_CONFIG_COMMANDS
    -f ${CMAKE_CURRENT_LIST_DIR}/openocd_cfgs/stm32h747.cfg)

# Config options for STM32Cube
# -------------------------------------------------------------

set(STM32CUBE_UPLOAD_ENABLED TRUE)
set(STM32CUBE_CONNECT_COMMAND -c port=SWD reset=HWrst)
set(STM32CUBE_GDBSERVER_ARGS --swd --initialize-reset --apid 0)

# Config options for stlink
# -------------------------------------------------------------

set(STLINK_UPLOAD_ENABLED TRUE)
set(STLINK_ARGS --connect-under-reset)

# Config options for dfu-util
# -------------------------------------------------------------

set(DFU_UTIL_UPLOAD_ENABLED TRUE)
set(DFU_UTIL_TARGET_VID_PID 2341:0366) # Arduino Giga bootloader
set(DFU_UTIL_TARGET_INTERFACE 0)

# Config options for STM32CUBE_DFU
# -------------------------------------------------------------

set(STM32CUBE_DFU_UPLOAD_ENABLED TRUE)
set(STM32CUBE_DFU_CONNECT_COMMAND port=USB vid=0x2341 pid=0x0366) # Arduino Giga bootloader
