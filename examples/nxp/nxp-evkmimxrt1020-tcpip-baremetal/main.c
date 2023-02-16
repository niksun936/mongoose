// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

/*
 * Application example: dashboard
 *  - Initialyze peripherals and clocks
 *  - Initialyze Mongoose
 *  - Start web server
 */

#include "hal.h"
#include "mongoose.h"
#include "web_server.h"

#define MG_USING_DHCP 1

#define ASSERT(expr)                                            \
  do {                                                          \
    if (!(expr)) {                                              \
      PRINTF("FAILURE %s:%d: %s\n", __FILE__, __LINE__, #expr); \
      abort();                                                  \
    }                                                           \
  } while (0)

void mg_log_set_fn(mg_pfn_t, void *);
void log_fn(char ch, void *param) {
	PUTCHAR(ch);
}

volatile uint32_t g_systick_timer;	 // System tick counter
volatile static uint64_t s_ticks;  	 // Counters, increased by IRQ handlers

void SysTick_Handler(void)
{
	s_ticks++;
	if (g_systick_timer != 0U) 		 // For busy loop timer
		g_systick_timer--;
}

void SysTick_delay_ms(uint32_t dt)
{
	g_systick_timer = dt;
	while (g_systick_timer != 0U) {} // SysTick_Handler() is called in interrupt context
}

uint64_t mg_millis(void) {  		 // Declare our own uptime function
  return s_ticks;           		 // Return number of milliseconds since boot
}

extern int mg_led_state;         // Only to display errors

// Shows UART/Debug port message + LED blinks on boot.
void startup_display() {
    char *version = "1.0";

    PRINTF("\r\nNXP i.MX RT1020-EVK: Mongoose Running over MIP\n\r");
    PRINTF("Version: %s\n\r", version);
    PRINTF("Starting ");
	// Blink led on startup a couple times
	int blink_count = 5;
	while (blink_count-- > 0) {
		SysTick_delay_ms(200U); // ms
		toggle_user_led();
		PRINTF(".");
	}
	PRINTF("\n\r");
}

int main(void)
{
	// System and peripherals init
    board_init();
    peripheral_enet_init();

    // Mongoose init
    startup_display();
    mg_log_set_fn(log_fn, 0); // Use custom log function

	// Events
	struct mg_mgr mgr;  // Event manager
	mg_mgr_init(&mgr);  // Initialise event manager
	mg_log_set(MG_LL_DEBUG);  // Set log level
	mg_timer_add(&mgr, 100, MG_TIMER_REPEAT, led_blink_cb, 0); // Blinks a LED.

	struct mg_tcpip_driver_imxrt1020_data driver_data; // See driver_imx_rt1020.h
	struct mg_tcpip_if mif = {
			.mac = {0x2, 0x4, 0x6, 0x2, 0x3, 0x78},
			.driver = &mg_tcpip_driver_imxrt1020,
			.driver_data = &driver_data,
	};

	// Note: sscanf() may not support MAC address hex conversion from char*

#if MG_USING_DHCP == 1
	mif.enable_dhcp_client = true;  // DHCP
#else
	mif.enable_dhcp_client = false;   // Static IP
	mif.ip = 0x0203000a;    // 10.0.3.32
	mif.mask = 0x0000ffff;  // 255.255.0.0
	mif.gw = 0x0103000a;    // 10.0.3.1
#endif

	mg_tcpip_init(&mgr, &mif);  		// Initialize MIP driver
	NVIC_EnableIRQ(ENET_IRQn);  		// Enable ENET IRQ

	// Stack initialization, Network configuration (DHCP lease, ...)
	{
#if MG_USING_DHCP == 0
		MG_INFO(("MIF configuration: Static IP"));
		ASSERT(mif.ip != 0);     // Check we have a satic IP assigned
		mg_mgr_poll(&mgr, 100);  // For initialisation
#else
		MG_INFO(("MIF configuration: DHCP"));
		ASSERT(!mif.ip);  // Check we are set for DHCP
		int pc = 500; // Timout on DHCP lease 500 ~ approx 5s (typical delay <1s)
		while (((pc--) > 0) && !mif.ip) {
		  mg_mgr_poll(&mgr, 100);
		  SysTick_delay_ms(10U); // Delay ms
		}
		if (!mif.ip) {
			MG_ERROR(("No ip assigned (DHCP lease may have failed).\n"));
			mg_led_state = MG_LED_ERROR;
		}
		// if (!mif.ip) loop_error_led_blink();
		// ASSERT(mif.ip);  // We have an IP (lease or static)
#endif
	}
	// Run Mongoose server (infinite loop)
	mg_run_server(&mgr, &mif, &mg_led_state);

	mg_mgr_free(&mgr);        // Clear
	mg_tcpip_free(&mif);      // Release after mg_mgr

	PRINTF("Server has quit.\r\n");
    while(1);
}
