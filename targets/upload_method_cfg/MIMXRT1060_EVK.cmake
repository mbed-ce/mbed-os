# Mbed OS upload method configuration file for target MIMXRT1060_EVK.
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT JLINK)

# Config options for JLINK
# -------------------------------------------------------------
set(JLINK_UPLOAD_ENABLED TRUE)
set(JLINK_CPU_NAME MIMXRT1062xxx5B?BankAddr=0x60000000&Loader=QSPI) # Extra parameters needed to select correct flash loader
set(JLINK_UPLOAD_INTERFACE SWD)
set(JLINK_CLOCK_SPEED 4000)

# TODO test all below upload methods

# Config options for MBED
# -------------------------------------------------------------
set(MBED_UPLOAD_ENABLED TRUE)
set(MBED_RESET_BAUDRATE 115200)

# Config options for PYOCD
# -------------------------------------------------------------
set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME mimxrt1050_hyperflash) # Note: change to "mimxrt1050_quadspi" if onboard QSPI flash is used
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for REDLINK
# -------------------------------------------------------------
set(REDLINK_UPLOAD_ENABLED TRUE)
set(REDLINK_PART_NUMBER MIMXRT1052xxxxB)
set(REDLINK_PART_XML_DIR ${CMAKE_CURRENT_LIST_DIR}/redlink_cfgs)
set(REDLINK_CLOCK_SPEED 4000)
set(REDLINK_CONNECT_ARGS
	--connectscript=RT1050_connect.scp
	--reset=
	--coreindex 0
	--cache disable
	--no-packed)