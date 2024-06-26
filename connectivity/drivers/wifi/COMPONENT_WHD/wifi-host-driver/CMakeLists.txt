if("COMPONENT_43012=1" IN_LIST MBED_TARGET_DEFINITIONS)
	target_include_directories(mbed-wifi
	    PUBLIC
	        resources/firmware/COMPONENT_43012
	)
	target_sources(mbed-wifi
	    PRIVATE
	        resources/firmware/COMPONENT_43012/43012C0-mfgtest_bin.c
	        resources/firmware/COMPONENT_43012/43012C0-mfgtest_clm_blob.c
	        resources/firmware/COMPONENT_43012/43012C0_bin.c
	        resources/firmware/COMPONENT_43012/43012C0_clm_blob.c
	)
endif()

if("COMPONENT_43438=1" IN_LIST MBED_TARGET_DEFINITIONS)
	target_include_directories(mbed-wifi
	    PUBLIC
	        resources/firmware/COMPONENT_43438
	)
	target_sources(mbed-wifi
	    PRIVATE
	        resources/firmware/COMPONENT_43438/43438A1-mfgtest_bin.c
	        resources/firmware/COMPONENT_43438/43438A1-mfgtest_clm_blob.c
	        resources/firmware/COMPONENT_43438/43438A1_bin.c
	        resources/firmware/COMPONENT_43438/43438A1_clm_blob.c
	)
endif()

if("COMPONENT_4343W=1" IN_LIST MBED_TARGET_DEFINITIONS)
	target_include_directories(mbed-wifi
	    PUBLIC
	        resources/firmware/COMPONENT_4343W
	)
	target_sources(mbed-wifi
	    PRIVATE
	        resources/firmware/COMPONENT_4343W/4343WA1-mfgtest_bin.c
	        resources/firmware/COMPONENT_4343W/4343WA1-mfgtest_clm_blob.c
	        resources/firmware/COMPONENT_4343W/4343WA1_bin.c
	        resources/firmware/COMPONENT_4343W/4343WA1_clm_blob.c
	)
endif()

if("COMPONENT_4343W_FS=1" IN_LIST MBED_TARGET_DEFINITIONS)
	target_include_directories(mbed-wifi
	    PUBLIC
	        resources/firmware/COMPONENT_4343W_FS
	)
	target_sources(mbed-wifi
	    PRIVATE
	        resources/firmware/COMPONENT_4343W_FS/4343WA1_bin.c
	        resources/firmware/COMPONENT_4343W_FS/4343WA1_clm_blob.c
	)
endif()

# Grab wifi_nvram_image.h and generated_mac_address.txt from the correct folder
set(TARGETS_WITH_NVRAM_IMAGE
	CY8CKIT_062S2_43012
	CY8CKIT_062S2_4343W
	CY8CKIT_062_4343W
	CY8CKIT_062_WIFI_BT
	CY8CKIT_064S1_4343W
	CY8CKIT_064S2_4343W
	CY8CMOD_062S3_4343W
	CY8CMOD_062_4343W
	CY8CPROTO_062S3_4343W
	CY8CPROTO_062_4343W
	CYTFM_064B0S2_4343W
	CYW943012P6EVB_01
	CYW943012WCD2
	CYW9P62S1_43012CAR_01
	CYW9P62S1_43012EVB_01
	CYW9P62S1_43438EVB_01
	CY8CKIT_064B0S2_4343W
	Cypress
	STM32H747_ARDUINO)

foreach(TARGET ${TARGETS_WITH_NVRAM_IMAGE})
	if("${TARGET}" IN_LIST MBED_TARGET_LABELS)
		target_include_directories(mbed-wifi PUBLIC resources/nvram/TARGET_${TARGET})
	endif()
endforeach()

target_include_directories(mbed-wifi
    PUBLIC
        wifi-host-driver
        inc
        resources/resource_imp
        src
        src/include
        src/bus_protocols
)

target_sources(mbed-wifi
    PRIVATE
        resources/resource_imp/whd_resources.c

        src/whd_ap.c
        src/whd_buffer_api.c
        src/whd_cdc_bdc.c
        src/whd_chip.c
        src/whd_chip_constants.c
        src/whd_clm.c
        src/whd_debug.c
        src/whd_events.c
        src/whd_logging.c
        src/whd_management.c
        src/whd_network_if.c
        src/whd_resource_if.c
        src/whd_sdpcm.c
        src/whd_thread.c
        src/whd_utils.c
        src/whd_wifi.c
        src/whd_wifi_api.c
        src/whd_wifi_p2p.c

        src/bus_protocols/whd_bus.c
        src/bus_protocols/whd_bus_common.c
        src/bus_protocols/whd_bus_sdio_protocol.c
        src/bus_protocols/whd_bus_spi_protocol.c
)

target_compile_definitions(mbed-wifi
    PUBLIC
        MBED_CONF_CY_PSOC6_WHD_PRESENT=1
)