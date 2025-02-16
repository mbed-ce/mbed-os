
# Only build the netsocket tests if this target has at least one network interface
string(FIND "${MBED_CONFIG_DEFINITIONS}" MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE DEFAULT_IFC_IDX)
if(DEFAULT_IFC_IDX EQUAL -1)
    set(TEST_SKIPPED "No default network interface on this target")
endif()

add_subdirectory(common)

# List of libraries for all netsocket tests to link.
# We have to be a bit careful here, because, in order to build tests with both lwip and nanostack, we have
# configured neither of the network stacks to provide the default implementation.
# Instead, the defaults are contained in the mbed-netsocket-tests-use-xxx libraries under common.
# Where we need to be careful is, any .a files that call OnboardNetworkStack::get_default_instance() need
# to appear BEFORE mbed-netsocket-tests-use-xxx on the link line, or you will get an undefined reference
# error. In CMake, there's no way to specify this kind of dependency except in the link line to the final
# executable. This means we need to list them out explicitly here.
set(MBED_NETSOCKET_TEST_COMMON_LIBS
    mbed-netsocket-tests-common-inc)

# Pull in cellular if cellular is the default network interface (used by NetworkInterfaceDefaults.cpp)
if("MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE=CELLULAR" IN_LIST MBED_CONFIG_DEFINITIONS)
    list(APPEND MBED_NETSOCKET_TEST_COMMON_LIBS mbed-cellular)
endif()

# Similarly if mesh networking is used bring in that library
if("MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE=MESH" IN_LIST MBED_CONFIG_DEFINITIONS)
    list(APPEND MBED_NETSOCKET_TEST_COMMON_LIBS mbed-nanostack-mbed_mesh_api)
endif()

# Similarly if wifi networking is used bring in that library
if("MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE=WIFI" IN_LIST MBED_CONFIG_DEFINITIONS)
    list(APPEND MBED_NETSOCKET_TEST_COMMON_LIBS mbed-wifi)
endif()

list(APPEND MBED_NETSOCKET_TEST_COMMON_LIBS mbed-netsocket-api)

set(MBED_NETSOCKET_TEST_COMMON_LIBS_LWIPSTACK ${MBED_NETSOCKET_TEST_COMMON_LIBS} mbed-netsocket-tests-use-lwipstack)
set(MBED_NETSOCKET_TEST_COMMON_LIBS_NANOSTACK ${MBED_NETSOCKET_TEST_COMMON_LIBS} mbed-netsocket-tests-use-nanostack)

add_subdirectory(netsocket)
add_subdirectory(network)