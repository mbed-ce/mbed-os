# Mbed OS upload method configuration file for SparkFun Artemis DK
# To change any of these parameters from their default values, set them in your build script between where you
# include mbed_toolchain_setup and where you add mbed-os as a subdirectory.

# Notes:
# 1. Support for this device exists in PyOCD main branch but has not been released yet (as of Jun 2025).
#   This version will be used automatically by Mbed if the python venv is enabled. If not, you need to install it via:
#   pip install git+https://github.com/pyocd/pyOCD.git
# 2. Unlike all other SparkFun Artemis boards, this board has a CMSIS-DAP interface MCU on it, so it
#    should be debuggable & flashable out of the box via PyOCD.

set(UPLOAD_METHOD_DEFAULT MBED)

# Config options for PYOCD
# -------------------------------------------------------------
set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME ama3b1kk_kbr)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for MBED
# -------------------------------------------------------------
set(MBED_UPLOAD_ENABLED TRUE)