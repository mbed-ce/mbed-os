/*
 * Mbed wrapper for the STM32F7 CMSIS device header.
 *
 * Mbed's CMake core settings define __FPU_PRESENT on the compiler command line,
 * while STM32 CMSIS device headers define it too.  Undefine the command-line
 * value here so the CMSIS device header remains the final source of truth and
 * GCC does not warn about a macro redefinition.
 */
#ifndef MBED_STM32F7XX_WRAPPER_H
#define MBED_STM32F7XX_WRAPPER_H

#ifdef __FPU_PRESENT
#undef __FPU_PRESENT
#endif

#include "cmsis-device-f7/Include/stm32f7xx.h"

#endif /* MBED_STM32F7XX_WRAPPER_H */