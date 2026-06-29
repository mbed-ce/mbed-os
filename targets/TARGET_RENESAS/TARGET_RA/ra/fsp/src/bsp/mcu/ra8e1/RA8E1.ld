/* generated memory regions file - do not edit */
RAM_START = 0x22060000;
RAM_LENGTH = 0x00080000;
FLASH_START = 0x02000000;
FLASH_LENGTH = 0x00100000;
DATA_FLASH_START = 0x27000000;
DATA_FLASH_LENGTH = 0x00003000;
OSPI0_CS0_START = 0x80000000;
OSPI0_CS0_LENGTH = 0x10000000;
OSPI0_CS1_START = 0x90000000;
OSPI0_CS1_LENGTH = 0x10000000;
ITCM_START = 0x00000000;
ITCM_LENGTH = 0x00004000;
DTCM_START = 0x20000000;
DTCM_LENGTH = 0x00004000;
MEMORY
{
  RAM (rwx) : ORIGIN = RAM_START, LENGTH = RAM_LENGTH
  FLASH (rx) : ORIGIN = FLASH_START, LENGTH = FLASH_LENGTH
  DATA_FLASH (rx) : ORIGIN = DATA_FLASH_START, LENGTH = DATA_FLASH_LENGTH
  OSPI0_CS0 (rwx) : ORIGIN = OSPI0_CS0_START, LENGTH = OSPI0_CS0_LENGTH
  OSPI0_CS1 (rx) : ORIGIN = OSPI0_CS1_START, LENGTH = OSPI0_CS1_LENGTH
  ITCM (rwx) : ORIGIN = ITCM_START, LENGTH = ITCM_LENGTH
  DTCM (rwx) : ORIGIN = DTCM_START, LENGTH = DTCM_LENGTH
}

/* code entry point...need to define to keep crt0 _start out */
ENTRY(Reset_Handler)
/* Library configurations */
GROUP(libgcc.a libc.a libm.a)


