target_include_directories(mbed-fpb-ra6e2 INTERFACE .)
target_sources(mbed-fpb-ra6e2
    INTERFACE
        board_init.c
        common_data.c
        hal_data.c
        pin_data.c
        hal_warmstart.c
        vector_data.c
)