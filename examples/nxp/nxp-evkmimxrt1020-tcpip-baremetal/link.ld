/*
 * Todo/Fixme
 * 
 *   Image made for internal flash will not work on MIMXRT1020-EVK.
 *   This NXP EVK flashes in QSPI external flash (Serial NOR) and
 *   requires a valid specific header ie at 0x6000_0000.
 * 
 * Allocations
 *
 *   Heap in OC RAM, size 0x20000, start
 *   stack in ITC RAM, size 0x4000, end
 * 
 * References
 * 
 *   Reference manual 9.6.3 9.6.3.1 9.6.3.2
 *   flash_header.txt 
 *   startup_mimxrt1021.c evkmimxrt1020_flexspi_nor_config.c/h fsl_flexspi_nor_boot.c/h
 * 
 *   Segger flash tool warning when invalid header:
 *   ("Invalid flash header detected.", J-Link).
 * 
 */

ENTRY(ResetISR)

MEMORY {
    sram_itc(rx)  : ORIGIN = 0x00000000, LENGTH = 64k      /* SRAM ITC */
    sram_dtc(rwx) : ORIGIN = 0x20000000, LENGTH = 64k      /* SRAM DTC */
    sram_oc(rwx)  : ORIGIN = 0x20200000, LENGTH = 128k     /* OC RAM (BSS, RW, STACK , etc...) */
    flash(rx)     : ORIGIN = 0x60000000, LENGTH = 8192k    /* On-board flash is 64 Mbits 8192 kB (0x1e00000) */
    sdram (rwx)   : ORIGIN = 0x80000000, LENGTH = 30720k   /* 30M bytes (0x1e00000) */
}

SECTIONS { 
  .boot_hdr : ALIGN(4)                                     /* Image Vector Table and Boot Data (ext flash boot) */
  {
    FILL(0xff)
    __boot_hdr_start__ = ABSOLUTE(.) ;
    KEEP(*(.flash_conf))  /* replacing .boot_hdr.conf */   /* Ref evkmimxrt1020_flexspi_nor_config.c/h */
    . = 0x1000 ;
    KEEP(*(.boot_hdr.ivt)) /* replacing .boot_hdr.ivt */  /* Ref fsl_flexspi_nor_boot.c/h */
    . = 0x1020 ;
    KEEP(*(.boot_hdr_boot_data)) /* replacing .boot_hdr.boot_data */  /* Ref fsl_flexspi_nor_boot.c/h */
    . = 0x1030 ;
    KEEP(*(.boot_hdr.dcd_data))
    __boot_hdr_end__ = ABSOLUTE(.) ;
    . = 0x2000 ;
  } >flash

/*.vectors  : { KEEP(*(.isr_vector)) }  > flash */
  .text     : { *(.text* .text.*) }     > flash
  .rodata   : { *(.rodata*) }           > flash

  .data : {
    _sdata = .;                                            /* for init_ram() */
    *(.first_data)
    *(.data SORT(.data.*))
    _edata = .;                                            /* for init_ram() */
  } > sram_oc AT > flash
  _sidata = LOADADDR(.data);

  .bss : {
    _sbss = .;                                             /* for init_ram() */
    *(.bss SORT(.bss.*) COMMON)
    _ebss = .;
  } > sram_oc

  . = ALIGN(8);
  _end = .;                                                /* for cmsis_gcc.h and init_ram() */
}

_estack     = ORIGIN(sram_oc) + LENGTH(sram_oc);           /* stack points to end of SRAM */
_vStackBase = ORIGIN(sram_oc);
_vStackTop = _estack;
__data_section_table = _sdata;
__data_section_table_end = _edata;
__bss_section_table_end = _sbss;
__section_table_end = _end;                                /* _ebss _end */
