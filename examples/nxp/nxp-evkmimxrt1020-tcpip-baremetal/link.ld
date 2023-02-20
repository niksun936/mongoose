/*
 * Fixme
 * 
 * "Invalid flash header detected." (J-Link)
 * 
 */

ENTRY(ResetISR)

MEMORY {
    sram_itc(rx) : ORIGIN = 0x00000000, LENGTH = 64k       /* SRAM ITC */
    flash(rx)    : ORIGIN = 0x60000000, LENGTH = 8192k     /* On-board flash is 64 Mbits 8192 kB (0x1e00000) */
    rom (rx)     : ORIGIN = 0x20000000, LENGTH = 64k       /* ROM boot code */
    sram(rwx)    : ORIGIN = 0x20200000, LENGTH = 128k      /* OC RAM, (BSS, RW, STACK , etc...) domain D_ (0x1e00000) */
    sdram (rwx)  : ORIGIN = 0x80000000, LENGTH = 30720k    /* 30M bytes (0x1e00000) */  
}

SECTIONS { 
  .boot_hdr : ALIGN(4)                                     /* Image Vector Table and Boot Data (ext flash boot) */
  {
    FILL(0xff)
    __boot_hdr_start__ = ABSOLUTE(.) ;
    KEEP(*(.boot_hdr.conf))
    . = 0x1000 ;
    KEEP(*(.boot_hdr.ivt))
    . = 0x1020 ;
    KEEP(*(.boot_hdr.boot_data))
    . = 0x1030 ;
    KEEP(*(.boot_hdr.dcd_data))
    __boot_hdr_end__ = ABSOLUTE(.) ;
    . = 0x2000 ;
  } >BOARD_FLASH
  .vectors  : { KEEP(*(.isr_vector)) }  > flash
  .text     : { *(.text* .text.*) }     > flash
  .rodata   : { *(.rodata*) }           > flash

  .data : {
    _sdata = .;                                            /* for init_ram() */
    *(.first_data)
    *(.data SORT(.data.*))
    _edata = .;                                            /* for init_ram() */
  } > sram AT > flash
  _sidata = LOADADDR(.data);

  .bss : {
    _sbss = .;                                             /* for init_ram() */
    *(.bss SORT(.bss.*) COMMON)
    _ebss = .;
  } > sram

  . = ALIGN(8);
  _end = .;                                                /* for cmsis_gcc.h and init_ram() */
}

/*
 * References startup_mimxrt1021.c, 
 */

_estack     = ORIGIN(sram) + LENGTH(sram);                 /* stack points to end of SRAM */
_vStackBase = ORIGIN(sram);
_vStackTop = _estack;
__data_section_table = _sdata;
__data_section_table_end = _edata;
__bss_section_table_end = _sbss;
__section_table_end = _end;                                /* _ebss _end */
