target_include_directories(mbed-mimxrt1050-evk INTERFACE .)

target_sources(mbed-mimxrt1050-evk
    INTERFACE
        evkbimxrt1050_sdram_ini_dcd.c
        xip/evkbimxrt1050_flexspi_nor_config.c
)