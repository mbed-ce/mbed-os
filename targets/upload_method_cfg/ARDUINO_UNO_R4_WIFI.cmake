# Mbed OS upload method configuration file for target ARDUINO_UNO_R4_WIFI.
#
# Upload notes for the WiFi board: the USB connector goes to the ESP32-S3
# bridge (VID:PID 2341:1002), not directly to the RA4M1. Uploads use bossac
# through the bridge: the 1200 bps touch is detected by the ESP32-S3, which
# resets the RA4M1 into the bootloader over SWD (IO8/IO9) regardless of the
# firmware running on the RA4M1, then bridges USB CDC <-> UART for bossac.
# This works repeatedly with mbed firmware running (verified).

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT ARDUINO_BOSSAC)

# Config options for ARDUINO_BOSSAC (default)
# -------------------------------------------------------------
set(ARDUINO_BOSSAC_UPLOAD_ENABLED TRUE)

# Config options for JLINK (fallback, direct SWD on the RA4M1)
# -------------------------------------------------------------
set(JLINK_UPLOAD_ENABLED TRUE)
set(JLINK_CPU_NAME R7FA4M1AB)
