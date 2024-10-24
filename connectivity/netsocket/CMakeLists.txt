# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(tests/emac_test_utils)

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(MBED_BUILD_GREENTEA_TESTS)
        add_subdirectory(tests/TESTS)
    else()
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

add_library(mbed-netsocket-api STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-netsocket-api
    PUBLIC
        .
        ./include
        ./include/netsocket
)

target_sources(mbed-netsocket-api
    PRIVATE
        source/DTLSSocket.cpp
        source/DTLSSocketWrapper.cpp
        source/EMACInterface.cpp
        source/EthernetInterface.cpp
        source/ICMPSocket.cpp
        source/InternetDatagramSocket.cpp
        source/InternetSocket.cpp
        source/L3IPInterface.cpp
        source/NetStackMemoryManager.cpp
        source/NetworkInterface.cpp
        source/NetworkInterfaceDefaults.cpp
        source/NetworkStack.cpp
        source/PPPInterface.cpp
        source/SocketAddress.cpp
        source/SocketStats.cpp
        source/TCPSocket.cpp
        source/TLSSocket.cpp
        source/TLSSocketWrapper.cpp
        source/UDPSocket.cpp
        source/WiFiAccessPoint.cpp
        source/nsapi_dns.cpp
        source/nsapi_ppp.cpp
)

target_compile_definitions(mbed-netsocket-api
    PUBLIC
        MBED_CONF_NSAPI_PRESENT=1
)

target_link_libraries(mbed-netsocket-api
    PUBLIC
        mbed-core-flags
        mbed-mbedtls
        mbed-nanostack-libservice
)

# Currently, netsocket-api needs to link to whichever library is providing an implementation
# of OnboardNetworkStack::get_default_instance().  Otherwise, the link order is not correct.
if("MBED_CONF_NSAPI_DEFAULT_STACK=LWIP" IN_LIST MBED_CONFIG_DEFINITIONS)
    target_link_libraries(mbed-netsocket-api PUBLIC mbed-lwipstack)
elseif("MBED_CONF_NSAPI_DEFAULT_STACK=NANOSTACK" IN_LIST MBED_CONFIG_DEFINITIONS)
    target_link_libraries(mbed-netsocket-api PUBLIC mbed-nanostack)
endif()

# Pull in cellular if cellular is the default network interface (used by NetworkInterfaceDefaults.cpp)
if("MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE=CELLULAR" IN_LIST MBED_CONFIG_DEFINITIONS)
    target_link_libraries(mbed-netsocket-api PUBLIC mbed-cellular)
endif()

# Similarly if mesh networking is used bring in that library
if("MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE=MESH" IN_LIST MBED_CONFIG_DEFINITIONS)
    target_link_libraries(mbed-netsocket-api PUBLIC mbed-nanostack-mbed_mesh_api)
endif()

# Similarly if wifi networking is used bring in that library
if("MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE=WIFI" IN_LIST MBED_CONFIG_DEFINITIONS)
    if(TARGET mbed-wifi)
        target_link_libraries(mbed-netsocket-api PUBLIC mbed-wifi)
    endif()
endif()

if("DEVICE_EMAC=1" IN_LIST MBED_TARGET_DEFINITIONS)
    target_link_libraries(mbed-netsocket-api
        INTERFACE
            mbed-emac
    )
endif()

add_library(mbed-netsocket INTERFACE)

target_link_libraries(mbed-netsocket
    INTERFACE
        mbed-mbedtls
)

target_link_libraries(mbed-netsocket
    INTERFACE
        mbed-cellular
        mbed-nanostack-libservice
)

# Link in mbed-wifi if we have any wifi drivers
if(TARGET mbed-wifi)
    target_link_libraries(mbed-netsocket
        INTERFACE
            mbed-wifi
    )
endif()