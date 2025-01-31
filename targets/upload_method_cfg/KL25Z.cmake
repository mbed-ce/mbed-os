# Mbed OS upload method configuration file for target KL25Z.
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# Note that KL25Z boards generally need a firmware update to DAPLink interface firmware before they can be used with modern tools.
# The following steps worked for me on a Windows 11 PC:
#
# 1. Download new bootloader from https://app.box.com/s/lyv3mug3dpt41g3uom0sfqyte3mn11sz (I got this from the PE Micro site)
# 2. Disable Storage Service in services.msc (type services.msc into the start menu to open it). It can be reenabled after the bootloader is updated.
#    This is to avoid a bug that happens with Windows 8+ and the original version of the bootloader (https://os.mbed.com/blog/entry/DAPLink-bootloader-update/).
# 3. Update bootloader via this guide: https://mcuoneclipse.com/2014/11/01/illustrated-step-by-step-instructions-updating-the-freescale-freedom-board-firmware/
#    Use the bootloader downloaded in step 1.
#    Note that if you are seeing 8 green LED flashes when in bootloader mode, you are running into the bootloader bug!
# 4. After the new bootloader comes up, install the DAPLink bin file from https://www.nxp.com/downloads/en/ide-debug-compile-build-tools/OpenSDAv2.2_DAPLink_frdmkl25z_rev0242.zip
#    (link from https://www.nxp.com/design/design-center/software/sensor-toolbox/opensda-serial-and-debug-adapter:OPENSDA#FRDM-KL25Z).
#
# After these steps your board should be in CMSIS-DAP mode and should work with standard debug tools!

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT MBED)

# Config options for MBED
# -------------------------------------------------------------
set(MBED_UPLOAD_ENABLED TRUE)
set(MBED_RESET_BAUDRATE 115200)

# Config options for PYOCD
# -------------------------------------------------------------

# In testing, PyOCD did not work for this device, even though it claims to support it.
# Error is:
# 0004887 W Timed out waiting for core to halt after reset (state is RESET) [cortex_m]
# 0004980 C cannot write registers pc, r0, r1, r2, r9, sp, lr because core #0 is not halted [__main__]

set(PYOCD_UPLOAD_ENABLED FALSE)
set(PYOCD_TARGET_NAME kl25z)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for OPENOCD
# -------------------------------------------------------------

set(OPENOCD_UPLOAD_ENABLED TRUE)
set(OPENOCD_CHIP_CONFIG_COMMANDS
        -f ${CMAKE_CURRENT_LIST_DIR}/openocd_cfgs/kl25z.cfg)

# Config options for LINKSERVER
# -------------------------------------------------------------
set(LINKSERVER_UPLOAD_ENABLED TRUE)
set(LINKSERVER_DEVICE MKL25Z4:FRDM-KL25Z)
