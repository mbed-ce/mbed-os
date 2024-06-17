/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
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

#ifndef PERIPHERAL_PINMAPS_H
#define PERIPHERAL_PINMAPS_H

#include <mstd_cstddef>

#include "pinmap.h"

#include "PeripheralNames.h"

// For LPC1768, PinMap entries are in the form
// (pin name, peripheral name, function number)
// where the function number is the 2-bit value that will be written into the PINSELn
// bitfield for this pin.
// See section 8.5 in the LPC1768 user manual for the source of these pinmappings.

static MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_UART_TX[] = {
    {P0_0,  UART_3, 2},
    {P0_2,  UART_0, 1},
    {P0_10, UART_2, 1},
    {P0_15, UART_1, 1},
    {P0_25, UART_3, 3},
    {P2_0 , UART_1, 2},
    {P2_8 , UART_2, 2},
    {P4_28, UART_3, 3},
    {NC   , NC    , 0}
};

static MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_UART_RX[] = {
    {P0_1 , UART_3, 2},
    {P0_3 , UART_0, 1},
    {P0_11, UART_2, 1},
    {P0_16, UART_1, 1},
    {P0_26, UART_3, 3},
    {P2_1 , UART_1, 2},
    {P2_9 , UART_2, 2},
    {P4_29, UART_3, 3},
    {NC   , NC    , 0}
};

// Only UART1 has hardware flow control on LPC176x
static MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_UART_RTS[] = {
    {P0_22, UART_1, 1},
    {P2_7,  UART_1, 2},
    {NC,    NC,     0}
};

static MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_UART_CTS[] = {
    {P0_17, UART_1, 1},
    {P2_2,  UART_1, 2},
    {NC,    NC,     0}
};

static MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_SPI_SCLK[] = {
    {P0_7 , SPI_1, 2},
    {P0_15, SPI_0, 2},
    {P1_20, SPI_0, 3},
    {P1_31, SPI_1, 2},
    {NC   , NC   , 0}
};

static MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_SPI_MOSI[] = {
    {P0_9 , SPI_1, 2},
    {P0_13, SPI_1, 2},
    {P0_18, SPI_0, 2},
    {P1_24, SPI_0, 3},
    {NC   , NC   , 0}
};

static MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_SPI_MISO[] = {
    {P0_8 , SPI_1, 2},
    {P0_12, SPI_1, 2},
    {P0_17, SPI_0, 2},
    {P1_23, SPI_0, 3},
    {NC   , NC   , 0}
};

static MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_SPI_SSEL[] = {
    {P0_6 , SPI_1, 2},
    {P0_11, SPI_1, 2},
    {P0_16, SPI_0, 2},
    {P1_21, SPI_0, 3},
    {NC   , NC   , 0}
};

static MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_ADC[] = {
    {P0_23, ADC0_0, 1},
    {P0_24, ADC0_1, 1},
    {P0_25, ADC0_2, 1},
    {P0_26, ADC0_3, 1},
    {P1_30, ADC0_4, 3},
    {P1_31, ADC0_5, 3},
    {P0_2,  ADC0_7, 2},
    {P0_3,  ADC0_6, 2},
    {NC,    NC,     0}
};

static MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_DAC[] = {
    {P0_26, DAC_0, 2},
    {NC   , NC   , 0}
};

// NOTE: For I2C, only the P0_27/P0_28 pinmapping is fully electrically compliant to the I2C standard.
static MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_I2C_SDA[] = {
    {P0_27, I2C_0, 1},
    {P0_0 , I2C_1, 3},
    {P0_19, I2C_1, 3},
    {P0_10, I2C_2, 2},
    {NC   , NC   , 0}
};

static MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_I2C_SCL[] = {
    {P0_28, I2C_0, 1},
    {P0_1 , I2C_1, 3},
    {P0_20, I2C_1, 3},
    {P0_11, I2C_2, 2},
    {NC   , NC,    0}
};

static MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_PWM[] = {
    {P1_18, PWM_1, 2},
    {P1_20, PWM_2, 2},
    {P1_21, PWM_3, 2},
    {P1_23, PWM_4, 2},
    {P1_24, PWM_5, 2},
    {P1_26, PWM_6, 2},
    {P2_0, PWM_1, 1},
    {P2_1, PWM_2, 1},
    {P2_2, PWM_3, 1},
    {P2_3, PWM_4, 1},
    {P2_4, PWM_5, 1},
    {P2_5, PWM_6, 1},
    {P3_25, PWM_2, 3},
    {P3_26, PWM_3, 3},
    {NC, NC, 0}
};

static MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_CAN_RD[] = {
    {P0_0 , CAN_1, 1},
    {P0_4 , CAN_2, 2},
    {P0_21, CAN_1, 3},
    {P2_7 , CAN_2, 1},
    {NC   , NC   , 0}
};

static MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_CAN_TD[] = {
    {P0_1 , CAN_1, 1},
    {P0_5 , CAN_2, 2},
    {P0_22, CAN_1, 3},
    {P2_8 , CAN_2, 1},
    {NC   , NC   , 0}
};

// Pinmap name macros
#define PINMAP_UART_TX PinMap_UART_TX
#define PINMAP_UART_RX PinMap_UART_RX
#define PINMAP_UART_RTS PinMap_UART_RTS
#define PINMAP_UART_CTS PinMap_UART_CTS
#define PINMAP_SPI_SCLK PinMap_SPI_SCLK
#define PINMAP_SPI_MOSI PinMap_SPI_MOSI
#define PINMAP_SPI_MISO PinMap_SPI_MISO
#define PINMAP_SPI_SSEL PinMap_SPI_SSEL
#define PINMAP_ANALOGIN PinMap_ADC
#define PINMAP_ANALOGOUT PinMap_DAC
#define PINMAP_I2C_SDA PinMap_I2C_SDA
#define PINMAP_I2C_SCL PinMap_I2C_SCL
#define PINMAP_PWM PinMap_PWM
#define PINMAP_CAN_RD PinMap_CAN_RD
#define PINMAP_CAN_TD PinMap_CAN_TD
#endif