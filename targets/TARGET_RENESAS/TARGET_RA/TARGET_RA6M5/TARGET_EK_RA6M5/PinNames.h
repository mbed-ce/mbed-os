/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

/* R7FA6M5BH3CFC, LQFP176. Ports P0..P9 plus port A (P10) and port B (P11). */
typedef enum {
    P0_0 = 0x0000, P0_1, P0_2, P0_3, P0_4, P0_5, P0_6, P0_7, P0_8, P0_9, P0_10, P0_11, P0_12, P0_13, P0_14, P0_15,
    P1_0 = 0x0100, P1_1, P1_2, P1_3, P1_4, P1_5, P1_6, P1_7, P1_8, P1_9, P1_10, P1_11, P1_12, P1_13, P1_14, P1_15,
    P2_0 = 0x0200, P2_1, P2_2, P2_3, P2_4, P2_5, P2_6, P2_7, P2_8, P2_9, P2_10, P2_11, P2_12, P2_13, P2_14, P2_15,
    P3_0 = 0x0300, P3_1, P3_2, P3_3, P3_4, P3_5, P3_6, P3_7, P3_8, P3_9, P3_10, P3_11, P3_12, P3_13, P3_14, P3_15,
    P4_0 = 0x0400, P4_1, P4_2, P4_3, P4_4, P4_5, P4_6, P4_7, P4_8, P4_9, P4_10, P4_11, P4_12, P4_13, P4_14, P4_15,
    P5_0 = 0x0500, P5_1, P5_2, P5_3, P5_4, P5_5, P5_6, P5_7, P5_8, P5_9, P5_10, P5_11, P5_12, P5_13, P5_14, P5_15,
    P6_0 = 0x0600, P6_1, P6_2, P6_3, P6_4, P6_5, P6_6, P6_7, P6_8, P6_9, P6_10, P6_11, P6_12, P6_13, P6_14, P6_15,
    P7_0 = 0x0700, P7_1, P7_2, P7_3, P7_4, P7_5, P7_6, P7_7, P7_8, P7_9, P7_10, P7_11, P7_12, P7_13, P7_14, P7_15,
    P8_0 = 0x0800, P8_1, P8_2, P8_3, P8_4, P8_5, P8_6, P8_7, P8_8, P8_9, P8_10, P8_11, P8_12, P8_13, P8_14, P8_15,
    P9_0 = 0x0900, P9_1, P9_2, P9_3, P9_4, P9_5, P9_6, P9_7, P9_8, P9_9, P9_10, P9_11, P9_12, P9_13, P9_14, P9_15,
    P10_0 = 0x0A00, P10_1, P10_8, P10_9, P10_10, /* Renesas port A */
    P11_0 = 0x0B00, P11_1,                       /* Renesas port B */

    // mbed Pin Names

    /* User LEDs. LED1 red, LED2 green, LED3 blue (all active low). */
#define LED1 P0_6
#define LED2 P0_7
#define LED3 P0_8

    /* User push buttons (SW1/SW2, active low). */
#define BUTTON1 P0_5
#define BUTTON2 P0_4
#define SW1 P0_5
#define SW2 P0_4

    /* Default serial console pins (Arduino/mikroBUS TX/RX on J1, SCI7).
     * Note: the on-board J-Link OB virtual COM port is NOT wired to the
     * RA6M5, so the default console for this target is SEGGER RTT. These
     * pins are only used if target.console-uart is enabled. */
    CONSOLE_TX = P6_13,
    CONSOLE_RX = P6_14,

 // QSPI Flash (MX25L25645G, 32MB)
    QSPI_FLASH_IO0 = P3_7,
    QSPI_FLASH_IO1 = P3_8,
    QSPI_FLASH_IO2 = P3_9,
    QSPI_FLASH_IO3 = P3_10,
    QSPI_FLASH_SCLK = P3_5,
    QSPI_FLASH_SSEL = P3_6,

 // OctalFlash/PSRAM (AP Memory AP6404L-6SIR, Octal SPI, not enabled in this port yet)
    OSPI_FLASH_IO0 = P1_6,
    OSPI_FLASH_IO1 = P1_2,
    OSPI_FLASH_IO2 = P6_1,
    OSPI_FLASH_IO3 = P1_7,
    OSPI_FLASH_IO4 = P6_0,
    OSPI_FLASH_IO5 = P1_5,
    OSPI_FLASH_IO6 = P1_3,
    OSPI_FLASH_IO7 = P1_1,
    OSPI_FLASH_DQS  = P1_4,
    OSPI_FLASH_SCLK = P1_0,
    OSPI_FLASH_SSEL = P6_2,
    OSPI_FLASH_RESET = P6_15,

 // Ethernet PHY (KSZ8091RNB, RMII, not enabled in this port yet)
    ETH_MDC   = P4_1,
    ETH_MDIO  = P4_2,
    ETH_RESET = P4_3,
    ETH_TXEN  = P4_5,
    ETH_TXD1  = P4_6,
    ETH_TXD0  = P7_0,
    ETH_RXD0  = P7_2,
    ETH_RXD1  = P7_3,
    ETH_RXER  = P7_4,
    ETH_CRSDV = P7_5,
    ETH_50REF = P7_1,
    ETH_IRQ   = P7_6,

 // CAN FD (CAN0 via transceiver, not enabled in this port yet)
    CAN_TXD  = P6_9,
    CAN_RXD  = P6_10,
    CAN_STBY = P6_11,

 // USB Full Speed (USBFS)
    USBFS_VBUS      = P4_7,
    USBFS_VBUS_EN   = P5_0,
    USBFS_OVERCURA  = P5_1,

 // USB High Speed (USBHS)
    USBHS_VBUS      = P11_1,
    USBHS_VBUS_EN   = P11_0,
    USBHS_OVERCURA  = P7_7,

 // Arduino Uno R3 compatible connector (J18/J19/J23/J24); signals are shared
 // with the mikroBUS socket (J21/J22). Mapping per EK-RA6M5 User's Manual
 // R20UT4829EG0101, Table 15. These names are required by the ARDUINO_UNO
 // form factor support (see hal/PinNameAliases.h): D0..D13 map to the digital
 // header, D14/D15 to the dedicated SDA/SCL pins, A0..A5 to the analog header.
    ARDUINO_UNO_D0  = P6_14,   /* RX   (RXD7)          */
    ARDUINO_UNO_D1  = P6_13,   /* TX   (TXD7)          */
    ARDUINO_UNO_D2  = P4_9,    /* INT0 (IRQ6)          */
    ARDUINO_UNO_D3  = P1_11,   /* INT1, PWM (GTIOC3A)  */
    ARDUINO_UNO_D4  = P1_12,
    ARDUINO_UNO_D5  = P1_13,   /* PWM (GTIOC2A)        */
    ARDUINO_UNO_D6  = P1_14,   /* PWM (GTIOC2B)        */
    ARDUINO_UNO_D7  = P6_8,
    ARDUINO_UNO_D8  = P2_7,
    ARDUINO_UNO_D9  = P1_15,   /* PWM (GTIOC4A)        */
    ARDUINO_UNO_D10 = P2_5,    /* SPI SS, PWM (GTIOC4A)   */
    ARDUINO_UNO_D11 = P2_3,    /* SPI MOSI, PWM (GTIOC5A) */
    ARDUINO_UNO_D12 = P2_2,    /* SPI MISO (MISOA)     */
    ARDUINO_UNO_D13 = P2_4,    /* SPI SCK  (RSPCKA)    */
    ARDUINO_UNO_D14 = P5_11,   /* I2C SDA (SDA1)       */
    ARDUINO_UNO_D15 = P5_12,   /* I2C SCL (SCL1)       */
    ARDUINO_UNO_A0  = P0_0,    /* AN000 */
    ARDUINO_UNO_A1  = P0_1,    /* AN001 */
    ARDUINO_UNO_A2  = P0_2,    /* AN002 */
    ARDUINO_UNO_A3  = P0_3,    /* AN003 */
    ARDUINO_UNO_A4  = P0_14,   /* AN012 */
    ARDUINO_UNO_A5  = P0_15,   /* AN013 */

    /* Aliases */
    ARDUINO_D0  = P6_14,
    ARDUINO_D1  = P6_13,
    ARDUINO_D2  = P4_9,
    ARDUINO_A0  = P0_0,
    ARDUINO_A1  = P0_1,
    ARDUINO_A2  = P0_2,
    ARDUINO_A3  = P0_3,
    ARDUINO_A4  = P0_14,
    ARDUINO_A5  = P0_15,
    ARDUINO_D3  = P1_11,    /* PWM (GTIOC3A) */
    ARDUINO_D4  = P1_12,
    ARDUINO_D5  = P1_13,    /* PWM (GTIOC2A) */
    ARDUINO_D6  = P1_14,    /* PWM (GTIOC2B) */
    ARDUINO_D7  = P6_8,
    ARDUINO_D8  = P2_7,
    ARDUINO_D9  = P1_15,    /* PWM (GTIOC4A) */
    ARDUINO_D10 = P2_5,     /* PWM (GTIOC4A), SPI SS   */
    ARDUINO_D11 = P2_3,     /* PWM (GTIOC5A), SPI MOSI */
    ARDUINO_D12 = P2_2,     /* SPI MISO */
    ARDUINO_D13 = P2_4,     /* SPI SCK  */
    ARDUINO_INT0 = P4_9,
    ARDUINO_RESET = P3_3,
    ARDUINO_SDA = P5_11,
    ARDUINO_SCL = P5_12,
    ARDUINO_SS  = P2_5,
    ARDUINO_MOSI = P2_3,
    ARDUINO_MISO = P2_2,
    ARDUINO_SCK  = P2_4,
    ARDUINO_TX  = P6_13,
    ARDUINO_RX  = P6_14,

 // PMOD1 header (SPI)
    PMOD1_MISO = P2_2,
    PMOD1_MOSI = P2_3,
    PMOD1_SCK  = P2_4,
    PMOD1_SS   = P2_6,
    PMOD1_SS2  = P3_1,
    PMOD1_SS3  = P3_2,
    PMOD1_RESET = P3_11,
    PMOD1_INT  = P9_5,

 // PMOD2 header (SPI)
    PMOD2_MISO = P4_10,
    PMOD2_MOSI = P4_11,
    PMOD2_SCK  = P4_12,
    PMOD2_SS   = P4_13,
    PMOD2_SS2  = P4_8,
    PMOD2_SS3  = P7_8,
    PMOD2_RESET = P4_4,
    PMOD2_INT  = P4_0,

 // Grove1 / Qwiic connector (I2C)
    GROVE1_SDA = P4_14,
    GROVE1_SCL = P4_15,

 // Grove2 connector (I2C)
    GROVE2_SDA = P5_6,
    GROVE2_SCL = P5_5,

/* QSPI memory-mapped window. The window location and size are properties of
 * the board (controller channel + fitted flash device), so boards override
 * these in their PinNames.h; the defaults below describe the legacy RA QSPI
 * peripheral (one 64MB bank at a time, see R_QSPI BankSet). */
#define QSPI_XIP_WINDOW_BASE    0x60000000UL
#define QSPI_XIP_WINDOW_SIZE    0x04000000UL

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

typedef enum {
    PullUp = 0,
    PullDown = 3,
    PullNone = 2,
    OpenDrain = 4,
    PullDefault = PullNone
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
