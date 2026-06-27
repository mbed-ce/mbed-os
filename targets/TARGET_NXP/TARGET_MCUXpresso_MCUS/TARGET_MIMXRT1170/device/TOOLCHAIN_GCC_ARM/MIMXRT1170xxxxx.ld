/*
** ###################################################################
**     Processors:          MIMXRT1176AVM8A_cm7
**                          MIMXRT1176CVM8A_cm7
**                          MIMXRT1176DVMAA_cm7
**
**     Compiler:            GNU C Compiler
**     Reference manual:    IMXRT1170RM, Rev E, 12/2019
**     Version:             rev. 0.1, 2018-03-05
**     Build:               b200828
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2020 NXP
**     All rights reserved.
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
** ###################################################################
*/

/* Entry Point */
ENTRY(Reset_Handler)

__ram_vector_table__ = 1;

/* If we are not configured to execute out of the start of ROM, then a bootloader is
 * present.  This tells us whether we need to add the Flash Configuration Field at the start of flash. */
#if MBED_CONFIGURED_ROM_BANK_EXT_FLASH_START == MBED_ROM_BANK_EXT_FLASH_START
#define IS_BOOTLOADER_PRESENT 0
#else
#define IS_BOOTLOADER_PRESENT 1
#endif

#if !defined(MBED_CONF_TARGET_BOOT_STACK_SIZE)
    #define MBED_CONF_TARGET_BOOT_STACK_SIZE 0x400
#endif

__stack_size__ = MBED_CONF_TARGET_BOOT_STACK_SIZE;

STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0400;
M_VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x0400 : 0x0;

/* Specify the memory areas */
MEMORY
{
  m_text                (RX)  : ORIGIN = MBED_CONFIGURED_ROM_BANK_EXT_FLASH_START, LENGTH = MBED_CONFIGURED_ROM_BANK_EXT_FLASH_SIZE
  m_dtcm                (RW)  : ORIGIN = MBED_RAM_BANK_SRAM_DTC_START, LENGTH = MBED_RAM_BANK_SRAM_DTC_SIZE
  m_itcm                (RX)  : ORIGIN = MBED_RAM_BANK_SRAM_ITC_START, LENGTH = MBED_RAM_BANK_SRAM_ITC_SIZE

  m_ocram_combined      (RW)  : ORIGIN = MBED_RAM_BANK_SRAM_OC_1_2_COMBINED_START, LENGTH = MBED_RAM_BANK_SRAM_OC_1_2_COMBINED_SIZE

 /* External SDRAM, if available */
#ifdef MBED_RAM_BANK_SDRAM_SIZE
  m_sdram               (RW)  : ORIGIN = MBED_RAM_BANK_SDRAM_START, LENGTH = MBED_RAM_BANK_SDRAM_SIZE
#endif
}

