/*
 * mbed Microcontroller Library
 * Copyright (c) 2026 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Linker script for Renesas RA4E1, restructured from the FSP-generated layout.
 *
 * The FSP BSP copies .data and zeroes .bss in Reset_Handler using the ranges
 * listed in g_init_info (bsp_linker.c). The corresponding symbols
 * (__ram_from_flash$$*, __ram_zero$$*) are therefore load bearing and must
 * stay exactly as they are.
 *
 * The OPTION_SETTING regions map the option bytes emitted by bsp_linker.c into
 * the final image. Do not remove them: programming a new or factory-reset chip
 * without these bytes leaves the part in an undefined state (debug port,
 * bank and block protection configuration).
 */

/* Space reserved for the relocatable vector table at the start of SRAM.
 * All RA MCUs have 16 system exceptions + 96 device IRQs = 112 vectors. */
__vector_table_size__ = 112 * 4;

M_CRASH_DATA_RAM_SIZE = 0x100;

MEMORY
{
    FLASH (rx)  : ORIGIN = MBED_CONFIGURED_ROM_BANK_Flash_START, LENGTH = MBED_CONFIGURED_ROM_BANK_Flash_SIZE
    RAM   (rwx) : ORIGIN = MBED_CONFIGURED_RAM_BANK_SRAM1_START, LENGTH = MBED_CONFIGURED_RAM_BANK_SRAM1_SIZE

    /* Option setting bytes (see bsp_linker.c) */
    OPTION_SETTING_OFS0     (r) : ORIGIN = 0x0100A100, LENGTH = 0x4
    OPTION_SETTING_OFS1     (r) : ORIGIN = 0x0100A180, LENGTH = 0x4
    OPTION_SETTING_BPS      (r) : ORIGIN = 0x0100A1C0, LENGTH = 0x4
    OPTION_SETTING_PBPS     (r) : ORIGIN = 0x0100A1E0, LENGTH = 0x4
    OPTION_SETTING_OFS1_SEC (r) : ORIGIN = 0x0100A200, LENGTH = 0x4
    OPTION_SETTING_BPS_SEC  (r) : ORIGIN = 0x0100A240, LENGTH = 0x4
    OPTION_SETTING_PBPS_SEC (r) : ORIGIN = 0x0100A260, LENGTH = 0x4
    OPTION_SETTING_OFS1_SEL (r) : ORIGIN = 0x0100A280, LENGTH = 0x4
    OPTION_SETTING_BPS_SEL  (r) : ORIGIN = 0x0100A2C0, LENGTH = 0x4
}

/* Linker script to place sections and symbols. It defines following symbols,
 * which code can use without definition:
 *   __etext, _sidata
 *   __data_start__, __data_end__, _sdata, _edata
 *   __preinit_array_start, __preinit_array_end
 *   __init_array_start, __init_array_end
 *   __fini_array_start, __fini_array_end
 *   __bss_start__, __bss_end__, _sbss, _ebss
 *   __StackTop, __StackLimit, __stack, _estack
 *   __HeapBase, __HeapLimit, __end__, end
 *   __exidx_start, __exidx_end
 *   __vector_ram_start__, __vector_ram_end__
 */
ENTRY(Reset_Handler)

