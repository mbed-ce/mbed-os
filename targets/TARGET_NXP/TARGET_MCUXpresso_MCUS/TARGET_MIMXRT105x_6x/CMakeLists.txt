target_sources(mbed-mcu-mimxrt105x_6x
    INTERFACE
        lpm.c
        mbed_overrides.c
        mimxrt_clock_adjustment.c
        sleep.c
        board/clock_config.c
        lpuart_device.c
        startup_MIMXRT105x_6x.S
        PeripheralPins.c
)

target_include_directories(mbed-mcu-mimxrt105x_6x
    INTERFACE
        .
        board
)

mbed_set_linker_script(mbed-mcu-mimxrt105x_6x ${CMAKE_CURRENT_SOURCE_DIR}/MIMXRT105x_6x.ld)

add_subdirectory(TARGET_MIMXRT1050_EVK)
add_subdirectory(TARGET_MIMXRT1060_EVK)