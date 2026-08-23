# Mbed OS upload method configuration file for target EFM32GG_STK3700 (EFM32 Giant Gecko, Series 0).
# To change any of these parameters from their default values, set them in your build script between where you
# include mbed_toolchain_setup and where you add mbed os as a subdirectory.
#
# Notes:
# 1. The STK3700 carries a SEGGER J-Link OB, so JLINK is the default method.
# 2. SWD only. Series 0 EFM32 parts have no JTAG (see OpenOCDs target/efm32.cfg), and the J-Link OB
#    leaves TDO un wired
# 3. Flash base is 0x00000000, so no load address is set below
# 4. PyOCD ships no EFM32/EFR32 support; a CMSIS pack is required:
#      pyocd pack update && pyocd pack install EFM32GG990F1024
# 5. On Windows OPENOCD may need the J-Link OBs vendor interface 2 rebound from SEGGERs
#    driver to WinUSB

# General config parameters
# -------------------------------------------------------------

set(UPLOAD_METHOD_DEFAULT JLINK)

# Config options for JLINK
# -------------------------------------------------------------

set(JLINK_UPLOAD_ENABLED TRUE)
set(JLINK_CPU_NAME EFM32GG990F1024)
set(JLINK_CLOCK_SPEED 4000)
set(JLINK_UPLOAD_INTERFACE SWD)

# Config options for PYOCD
# -------------------------------------------------------------

set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME efm32gg990f1024)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for OPENOCD
# -------------------------------------------------------------
set(OPENOCD_UPLOAD_ENABLED TRUE)
set(OPENOCD_CHIP_CONFIG_COMMANDS
    -f board/efm32.cfg)

# Config options for MBED
# -------------------------------------------------------------
set(MBED_UPLOAD_ENABLED TRUE)
