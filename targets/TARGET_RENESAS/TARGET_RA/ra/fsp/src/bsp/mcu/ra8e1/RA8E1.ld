/*
 * mbed Microcontroller Library
 * Copyright (c) 2026 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Linker script for Renesas RA8E1, restructured from the FSP-generated layout.
 *
 * The FSP BSP copies .data and zeroes .bss in Reset_Handler using the ranges
 * listed in g_init_info (bsp_linker.c). The corresponding symbols
 * (__ram_from_flash$$*, __ram_zero$$*, __*itcm/dtcm_zero$$*,
 * __ram_*_nocache$$*) are therefore load bearing and must stay exactly as
 * they are.
 *
 * The relocatable vector table lives in DTCM, which is the fastest memory
 * reachable by the Cortex-M85 core.
 *
 * The OPTION_SETTING regions map the option bytes emitted by bsp_linker.c into
 * the final image. Do not remove them: programming a new or factory-reset chip
 * without these bytes leaves the part in an undefined state (debug port,
 * bank and block protection configuration).
 */

/* Space reserved for the relocatable vector table.
 * All RA MCUs have 16 system exceptions + 96 device IRQs = 112 vectors. */
__vector_table_size__ = 112 * 4;

M_CRASH_DATA_RAM_SIZE = 0x100;

