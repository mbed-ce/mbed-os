
# Only build the netsocket tests if this target has at least one network interface
string(FIND "${MBED_CONFIG_DEFINITIONS}" MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE DEFAULT_IFC_IDX)
if(DEFAULT_IFC_IDX EQUAL -1)
    set(TEST_SKIPPED "No default network interface on this target")
endif()

# TEMPORARY: netsocket tests will not compile on STM32H7 until new EMAC driver is merged because the old EMAC driver
# pulls in a dependency on lwip
if("TARGET_STM32H7" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "Will not compile on STM32H7")
endif()

add_subdirectory(common)
add_subdirectory(netsocket)
add_subdirectory(network)