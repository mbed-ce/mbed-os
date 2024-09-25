/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
 * Copyright (c) 2019 Cypress Semiconductor Corporation
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

/* MBED TARGET LIST: CY8CPROTO_062_4343W, CY8CKIT_062S2_43012,
 * CY8CPROTO_062S3_4343W, CY8CKIT_062_WIFI_BT, CY8CKIT_062_BLE,
 * CYW9P62S1_43438EVB_01, CYW9P62S1_43012EVB_01, CY8CKIT064B0S2_4343W,
 * CYTFM_064B0S2_4343W, CYSBSYSKIT_01
 */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "PinNamesTypes.h"
#include "cyhal_pin_package.h"
#include "cybsp_types.h"

// I2C
#ifdef CYBSP_I2C_SCL
#define I2C_SCL CYBSP_I2C_SCL
#endif
#ifdef CYBSP_I2C_SDA
#define I2C_SDA CYBSP_I2C_SDA
#endif

// SPI
#ifdef CYBSP_SPI_MOSI
#define SPI_MOSI CYBSP_SPI_MOSI
#endif
#ifdef CYBSP_SPI_MISO
#define SPI_MISO CYBSP_SPI_MISO
#endif
#ifdef CYBSP_SPI_CLK
#define SPI_CLK CYBSP_SPI_CLK
#endif
#ifdef CYBSP_SPI_CS
#define SPI_CS CYBSP_SPI_CS
#endif

// UART
#ifdef CYBSP_DEBUG_UART_RX
#define UART_RX CYBSP_DEBUG_UART_RX
#if defined(MBED_CONF_TARGET_STDIO_UART_RX)
#define STDIO_UART_RX MBED_CONF_TARGET_STDIO_UART_RX
#else
#define STDIO_UART_RX CYBSP_DEBUG_UART_RX
#endif
#define CONSOLE_RX CYBSP_DEBUG_UART_RX
#endif
#ifdef CYBSP_DEBUG_UART_TX
#define UART_TX CYBSP_DEBUG_UART_TX
#if defined(MBED_CONF_TARGET_STDIO_UART_TX)
#define STDIO_UART_TX MBED_CONF_TARGET_STDIO_UART_TX
#else
#define STDIO_UART_TX CYBSP_DEBUG_UART_TX
#endif
#define CONSOLE_TX CYBSP_DEBUG_UART_TX
#endif
#ifdef CYBSP_DEBUG_UART_RTS
#define UART_RTS CYBSP_DEBUG_UART_RTS
#define STDIO_UART_RTS CYBSP_DEBUG_UART_RTS
#define USBRTS CYBSP_DEBUG_UART_RTS
#endif
#ifdef CYBSP_DEBUG_UART_CTS
#define UART_CTS CYBSP_DEBUG_UART_CTS
#define STDIO_UART_CTS CYBSP_DEBUG_UART_CTS
#define USBCTS CYBSP_DEBUG_UART_CTS
#endif

// LEDs
#ifdef CYBSP_USER_LED1
#define LED1 CYBSP_USER_LED1
#endif
#ifdef CYBSP_USER_LED2
#define LED2 CYBSP_USER_LED2
#endif
#ifdef CYBSP_USER_LED3
#define LED3 CYBSP_USER_LED3
#endif
#ifdef CYBSP_USER_LED4
#define LED4 CYBSP_USER_LED4
#endif
#ifdef CYBSP_USER_LED5
#define LED5 CYBSP_USER_LED5
#endif

#ifdef CYBSP_LED_RGB_RED
#define LED_RED CYBSP_LED_RGB_RED
#endif
#ifdef CYBSP_LED_RGB_BLUE
#define LED_BLUE CYBSP_LED_RGB_BLUE
#endif
#ifdef CYBSP_LED_RGB_GREEN
#define LED_GREEN CYBSP_LED_RGB_GREEN
#endif

// User button
#ifdef CYBSP_USER_BTN
#define USER_BUTTON CYBSP_USER_BTN
#define BUTTON1 CYBSP_USER_BTN
#endif

// QSPI
#ifdef CYBSP_QSPI_D0
#define QSPI_IO_0 CYBSP_QSPI_D0
#define QSPI_FLASH1_IO0 CYBSP_QSPI_D0
#endif
#ifdef CYBSP_QSPI_D1
#define QSPI_IO_1 CYBSP_QSPI_D1
#define QSPI_FLASH1_IO1 CYBSP_QSPI_D1
#endif
#ifdef CYBSP_QSPI_D2
#define QSPI_IO_2 CYBSP_QSPI_D2
#define QSPI_FLASH1_IO2 CYBSP_QSPI_D2
#endif
#ifdef CYBSP_QSPI_D3
#define QSPI_IO_3 CYBSP_QSPI_D3
#define QSPI_FLASH1_IO3 CYBSP_QSPI_D3
#endif
#ifdef CYBSP_QSPI_SCK
#define QSPI_CLK CYBSP_QSPI_SCK
#define QSPI_FLASH1_SCK CYBSP_QSPI_SCK
#endif
#ifdef CYBSP_QSPI_SS
#define QSPI_SEL CYBSP_QSPI_SS
#define QSPI_FLASH1_CSN CYBSP_QSPI_SS
#endif

// Arduino Header
#ifdef TARGET_FF_ARDUINO_UNO
#define ARDUINO_UNO_A0 CYBSP_A0
#define ARDUINO_UNO_A1 CYBSP_A1
#define ARDUINO_UNO_A2 CYBSP_A2
#define ARDUINO_UNO_A3 CYBSP_A3
#define ARDUINO_UNO_A4 CYBSP_A4
#define ARDUINO_UNO_A5 CYBSP_A5
#define ARDUINO_UNO_D0 CYBSP_D0
#define ARDUINO_UNO_D1 CYBSP_D1
#define ARDUINO_UNO_D2 CYBSP_D2
#define ARDUINO_UNO_D3 CYBSP_D3
#define ARDUINO_UNO_D4 CYBSP_D4
#define ARDUINO_UNO_D5 CYBSP_D5
#define ARDUINO_UNO_D6 CYBSP_D6
#define ARDUINO_UNO_D7 CYBSP_D7
#define ARDUINO_UNO_D8 CYBSP_D8
#define ARDUINO_UNO_D9 CYBSP_D9
#define ARDUINO_UNO_D10 CYBSP_D10
#define ARDUINO_UNO_D11 CYBSP_D11
#define ARDUINO_UNO_D12 CYBSP_D12
#define ARDUINO_UNO_D13 CYBSP_D13
#define ARDUINO_UNO_D14 CYBSP_D14
#define ARDUINO_UNO_D15 CYBSP_D15
#endif

#endif // MBED_PINNAMES_H
