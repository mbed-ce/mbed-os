add_library(mbed-arduino-opta-m4 INTERFACE)

target_sources(mbed-arduino-opta-m4
    INTERFACE
        opta_power.cpp
)

target_include_directories(mbed-arduino-opta-m4
    INTERFACE
        .
)

target_link_libraries(mbed-arduino-opta-m4 INTERFACE mbed-stm32h747xi-cm4)

add_library(mbed-arduino-opta-m7 INTERFACE)

target_sources(mbed-arduino-opta-m7
    INTERFACE
        opta_power.cpp
)

target_include_directories(mbed-arduino-opta-m7
    INTERFACE
        .
)

target_link_libraries(mbed-arduino-opta-m7 INTERFACE mbed-stm32h747-arduino)