MEMORY
{
    FLASH (rx)  : ORIGIN = MBED_CONFIGURED_ROM_BANK_Flash_START, LENGTH = MBED_CONFIGURED_ROM_BANK_Flash_SIZE
    RAM   (rwx) : ORIGIN = MBED_CONFIGURED_RAM_BANK_SRAM_START,  LENGTH = MBED_CONFIGURED_RAM_BANK_SRAM_SIZE
    ITCM  (rwx) : ORIGIN = MBED_CONFIGURED_RAM_BANK_ITCM_START,  LENGTH = MBED_CONFIGURED_RAM_BANK_ITCM_SIZE
    DTCM  (rwx) : ORIGIN = MBED_CONFIGURED_RAM_BANK_DTCM_START,  LENGTH = MBED_CONFIGURED_RAM_BANK_DTCM_SIZE

    /* Option setting bytes (see bsp_linker.c). The 0x1300xxxx areas are the
     * bank-1 mirrors used on dual-bank devices. */
    OPTION_SETTING_OFS0        (r) : ORIGIN = 0x0300A100, LENGTH = 0x4
    OPTION_SETTING_OFS2        (r) : ORIGIN = 0x0300A104, LENGTH = 0x4
    OPTION_SETTING_DUALSEL     (r) : ORIGIN = 0x0300A110, LENGTH = 0x4
    OPTION_SETTING_OFS1        (r) : ORIGIN = 0x1300A180, LENGTH = 0x4
    OPTION_SETTING_BANKSEL     (r) : ORIGIN = 0x1300A190, LENGTH = 0x4
    OPTION_SETTING_BPS         (r) : ORIGIN = 0x1300A1C0, LENGTH = 0x10
    OPTION_SETTING_PBPS        (r) : ORIGIN = 0x1300A1E0, LENGTH = 0x10
    OPTION_SETTING_OFS1_SEC    (r) : ORIGIN = 0x0300A200, LENGTH = 0x4
    OPTION_SETTING_BANKSEL_SEC (r) : ORIGIN = 0x0300A210, LENGTH = 0x4
    OPTION_SETTING_BPS_SEC     (r) : ORIGIN = 0x0300A240, LENGTH = 0x10
    OPTION_SETTING_PBPS_SEC    (r) : ORIGIN = 0x0300A260, LENGTH = 0x10
    OPTION_SETTING_OFS1_SEL    (r) : ORIGIN = 0x0300A280, LENGTH = 0x4
    OPTION_SETTING_BANKSEL_SEL (r) : ORIGIN = 0x0300A290, LENGTH = 0x4
    OPTION_SETTING_BPS_SEL     (r) : ORIGIN = 0x0300A2C0, LENGTH = 0x10
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

    /* ================================= DTCM ================================ */

    /* Relocatable vector table used by the mbed NVIC API */
    .vector_ram (NOLOAD) :
    {
        __vector_ram_start__ = .;
        . += __vector_table_size__;
        __vector_ram_end__ = .;
    } > DTCM

    /* Non-initialized dtcm */
    __dtcm_noinit$$ (NOLOAD) :
    {
        __dtcm_noinit$$Base = .;
        *(.dtcm_noinit)
        __dtcm_noinit$$Limit = .;
    } > DTCM

    /* Zeroed dtcm. Zeroed by Reset_Handler through the __dtcm_zero$$ range. */
    __dtcm_zero$$ (NOLOAD) :
    {
        __dtcm_zero$$Base = .;
        *(.dtcm)
        __dtcm_zero$$Limit = .;
    } > DTCM

    /* ================================= ITCM ================================ */

    /* Non-initialized itcm */
    __itcm_noinit$$ (NOLOAD) :
    {
        __itcm_noinit$$Base = .;
        *(.itcm_noinit)
        __itcm_noinit$$Limit = .;
    } > ITCM

    /* Zeroed itcm. Zeroed by Reset_Handler through the __itcm_zero$$ range. */
    __itcm_zero$$ (NOLOAD) :
    {
        __itcm_zero$$Base = .;
        *(.itcm)
        __itcm_zero$$Limit = .;
    } > ITCM

    /* ================================= RAM ================================= */

    /* DTC vector table (r_dtc driver). Left un-KEPT so that it is garbage
     * collected when the DTC driver is not used. */
    .fsp_dtc_vector_table (NOLOAD) :
    {
        __ram_dtc_vector$$Base = .;
        *(.fsp_dtc_vector_table)
        __ram_dtc_vector$$Limit = .;
    } > RAM

#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
    /* Crash data ram, right after the DTC vector table */
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

    /* Non-cached RAM. Covered by the MPU nocache range in g_init_info, which
     * spans from __ram_noinit_nocache$$Base to __ram_zero_nocache$$Limit.
     * This includes the Mbed .noncached section so that it is genuinely
     * non-cacheable on the Cortex-M85. */
    __ram_noinit_nocache$$ (NOLOAD) : ALIGN(32)
    {
        __bss_start__ = .;
        _sbss = .;
        __ram_noinit_nocache$$Base = .;
        __noncached_start = .;
        *(.noncached)
        *(.ram_noinit_nocache)
        . = ALIGN(32);
        __noncached_end = .;
        __ram_noinit_nocache$$Limit = .;
    } > RAM

    __ram_zero_nocache$$ (NOLOAD) :
    {
        __ram_zero_nocache$$Base = .;
        *(.ram_nocache)
        . = ALIGN(32);
        __ram_zero_nocache$$Limit = .;
    } > RAM

    /* Main stack and no-init data. The main stack is entered before the C
     * runtime runs, so it must not be zeroed or copied. */
    .noinit (NOLOAD) :
    {
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
        . = ALIGN(32);
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

    /* ===================== EMPTY FSP INIT TABLE RANGES ===================== */

    /* Zero-length ranges referenced by g_init_info (bsp_linker.c). No Mbed
     * build copies data out of OSPI or data flash, so Base == Limit and the
     * BSP init code skips them. */
    __ospi0_cs0_zero_nocache$$Base        = ORIGIN(RAM);
    __ospi0_cs0_zero_nocache$$Limit       = ORIGIN(RAM);
    __ospi0_cs0_zero$$Base                = ORIGIN(RAM);
    __ospi0_cs0_zero$$Limit               = ORIGIN(RAM);
    __ospi0_cs0_noinit_nocache$$Base      = ORIGIN(RAM);
    __ospi0_cs0_noinit_nocache$$Limit     = ORIGIN(RAM);
    __ospi0_cs0_from_ospi0_cs1$$Base      = ORIGIN(RAM);
    __ospi0_cs0_from_ospi0_cs1$$Limit     = ORIGIN(RAM);
    __ospi0_cs0_from_ospi0_cs1$$Load      = ORIGIN(RAM);
    __ospi0_cs0_from_data_flash$$Base     = ORIGIN(RAM);
    __ospi0_cs0_from_data_flash$$Limit    = ORIGIN(RAM);
    __ospi0_cs0_from_data_flash$$Load     = ORIGIN(RAM);
    __ospi0_cs0_from_flash$$Base          = ORIGIN(RAM);
    __ospi0_cs0_from_flash$$Limit         = ORIGIN(RAM);
    __ospi0_cs0_from_flash$$Load          = ORIGIN(RAM);
    __itcm_from_ospi0_cs1$$Base           = ORIGIN(ITCM);
    __itcm_from_ospi0_cs1$$Limit          = ORIGIN(ITCM);
    __itcm_from_ospi0_cs1$$Load           = ORIGIN(ITCM);
    __itcm_from_data_flash$$Base          = ORIGIN(ITCM);
    __itcm_from_data_flash$$Limit         = ORIGIN(ITCM);
    __itcm_from_data_flash$$Load          = ORIGIN(ITCM);
    __itcm_from_flash$$Base               = ORIGIN(ITCM);
    __itcm_from_flash$$Limit              = ORIGIN(ITCM);
    __itcm_from_flash$$Load               = ORIGIN(ITCM);
    __dtcm_from_ospi0_cs1$$Base           = ORIGIN(DTCM);
    __dtcm_from_ospi0_cs1$$Limit          = ORIGIN(DTCM);
    __dtcm_from_ospi0_cs1$$Load           = ORIGIN(DTCM);
    __dtcm_from_data_flash$$Base          = ORIGIN(DTCM);
    __dtcm_from_data_flash$$Limit         = ORIGIN(DTCM);
    __dtcm_from_data_flash$$Load          = ORIGIN(DTCM);
    __dtcm_from_flash$$Base               = ORIGIN(DTCM);
    __dtcm_from_flash$$Limit              = ORIGIN(DTCM);
    __dtcm_from_flash$$Load               = ORIGIN(DTCM);
    __ram_from_ospi0_cs1$$Base            = ORIGIN(RAM);
    __ram_from_ospi0_cs1$$Limit           = ORIGIN(RAM);
    __ram_from_ospi0_cs1$$Load            = ORIGIN(RAM);
    __ram_from_data_flash$$Base           = ORIGIN(RAM);
    __ram_from_data_flash$$Limit          = ORIGIN(RAM);
    __ram_from_data_flash$$Load           = ORIGIN(RAM);

    /* ============================ OPTION SETTINGS =========================== */

    /* Option Function Select Register 0 */
    .option_setting_ofs0 :
    {
        KEEP(*(.option_setting_ofs0))
    } > OPTION_SETTING_OFS0

    /* Option Function Select Register 2 */
    .option_setting_ofs2 :
    {
        KEEP(*(.option_setting_ofs2))
    } > OPTION_SETTING_OFS2

    /* Dual Mode Select Register */
    .option_setting_dualsel :
    {
        KEEP(*(.option_setting_dualsel))
    } > OPTION_SETTING_DUALSEL

    /* Option Function Select Register 1 */
    .option_setting_ofs1 :
    {
        KEEP(*(.option_setting_ofs1))
    } > OPTION_SETTING_OFS1

    /* Bank Select Register */
    .option_setting_banksel :
    {
        KEEP(*(.option_setting_banksel))
    } > OPTION_SETTING_BANKSEL

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

    /* Bank Select Register Secure */
    .option_setting_banksel_sec :
    {
        KEEP(*(.option_setting_banksel_sec))
    } > OPTION_SETTING_BANKSEL_SEC

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

    /* Banksel Security Attribution */
    .option_setting_banksel_sel :
    {
        KEEP(*(.option_setting_banksel_sel))
    } > OPTION_SETTING_BANKSEL_SEL

    /* BPS Security Attribution */
    .option_setting_bps_sel :
    {
        KEEP(*(.option_setting_bps_sel))
    } > OPTION_SETTING_BPS_SEL
}
