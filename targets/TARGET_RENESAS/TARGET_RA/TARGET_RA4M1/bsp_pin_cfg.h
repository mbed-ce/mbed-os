/* generated configuration header file - do not edit */
#ifndef BSP_PIN_CFG_H_
#define BSP_PIN_CFG_H_
#include "r_ioport.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

#define ARDUINO_A0 (BSP_IO_PORT_00_PIN_00)
#define ARDUINO_A1 (BSP_IO_PORT_00_PIN_01) /* Enable when connected */
#define ARDUINO_A2 (BSP_IO_PORT_00_PIN_02) /* Reference E42 / E40 */
#define ARDUINO_A3 (BSP_IO_PORT_00_PIN_03) /* Enable when connected */
#define ARDUINO_A4 (BSP_IO_PORT_00_PIN_04) /* Enable when connected */
#define ARDUINO_A5 (BSP_IO_PORT_00_PIN_13) /* Enable I3C_SEL_L to use */
#define PMOD1_RESET (BSP_IO_PORT_00_PIN_14) /* Enable when connected */
#define PMOD1_INT (BSP_IO_PORT_00_PIN_15) /* Enable when connected */
#define ARDUINO_D12_MISO_PMOD1_MISO (BSP_IO_PORT_01_PIN_00) /* See SW4 in manual */
#define ARDUINO_D11_MOSI_PMOD1_MOSI (BSP_IO_PORT_01_PIN_01) /* See SW4 in manual */
#define ARDUINO_D13_CLK_PMOD1_CLK (BSP_IO_PORT_01_PIN_02) /* See SW4 in manual */
#define ARDUINO_D10_SS_PMOD1_SS (BSP_IO_PORT_01_PIN_03) /* See SW4 in manual */
#define ARDUINO_D3 (BSP_IO_PORT_01_PIN_04) /* See SW4 in manual */
#define PMOD1_GPIO_SSLA2 (BSP_IO_PORT_01_PIN_05) /* See SW4 in manual */
#define PMOD1_GPIO_SSLA3 (BSP_IO_PORT_01_PIN_06) /* See SW4 in manual */
#define ARDUINO_D4 (BSP_IO_PORT_01_PIN_07)
#define TMS (BSP_IO_PORT_01_PIN_08) /* Enable when connected */
#define TDO (BSP_IO_PORT_01_PIN_09) /* Enable I3C_SEL_L to use */
#define TDI (BSP_IO_PORT_01_PIN_10) /* Enable when connected */
#define PMOD2_CLK9 (BSP_IO_PORT_01_PIN_11) /* See SW4 in manual */
#define PMOD2_RESET (BSP_IO_PORT_01_PIN_12)
#define ARDUINO_D7 (BSP_IO_PORT_01_PIN_13)
#define NMI (BSP_IO_PORT_02_PIN_00)
#define MD (BSP_IO_PORT_02_PIN_01) /* See Manual */
#define SW1 (BSP_IO_PORT_02_PIN_05)
#define ETHERNET_RXCTL (BSP_IO_PORT_02_PIN_06) /* Reference E38 in manual */
#define EXTAL (BSP_IO_PORT_02_PIN_12) /* Main clock */
#define XTAL (BSP_IO_PORT_02_PIN_13) /* Main clock */
#define TCK (BSP_IO_PORT_03_PIN_00)
#define PMOD2_CTS9 (BSP_IO_PORT_03_PIN_01)
#define ARDUINO_D9 (BSP_IO_PORT_03_PIN_02)
#define PMOD2_GPIO3 (BSP_IO_PORT_03_PIN_03)
#define PMOD2_IRQ9 (BSP_IO_PORT_03_PIN_04) /* Reference SW4 /  E44 / E18 in manual */
#define ARDUINO_SCL (BSP_IO_PORT_04_PIN_00)
#define ARDUINO_SDA (BSP_IO_PORT_04_PIN_01)
#define ARDUINO_D8 (BSP_IO_PORT_04_PIN_02) /* Enable when connected */
#define LED2 (BSP_IO_PORT_04_PIN_07)
#define LED1 (BSP_IO_PORT_04_PIN_08)
#define ARDUINO_D2 (BSP_IO_PORT_04_PIN_09) /* Enable when connected */
#define PMOD2_GPIO2 (BSP_IO_PORT_04_PIN_10) /* Enable when connected */
#define MIPI_TE (BSP_IO_PORT_04_PIN_11)
#define ARDUINO_D6 (BSP_IO_PORT_05_PIN_00)

extern const ioport_cfg_t g_bsp_pin_cfg; /* FPB_RA4E1.pincfg */

void BSP_PinConfigSecurityInit();

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER
#endif /* BSP_PIN_CFG_H_ */
