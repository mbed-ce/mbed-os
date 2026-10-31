if(NOT "MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE=CELLULAR" IN_LIST MBED_CONFIG_DEFINITIONS)
    # Check connectivity to the IPv6 internet.
    # If there is no IPv6 connectivity we cannot run nanostack tests as it only supports IPv6.
    if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
        set(IPV6_PING_COMMAND ping -n 1 -6 google.com)
    else()
        set(IPV6_PING_COMMAND ping -c 1 -6 google.com)
    endif()
    execute_process(
            COMMAND ${IPV6_PING_COMMAND}
            RESULT_VARIABLE IPV6_PING_COMMAND_RESULT
            OUTPUT_QUIET
            ERROR_QUIET
    )
    if(NOT IPV6_PING_COMMAND_RESULT EQUAL 0)
        message(STATUS "Mbed: You do not appear to have access to the IPv6 internet. Nanostack tests will be disabled as it requires IPv6.")
        set(NETWORK_SUPPORTS_IPV6 FALSE)
    else()
        set(NETWORK_SUPPORTS_IPV6 TRUE)
    endif()
endif()

add_subdirectory(dns)
add_subdirectory(nidd)
add_subdirectory(tcp)
add_subdirectory(tls)
add_subdirectory(udp)
