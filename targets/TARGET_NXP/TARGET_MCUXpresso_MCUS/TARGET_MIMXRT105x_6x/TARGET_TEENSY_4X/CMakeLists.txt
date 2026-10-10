target_sources(mbed-teensy-4x INTERFACE
	bootdata.c)
target_include_directories(mbed-teensy-4x INTERFACE .)

target_include_directories(mbed-teensy-40 INTERFACE TARGET_TEENSY_40)

target_include_directories(mbed-teensy-41 INTERFACE TARGET_TEENSY_41)