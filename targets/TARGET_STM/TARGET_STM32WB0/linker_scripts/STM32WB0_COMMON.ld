/**
 * Copyright (c) 2026 MbedCE Community Contributors (Jan Kamidra)
 * SPDX-License-Identifier: Apache-2.0
 */

M_CRASH_DATA_RAM_SIZE = 0x100;

#if !defined(MBED_CONF_TARGET_BOOT_STACK_SIZE)
#define MBED_CONF_TARGET_BOOT_STACK_SIZE 0x400
#endif
STACK_SIZE = MBED_CONF_TARGET_BOOT_STACK_SIZE;

MEMORY
{
    FLASH (rx) : ORIGIN = MBED_CONFIGURED_ROM_BANK_ROM_START, LENGTH = MBED_CONFIGURED_ROM_BANK_ROM_SIZE
    RAM (rwx)  : ORIGIN = MBED_RAM_BANK_SRAM_START, LENGTH = MBED_RAM_BANK_SRAM_SIZE
}

ENTRY(Reset_Handler)

SECTIONS
{
    .intvec :
    {
        __vector_flash_start__ = ABSOLUTE(.);
        KEEP(*(.intvec))
        __vector_flash_end__ = ABSOLUTE(.);
        __vector_table_size__ = ABSOLUTE(__vector_flash_end__ - __vector_flash_start__);
    } > FLASH

    .text :
    {
        *(.text*)
        KEEP(*(.init))
        KEEP(*(.fini))

        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

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

    /* STM32WB0 ROM ABI data occupies the first 0x100 bytes of SRAM. */
    .ram_vr MBED_RAM_BANK_SRAM_START + 0x4 (NOLOAD) :
    {
        KEEP(*(.ram_vr))
    } > RAM

    .bss.blueRAM MBED_RAM_BANK_SRAM_START + 0xC0 (NOLOAD) :
    {
        _sbssblue = .;
        KEEP(*(.bss.__blue_RAM))
        _ebssblue = .;
    } > RAM

    /* Place the runtime vector table immediately after the ROM ABI area. */
    .vector_ram MBED_RAM_BANK_SRAM_START + 0x100 (NOLOAD) :
    {
        __vector_ram_start__ = .;
        . += __vector_table_size__;
        __vector_ram_end__ = .;
    } > RAM

#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
    .crash_data_ram (NOLOAD) :
    {
        . = ALIGN(8);
        __CRASH_DATA_RAM__ = .;
        __CRASH_DATA_RAM_START__ = .;
        KEEP(*(.keep.crash_data_ram))
        *(.m_crash_data_ram)
        . += M_CRASH_DATA_RAM_SIZE;
        . = ALIGN(8);
        __CRASH_DATA_RAM_END__ = .;
    } > RAM
#endif

    .data : AT (__etext)
    {
        __data_start__ = .;
        _sdata = .;
        *(vtable)
        *(.data*)

        . = ALIGN(8);
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN(__preinit_array_end = .);
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN(__init_array_end = .);
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN(__fini_array_end = .);
        KEEP(*(.jcr*))
        . = ALIGN(8);

        __data_end__ = .;
        _edata = .;
    } > RAM

    .uninitialized (NOLOAD) :
    {
        . = ALIGN(32);
        __uninitialized_start = .;
        *(.uninitialized)
        KEEP(*(.keep.uninitialized))
        . = ALIGN(32);
        __uninitialized_end = .;
    } > RAM

    .bss (NOLOAD) :
    {
        . = ALIGN(8);
        __bss_start__ = .;
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end__ = .;
        _ebss = .;
    } > RAM

    .noinit (NOLOAD) :
    {
        *(.noinit*)
    } > RAM

    .heap (COPY) :
    {
        __end__ = .;
        PROVIDE(end = .);
        *(.heap*)
        . = ORIGIN(RAM) + LENGTH(RAM) - STACK_SIZE;
        __HeapLimit = .;
    } > RAM

    .stack_dummy (COPY) :
    {
        *(.stack*)
    } > RAM

    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    _estack = __StackTop;
    __StackLimit = __StackTop - STACK_SIZE;
    PROVIDE(__stack = __StackTop);

    ASSERT((__vector_table_size__ % 4) == 0, "vector table size is not word aligned")
    ASSERT(__vector_ram_end__ <= ORIGIN(RAM) + LENGTH(RAM), "vector table exceeds RAM")
    ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}
