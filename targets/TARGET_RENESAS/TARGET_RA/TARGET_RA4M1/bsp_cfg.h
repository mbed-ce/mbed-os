/* generated configuration header file - do not edit */
#ifndef BSP_CFG_H_
#define BSP_CFG_H_
#ifdef __cplusplus
            extern "C" {
            #endif

            /* The FSP system code enables the FPU (SCB->CPACR) under
             * "#if __FPU_USED". That macro was provided by the CMSIS 5
             * compiler headers, but Mbed CE ships CMSIS 6 which no longer
             * defines it. Derive it from the compiler builtin so the FPU
             * actually gets switched on when the toolchain generates
             * hardware floating point code (ATfE/clang default for the
             * Cortex-M4F is softfp with FPU instructions enabled - without
             * CPACR the first FPU instruction raises a UsageFault). */
            #if !defined(__FPU_USED)
             #if defined(__ARM_FP) && (__ARM_FP >= 1)
              #define __FPU_USED 1
             #else
              #define __FPU_USED 0
             #endif
            #endif

            #include "bsp_clock_cfg.h"
            #include "bsp_mcu_family_cfg.h"
            #include "bsp_mcu_ofs_cfg.h"
            #include "board_cfg.h"
            #include "vector_data.h"
            #define RA_NOT_DEFINED 0
            #ifndef BSP_CFG_RTOS
             #if (RA_NOT_DEFINED) != (RA_NOT_DEFINED)
              #define BSP_CFG_RTOS (2)
             #elif (RA_NOT_DEFINED) != (RA_NOT_DEFINED)
              #define BSP_CFG_RTOS (1)
             #else
              #define BSP_CFG_RTOS (0)
             #endif
            #endif
            #ifndef BSP_CFG_RTC_USED
             #define BSP_CFG_RTC_USED (1)
            #endif
            #undef RA_NOT_DEFINED
            /* Applications running on top of the Arduino UNO R4 bootloader must
             * not emit option setting bytes: the OFS/SECMPU area (0x400-0x47F)
             * is owned by the bootloader. */
            #ifndef BSP_BOOTLOADED_APPLICATION
             #define BSP_BOOTLOADED_APPLICATION (1)
            #endif
            #if defined(_RA_BOOT_IMAGE)
             #define BSP_CFG_BOOT_IMAGE (1)
            #endif
            #define BSP_CFG_MCU_VCC_MV (3300)
            #define BSP_CFG_STACK_MAIN_BYTES (MBED_CONF_TARGET_BOOT_STACK_SIZE)
            #define BSP_CFG_HEAP_BYTES 0
            #define BSP_CFG_PARAM_CHECKING_ENABLE (0)
            #define BSP_CFG_ASSERT (0)

            #define BSP_CFG_PFS_PROTECT ((1))

            #define BSP_CFG_C_RUNTIME_INIT ((1))
            #define BSP_CFG_EARLY_INIT     ((0))

            #define BSP_CFG_STARTUP_CLOCK_REG_NOT_RESET ((0))

            #ifndef BSP_CLOCK_CFG_MAIN_OSC_POPULATED
            #define BSP_CLOCK_CFG_MAIN_OSC_POPULATED (0)
            #endif

            #ifndef BSP_CLOCK_CFG_MAIN_OSC_CLOCK_SOURCE
            #define BSP_CLOCK_CFG_MAIN_OSC_CLOCK_SOURCE (0)
            #endif
            #ifndef BSP_CLOCK_CFG_SUBCLOCK_DRIVE
            #define BSP_CLOCK_CFG_SUBCLOCK_DRIVE (0)
            #endif
            #ifndef BSP_CLOCK_CFG_SUBCLOCK_POPULATED
            #define BSP_CLOCK_CFG_SUBCLOCK_POPULATED (0)
            #endif
            #ifndef BSP_CLOCK_CFG_SUBCLOCK_STABILIZATION_MS
            #define BSP_CLOCK_CFG_SUBCLOCK_STABILIZATION_MS 1000
            #endif

            #ifdef __cplusplus
            }
            #endif
#endif /* BSP_CFG_H_ */
