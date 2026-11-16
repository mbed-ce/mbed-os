target_include_directories(mbed-emac
    PUBLIC
        .
)

target_sources(mbed-emac
    PRIVATE
        ENETEthMac.cpp
)

if("MIMXRT1050_EVK" IN_LIST MBED_TARGET_LABELS OR "MIMXRT1060_EVK" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MIMXRT105x_EVK)
endif()
if("TEENSY_41" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_TEENSY_41)
endif()
if("MCU_K64F" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MCU_K64F)
endif()
if("MCU_K66F" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MCU_K66F)
endif()
