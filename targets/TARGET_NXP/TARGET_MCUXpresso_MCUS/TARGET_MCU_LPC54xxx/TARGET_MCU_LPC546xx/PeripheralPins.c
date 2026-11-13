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
    {P0_1, I2C_3, 2},
    {P0_2, I2C_3, 1},
    {P0_9, I2C_5, 3},
    {P0_10, I2C_1, 4},
    {P0_12, I2C_3, 1},
    {P0_13, I2C_1, 1},
    {P0_15, I2C_6, 1},
    {P0_16, I2C_4, 1},
    {P0_18, I2C_4, 1},
    {P0_19, I2C_7, 7},
    {P0_20, I2C_3, 1},
    {P0_22, I2C_6, 1},
    {P0_25, I2C_0, 1},
    {P0_27, I2C_2, 1},
    {P0_30, I2C_0, 1},
    {P0_31, I2C_0, 1},
    {P1_6, I2C_0, 1},
    {P1_8, I2C_0, 1},
    {P1_9, I2C_4, 5},
    {P1_11, I2C_1, 2},
    {P1_14, I2C_5, 4},
    {P1_16, I2C_6, 2},
    {P1_18, I2C_8, 2},
    {P1_20, I2C_4, 5},
    {P1_21, I2C_7, 1},
    {P1_25, I2C_2, 1},
    {P1_26, I2C_2, 1},
    {P1_29, I2C_7, 1},
    {P1_31, I2C_8, 5},
    {P2_1, I2C_0, 2},
    {P2_4, I2C_1, 3},
    {P2_5, I2C_1, 3},
    {P2_13, I2C_5, 5},
    {P2_14, I2C_5, 5},
    {P2_19, I2C_3, 2},
    {P2_20, I2C_7, 3},
    {P2_21, I2C_3, 2},
    {P2_28, I2C_7, 2},
    {P2_29, I2C_8, 3},
    {P3_3, I2C_9, 2},
    {P3_4, I2C_8, 3},
    {P3_13, I2C_9, 2},
    {P3_17, I2C_8, 1},
    {P3_18, I2C_8, 1},
    {P3_22, I2C_9, 1},
    {P3_23, I2C_2, 1},
    {P3_27, I2C_4, 3},
    {P3_28, I2C_4, 3},
    {P3_30, I2C_9, 1},
    {P4_0, I2C_6, 2},
    {P4_3, I2C_6, 2},
    {P4_5, I2C_9, 2}, // This pin has FC9 and FC0 available, chose FC9 for now
    {P4_10, I2C_2, 2},
    {P4_11, I2C_2, 2},
    {P4_16, I2C_9, 3},
    {NC   , NC   , 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {P0_3, I2C_3, 1},
    {P0_5, I2C_4, 2},
    {P0_7, I2C_3, 1},
    {P0_8, I2C_5, 3},
    {P0_11, I2C_6, 1},
    {P0_14, I2C_1, 1},
    {P0_19, I2C_4, 1},
    {P0_20, I2C_7, 7},
    {P0_21, I2C_3, 1},
    {P0_24, I2C_0, 1},
    {P0_26, I2C_2, 1},
    {P0_29, I2C_0, 1},
    {P1_0, I2C_0, 1},
    {P1_1, I2C_3, 1},
    {P1_5, I2C_0, 1},
    {P1_7, I2C_0, 1},
    {P1_10, I2C_1, 2},
    {P1_13, I2C_6, 2},
    {P1_15, I2C_5, 4}, // This pin has FC4 and FC5 available, chose FC5 for now
    {P1_17, I2C_8, 2},
    {P1_20, I2C_7, 1},
    {P1_21, I2C_4, 5},
    {P1_22, I2C_8, 1},
    {P1_24, I2C_2, 1},
    {P1_27, I2C_2, 1},
    {P1_30, I2C_7, 1},
    {P2_0, I2C_0, 2},
    {P2_3, I2C_1, 3},
    {P2_6, I2C_1, 3},
    {P2_12, I2C_5, 5},
    {P2_15, I2C_5, 5},
    {P2_17, I2C_8, 5},
    {P2_18, I2C_3, 5},
    {P2_19, I2C_7, 3},
    {P2_20, I2C_3, 2},
    {P2_29, I2C_7, 2},
    {P3_2, I2C_9, 2},
    {P3_5, I2C_8, 3},
    {P3_14, I2C_9, 2},
    {P3_16, I2C_8, 1},
    {P3_19, I2C_8, 1},
    {P3_21, I2C_9, 1},
    {P3_24, I2C_2, 1},
    {P3_26, I2C_4, 3},
    {P3_29, I2C_4, 3},
    {P3_31, I2C_9, 1},
    {P4_2, I2C_6, 2},
    {P4_4, I2C_0, 3},
    {P4_6, I2C_9, 2},
    {P4_9, I2C_2, 2},
    {P4_12, I2C_2, 2},
    {P4_15, I2C_9, 3},
    {NC   , NC   , 0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {P0_30, UART_0, 1},
    {P3_27, UART_1, 3},
    {NC   ,  NC   , 0}
};

const PinMap PinMap_UART_RX[] = {
    {P0_29, UART_0, 1},
    {P3_26, UART_1, 3},
    {NC   ,  NC   , 0}
};

const PinMap PinMap_UART_CTS[] = {
    {P3_28, UART_1, 1},
    {NC   , NC    , 0}
};

const PinMap PinMap_UART_RTS[] = {
    {P3_29, UART_1, 1},
    {NC   , NC    , 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {P0_0,  SPI_0, 2},
    {P3_20, SPI_1, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {P0_3,  SPI_0, 1},
    {P3_21, SPI_1, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {P0_2,  SPI_0, 1},
    {P3_22, SPI_1, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {P0_1,  SPI_0, 2},
    {P3_30, SPI_1, 1},
    {P4_6,  SPI_1, ((1 << SSELNUM_SHIFT) | 2)},
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

