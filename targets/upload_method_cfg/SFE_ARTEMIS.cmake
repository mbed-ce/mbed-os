# Mbed OS upload method configuration file for target NUMAKER_PFM_M487.
# To change any of these parameters from their default values, set them in your build script between where you
# include mbed_toolchain_setup and where you add mbed-os as a subdirectory.

# Notes:
# 1. Support for this device exists in PyOCD main branch but has not been released yet (as of Jun 2025).
#   To use PyOCD, you need to manually install it from the git repository by running (inside the Mbed OS venv):
#   pip install git+https://github.com/pyocd/pyOCD.git

set(UPLOAD_METHOD_DEFAULT NONE)

# Config options for PYOCD
# -------------------------------------------------------------
set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME ama3b1kk_kbr)
set(PYOCD_CLOCK_SPEED 4000k)
