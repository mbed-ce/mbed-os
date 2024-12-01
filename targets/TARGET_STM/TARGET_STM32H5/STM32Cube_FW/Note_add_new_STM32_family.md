# How to add new STM32 family
1. 	In path mbed-os\targets\TARGET_STM\ add new family folder, in our my case TARGET_STM32H5, and also do not forget add same name into CmakeLists.txt in same folder level.
2.	In path mbed-os\targets\TARGET_STM\TARGET_STM32H5\ prepare another folders. First is STM32Cube_FW and second CMSIS
3. 	Clone or download the [stm32h5xx_hal_driver](https://github.com/STMicroelectronics/stm32h5xx_hal_driver) repository
4.  Copy `stm32h5xx_hal_driver/*.h` and `stm32h5xx_hal_driver/*.c` (but not the _template files) into `mbed-os\targets\TARGET_STM\TARGET_STM32H5\STM32Cube_FW\STM32H5xx_HAL_Driver`
5.  Clone or download [cmsis_device_h5](https://github.com/STMicroelectronics/cmsis_device_h5/tree/main)
6.  Copy `cmsis_device_h5\Include\*.h` to mbed-os\targets\TARGET_STM\TARGET_STM32H5\STM32Cube_FW\CMSIS\
6.	From `cmsis_device_h5\Source\Templates` copy system_stm32h5xx.c into mbed-os\targets\TARGET_STM\TARGET_STM32H5\STM32Cube_FW\
10.	From `stm32h5xx_hal_driver\Inc` move file stm32h5xx_hal_conf_template.h to one folder level up. 
	- Rest of templates could be deleted.
	- The moved file stm32h5xx_hal_conf_template.h should be renamed to stm32h5xx_hal_conf.h
	- inside of stm32h5xx_hal_conf_template.h file should be all macros #define  USE_HAL_XXXXXXXXX covered by macro #if !defined(USE_HAL_XXXXXXXXX) statement
	- inside of stm32h5xx_hal_conf_template.h file should be the true part of #ifdef  USE_FULL_ASSERT filled with #include "stm32_assert.h"
11.	In mbed-os\targets\TARGET_STM\TARGET_STM32H5\STM32Cube_FW\ do not forget create correct CmakeLists.txt
12.	In mbed-os\targets\TARGET_STM\TARGET_STM32H5\ do not forget create correct CmakeLists.txt
12. In mbed-os\targets\TARGET_STM\ do not forget update CmakeLists.txt with new family
13. Lets get add new targets for that family.