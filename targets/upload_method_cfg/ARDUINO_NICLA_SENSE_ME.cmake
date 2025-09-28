# Mbed OS upload method configuration file for target ARDUINO_NANO33BLE_SWD.
# To change any of these parameters from their default values, set them in your build script between where you
# include mbed_toolchain_setup and where you add mbed os as a subdirectory.

# Notes:
# 1. Using pyocd with this device requires installing a pack: `pyocd pack install nrf52`.


# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT OPENOCD)

# Config options for PYOCD
# -------------------------------------------------------------

set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME nrf52832)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for OPENOCD
# -------------------------------------------------------------

set(OPENOCD_UPLOAD_ENABLED TRUE)
set(OPENOCD_CHIP_CONFIG_COMMANDS
    -f interface/cmsis-dap.cfg
	-c "transport select swd"
	-f target/nrf52.cfg)