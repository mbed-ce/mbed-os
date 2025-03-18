/* Linker script for all STM32L4xx targets that have one flash bank
 * and two RAM banks. */

/*
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2016-2020 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
*/

#include "cmsis_nvic.h"


M_CRASH_DATA_RAM_SIZE = 0x100;

#if !defined(MBED_CONF_TARGET_BOOT_STACK_SIZE)
    /* This value is normally defined by the tools
       to 0x1000 for bare metal and 0x400 for RTOS */
    #define MBED_CONF_TARGET_BOOT_STACK_SIZE  0x400
#endif

/* Round up VECTORS_SIZE to 8 bytes */
#define VECTORS_SIZE  (((NVIC_NUM_VECTORS * 4) + 7) & 0xFFFFFFF8)

MEMORY
{
  FLASH (rx)   : ORIGIN = MBED_CONFIGURED_ROM_START, LENGTH = MBED_CONFIGURED_ROM_SIZE
  SRAM2 (rwx)  : ORIGIN = MBED_RAM_BANK_IRAM2_START + VECTORS_SIZE, LENGTH = MBED_RAM_BANK_IRAM2_SIZE - VECTORS_SIZE
  SRAM1 (rwx)  : ORIGIN = MBED_RAM_BANK_IRAM1_START, LENGTH = MBED_RAM_BANK_IRAM1_SIZE
}

/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 *
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __etext
 *   __data_start__
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __data_end__
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   end
 *   __HeapLimit
 *   __StackLimit
 *   __StackTop
 *   __stack
 *   _estack
 */
ENTRY(Reset_Handler)

SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))
        *(.text*)

        KEEP(*(.init))
        KEEP(*(.fini))

        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        /* .dtors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)

        *(.rodata*)

        KEEP(*(.eh_frame*))
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

    __etext = .;
    _sidata = .;

    .crash_data_ram :
    {
        . = ALIGN(8);
        __CRASH_DATA_RAM__ = .;
        __CRASH_DATA_RAM_START__ = .; /* Create a global symbol at data start */
        KEEP(*(.keep.crash_data_ram))
        *(.m_crash_data_ram)     /* This is a user defined section */
        . += M_CRASH_DATA_RAM_SIZE;
        . = ALIGN(8);
        __CRASH_DATA_RAM_END__ = .; /* Define a global symbol at data end */
    } > SRAM2

    /* Use all remaining SRAM2 as heap 0 */
    .heap_0 (COPY):
    {
        __mbed_sbrk_start_0 = .;
        . += (ORIGIN(SRAM2) + LENGTH(SRAM2) - .);
        __mbed_krbs_start_0 = .;
    } > SRAM2

    .data : AT (__etext)
    {
        __data_start__ = .;
        _sdata = .;
        *(vtable)
        *(.data*)

        . = ALIGN(8);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(8);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(8);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))
        . = ALIGN(8);
        /* All data end */
        __data_end__ = .;
        _edata = .;

    } > SRAM1

    /* Check if bss exceeds SRAM1 */
    ASSERT(__data_end__ <= (ORIGIN(SRAM1)+LENGTH(SRAM1)), ".data is too big for SRAM1")
    
    .bss :
    {
        . = ALIGN(8);
        __bss_start__ = .;
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end__ = .;
        _ebss = .;
    } > SRAM1
    
    /* Placeholder for heap 0 space */
    .heap (COPY):
    {
        __end__ = .;
        end = __end__;
        __mbed_sbrk_start = .;
        *(.heap*)
        . += (ORIGIN(SRAM1) + LENGTH(SRAM1) - .);
        __mbed_krbs_start = .;
        __HeapLimit = .;
    } > SRAM1

    /* Set stack top to end of SRAM1, and stack limit to be less than that by the boot stack size */
    __StackTop = ORIGIN(SRAM1) + LENGTH(SRAM1);
    _estack = __StackTop;
    PROVIDE(__stack = __StackTop);
    __StackLimit = __StackTop - MBED_CONF_TARGET_BOOT_STACK_SIZE;

    /* Check if bss exceeds SRAM1 */
    ASSERT(__bss_end__ < __StackLimit, "Size of static variables does not leave enough room for minimum stack size")
}
