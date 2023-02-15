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

#ifndef UART_DEBUG
#define UART_DEBUG 0 // Todo USART1
#endif

// Todo
typedef struct {
  uint16_t reserved;
} USART_TypeDef;

void set_user_led(int);
void toggle_user_led();
void peripheral_enet_init(void);
void clock_init(void);
void board_init(void);

static inline void uart_write_byte(USART_TypeDef *uart, uint8_t byte);
static inline void uart_write_buf(USART_TypeDef *uart, char *buf, size_t len);
static inline int uart_read_ready(USART_TypeDef *uart);
static inline uint8_t uart_read_byte(USART_TypeDef *uart);
static inline void uart_init(USART_TypeDef *uart, unsigned long baud);
