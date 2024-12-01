if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
	mbed_set_linker_script(mbed-stm32h745xi-cm4 STM32H745_H747_CM4.ld)
	mbed_set_linker_script(mbed-stm32h747xi-cm4 STM32H745_H747_CM4.ld)
endif()

# Pick up cmsis-nvic.h
target_include_directories(mbed-stm32h745xi-cm4 INTERFACE .)
target_include_directories(mbed-stm32h747xi-cm4 INTERFACE .)