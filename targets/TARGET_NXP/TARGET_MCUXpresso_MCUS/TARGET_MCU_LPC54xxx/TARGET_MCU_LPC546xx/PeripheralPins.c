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

#include "PeripheralPins.h"

// Note: For pin functions refer to Tables 256 through 258 in the datasheet

// The Flexcomm modules provide functionality for multiple busses via the same pins. So, their
// pinmaps are declared as macros which can be included into multiple pinmaps.

// Pins of type FCx_RXD_SDA_MOSI
#define FLEXCOMM_PINMAP_RXD_SDA_MOSI \
    {P1_29, Flexcomm7, 1},\
    {P0_3, Flexcomm3, 1},\
    {P0_5, Flexcomm4, 2},\
    {P0_8, Flexcomm5, 3},\
    {P0_11, Flexcomm6, 1},\
    {P2_0, Flexcomm0, 2},\
    {P3_21, Flexcomm9, 1},\
    {P0_24, Flexcomm0, 1},\
    {P0_26, Flexcomm2, 1},\
    {P0_29, Flexcomm0, 1},\
    {P1_1, Flexcomm3, 1},\
    {P1_5, Flexcomm0, 1},\
    {P1_10, Flexcomm1, 2},\
    {P1_13, Flexcomm6, 2},\
    {P1_17, Flexcomm8, 2},\
    {P1_21, Flexcomm4, 5}, /* This pin has FC4 and FC7 available, chose FC4 for now */\
    {P1_24, Flexcomm2, 1},\
    {P2_3, Flexcomm1, 3},\
    {P2_12, Flexcomm5, 5},\
    {P2_17, Flexcomm8, 5},\
    {P2_18, Flexcomm3, 5},\
    {P2_19, Flexcomm7, 3},\
    {P3_2, Flexcomm9, 2},\
    {P3_16, Flexcomm8, 1},\
    {P3_26, Flexcomm4, 3},\
    {P4_2, Flexcomm6, 2},\
    {P4_9, Flexcomm2, 2},\
    {P4_15, Flexcomm9, 3}

// Pins of type FCx_TXD_SCL_MISO
#define FLEXCOMM_PINMAP_TXD_SCL_MISO \
    {P0_2, Flexcomm3, 1},\
    {P0_9, Flexcomm5, 3},\
    {P0_10, Flexcomm1, 4},\
    {P0_12, Flexcomm3, 1},\
    {P0_16, Flexcomm4, 1},\
    {P2_1, Flexcomm0, 2},\
    {P3_22, Flexcomm9, 1},\
    {P0_19, Flexcomm7, 7},\
    {P0_22, Flexcomm6, 1},\
    {P0_25, Flexcomm0, 1},\
    {P0_27, Flexcomm2, 1},\
    {P0_30, Flexcomm0, 1},\
    {P1_6, Flexcomm0, 1},\
    {P1_11, Flexcomm1, 2},\
    {P1_16, Flexcomm6, 2},\
    {P1_18, Flexcomm8, 2},\
    {P1_20, Flexcomm4, 5}, /* This pin has FC7 and FC4 available, chose FC4 for now */\
    {P1_25, Flexcomm2, 1},\
    {P1_30, Flexcomm7, 1},\
    {P2_4, Flexcomm1, 3},\
    {P2_13, Flexcomm5, 5},\
    {P2_19, Flexcomm3, 2},\
    {P2_29, Flexcomm8, 3}, /* This pin has FC7 and FC8 available, chose FC8 for now */\
    {P3_3, Flexcomm9, 2},\
    {P3_17, Flexcomm8, 1},\
    {P3_27, Flexcomm4, 3},\
    {P4_3, Flexcomm6, 2},\
    {P4_10, Flexcomm2, 2},\
    {P4_16, Flexcomm9, 3}

// Pins of type FCx_CTS_SDA_SSEL0
#define FLEXCOMM_PINMAP_CTS_SDA_SSEL0 \
    {P0_1, Flexcomm3, 2},\
    {P0_13, Flexcomm1, 1},\
    {P0_15, Flexcomm6, 1},\
    {P0_31, Flexcomm0, 1},\
    {P3_23, Flexcomm2, 1},\
    {P0_18, Flexcomm4, 1},\
    {P0_20, Flexcomm3, 1},/* This pin has FC3 and FC7 available, chose FC3 for now */\
    {P1_8, Flexcomm0, 1},\
    {P1_9, Flexcomm4, 5},\
    {P1_14, Flexcomm5, 4},\
    {P1_26, Flexcomm2, 1},\
    {P1_31, Flexcomm8, 5},\
    {P2_5, Flexcomm1, 3},\
    {P2_14, Flexcomm5, 5},\
    {P2_21, Flexcomm3, 2},\
    {P2_28, Flexcomm7, 2},\
    {P3_4, Flexcomm8, 3},\
    {P3_13, Flexcomm9, 2},\
    {P3_18, Flexcomm8, 1},\
    {P3_28, Flexcomm4, 3},\
    {P3_30, Flexcomm9, 1},\
    {P4_0, Flexcomm6, 2},\
    {P4_5, Flexcomm9, 2}, /* This pin has FC9 and FC0 available, chose FC9 for now */\
    {P4_11, Flexcomm2, 2}

