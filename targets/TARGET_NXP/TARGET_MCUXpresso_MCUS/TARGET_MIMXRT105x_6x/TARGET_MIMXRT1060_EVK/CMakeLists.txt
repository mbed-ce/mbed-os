target_include_directories(mbed-mimxrt1060-evk INTERFACE .)

target_sources(mbed-mimxrt1060-evk
    INTERFACE
        dcd.c
        xip/evkbmimxrt1060_flexspi_nor_config.c
)