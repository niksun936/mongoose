// Copyright (c) 2022-2023 Cesanta Software Limited
// All rights reserved
//
// Reference manual: IMXRT1020RM, devboard manual: MIMXRT1020EVKHUG 
// https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/i-mx-rt-crossover-mcus/i-mx-rt1020-crossover-mcu-with-arm-cortex-m7-core:i.MX-RT1020

#pragma once

#include "MIMXRT1021.h"
#include "MIMXRT1021_features.h"
#include "board.h"
#include <stdlib.h>
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_enet.h"
#include "fsl_phy.h"
#include "fsl_silicon_id.h"

#include "fsl_enet_mdio.h"
#include "fsl_phyksz8081.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

void set_user_led(int);
void toggle_user_led();
void peripheral_enet_init(void);
void clock_init(void);
void board_init(void);

#ifndef UART_DEBUG
#define UART_DEBUG SDK_DEBUGCONSOLE
#endif
