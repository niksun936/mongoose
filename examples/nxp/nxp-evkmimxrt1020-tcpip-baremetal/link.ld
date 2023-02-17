ENTRY(Reset_Handler);
MEMORY {
  flash(rx) : ORIGIN = 0x08000000, LENGTH = 2048k
  sram(rwx) : ORIGIN = 0x24000000, LENGTH = 512k  /* AXI SRAM in domain D1 */
  /* 2.3.2: remaining SRAM is in other (non-contiguous) banks,
     DTCM @0x20000000 is in domain D1 and not accessible by the ETH DMA controller in domain D2
     SRAM @0x30000000 is in domain D2 and not directly available at startup to be used as stack (STM 8.5.9 page 366) */
}
_estack     = ORIGIN(sram) + LENGTH(sram);    /* stack points to end of SRAM */

/* Std sections */

SECTIONS {
  .vectors  : { KEEP(*(.isr_vector)) }  > flash
  .text     : { *(.text* .text.*) }     > flash
  .rodata   : { *(.rodata*) }           > flash

  .data : {
    _sdata = .;   /* for init_ram() */
    *(.first_data)
    *(.data SORT(.data.*))
    _edata = .;  /* for init_ram() */
  } > sram AT > flash
  _sidata = LOADADDR(.data);

  .bss : {
    _sbss = .;              /* for init_ram() */
    *(.bss SORT(.bss.*) COMMON)
    _ebss = .;              /* for init_ram() */
  } > sram

  . = ALIGN(8);
  _end = .;     /* for cmsis_gcc.h and init_ram() */
}

/* Satisfy startup_mimxrt1021.c */

ENTRY(ResetISR)
_vStackBase = ORIGIN(sram);
_vStackTop = _estack;
__data_section_table = _sdata;
__data_section_table_end = _edata;
__bss_section_table_end = _sbss;
__section_table_end = _ebss;
