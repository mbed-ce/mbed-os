target_include_directories(mbed-mcu-k6x INTERFACE .)
mbed_set_linker_script(mbed-mcu-k6x ${CMAKE_CURRENT_SOURCE_DIR}/MK6x.ld)

target_include_directories(mbed-frdm-k64f INTERFACE TARGET_FRDM_K64F)