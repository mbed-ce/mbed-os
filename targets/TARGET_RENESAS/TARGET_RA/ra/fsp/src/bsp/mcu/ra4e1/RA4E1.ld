/* generated memory regions file - do not edit */
RAM_START = 0x20000000;
RAM_LENGTH = 0x00020000;
FLASH_START = 0x00000000;
FLASH_LENGTH = 0x00080000;
DATA_FLASH_START = 0x08000000;
DATA_FLASH_LENGTH = 0x00002000;
QSPI_FLASH_START = 0x60000000;
QSPI_FLASH_LENGTH = 0x04000000;

MEMORY
{
  RAM (rwx) : ORIGIN = RAM_START, LENGTH = RAM_LENGTH
  FLASH (rx) : ORIGIN = FLASH_START, LENGTH = FLASH_LENGTH
  DATA_FLASH (rx) : ORIGIN = DATA_FLASH_START, LENGTH = DATA_FLASH_LENGTH
  QSPI_FLASH (rx) : ORIGIN = QSPI_FLASH_START, LENGTH = QSPI_FLASH_LENGTH
}

/* code entry point...need to define to keep crt0 _start out */
ENTRY(Reset_Handler)
/* Library configurations */
GROUP(libgcc.a libc.a libm.a)


SECTIONS
{
  /***** QSPI_FLASH memory section allocations ******/
  .qspi_flash.startof  (READONLY) :
  {
       __ddsc_QSPI_FLASH_START = . ;

  }  > QSPI_FLASH
  /***** RAM memory section allocations ******/
  .ram.startof  :
  {
       __ddsc_RAM_START = . ;

  }  > RAM
  __ram_dtc_vector$$  (NOLOAD) :
  {
      __ram_dtc_vector$$Base = . ;
  *(.fsp_dtc_vector_table)
      __ram_dtc_vector$$Limit = . ;
  }  > RAM
  /* ram initialized from qspi_flash */
  __ram_from_qspi_flash$$  :
  {
      __ram_from_qspi_flash$$Base = . ;__ram_from_qspi_flash$$Load = LOADADDR(__ram_from_qspi_flash$$) ;
  /* section.ram.from_qspi_flash */
  *(.ram_from_qspi_flash)
  /* section.ram.code_from_qspi_flash */
  *(.ram_code_from_qspi_flash)
      __ram_from_qspi_flash$$Limit = . ;
  }  > RAM AT > QSPI_FLASH

  __qspi_flash_readonly$$  (READONLY) :
  {
      __qspi_flash_readonly$$Base = . ;
  /* section.qspi_flash.readonly */
  *(.qspi_flash)
  /* section.qspi_flash.code */
  *(.qspi_flash_code)
      __qspi_flash_readonly$$Limit = . ;
  }  > QSPI_FLASH
  __qspi_flash_noinit$$  (NOLOAD) :
  {
      __qspi_flash_noinit$$Base = . ;
  /* section.qspi_flash.noinit */
  *(.qspi_flash_noinit)
      __qspi_flash_noinit$$Limit = . ;
  }  > QSPI_FLASH
  .qspi_flash.endof ALIGN(.,512) (READONLY) :
  {
       __ddsc_QSPI_FLASH_END = . ;

  }  > QSPI_FLASH

  /***** DATA_FLASH memory section allocations ******/
  .data_flash.startof  (READONLY) :
  {
       __ddsc_DATA_FLASH_START = . ;

  }  > DATA_FLASH
  /***** RAM memory section allocations ******/
  /* ram initialized from data_flash */
  __ram_from_data_flash$$  :
  {
      __ram_from_data_flash$$Base = . ;__ram_from_data_flash$$Load = LOADADDR(__ram_from_data_flash$$) ;
  /* section.ram.from_data_flash */
  *(.ram_from_data_flash)
  /* section.ram.code_from_data_flash */
  *(.ram_code_from_data_flash)
      __ram_from_data_flash$$Limit = . ;
  }  > RAM AT > DATA_FLASH

  __data_flash_readonly$$  (READONLY) :
  {
      __data_flash_readonly$$Base = . ;
  /* section.data_flash.readonly */
  *(.data_flash)
  /* section.data_flash.code */
  *(.data_flash_code)
      __data_flash_readonly$$Limit = . ;
  }  > DATA_FLASH
  __data_flash_noinit$$  (NOLOAD) :
  {
      __data_flash_noinit$$Base = . ;
  /* section.data_flash.noinit */
  *(.data_flash_noinit)
      __data_flash_noinit$$Limit = . ;
  }  > DATA_FLASH
  .data_flash.endof ALIGN(.,512) (READONLY) :
  {
       __ddsc_DATA_FLASH_END = . ;

  }  > DATA_FLASH

  /***** FLASH memory section allocations ******/
  .flash.startof  (READONLY) :
  {
       __ddsc_FLASH_START = . ;

  }  > FLASH
  /* MCU vector table */
  .text  (READONLY) :
  {
      __flash_vectors$$Base = . ; _VECTORS = . ;
  KEEP(*(.fixed_vectors))
  KEEP(*(.application_vectors))
      __flash_vectors$$Limit = . ;
  }  > FLASH
  __flash_noinit$$  (NOLOAD) :
  {
      __flash_noinit$$Base = . ;
  /* section.flash.noinit */
  *(.flash_noinit)
      __flash_noinit$$Limit = . ;
  }  > FLASH
  /***** RAM memory section allocations ******/
  /* ram initialized from flash */
  .data  :
  {
      __ram_from_flash$$Base = . ;__ram_from_flash$$Load = LOADADDR(.data) ;
  /* section.ram.from_flash */
  *(.ram_from_flash)
  /* section.ram.code_from_flash */
  *(.ram_code_from_flash)
  *(.data*)
  *(vtable)
      __ram_from_flash$$Limit = . ;
  }  > RAM AT > FLASH
  /* Non-initialized ram */
  .bss  (NOLOAD) :
  {
    __ram_noinit$$Base = . ;
    /* Main stack region */
    __StackLimit = .;
    *(.bss.g_main_stack)
    __StackTop = .;
    /* Other no-init data */
    *(.ram_noinit)
    *(.noinit)
    __ram_noinit$$Limit = . ;
  } > RAM
  /* Zeroed ram */
  __ram_zero$$  (NOLOAD) :
  {
      __ram_zero$$Base = . ;
  /* section.ram.zero */
  *(.ram)
  *(.bss*)
      __ram_zero$$Limit = . ;
  }  > RAM
  /* Thread Stacks */
  __ram_thread_stack$$  (NOLOAD) : ALIGN(8)
  {
      __ram_thread_stack$$Base = . ;
  KEEP(*(.stack?*))
      __ram_thread_stack$$Limit = . ;
  }  > RAM
  .ram.endof ALIGN(.,512) :
  {
       __ddsc_RAM_END = . ;
        /* Heap region */
        __HeapBase = .;
        __end__ = .;
        *(.bss.g_heap)
        __HeapLimit = ORIGIN(RAM) + LENGTH(RAM);
  }  > RAM

  .text  (READONLY) :
  {
      __flash_readonly$$Base = . ;
  /* section.flash.readonly */
  *(.flash)
  /* section.flash.code */
  *(.flash_code)
  *(.text*)
  *(.rodata*)
  KEEP(*(.mcuboot_sce9_key))
  KEEP(*(.version))
      __flash_readonly$$Limit = . ;
  }  > FLASH
  __flash_ctor$$  (READONLY) :
  {

  *crtbegin.o(.ctors)
  *crtbegin?.o(.ctors)
  EXCLUDE_FILE (*crtend?.o *crtend.o) *(.ctors)
  *(SORT(.ctors.*))
  *(.ctors)
  *crtbegin.o(.dtors)
  *crtbegin?.o(.dtors)
  EXCLUDE_FILE (*crtend?.o *crtend.o) *(.dtors)
  *(SORT(.dtors.*))
  *(.dtors)

  }  > FLASH
  __flash_preinit_array$$  (READONLY) :
  {
       __preinit_array_start = . ;
  KEEP(*(.preinit_array))
       __preinit_array_end = . ;
  }  > FLASH
  __flash_.got$$  (READONLY) :
  {

  *(.got.plt)
  *(.got)

  }  > FLASH
  __flash_init_array$$  (READONLY) :
  {
       __init_array_start = . ;
  KEEP(*(SORT(.init_array.*)))
  KEEP(*(.init_array))
       __init_array_end = . ;
  }  > FLASH
  __flash_fini_array$$  (READONLY) :
  {
       __fini_array_start = . ;
  KEEP(*(SORT(.fini_array.*)))
  KEEP(*(.fini_array))
       __fini_array_end = . ;
  }  > FLASH
  /* .ARM.extab sections contain exception unwinding information. */
  __flash_arm.extab$$  (READONLY) :
  {

  *(.ARM.extab*)
  *(.gnu.linkonce.armextab.*)

  }  > FLASH
  /* .ARM.exidx sections contains index entries for section unwinding. */
  __flash_arm.exidx$$  (READONLY) :
  {
       __exidx_start = . ;
  *(.ARM.exidx*)
  *(.gnu.linkonce.armexidx.*)
       __exidx_end = . ;
  }  > FLASH
  .flash.endof ALIGN(.,512) (READONLY) :
  {
       __ddsc_FLASH_END = . ;

  }  > FLASH

}