/* Define output sections */
SECTIONS
{

#if !defined(IS_BOOTLOADER_PRESENT)
  /* Flash config goes first, at the start of flash */
  .flash_config :
  {
    . = ALIGN(8);
    __FLASH_BASE = .;
    KEEP(* (.boot_hdr.conf))     /* flash config section */
    . = ALIGN(8);
  } > m_text

  /* Then IVT at offset 0x1000 */
  .ivt MBED_ROM_BANK_EXT_FLASH_START + 0x1000 :
  {
    . = ALIGN(8);
    KEEP(* (.boot_hdr.ivt))           /* ivt section */
    KEEP(* (.boot_hdr.boot_data))     /* boot section */
    KEEP(* (.boot_hdr.dcd_data))      /* dcd section */
    . = ALIGN(8);
  } > m_text
  /* Note that we do not want to use AT> above, because that would result in this section being
   * placed in flash immediately after .flash_config.  Instead we don't use AT> to set the LMA equal
   * to the VMA. */

  /* Interrupts go after that */
  #define INTERRUPT_TABLE_ADDR MBED_ROM_BANK_EXT_FLASH_START + 0x2000
#else
  /* We have a bootloader so we don't need the flash config or IVT */
  /DISCARD/ : {
    *(.boot_hdr.conf)
    *(.boot_hdr.ivt)
    *(.boot_hdr.boot_data)
    *(.boot_hdr.dcd_data)
  }

  /* Interrupts go at the start of configured flash area */
  #define INTERRUPT_TABLE_ADDR MBED_CONFIGURED_ROM_BANK_EXT_FLASH_START
#endif

  /* The startup code goes first into internal RAM */
  .interrupts INTERRUPT_TABLE_ADDR :
  {
    __VECTOR_TABLE = .;
    . = ALIGN(8);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(8);
  } > m_text

  /* The program code and other data goes into FlexSPI */
  .text :
  {
    . = ALIGN(8);
    *(.text)                 /* .text sections (code) */
    *(.text*)                /* .text* sections (code) */
    *(.rodata)               /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(8);
  } > m_text

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_text

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_text

 .ctors :
  {
    __CTOR_LIST__ = .;
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    __CTOR_END__ = .;
  } > m_text

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_text

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_text

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_text

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_text

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

  .interrupts_ram :
  {
    . = ALIGN(8);
    __VECTOR_RAM__ = .;
    __interrupts_ram_start__ = .; /* Create a global symbol at data start */
    *(.m_interrupts_ram)     /* This is a user defined section */
    . += M_VECTOR_RAM_SIZE;
    . = ALIGN(8);
    __interrupts_ram_end__ = .; /* Define a global symbol at data end */
  } > m_dtcm

  __VECTOR_RAM = DEFINED(__ram_vector_table__) ? __VECTOR_RAM__ : ORIGIN(m_interrupts);
  __RAM_VECTOR_TABLE_SIZE_BYTES = DEFINED(__ram_vector_table__) ? (__interrupts_ram_end__ - __interrupts_ram_start__) : 0x0;

  /* Note: MIMXRT1170 has a major errata (ERR050396) which causes many DMA-enabled peripherals to be unable to write to
   * DTCM. So, we currently do not use DTCM, even though this does hurt performance a fair bit. */

   /* Non-cached data section.
    * This section is configured in the MPU as uncached, so reads and writes go directly through to memory.
    * We put this first in OCRAM so it's very, very aligned. */
   .ncache.init : AT(__NDATA_ROM)
   {
     __noncachedata_start__ = .;   /* create a global symbol at ncache data start */
     *(NonCacheable.init)
     . = ALIGN(8);
     __noncachedata_init_end__ = .;   /* create a global symbol at initialized ncache data end */
   } > m_ocram_combined
   . = __noncachedata_init_end__;
   .ncache :
   {
     *(NonCacheable)
     *(.noncached)

     /* For the MPU configuration, we need to align the size of this section up to
      * the next power of two. Also note that the MPU does not support regions
      * smaller than 32 bytes so round up if smaller than that. */
     __noncached_data_size = (. - __noncachedata_start__);
     __noncached_aligned_size = MAX(32, 1 << LOG2CEIL(__noncached_data_size));

     . += (__noncached_aligned_size - __noncached_data_size);

     __noncachedata_end__ = .;     /* define a global symbol at ncache data end */
   } > m_ocram_combined

  .data :
  {
    . = ALIGN(8);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(m_usb_dma_init_data)
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(8);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_ocram_combined AT> m_text
  __ram_function_flash_start = __DATA_ROM + (__data_end__ - __data_start__); /* Symbol is used by startup for TCM data initialization */

  .ram_function :
  {
    . = ALIGN(32);
    __ram_function_start__ = .;
    *(CodeQuickAccess)
    . = ALIGN(128);
    __ram_function_end__ = .;
  } > m_itcm AT> m_text

  __ram_function_size = SIZEOF(.ram_function);

  __NDATA_ROM = __ram_function_flash_start + SIZEOF(.ram_function);

  __DATA_END = __NDATA_ROM + (__noncachedata_init_end__ - __noncachedata_start__);
  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")

  /* Uninitialized data section */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(8);
    __START_BSS = .;
    __bss_start__ = .;
    *(m_usb_dma_noninit_data)
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(8);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_ocram_combined

  .heap :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    __HeapBase = .;
    . =  ORIGIN(m_ocram_combined) + LENGTH(m_ocram_combined) - STACK_SIZE;
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
  } > m_ocram_combined

  .stack :
  {
    . = ALIGN(8);
    . += STACK_SIZE;
  } > m_ocram_combined

  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(m_ocram_combined) + LENGTH(m_ocram_combined);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }

  ASSERT(__StackLimit >= __HeapLimit, "region m_ocram_combined overflowed with stack and heap")
}

