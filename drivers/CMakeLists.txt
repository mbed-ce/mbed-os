# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(MBED_BUILD_GREENTEA_TESTS)
        add_subdirectory(tests/TESTS)
    else()
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

target_include_directories(mbed-core-flags
    INTERFACE
        .
        ./include
        ./include/drivers
        ./include/drivers/interfaces
)

target_sources(mbed-core-sources
    INTERFACE
        source/AnalogIn.cpp
        source/AnalogOut.cpp
        source/BufferedSerial.cpp
        source/BusIn.cpp
        source/BusInOut.cpp
        source/BusOut.cpp
        source/CAN.cpp
        source/DigitalIn.cpp
        source/DigitalInOut.cpp
        source/DigitalOut.cpp
        source/FlashIAP.cpp
        source/I2C.cpp
        source/I2CSlave.cpp
        source/InterruptIn.cpp
        source/MbedCRC.cpp
        source/OSPI.cpp
        source/PortIn.cpp
        source/PortInOut.cpp
        source/PortOut.cpp
        source/PwmOut.cpp
        source/QSPI.cpp
        source/ResetReason.cpp
        source/SPI.cpp
        source/SPISlave.cpp
        source/SerialBase.cpp
        source/SerialWireOutput.cpp
        source/Ticker.cpp
        source/Timeout.cpp
        source/Timer.cpp
        source/TimerEvent.cpp
        source/UnbufferedSerial.cpp
        source/Watchdog.cpp
)

add_subdirectory(usb)
