
# Target for Teensy 4.x common files
target_sources(mbed-teensy-4x INTERFACE
	bootdata.c)
target_include_directories(mbed-teensy-4x INTERFACE .)


# Target for Teensy 4.0

target_include_directories(mbed-teensy-40 INTERFACE TARGET_TEENSY_40)

# Target for Teensy 4.1

target_include_directories(mbed-teensy-41 INTERFACE TARGET_TEENSY_41)