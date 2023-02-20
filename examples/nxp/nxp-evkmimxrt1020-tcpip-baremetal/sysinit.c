// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved
//
// This file contains essentials required by the CMSIS:
// uint32_t SystemCoreClock - holds the system core clock value
// SystemInit() - initialises the system, e.g. sets up clocks

#include "hal.h"

// Prevent opt ou of Flash header
extern int __boot_hdr_dummy;
const int *boot_hdr = &__boot_hdr_dummy;

void SystemInit(void) {
  // Todo
}