// Pins of type FCx_RTS_SCL_SSEL1
#define FLEXCOMM_FCx_RTS_SCL_SSEL1 \
    {P2_20, Flexcomm3, SSEL1 | 2}, /* This pin has FC3 and FC7 available, chose FC3 for now */\
    {P2_29, Flexcomm7, SSEL1 | 2}, /* This pin has FC7 and FC8 available, chose FC8 for now */\
    {P0_7, Flexcomm3, SSEL1 | 1},\
    {P0_14, Flexcomm1, SSEL1 | 1},\
    {P3_24, Flexcomm2, SSEL1 | 1},\
    {P1_0, Flexcomm0, SSEL1 | 1},\
    {P0_19, Flexcomm4, SSEL1 | 1}, /* This pin has FC4 and FC7 available, chose FC4 for now */\
    {P0_21, Flexcomm3, SSEL1 | 1},\
    {P1_7, Flexcomm0, SSEL1 | 1},\
    {P1_15, Flexcomm5, SSEL1 | 4}, /* This pin has FC4 and FC5 available, chose FC5 for now */\
    {P1_22, Flexcomm8, SSEL1 | 1},\
    {P1_27, Flexcomm2, SSEL1 | 1},\
    {P2_6, Flexcomm1, SSEL1 | 3},\
    {P2_15, Flexcomm5, SSEL1 | 5},\
    {P3_5, Flexcomm8, SSEL1 | 3},\
    {P3_14, Flexcomm9, SSEL1 | 2},\
    {P3_19, Flexcomm8, SSEL1 | 1},\
    {P3_29, Flexcomm4, SSEL1 | 3},\
    {P3_31, Flexcomm9, SSEL1 | 1},\
    {P4_4, Flexcomm0, SSEL1 | 3},\
    {P4_6, Flexcomm9, SSEL1 | 2},\
    {P4_12, Flexcomm2, SSEL1 | 2}

#define SSEL1 (1 << MBED_LPC_PIN_FUNCTION_SSELNUM_Pos)

/************RTC***************/
const PinMap PinMap_RTC[] = {
    {NC, OSC32KCLK, 0},
};

/************ADC***************/
const PinMap PinMap_ADC[] = {
    {P0_16, ADC0_SE4,  0},
    {P0_31, ADC0_SE5,  0},
    {P1_0,  ADC0_SE6,  0},
    {P2_0,  ADC0_SE7,  0},
    {NC   , NC      ,  0}
};

/************CAN***************/
const PinMap PinMap_CAN_TD[] = {
    {P3_18, CAN_0,  4},
    {P1_17, CAN_1,  5},
    {NC   , NC   ,  0}
};

const PinMap PinMap_CAN_RD[] = {
    {P3_19, CAN_0,  4},
    {P1_18, CAN_1,  5},
    {NC   , NC   ,  0}
};


/************DAC***************/
const PinMap PinMap_DAC[] = {
    {NC      , NC   , 0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    FLEXCOMM_PINMAP_CTS_SDA_SSEL0,
    FLEXCOMM_PINMAP_RXD_SDA_MOSI,
    {NC   , NC   , 0}
};

const PinMap PinMap_I2C_SCL[] = {
    FLEXCOMM_PINMAP_TXD_SCL_MISO,
    FLEXCOMM_FCx_RTS_SCL_SSEL1,
    {NC   , NC   , 0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    FLEXCOMM_PINMAP_TXD_SCL_MISO,
    {NC   ,  NC   , 0}
};

const PinMap PinMap_UART_RX[] = {
    FLEXCOMM_PINMAP_RXD_SDA_MOSI,
    {NC   ,  NC   , 0}
};

const PinMap PinMap_UART_CTS[] = {
    FLEXCOMM_PINMAP_CTS_SDA_SSEL0,
    {NC   , NC    , 0}
};

const PinMap PinMap_UART_RTS[] = {
    FLEXCOMM_FCx_RTS_SCL_SSEL1,
    {NC   , NC    , 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {P0_0,  Flexcomm0, 2},
    {P3_20, Flexcomm1, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    FLEXCOMM_PINMAP_RXD_SDA_MOSI,
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MISO[] = {
    FLEXCOMM_PINMAP_TXD_SCL_MISO,
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    FLEXCOMM_PINMAP_CTS_SDA_SSEL0,
    FLEXCOMM_FCx_RTS_SCL_SSEL1,
    {NC  ,  NC   , 0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {NC   , NC    , 0}
};

/************QSPI***************/

const PinMap PinMap_QSPI_DATA0[] = {
    {P0_24, QSPI_0, 6},
    {NC, NC, 0}
};

const PinMap PinMap_QSPI_DATA1[] = {
    {P0_25, QSPI_0, 6},
    {NC, NC, 0}
};

const PinMap PinMap_QSPI_DATA2[] = {
    {P0_28, QSPI_0, 6},
    {NC, NC, 0}
};

const PinMap PinMap_QSPI_DATA3[] = {
    {P0_27, QSPI_0, 6},
    {NC, NC, 0}
};
 const PinMap PinMap_QSPI_SCLK[] = {
    {P0_26, QSPI_0, 6},
    {NC   , NC    , 0}
};
 const PinMap PinMap_QSPI_SSEL[] = {
    {P0_23, QSPI_0, 6},
    {NC   , NC    , 0}
};

