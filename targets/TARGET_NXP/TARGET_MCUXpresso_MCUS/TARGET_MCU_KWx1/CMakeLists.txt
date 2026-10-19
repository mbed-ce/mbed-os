mbed_set_linker_script(mbed-mcu-kwx1 ${CMAKE_CURRENT_SOURCE_DIR}/MKWx1.ld)

target_include_directories(mbed-mcu-kwx1 INTERFACE .)

add_subdirectory(TARGET_MCU_KW41Z)