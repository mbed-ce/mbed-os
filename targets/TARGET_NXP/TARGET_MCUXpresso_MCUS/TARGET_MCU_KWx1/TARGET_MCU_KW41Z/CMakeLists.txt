target_include_directories(mbed-mcu-kw41z INTERFACE .)

target_sources(mbed-mcu-kw41z INTERFACE
    PeripheralPins.c
)

add_subdirectory(TARGET_FRDM_KW41Z)
