add_library(mbed-arduino-giga-cm7 INTERFACE)

target_include_directories(mbed-arduino-giga-cm7
    INTERFACE
        .
)

target_link_libraries(mbed-arduino-giga-cm7 INTERFACE mbed-stm32h747-arduino)