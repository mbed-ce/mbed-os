add_library(mbed-frdm-kw41z INTERFACE)
target_link_libraries(mbed-frdm-kw41z INTERFACE mbed-mcu-kw41z)

target_include_directories(mbed-frdm-kw41z INTERFACE .)

target_sources(mbed-frdm-kw41z INTERFACE
        fsl_clock_config.c
)