SECTIONS
{
    /* ================================ FLASH ================================ */

    .text :
    {
        /* MCU vector table */
        KEEP(*(.fixed_vectors))
        KEEP(*(.application_vectors))

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

        *(.got.plt)
        *(.got)

        KEEP(*(.mcuboot_sce9_key))
        KEEP(*(.version))
    } > FLASH

    .preinit_array :
    {
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN(__preinit_array_end = .);
    } > FLASH

    .init_array :
    {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN(__init_array_end = .);
    } > FLASH

    .fini_array :
    {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } > FLASH

    /* .ARM.extab sections contain exception unwinding information */
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    /* .ARM.exidx sections contain index entries for section unwinding */
    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

    __etext = .;
    _sidata = LOADADDR(.data);

    /* ================================= RAM ================================= */

    /* Relocatable vector table used by the mbed NVIC API */
    .vector_ram (NOLOAD) :
    {
        __vector_ram_start__ = .;
        . += __vector_table_size__;
        __vector_ram_end__ = .;
    } > RAM

#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
    /* Crash data ram, right after the vector table */
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

    /* DTC vector table (r_dtc driver). Left un-KEPT so that it is garbage
     * collected when the DTC driver is not used. */
    .fsp_dtc_vector_table (NOLOAD) :
    {
        __ram_dtc_vector$$Base = .;
        *(.fsp_dtc_vector_table)
        __ram_dtc_vector$$Limit = .;
    } > RAM

    /* Initialized data. Copied from FLASH by Reset_Handler through the
     * __ram_from_flash$$* range in g_init_info. */
    .data :
    {
        __data_start__ = .;
        _sdata = .;
        __ram_from_flash$$Base = .;
        *(.ram_from_flash)
        *(.ram_code_from_flash)
        *(.data*)
        __ram_from_flash$$Limit = .;
        __data_end__ = .;
        _edata = .;
    } > RAM AT > FLASH
    __ram_from_flash$$Load = LOADADDR(.data);

    /* Main stack and no-init data. The main stack is entered before the C
     * runtime runs, so it must not be zeroed or copied. */
    .noinit (NOLOAD) :
    {
        __bss_start__ = .;
        _sbss = .;
        __ram_noinit$$Base = .;
        __StackLimit = .;
        *(.bss.g_main_stack)
        __StackTop = .;
        _estack = __StackTop;
        PROVIDE(__stack = __StackTop);
        *(.ram_noinit)
        *(.noinit)
        __ram_noinit$$Limit = .;
    } > RAM

    /* Zeroed data. Zeroed by Reset_Handler through the __ram_zero$$ range. */
    .bss (NOLOAD) :
    {
        __ram_zero$$Base = .;
        *(.ram)
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        __ram_zero$$Limit = .;
        __bss_end__ = .;
        _ebss = .;
    } > RAM

    /* Thread stacks placed with an explicit section attribute */
    __ram_thread_stack$$ (NOLOAD) : ALIGN(8)
    {
        __ram_thread_stack$$Base = .;
        KEEP(*(.stack?*))
        __ram_thread_stack$$Limit = .;
    } > RAM

    /* Heap, filling the remainder of SRAM */
    .heap (NOLOAD) :
    {
        __HeapBase = .;
        __end__ = .;
        PROVIDE(end = .);
        *(.bss.g_heap)
        . = ORIGIN(RAM) + LENGTH(RAM);
        __HeapLimit = .;
    } > RAM

    ASSERT(__HeapBase <= ORIGIN(RAM) + LENGTH(RAM), "region RAM overflowed with stack, data and bss")

    /* ============================ OPTION SETTINGS =========================== */

    /* Option Function Select Register 0 */
    .option_setting_ofs0 :
    {
        KEEP(*(.option_setting_ofs0))
    } > OPTION_SETTING_OFS0

    /* Option Function Select Register 1 */
    .option_setting_ofs1 :
    {
        KEEP(*(.option_setting_ofs1))
    } > OPTION_SETTING_OFS1

    /* Block Protect Setting Register */
    .option_setting_bps :
    {
        KEEP(*(.option_setting_bps))
    } > OPTION_SETTING_BPS

    /* Permanent Block Protect Setting Register */
    .option_setting_pbps :
    {
        KEEP(*(.option_setting_pbps))
    } > OPTION_SETTING_PBPS

    /* Option Function Select Register 1 Secure */
    .option_setting_ofs1_sec :
    {
        KEEP(*(.option_setting_ofs1_sec))
    } > OPTION_SETTING_OFS1_SEC

    /* Block Protect Setting Register Secure */
    .option_setting_bps_sec :
    {
        KEEP(*(.option_setting_bps_sec))
    } > OPTION_SETTING_BPS_SEC

    /* Permanent Block Protect Setting Register Secure */
    .option_setting_pbps_sec :
    {
        KEEP(*(.option_setting_pbps_sec))
    } > OPTION_SETTING_PBPS_SEC

    /* OFS1 Security Attribution */
    .option_setting_ofs1_sel :
    {
        KEEP(*(.option_setting_ofs1_sel))
    } > OPTION_SETTING_OFS1_SEL

    /* BPS Security Attribution */
    .option_setting_bps_sel :
    {
        KEEP(*(.option_setting_bps_sel))
    } > OPTION_SETTING_BPS_SEL
}
