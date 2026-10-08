add_library(mbed-mcu-k6x INTERFACE)
add_library(mbed-mcu-k64f INTERFACE)
target_link_libraries(mbed-mcu-k64f INTERFACE mbed-mcu-k6x)
add_library(mbed-mcu-k66f INTERFACE)
target_link_libraries(mbed-mcu-k66f INTERFACE mbed-mcu-k6x)
add_library(mbed-hexiwear INTERFACE)
target_link_libraries(mbed-hexiwear INTERFACE mbed-mcu-k64f)
add_library(mbed-sdt64b INTERFACE)
target_link_libraries(mbed-sdt64b INTERFACE mbed-mcu-k64f)
add_library(mbed-frdm-k64f INTERFACE)
target_link_libraries(mbed-frdm-k64f INTERFACE mbed-mcu-k64f)
add_library(mbed-frdm-k66f INTERFACE)
target_link_libraries(mbed-frdm-k66f INTERFACE mbed-mcu-k66f)

# Link appropriate SDK drivers for K6x
target_sources(mbed-mcu-kl4x INTERFACE
        mbed-mcux-sdk/drivers/common/fsl_common.c
        mbed-mcux-sdk/drivers/common/fsl_common_arm.c
        mbed-mcux-sdk/drivers/adc16/fsl_adc16.c
        mbed-mcux-sdk/drivers/cmp/fsl_cmp.c
        mbed-mcux-sdk/drivers/edma/fsl_edma.c
        mbed-mcux-sdk/drivers/dmamux/fsl_dmamux.c
        mbed-mcux-sdk/drivers/dspi/fsl_dspi.c
        mbed-mcux-sdk/drivers/dspi/fsl_dspi_edma.c
        mbed-mcux-sdk/drivers/uart/fsl_uart.c
        mbed-mcux-sdk/drivers/uart/fsl_uart_edma.c
        mbed-mcux-sdk/drivers/lpuart/fsl_lpuart.c
        mbed-mcux-sdk/drivers/lpuart/fsl_lpuart_edma.c
        mbed-mcux-sdk/drivers/i2c/fsl_i2c.c
        mbed-mcux-sdk/drivers/i2c/fsl_i2c_edma.c
        mbed-mcux-sdk/drivers/vref/fsl_vref.c
        mbed-mcux-sdk/drivers/rcm/fsl_rcm.c
        mbed-mcux-sdk/drivers/smc/fsl_smc.c
        mbed-mcux-sdk/drivers/sim/fsl_sim.c
        mbed-mcux-sdk/drivers/gpio/fsl_gpio.c
        mbed-mcux-sdk/drivers/llwu/fsl_llwu.c
        mbed-mcux-sdk/drivers/pit/fsl_pit.c
        mbed-mcux-sdk/drivers/cmt/fsl_cmt.c
        mbed-mcux-sdk/drivers/ftm/fsl_ftm.c
        mbed-mcux-sdk/drivers/wdog/fsl_wdog.c
        mbed-mcux-sdk/drivers/flash/fsl_ftfx_controller.c
        mbed-mcux-sdk/drivers/flash/fsl_ftfx_flash.c
        mbed-mcux-sdk/drivers/flash/fsl_ftfx_cache.c
        mbed-mcux-sdk/drivers/flash/fsl_ftfx_flexnvm.c
        mbed-mcux-sdk/drivers/rnga/fsl_rnga.c
        mbed-mcux-sdk/drivers/sysmpu/fsl_sysmpu.c
)

target_include_directories(mbed-mcu-kl4x
    INTERFACE
        mbed-mcux-sdk/drivers/common
        mbed-mcux-sdk/drivers/adc16
        mbed-mcux-sdk/drivers/cmp
        mbed-mcux-sdk/drivers/edma
        mbed-mcux-sdk/drivers/dmamux
        mbed-mcux-sdk/drivers/dspi
        mbed-mcux-sdk/drivers/uart
        mbed-mcux-sdk/drivers/lpuart
        mbed-mcux-sdk/drivers/i2c
        mbed-mcux-sdk/drivers/vref
        mbed-mcux-sdk/drivers/rcm
        mbed-mcux-sdk/drivers/smc
        mbed-mcux-sdk/drivers/sim
        mbed-mcux-sdk/drivers/gpio
        mbed-mcux-sdk/drivers/llwu
        mbed-mcux-sdk/drivers/pit
        mbed-mcux-sdk/drivers/cmt
        mbed-mcux-sdk/drivers/ftm
        mbed-mcux-sdk/drivers/wdog
        mbed-mcux-sdk/drivers/flash
        mbed-mcux-sdk/drivers/rnga
        mbed-mcux-sdk/drivers/sysmpu
)

# Link appropriate headers and startup files

target_compile_definitions(mbed-mcu-k6x INTERFACE __PERFORMANCE_IMPLEMENTATION) # for startup ASM

target_sources(mbed-mcu-k64f INTERFACE
        mbed-mcux-sdk/devices/kinetis/MK64F12/system_MK64F12.c
        mbed-mcux-sdk/devices/kinetis/MK64F12/drivers/fsl_clock.c
        mbed-mcux-sdk/devices/kinetis/MK64F12/gcc/startup_MK64F12.S
)
target_include_directories(mbed-mcu-k64f INTERFACE
        mbed-mcux-sdk/devices/kinetis/MK64F12/
        mbed-mcux-sdk/devices/kinetis/MK64F12/drivers
)

target_sources(mbed-mcu-k66f INTERFACE
        mbed-mcux-sdk/devices/kinetis/MK66F18/system_MK66F18.c
        mbed-mcux-sdk/devices/kinetis/MK66F18/drivers/fsl_clock.c
        mbed-mcux-sdk/devices/kinetis/MK66F18/gcc/startup_MK66F18.S
)
target_include_directories(mbed-mcu-k66f INTERFACE
        mbed-mcux-sdk/devices/kinetis/MK66F18/
        mbed-mcux-sdk/devices/kinetis/MK66F18/drivers
)
