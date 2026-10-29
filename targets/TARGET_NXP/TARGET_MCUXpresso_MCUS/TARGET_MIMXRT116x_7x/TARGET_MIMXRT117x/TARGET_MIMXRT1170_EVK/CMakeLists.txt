add_library(mbed-mimxrt1170-evk INTERFACE)
target_link_libraries(mbed-mimxrt1170-evk INTERFACE mbed-mcu-mimxrt117x)

target_include_directories(mbed-mimxrt1170-evk INTERFACE . xip)

target_sources(mbed-mimxrt1170-evk INTERFACE
    xip/dcd.c
    xip/evkmimxrt1170_flexspi_nor_config.c
    clock_config.c
)
