/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MBED_PERIPHERALPINS_H
#define MBED_PERIPHERALPINS_H

#include "pinmap.h"
#include "PeripheralNames.h"

//*** GPIO ***
#if GPIO_PINMAP_READY
/* If this macro is defined, then PinMap_GPIO is present in PeripheralPins.c */
extern const PinMap PinMap_GPIO[];
#endif

//*** GPIO IRQ ***
#if DEVICE_INTERRUPTIN
extern const PinMap PinMap_IRQ[];
#endif

//*** ADC ***
#if DEVICE_ANALOGIN
extern const PinMap PinMap_ADC[];
#endif

//*** DAC ***
#if DEVICE_ANALOGOUT
extern const PinMap PinMap_DAC[];
#endif

//*** I2C ***
#if DEVICE_I2C
extern const PinMap PinMap_I2C_SDA[];
extern const PinMap PinMap_I2C_SCL[];
#endif

//*** PWM ***
#if DEVICE_PWMOUT
extern const PinMap PinMap_PWM[];
#endif

//*** SERIAL ***
#if DEVICE_SERIAL
extern const PinMap PinMap_UART_TX[];
extern const PinMap PinMap_UART_RX[];
#if DEVICE_SERIAL_FC
extern const PinMap PinMap_UART_RTS[];
extern const PinMap PinMap_UART_CTS[];
#endif
#endif

//*** SPI ***
#if DEVICE_SPI
extern const PinMap PinMap_SPI_MOSI[];
extern const PinMap PinMap_SPI_MISO[];
extern const PinMap PinMap_SPI_SCLK[];
extern const PinMap PinMap_SPI_SSEL[];
#endif

//*** CAN ***
#if DEVICE_CAN
extern const PinMap PinMap_CAN_RD[];
extern const PinMap PinMap_CAN_TD[];
#endif

//*** QSPI ***
#if DEVICE_QSPI
extern const PinMap PinMap_QSPI_DATA0[];
extern const PinMap PinMap_QSPI_DATA1[];
extern const PinMap PinMap_QSPI_DATA2[];
extern const PinMap PinMap_QSPI_DATA3[];
extern const PinMap PinMap_QSPI_SCLK[];
extern const PinMap PinMap_QSPI_SSEL[];
#endif

//*** OSPI ***
#if DEVICE_OSPI
extern const PinMap PinMap_OSPI_DATA0[];
extern const PinMap PinMap_OSPI_DATA1[];
extern const PinMap PinMap_OSPI_DATA2[];
extern const PinMap PinMap_OSPI_DATA3[];
extern const PinMap PinMap_OSPI_DATA4[];
extern const PinMap PinMap_OSPI_DATA5[];
extern const PinMap PinMap_OSPI_DATA6[];
extern const PinMap PinMap_OSPI_DATA7[];
extern const PinMap PinMap_OSPI_DQS[];
extern const PinMap PinMap_OSPI_SCLK[];
extern const PinMap PinMap_OSPI_SSEL[];
#endif

//*** USB ***
#define USE_USB_NO_OTG   0
#define USE_USB_OTG_FS   1
#define USE_USB_OTG_HS   2
#define USE_USB_HS_IN_FS 3

#if DEVICE_USBDEVICE
extern const PinMap PinMap_USB_HS[];
extern const PinMap PinMap_USB_FS[];
#endif /* DEVICE_USBDEVICE */

#endif
