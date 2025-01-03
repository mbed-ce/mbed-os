add_library(mbed-emac-test-utils STATIC EXCLUDE_FROM_ALL)
target_sources(mbed-emac-test-utils PRIVATE
    emac_membuf.cpp
    emac_ctp.cpp
    emac_util.cpp
    EmacTestMemoryManager.cpp
    EmacTestNetworkStack.cpp)
target_include_directories(mbed-emac-test-utils PUBLIC .)
target_link_libraries(mbed-emac-test-utils PUBLIC
    mbed-core-flags
    mbed-netsocket-api
	mbed-nanostack-libservice
	mbed-nanostack-hal_mbed_cmsis_rtos)