SECTIONS
{
  /***** OSPI0_CS1 memory section allocations ******/
  .ospi0_cs1.startof  (READONLY) :
  {
       __ddsc_OSPI0_CS1_START = . ;

  }  > OSPI0_CS1
  /***** OSPI0_CS0 memory section allocations ******/
  .ospi0_cs0.startof  :
  {
       __ddsc_OSPI0_CS0_START = . ;

  }  > OSPI0_CS0
  /* ospi0_cs0 initialized from ospi0_cs1 */
  __ospi0_cs0_from_ospi0_cs1$$  :
  {
      __ospi0_cs0_from_ospi0_cs1$$Base = . ;__ospi0_cs0_from_ospi0_cs1$$Load = LOADADDR(__ospi0_cs0_from_ospi0_cs1$$) ;
  /* section.ospi0_cs0.from_ospi0_cs1 */
  *(.ospi0_cs0_from_ospi0_cs1)
  /* section.ospi0_cs0.code_from_ospi0_cs1 */
  *(.ospi0_cs0_code_from_ospi0_cs1)
      __ospi0_cs0_from_ospi0_cs1$$Limit = . ;
  }  > OSPI0_CS0 AT > OSPI0_CS1

  /***** ITCM memory section allocations ******/
  .itcm.startof  :
  {
       __ddsc_ITCM_START = . ;

  }  > ITCM
  /* itcm initialized from ospi0_cs1 */
  __itcm_from_ospi0_cs1$$  :
  {
      __itcm_from_ospi0_cs1$$Base = . ;__itcm_from_ospi0_cs1$$Load = LOADADDR(__itcm_from_ospi0_cs1$$) ;
  /* section.itcm.from_ospi0_cs1 */
  *(.itcm_from_ospi0_cs1)
  /* section.itcm.code_from_ospi0_cs1 */
  *(.itcm_code_from_ospi0_cs1)
      __itcm_from_ospi0_cs1$$Limit = . ;
  }  > ITCM AT > OSPI0_CS1

  /***** DTCM memory section allocations ******/
  .dtcm.startof  :
  {
       __ddsc_DTCM_START = . ;

  }  > DTCM
  /* dtcm initialized from ospi0_cs1 */
  __dtcm_from_ospi0_cs1$$  :
  {
      __dtcm_from_ospi0_cs1$$Base = . ;__dtcm_from_ospi0_cs1$$Load = LOADADDR(__dtcm_from_ospi0_cs1$$) ;
  /* section.dtcm.from_ospi0_cs1 */
  *(.dtcm_from_ospi0_cs1)
  /* section.dtcm.code_from_ospi0_cs1 */
  *(.dtcm_code_from_ospi0_cs1)
      __dtcm_from_ospi0_cs1$$Limit = . ;
  }  > DTCM AT > OSPI0_CS1

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
  /* ram initialized from ospi0_cs1 */
  __ram_from_ospi0_cs1$$  :
  {
      __ram_from_ospi0_cs1$$Base = . ;__ram_from_ospi0_cs1$$Load = LOADADDR(__ram_from_ospi0_cs1$$) ;
  /* section.ram.from_ospi0_cs1 */
  *(.ram_from_ospi0_cs1)
  /* section.ram.code_from_ospi0_cs1 */
  *(.ram_code_from_ospi0_cs1)
      __ram_from_ospi0_cs1$$Limit = . ;
  }  > RAM AT > OSPI0_CS1

  __ospi0_cs1_readonly$$  (READONLY) :
  {
      __ospi0_cs1_readonly$$Base = . ;
  /* section.ospi0_cs1.readonly */
  *(.ospi0_cs1)
  /* section.ospi0_cs1.code */
  *(.ospi0_cs1_code)
      __ospi0_cs1_readonly$$Limit = . ;
  }  > OSPI0_CS1
  __ospi0_cs1_noinit$$  (NOLOAD) :
  {
      __ospi0_cs1_noinit$$Base = . ;
  /* section.ospi0_cs1.noinit */
  *(.ospi0_cs1_noinit)
      __ospi0_cs1_noinit$$Limit = . ;
  }  > OSPI0_CS1
  .ospi0_cs1.endof ALIGN(.,512) (READONLY) :
  {
       __ddsc_OSPI0_CS1_END = . ;

  }  > OSPI0_CS1

  /***** DATA_FLASH memory section allocations ******/
  .data_flash.startof  (READONLY) :
  {
       __ddsc_DATA_FLASH_START = . ;

  }  > DATA_FLASH
  /***** OSPI0_CS0 memory section allocations ******/
  /* ospi0_cs0 initialized from data_flash */
  __ospi0_cs0_from_data_flash$$  :
  {
      __ospi0_cs0_from_data_flash$$Base = . ;__ospi0_cs0_from_data_flash$$Load = LOADADDR(__ospi0_cs0_from_data_flash$$) ;
  /* section.ospi0_cs0.from_data_flash */
  *(.ospi0_cs0_from_data_flash)
  /* section.ospi0_cs0.code_from_data_flash */
  *(.ospi0_cs0_code_from_data_flash)
      __ospi0_cs0_from_data_flash$$Limit = . ;
  }  > OSPI0_CS0 AT > DATA_FLASH

  /***** ITCM memory section allocations ******/
  /* itcm initialized from data_flash */
  __itcm_from_data_flash$$  :
  {
      __itcm_from_data_flash$$Base = . ;__itcm_from_data_flash$$Load = LOADADDR(__itcm_from_data_flash$$) ;
  /* section.itcm.from_data_flash */
  *(.itcm_from_data_flash)
  /* section.itcm.code_from_data_flash */
  *(.itcm_code_from_data_flash)
      __itcm_from_data_flash$$Limit = . ;
  }  > ITCM AT > DATA_FLASH

  /***** DTCM memory section allocations ******/
  /* dtcm initialized from data_flash */
  __dtcm_from_data_flash$$  :
  {
      __dtcm_from_data_flash$$Base = . ;__dtcm_from_data_flash$$Load = LOADADDR(__dtcm_from_data_flash$$) ;
  /* section.dtcm.from_data_flash */
  *(.dtcm_from_data_flash)
  /* section.dtcm.code_from_data_flash */
  *(.dtcm_code_from_data_flash)
      __dtcm_from_data_flash$$Limit = . ;
  }  > DTCM AT > DATA_FLASH

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
  /***** OSPI0_CS0 memory section allocations ******/
  /* ospi0_cs0 initialized from flash */
  __ospi0_cs0_from_flash$$  :
  {
      __ospi0_cs0_from_flash$$Base = . ;__ospi0_cs0_from_flash$$Load = LOADADDR(__ospi0_cs0_from_flash$$) ;
  /* section.ospi0_cs0.from_flash */
  *(.ospi0_cs0_from_flash)
  /* section.ospi0_cs0.code_from_flash */
  *(.ospi0_cs0_code_from_flash)
      __ospi0_cs0_from_flash$$Limit = . ;
  }  > OSPI0_CS0 AT > FLASH
  /* Non-initialized, non-cached ospi0_cs0 */
  __ospi0_cs0_noinit_nocache$$  (NOLOAD) : ALIGN(32)
  {
      __ospi0_cs0_noinit_nocache$$Base = . ;
  /* section.ospi0_cs0.noinit_nocache */
  *(.ospi0_cs0_noinit_nocache)
      __ospi0_cs0_noinit_nocache$$Limit = . ;
  }  > OSPI0_CS0
  /* Zeroed, non-cached ospi0_cs0 */
  __ospi0_cs0_zero_nocache$$  (NOLOAD) :
  {
      __ospi0_cs0_zero_nocache$$Base = . ;
  /* section.ospi0_cs0.zero_nocache */
  *(.ospi0_cs0_nocache)
    . = ALIGN(32);
      __ospi0_cs0_zero_nocache$$Limit = . ;
  }  > OSPI0_CS0
  /* Non-initialized ospi0_cs0 */
  __ospi0_cs0_noinit$$  (NOLOAD) :
  {
      __ospi0_cs0_noinit$$Base = . ;
  /* section.ospi0_cs0.noinit */
  *(.ospi0_cs0_noinit)
      __ospi0_cs0_noinit$$Limit = . ;
  }  > OSPI0_CS0
  /* Zeroed ospi0_cs0 */
  __ospi0_cs0_zero$$  (NOLOAD) :
  {
      __ospi0_cs0_zero$$Base = . ;
  /* section.ospi0_cs0.zero */
  *(.ospi0_cs0)
      __ospi0_cs0_zero$$Limit = . ;
  }  > OSPI0_CS0
  .ospi0_cs0.endof ALIGN(.,512) :
  {
       __ddsc_OSPI0_CS0_END = . ;

  }  > OSPI0_CS0

  /***** ITCM memory section allocations ******/
  /* itcm initialized from flash */
  __itcm_from_flash$$  :
  {
      __itcm_from_flash$$Base = . ;__itcm_from_flash$$Load = LOADADDR(__itcm_from_flash$$) ;
  /* section.itcm.from_flash */
  *(.itcm_from_flash)
  /* section.itcm.code_from_flash */
  *(.itcm_code_from_flash)
      __itcm_from_flash$$Limit = . ;
  }  > ITCM AT > FLASH
  /* Non-initialized itcm */
  __itcm_noinit$$  (NOLOAD) :
  {
      __itcm_noinit$$Base = . ;
  /* section.itcm.noinit */
  *(.itcm_noinit)
      __itcm_noinit$$Limit = . ;
  }  > ITCM
  /* Zeroed itcm */
  __itcm_zero$$  (NOLOAD) :
  {
      __itcm_zero$$Base = . ;
  /* section.itcm.zero */
  *(.itcm)
      __itcm_zero$$Limit = . ;
  }  > ITCM
  .itcm.endof ALIGN(.,512) :
  {
       __ddsc_ITCM_END = . ;

  }  > ITCM

  /***** DTCM memory section allocations ******/
  /* dtcm initialized from flash */
  __dtcm_from_flash$$  :
  {
      __dtcm_from_flash$$Base = . ;__dtcm_from_flash$$Load = LOADADDR(__dtcm_from_flash$$) ;
  /* section.dtcm.from_flash */
  *(.dtcm_from_flash)
  /* section.dtcm.code_from_flash */
  *(.dtcm_code_from_flash)
      __dtcm_from_flash$$Limit = . ;
  }  > DTCM AT > FLASH
  /* Non-initialized dtcm */
  __dtcm_noinit$$  (NOLOAD) :
  {
      __dtcm_noinit$$Base = . ;
  /* section.dtcm.noinit */
  *(.dtcm_noinit)
      __dtcm_noinit$$Limit = . ;
  }  > DTCM
  /* Zeroed dtcm */
  __dtcm_zero$$  (NOLOAD) :
  {
      __dtcm_zero$$Base = . ;
  /* section.dtcm.zero */
  *(.dtcm)
      __dtcm_zero$$Limit = . ;
  }  > DTCM
  .dtcm.endof ALIGN(.,512) :
  {
       __ddsc_DTCM_END = . ;

  }  > DTCM

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
  /* Non-initialized, non-cached ram */
  .bss  (NOLOAD) : ALIGN(32)
  {
      __ram_noinit_nocache$$Base = . ;
  /* section.ram.noinit_nocache */
  *(.ram_noinit_nocache)
      __ram_noinit_nocache$$Limit = . ;
  }  > RAM
  /* Zeroed, non-cached ram */
  __ram_zero_nocache$$  (NOLOAD) :
  {
      __ram_zero_nocache$$Base = . ;
  /* section.ram.zero_nocache */
  *(.ram_nocache)
    . = ALIGN(32);
      __ram_zero_nocache$$Limit = . ;
  }  > RAM
  /* Non-initialized ram */
  __ram_noinit$$  (NOLOAD) :
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
  }  > RAM
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
