/*
** ###################################################################
**     Processors:          LPC54628J512
**
**     Compiler:            GNU C Compiler
**     Reference manual:    LPC54628 Series Reference Manual, Rev. 0 , 06/2017
**     Version:             rev. 1.0, 2017-6-06
**     Build:               b161214
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2017 NXP
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     1. Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     2. Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     3. Neither the name of the copyright holder nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
** ###################################################################
*/

/* Entry Point */
ENTRY(Reset_Handler)

__ram_vector_table__ = 1;

#if !defined(MBED_CONF_TARGET_BOOT_STACK_SIZE)
    #define MBED_CONF_TARGET_BOOT_STACK_SIZE 0x400
#endif

__stack_size__ = MBED_CONF_TARGET_BOOT_STACK_SIZE;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0800;
M_VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x400 : 0x0;
M_CRASH_DATA_RAM_SIZE = 0x100;

/* Specify the ELF segments (program headers) */
PHDRS
{
  text PT_LOAD FLAGS(5); /* read + execute */
  ram_vector_table PT_LOAD FLAGS(6); /* read + write */
  ram_noinit PT_LOAD FLAGS(6); /* read + write */
  ram_init PT_LOAD FLAGS(6); /* read + write */
  sram_x PT_LOAD FLAGS(6); /* read + write */
  sram_usb PT_LOAD FLAGS(6); /* read + write */
}

/* Specify the memory areas */
MEMORY
{
  m_text                (RX)  : ORIGIN = MBED_CONFIGURED_ROM_BANK_PROGRAM_FLASH_START, LENGTH = MBED_CONFIGURED_ROM_BANK_PROGRAM_FLASH_SIZE
  m_data                (RW)  : ORIGIN = MBED_RAM_BANK_SRAM_UPPER_START, LENGTH = MBED_RAM_BANK_SRAM_UPPER_SIZE
  m_sramx               (RW)  : ORIGIN = MBED_RAM_BANK_SRAMX_START, LENGTH = MBED_RAM_BANK_SRAMX_SIZE
  m_usb_sram            (RW)  : ORIGIN = MBED_RAM_BANK_USB_RAM_START, LENGTH = MBED_RAM_BANK_USB_RAM_SIZE
}

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into internal flash */
  .interrupts :
  {
    __VECTOR_TABLE = .;
    KEEP(*(.isr_vector))     /* Startup code */
  } > m_text :text

  /* The program code and other data goes into internal flash */
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
  } > m_text :text

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_text :text

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_text :text

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
  } > m_text :text

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_text :text

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_text :text

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_text :text

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_text :text

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
  } > m_data :ram_vector_table

  __VECTOR_RAM = DEFINED(__ram_vector_table__) ? __VECTOR_RAM__ : ORIGIN(m_interrupts);
  __RAM_VECTOR_TABLE_SIZE_BYTES = DEFINED(__ram_vector_table__) ? (__interrupts_ram_end__ - __interrupts_ram_start__) : 0x0;

  .data : AT(__DATA_ROM)
  {
    . = ALIGN(8);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(.ramfunc*)             /* for functions in ram */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(8);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data :ram_init

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);
  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")

  /* Uninitialized data section */
  .bss (NOLOAD):
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(8);
    __START_BSS = .;
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(8);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_data :ram_noinit

  ASSERT(__bss_end__ <= ORIGIN(m_data) + LENGTH(m_data) - STACK_SIZE, "Size of globals + stack exceeds size of combined SRAM!")

  .heap (NOLOAD):
  {
    . = ALIGN(8);
    __end__ = .;
    __mbed_sbrk_start = .;
    . = ORIGIN(m_data) + LENGTH(m_data) - STACK_SIZE;
    __mbed_krbs_start = .;
  } > m_data :ram_noinit

  .stack :
  {
    . = ALIGN(8);
    . += STACK_SIZE;
  } > m_data :ram_noinit

  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(m_data) + LENGTH(m_data);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  m_usb_bdt (NOLOAD) :
  {
    . = ALIGN(512);
    *(m_usb_bdt)
  } > m_usb_sram :sram_usb

  m_usb_global (NOLOAD) :
  {
    *(m_usb_global)
  } > m_usb_sram :sram_usb

#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
  /* Stick the crash data ram at the start of SRAMX */
  .crash_data_ram (NOLOAD): ALIGN(8)
  {
    __CRASH_DATA_RAM__ = .;
    __CRASH_DATA_RAM_START__ = .; /* Create a global symbol at data start */
    KEEP(*(.keep.crash_data_ram))
    *(.m_crash_data_ram)     /* This is a user defined section */
    . += M_CRASH_DATA_RAM_SIZE;
    . = ALIGN(8);
    __CRASH_DATA_RAM_END__ = .; /* Define a global symbol at data end */
  } > m_sramx :sram_x
#endif

  /* Use SRAMX for additional heap */
  .heap_0 (NOLOAD):
  {
    . = ALIGN(8);
    __mbed_sbrk_start_0 = .;
    . = ORIGIN(m_sramx) + LENGTH(m_sramx);
    __mbed_krbs_start_0 = .;
  } > m_sramx :sram_x

  .ARM.attributes 0 : { *(.ARM.attributes) }
}

