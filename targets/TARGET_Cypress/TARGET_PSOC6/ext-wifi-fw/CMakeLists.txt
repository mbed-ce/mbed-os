add_library(mbed-cy-external-wifi-fw STATIC
    cy_ext_wifi_fw_reserved_region_bd.cpp
    CyReservedRegionBlockDevice.cpp)

target_include_directories(mbed-cy-external-wifi-fw PUBLIC .)

target_link_libraries(mbed-cy-external-wifi-fw PUBLIC
    mbed-core-flags
    mbed-storage-qspif)

# Force linking of the cy_ext_wifi_fw_reserved_region_bd.o object file to correctly resolve weak symbols
target_link_options(mbed-cy-external-wifi-fw
    INTERFACE
        LINKER:--undefined=force_link_cy_ext_wifi_fw_reserved_region_bd_o
)