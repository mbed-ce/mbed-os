# Mbed OS upload method configuration file for target NUCLEO_H753ZI.

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT MBED)

# Config options for MBED
# -------------------------------------------------------------
set(MBED_UPLOAD_ENABLED TRUE)
set(MBED_RESET_BAUDRATE 115200)

# Config options for JLINK
# -------------------------------------------------------------
set(JLINK_UPLOAD_ENABLED FALSE)
set(JLINK_CPU_NAME STM32H753ZI)
set(JLINK_CLOCK_SPEED 4000)
set(JLINK_UPLOAD_INTERFACE SWD)

# Config options for PYOCD
# -------------------------------------------------------------
set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME STM32H753ZITx)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for OPENOCD
# -------------------------------------------------------------
set(OPENOCD_UPLOAD_ENABLED TRUE)
set(OPENOCD_CHIP_CONFIG_COMMANDS
    -f board/st_nucleo_h743zi.cfg)

# Config options for STM32Cube
# -------------------------------------------------------------
set(STM32CUBE_UPLOAD_ENABLED TRUE)
set(STM32CUBE_CONNECT_COMMAND -c port=SWD reset=HWrst)
set(STM32CUBE_GDBSERVER_ARGS --swd --initialize-reset)

# Config options for stlink
# -------------------------------------------------------------
set(STLINK_UPLOAD_ENABLED TRUE)
set(STLINK_LOAD_ADDRESS 0x8000000)
set(STLINK_ARGS --connect-under-reset)
