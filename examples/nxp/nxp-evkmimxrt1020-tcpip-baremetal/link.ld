ENTRY(Reset_Handler);
MEMORY {
    flash(rx) : ORIGIN = 0x60000000, LENGTH = 8192k        /* On-board flash is 64 Mbits 8192 kB */
    sram(rwx) : ORIGIN = 0x20200000, LENGTH = 128k         /* OC RAM in domain D_ */
    sdram (rwx) : ORIGIN = 0x80000000, LENGTH = 0x1e00000  /* 30M bytes */  
}
_estack     = ORIGIN(sram) + LENGTH(sram);                 /* stack points to end of SRAM */

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
