target_include_directories(mbed-mcu-k6x INTERFACE .)
target_sources(mbed-mcu-k6x
    INTERFACE
        mbed_overrides.c
)
mbed_set_linker_script(mbed-mcu-k6x ${CMAKE_CURRENT_SOURCE_DIR}/MK6x.ld)

target_include_directories(mbed-mcu-k64f INTERFACE TARGET_MCU_K64F)
target_sources(mbed-mcu-k64f
    INTERFACE
        TARGET_MCU_K64F/PeripheralPins.c
)

target_include_directories(mbed-frdm-k64f INTERFACE TARGET_FRDM_K64F)
target_sources(mbed-frdm-k64f
    INTERFACE
        TARGET_FRDM_K64F/fsl_clock_config.c
)

target_include_directories(mbed-hexiwear INTERFACE TARGET_HEXIWEAR)
target_sources(mbed-hexiwear
    INTERFACE
        TARGET_HEXIWEAR/fsl_clock_config.c
)

target_include_directories(mbed-sdt64b INTERFACE TARGET_SDT64B)
target_sources(mbed-sdt64b
    INTERFACE
        TARGET_SDT64B/fsl_clock_config.c
)