

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "hal.h"

#define ASSERT(expr)                                            \
  do {                                                          \
    if (!(expr)) {                                              \
      PRINTF("FAILURE %s:%d: %s\n", __FILE__, __LINE__, #expr); \
      abort();                                                  \
    }                                                           \
  } while (0)

#define BIT(x) (1UL << (x))
#define SETBITS(R, CLEARMASK, SETMASK) (R) = ((R) & ~(CLEARMASK)) | (SETMASK)

void set_user_led(int i) {
	if (!i)
		GPIO_PinWrite(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, 1U); // Led OFF
	else
		GPIO_PinWrite(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, 0U); // Led ON
}

void toggle_user_led()
{
	static bool led_is_on = 0;
	if (!led_is_on) {
		led_is_on = 1;
		set_user_led(1);
	} else {
		led_is_on = 0;
		set_user_led(0);
	}
}

/*
 * Initialyzer setup the followings:
 *  - Initialyze board IO peripherals: standard boot/init
 *  - Peripheral ENET init: Setup all related ENET/PHY (IO Muxes, clocks, ...)
 */

void board_init(void) {
    // Hardware Initialization.
    const gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    // MPU and peripheral setup
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    clock_init();

    // User LED
    GPIO_PinInit(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, &gpio_config);
    GPIO_PinWrite(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, 1U);

    ASSERT(!SysTick_Config(SystemCoreClock / 1000U)); // 1ms
}

/*
 * Default start 24 MHz, might be altered by the build configuration.
 * Clock/enet setup only before clocks and gates are setup.
 * No enet peripheral access or DMA enable before clocks/gates are configured.
 */

void clock_init(void)
{
	// PHY: clk 50 MHz
    const clock_enet_pll_config_t config = {.enableClkOutput = true, .enableClkOutput500M = true, .loopDivider = 1};
    CLOCK_InitEnetPll(&config);
    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);
}

// for Clocks, IOs and ENET
void peripheral_enet_init(void) {
	const gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    // GPIO Setup
    GPIO_PinInit(GPIO1, 4, &gpio_config);  // PHY: #RST
    GPIO_PinInit(GPIO1, 22, &gpio_config); // PHY: INTRP
    // Pull up the ENET_INT before RESET.
    GPIO_PinWrite(GPIO1, 22, 1); // PHY: interrupt
    GPIO_PinWrite(GPIO1, 4, 0);  // PHY: reset
    SDK_DelayAtLeastUs(1000000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_PinWrite(GPIO1, 4, 1);  // PHY: active

	static mdio_handle_t mdioHandle = {.ops = &enet_ops};
	static phy_handle_t phyHandle   = {.phyAddr = 0, .mdioHandle = &mdioHandle, .ops = &phyksz8081_ops};

	phy_config_t phyConfig = {0};
    phyConfig.phyAddr               = 0x02U; // PHY address
    phyConfig.autoNeg               = true;
    mdioHandle.resource.base        = ENET;
    mdioHandle.resource.csrClock_Hz = CLOCK_GetFreq(kCLOCK_IpgClk);

	PHY_Init(&phyHandle, &phyConfig);
	CLOCK_EnableClock(kCLOCK_Enet);
}

static inline void uart_init(USART_TypeDef *uart, unsigned long baud) {
  // Todo
}
static inline void uart_write_byte(USART_TypeDef *uart, uint8_t byte) {
  // Todo
}
static inline void uart_write_buf(USART_TypeDef *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}
static inline int uart_read_ready(USART_TypeDef *uart) {
  // Todo
  return 0;
}
static inline uint8_t uart_read_byte(USART_TypeDef *uart) {
  // Todo
  return (uint8_t) 0;
}
