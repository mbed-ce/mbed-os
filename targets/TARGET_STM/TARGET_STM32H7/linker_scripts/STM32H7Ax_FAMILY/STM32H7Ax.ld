/* Linker script to configure memory regions. */
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
#include "../stm32_eth_region_size_calcs.h"

M_CRASH_DATA_RAM_SIZE = 0x100;

#if !defined(MBED_CONF_TARGET_BOOT_STACK_SIZE)
    /* This value is normally defined by the tools
       to 0x1000 for bare metal and 0x400 for RTOS */
    #define MBED_CONF_TARGET_BOOT_STACK_SIZE  0x400
#endif

STACK_SIZE = MBED_CONF_TARGET_BOOT_STACK_SIZE;

/* Round up MBED_VECTTABLE_RAM_SIZE to 8 bytes */
#define MBED_VECTTABLE_RAM_SIZE  (((NVIC_NUM_VECTORS * 4) + 7) & 0xFFFFFFF8)

MEMORY
{
    FLASH       (rx)  : ORIGIN = MBED_CONFIGURED_ROM_BANK_IROM1_START, LENGTH = MBED_CONFIGURED_ROM_BANK_IROM1_SIZE
    SRAM_DTC   (xrw)  : ORIGIN = MBED_RAM_BANK_SRAM_DTC_START + MBED_VECTTABLE_RAM_SIZE, LENGTH = MBED_RAM_BANK_SRAM_DTC_SIZE - MBED_VECTTABLE_RAM_SIZE
    SRAM       (xrw)  : ORIGIN = MBED_RAM_BANK_SRAM_AHB_START, LENGTH = MBED_RAM_BANK_SRAM_AHB_SIZE
    SRAM_AXI   (xrw)  : ORIGIN = MBED_RAM_BANK_SRAM_AXI_START, LENGTH = MBED_RAM_BANK_SRAM_AXI_SIZE
    SRAM_D3    (xrw)  : ORIGIN = MBED_RAM_BANK_SRAM_D3_START, LENGTH = MBED_RAM_BANK_SRAM_D3_SIZE
    SRAM_ITC   (xrw)  : ORIGIN = MBED_RAM_BANK_SRAM_ITC_START, LENGTH = MBED_RAM_BANK_SRAM_ITC_SIZE
}

/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and SRAM.
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

    } > SRAM

    /* Uninitialized data section
     * This region is not initialized by the C/C++ library and can be used to
     * store state across soft reboots. */
    .uninitialized (NOLOAD):
    {
        . = ALIGN(32);
        __uninitialized_start = .;
        *(.uninitialized)
        KEEP(*(.keep.uninitialized))
        . = ALIGN(32);
        __uninitialized_end = .;
    } > SRAM
    
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
    } > SRAM

    /* Check if data + stack will exceed SRAM limit */
    ASSERT(_ebss < ORIGIN(SRAM) + LENGTH(SRAM) - MBED_CONF_TARGET_BOOT_STACK_SIZE, "region SRAM does not have enough space for boot stack size")

    .heap_0 (NOLOAD):
    {
        PROVIDE(__mbed_sbrk_start_0 = .);
        . = ORIGIN(SRAM) + LENGTH(SRAM) - MBED_CONF_TARGET_BOOT_STACK_SIZE;
        PROVIDE(__mbed_krbs_start_0 = .);
    } > SRAM

    /* .stack_dummy section doesn't contains any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later */
    .stack_dummy (COPY):
    {
        *(.stack*)
    } > SRAM

    /* Set stack top to end of SRAM, and stack limit move down by
     * size of stack_dummy section */
    __StackTop = ORIGIN(SRAM) + LENGTH(SRAM);
    _estack = __StackTop;
    __StackLimit = __StackTop - MBED_CONF_TARGET_BOOT_STACK_SIZE;
    PROVIDE(__stack = __StackTop);

    /* Put crash data in the otherwise unused D3 SRAM */
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
    } > SRAM_D3

    /* Ethernet DMA descriptors should be at the start of SRAM_AXI because they need an MPU region.*/
    .eth_descriptors (NOLOAD) : {
        ASSERT(. == ORIGIN(SRAM_AXI), "Eth Descriptors region must be at the start of SRAM_D2");
        PROVIDE(__eth_descriptors_start = .);
        *(.EthDescriptors)
        . = __eth_descriptors_start + STM32_DMA_DESCRIP_REGION_SIZE;
    } >SRAM_AXI

    /* Ethernet buffers are recommended to be in SRAM_AXI but don't need any special MPU region. */
    .eth_buffers (NOLOAD) : {
        *(.EthBuffers)
    } >SRAM_AXI 

    /* Use SRAM_AXI as additional heap */
    .heap (NOLOAD):
    {
        PROVIDE(__mbed_sbrk_start = .);
        . += (ORIGIN(SRAM_AXI) + LENGTH(SRAM_AXI) - .);
        PROVIDE(__mbed_krbs_start = .);
    } > SRAM_AXI
}
