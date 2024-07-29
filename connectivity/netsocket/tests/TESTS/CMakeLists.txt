
# Only build the netsocket tests if this target has at least one network interface
string(FIND "${MBED_CONFIG_DEFINITIONS}" MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE DEFAULT_IFC_IDX)
if(DEFAULT_IFC_IDX EQUAL -1)
    set(TEST_SKIPPED "No default network interface on this target")
endif()

# Set up variables for wi-fi SSID and password
if("MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE=WIFI" IN_LIST MBED_CONFIG_DEFINITIONS)
    set(MBED_GREENTEA_WIFI_SECURE_SSID "" CACHE STRING "SSID of a secured wi-fi network with internet access, for greentea tests which need to connect to wifi")
    set(MBED_GREENTEA_WIFI_SECURE_PASSWORD "" CACHE STRING "Password to the network given by MBED_GREENTEA_WIFI_SECURE_SSID")

    if("${MBED_GREENTEA_WIFI_SECURE_SSID}" STREQUAL "" OR "${MBED_GREENTEA_WIFI_SECURE_PASSWORD}" STREQUAL "")
        message(WARNING "MBED_GREENTEA_WIFI_SECURE_SSID and MBED_GREENTEA_WIFI_SECURE_PASSWORD must be set in order for wi-fi greentea tests to pass")
    else()
        add_compile_definitions(
            "MBED_GREENTEA_WIFI_SECURE_SSID=\"${MBED_GREENTEA_WIFI_SECURE_SSID}\""
            "MBED_GREENTEA_WIFI_SECURE_PASSWORD=\"${MBED_GREENTEA_WIFI_SECURE_PASSWORD}\"")
    endif()
endif()

add_subdirectory(netsocket)
add_subdirectory(network)