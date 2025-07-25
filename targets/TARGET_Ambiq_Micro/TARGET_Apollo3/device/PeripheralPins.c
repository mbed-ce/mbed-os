/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
// SPDX-License-Identifier: Apache-2.0
#include "PeripheralPins.h"
#include "PeripheralPinConfigs.h"

#include "am_hal_adc.h"
#include "objects.h"

/************RTC***************/
const PinMap PinMap_RTC[] = {
    {NC, 0, 0},
};

/************ADC***************/
const PinMap PinMap_ADC[] = {
    {11, ADC0_2, AM_HAL_PIN_11_ADCSE2},
    {12, ADC0_9, AM_HAL_PIN_12_ADCD0NSE9},
    {13, ADC0_8, AM_HAL_PIN_13_ADCD0PSE8},
    {16, ADC0_0, AM_HAL_PIN_16_ADCSE0},
    {29, ADC0_1, AM_HAL_PIN_29_ADCSE1},
    {31, ADC0_3, AM_HAL_PIN_31_ADCSE3},
    {32, ADC0_4, AM_HAL_PIN_32_ADCSE4},
    {33, ADC0_5, AM_HAL_PIN_33_ADCSE5},
    {34, ADC0_6, AM_HAL_PIN_34_ADCSE6},
    {35, ADC0_7, AM_HAL_PIN_35_ADCSE7},
    {INT_TEMP_SENSOR, ADC0_TEMP, 0},
    {NC, NC, 0}
};

/************DAC***************/
const PinMap PinMap_DAC[] = {
    {NC, NC, 0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {AP3_PER_IOM0_SDA, IOM_0, (uint32_t) &g_AP3_PER_IOM0_SDA},
    {AP3_PER_IOM1_SDA, IOM_1, (uint32_t) &g_AP3_PER_IOM1_SDA},
    {AP3_PER_IOM2_SDA, IOM_2, (uint32_t) &g_AP3_PER_IOM2_SDA},
    {AP3_PER_IOM4_SDA, IOM_4, (uint32_t) &g_AP3_PER_IOM4_SDA},
    {AP3_PER_IOM5_SDA, IOM_5, (uint32_t) &g_AP3_PER_IOM5_SDA},
#if defined (AM_PACKAGE_BGA)
    {AP3_PER_IOM3_SDA, IOM_3, (uint32_t) &g_AP3_PER_IOM3_SDA},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {AP3_PER_IOM0_SCL, IOM_0, (uint32_t) &g_AP3_PER_IOM0_SCL},
    {AP3_PER_IOM1_SCL, IOM_1, (uint32_t) &g_AP3_PER_IOM1_SCL},
    {AP3_PER_IOM2_SCL, IOM_2, (uint32_t) &g_AP3_PER_IOM2_SCL},
    {AP3_PER_IOM4_SCL, IOM_4, (uint32_t) &g_AP3_PER_IOM4_SCL},
    {AP3_PER_IOM5_SCL, IOM_5, (uint32_t) &g_AP3_PER_IOM5_SCL},
#if defined (AM_PACKAGE_BGA)
    {AP3_PER_IOM3_SCL, IOM_3, (uint32_t) &g_AP3_PER_IOM3_SCL},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {AP3_PER_UART0_TX_1, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_1},
    {AP3_PER_UART0_TX_7, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_7},
    {AP3_PER_UART0_TX_16, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_16},
    {AP3_PER_UART0_TX_20, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_20},
    {AP3_PER_UART0_TX_22, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_22},
    {AP3_PER_UART0_TX_26, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_26},
    {AP3_PER_UART0_TX_28, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_28},
    {AP3_PER_UART0_TX_39, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_39},
    {AP3_PER_UART0_TX_41, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_41},
    {AP3_PER_UART0_TX_44, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_44},
    {AP3_PER_UART0_TX_48, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_48},
    {AP3_PER_UART1_TX_8, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_8},
    {AP3_PER_UART1_TX_10, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_10},
    {AP3_PER_UART1_TX_12, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_12},
    {AP3_PER_UART1_TX_14, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_14},
    {AP3_PER_UART1_TX_18, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_18},
    {AP3_PER_UART1_TX_20, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_20},
    {AP3_PER_UART1_TX_24, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_24},
    {AP3_PER_UART1_TX_39, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_39},
#if defined (AM_PACKAGE_BGA)
    {AP3_PER_UART0_TX_30, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_30},
    {AP3_PER_UART1_TX_35, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_35},
    {AP3_PER_UART1_TX_37, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_37},
    {AP3_PER_UART1_TX_42, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_42},
    {AP3_PER_UART1_TX_46, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_46},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

const PinMap PinMap_UART_RX[] = {
    {AP3_PER_UART0_RX_2, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_2},
    {AP3_PER_UART0_RX_11, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_11},
    {AP3_PER_UART0_RX_17, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_17},
    {AP3_PER_UART0_RX_21, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_21},
    {AP3_PER_UART0_RX_23, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_23},
    {AP3_PER_UART0_RX_27, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_27},
    {AP3_PER_UART0_RX_29, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_29},
    {AP3_PER_UART0_RX_40, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_40},
    {AP3_PER_UART0_RX_49, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_49},
    {AP3_PER_UART1_RX_2, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_2},
    {AP3_PER_UART1_RX_4, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_4},
    {AP3_PER_UART1_RX_9, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_9},
    {AP3_PER_UART1_RX_13, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_13},
    {AP3_PER_UART1_RX_15, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_15},
    {AP3_PER_UART1_RX_19, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_19},
    {AP3_PER_UART1_RX_21, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_21},
    {AP3_PER_UART1_RX_25, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_25},
    {AP3_PER_UART1_RX_40, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_40},
    {AP3_PER_UART1_RX_47, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_47},
#if defined (AM_PACKAGE_BGA)
    {AP3_PER_UART0_RX_31, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_31},
    {AP3_PER_UART0_RX_34, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_34},
    {AP3_PER_UART0_RX_45, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_45},
    {AP3_PER_UART1_RX_36, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_36},
    {AP3_PER_UART1_RX_38, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_38},
    {AP3_PER_UART1_RX_43, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_43},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

const PinMap PinMap_UART_RTS[] = {
    {3, UART_0, AM_HAL_PIN_3_UART0RTS},
    {5, UART_0, AM_HAL_PIN_5_UART0RTS},
    {13, UART_0, AM_HAL_PIN_13_UART0RTS},
    {18, UART_0, AM_HAL_PIN_18_UART0RTS},
    {41, UART_0, AM_HAL_PIN_41_UART0RTS},
    {10, UART_1, AM_HAL_PIN_10_UART1RTS},
    {16, UART_1, AM_HAL_PIN_16_UART1RTS},
    {20, UART_1, AM_HAL_PIN_20_UART1RTS},
    {41, UART_1, AM_HAL_PIN_41_UART1RTS},
    {44, UART_1, AM_HAL_PIN_44_UART1RTS},
#if defined (AM_PACKAGE_BGA)
    {34, UART_0, AM_HAL_PIN_34_UART0RTS},
    {35, UART_0, AM_HAL_PIN_35_UART0RTS},
    {37, UART_0, AM_HAL_PIN_37_UART0RTS},
    {30, UART_1, AM_HAL_PIN_30_UART1RTS},
    {31, UART_1, AM_HAL_PIN_31_UART1RTS},
    {34, UART_1, AM_HAL_PIN_34_UART1RTS},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

const PinMap PinMap_UART_CTS[] = {
    {4, UART_0, AM_HAL_PIN_4_UART0CTS},
    {6, UART_0, AM_HAL_PIN_6_UART0CTS},
    {12, UART_0, AM_HAL_PIN_12_UART0CTS},
    {24, UART_0, AM_HAL_PIN_24_UART0CTS},
    {29, UART_0, AM_HAL_PIN_29_UART0CTS},
    {11, UART_1, AM_HAL_PIN_11_UART1CTS},
    {17, UART_1, AM_HAL_PIN_17_UART1CTS},
    {21, UART_1, AM_HAL_PIN_21_UART1CTS},
    {26, UART_1, AM_HAL_PIN_26_UART1CTS},
    {29, UART_1, AM_HAL_PIN_29_UART1CTS},
#if defined (AM_PACKAGE_BGA)
    {33, UART_0, AM_HAL_PIN_33_UART0CTS},
    {36, UART_0, AM_HAL_PIN_36_UART0CTS},
    {38, UART_0, AM_HAL_PIN_38_UART0CTS},
    {32, UART_1, AM_HAL_PIN_32_UART1CTS},
    {36, UART_1, AM_HAL_PIN_36_UART1CTS},
    {45, UART_1, AM_HAL_PIN_45_UART1CTS},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {AP3_PER_IOM0_SCK, IOM_0, (uint32_t) &g_AP3_PER_IOM0_SCK},
    {AP3_PER_IOM1_SCK, IOM_1, (uint32_t) &g_AP3_PER_IOM1_SCK},
    {AP3_PER_IOM2_SCK, IOM_2, (uint32_t) &g_AP3_PER_IOM2_SCK},
    {AP3_PER_IOM4_SCK, IOM_4, (uint32_t) &g_AP3_PER_IOM4_SCK},
    {AP3_PER_IOM5_SCK, IOM_5, (uint32_t) &g_AP3_PER_IOM5_SCK},
#if defined (AM_PACKAGE_BGA)
    {AP3_PER_IOM3_SCK, IOM_3, (uint32_t) &g_AP3_PER_IOM3_SCK},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {AP3_PER_IOM0_MOSI, IOM_0, (uint32_t) &g_AP3_PER_IOM0_MOSI},
    {AP3_PER_IOM1_MOSI, IOM_1, (uint32_t) &g_AP3_PER_IOM1_MOSI},
    {AP3_PER_IOM2_MOSI, IOM_2, (uint32_t) &g_AP3_PER_IOM2_MOSI},
    {AP3_PER_IOM4_MOSI, IOM_4, (uint32_t) &g_AP3_PER_IOM4_MOSI},
    {AP3_PER_IOM5_MOSI, IOM_5, (uint32_t) &g_AP3_PER_IOM5_MOSI},
#if defined (AM_PACKAGE_BGA)
    {AP3_PER_IOM3_MOSI, IOM_3, (uint32_t) &g_AP3_PER_IOM3_MOSI},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {AP3_PER_IOM0_MISO, IOM_0, (uint32_t) &g_AP3_PER_IOM0_MISO},
    {AP3_PER_IOM1_MISO, IOM_1, (uint32_t) &g_AP3_PER_IOM1_MISO},
    {AP3_PER_IOM2_MISO, IOM_2, (uint32_t) &g_AP3_PER_IOM2_MISO},
    {AP3_PER_IOM4_MISO, IOM_4, (uint32_t) &g_AP3_PER_IOM4_MISO},
    {AP3_PER_IOM5_MISO, IOM_5, (uint32_t) &g_AP3_PER_IOM5_MISO},
#if defined (AM_PACKAGE_BGA)
    {AP3_PER_IOM3_MISO, IOM_3, (uint32_t) &g_AP3_PER_IOM3_MISO},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {AP3_PER_NCE_0, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_0},
    {AP3_PER_NCE_1, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_1},
    {AP3_PER_NCE_2, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_2},
    {AP3_PER_NCE_3, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_3},
    {AP3_PER_NCE_4, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_4},
    {AP3_PER_NCE_7, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_7},
    {AP3_PER_NCE_8, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_8},
    {AP3_PER_NCE_9, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_9},
    {AP3_PER_NCE_10, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_10},
    {AP3_PER_NCE_11, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_11},
    {AP3_PER_NCE_12, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_12},
    {AP3_PER_NCE_13, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_13},
    {AP3_PER_NCE_14, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_14},
    {AP3_PER_NCE_15, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_15},
    {AP3_PER_NCE_16, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_16},
    {AP3_PER_NCE_17, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_17},
    {AP3_PER_NCE_18, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_18},
    {AP3_PER_NCE_19, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_19},
    {AP3_PER_NCE_20, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_20},
    {AP3_PER_NCE_21, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_21},
    {AP3_PER_NCE_22, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_22},
    {AP3_PER_NCE_23, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_23},
    {AP3_PER_NCE_24, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_24},
    {AP3_PER_NCE_25, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_25},
    {AP3_PER_NCE_26, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_26},
    {AP3_PER_NCE_27, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_27},
    {AP3_PER_NCE_28, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_28},
    {AP3_PER_NCE_29, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_29},
    {AP3_PER_NCE_41, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_41},
    {AP3_PER_NCE_44, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_44},
    {AP3_PER_NCE_47, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_47},
    {AP3_PER_NCE_48, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_48},
    {AP3_PER_NCE_49, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_49},
#if defined (AM_PACKAGE_BGA)
    {AP3_PER_NCE_30, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_30},
    {AP3_PER_NCE_31, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_31},
    {AP3_PER_NCE_32, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_32},
    {AP3_PER_NCE_33, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_33},
    {AP3_PER_NCE_34, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_34},
    {AP3_PER_NCE_35, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_35},
    {AP3_PER_NCE_36, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_36},
    {AP3_PER_NCE_37, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_37},
    {AP3_PER_NCE_38, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_38},
    {AP3_PER_NCE_42, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_42},
    {AP3_PER_NCE_43, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_43},
    {AP3_PER_NCE_45, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_45},
    {AP3_PER_NCE_46, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_46},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

/************PWM***************/
// Note: The Apollo3 has fairly flexible PWM pin mapping options. Each IO pin which has a PWM function
// can actually map to one of 6 different PWM module outputs. However, there are as many PWM module
// outputs as there are pins, so we don't need to use this just to give every pin its own PWM output.
// For now, we always use the first possible option (Output Selection 2 in Table 814).
const PinMap PinMap_PWM_OUT[] = {
    {IO_12, CTIMER_A0_OUT1, AM_HAL_PIN_12_CTIM0},
    {IO_25, CTIMER_A0_OUT2, AM_HAL_PIN_25_CTIM1},
    {IO_13, CTIMER_B0_OUT1, AM_HAL_PIN_13_CTIM2},
    {IO_26, CTIMER_B0_OUT2, AM_HAL_PIN_26_CTIM3},
    {IO_18, CTIMER_A1_OUT1, AM_HAL_PIN_18_CTIM4},
    {IO_27, CTIMER_A1_OUT2, AM_HAL_PIN_27_CTIM5},
    {IO_19, CTIMER_B1_OUT1, AM_HAL_PIN_19_CTIM6},
    {IO_28, CTIMER_B1_OUT2, AM_HAL_PIN_28_CTIM7},
    {IO_5,  CTIMER_A2_OUT1, AM_HAL_PIN_5_CTIM8},
    {IO_29, CTIMER_A2_OUT2, AM_HAL_PIN_29_CTIM9},
    {IO_6,  CTIMER_B2_OUT1, AM_HAL_PIN_6_CTIM10},
    {IO_30, CTIMER_B2_OUT2, AM_HAL_PIN_30_CTIM11},
    {IO_22, CTIMER_A3_OUT1, AM_HAL_PIN_22_CTIM12},
    {IO_31, CTIMER_A3_OUT2, AM_HAL_PIN_31_CTIM13},
    {IO_23, CTIMER_B3_OUT1, AM_HAL_PIN_23_CTIM14},
    {IO_32, CTIMER_B3_OUT2, AM_HAL_PIN_32_CTIM15},
    {IO_42, CTIMER_A4_OUT1, AM_HAL_PIN_42_CTIM16},
    {IO_4,  CTIMER_A4_OUT2, AM_HAL_PIN_4_CTIM17},
    {IO_43, CTIMER_B4_OUT1, AM_HAL_PIN_43_CTIM18},
    {IO_7,  CTIMER_B4_OUT2, AM_HAL_PIN_7_CTIM19},
    {IO_44, CTIMER_A5_OUT1, AM_HAL_PIN_44_CTIM20},
    {IO_24, CTIMER_A5_OUT2, AM_HAL_PIN_24_CTIM21},
    {IO_45, CTIMER_B5_OUT2, AM_HAL_PIN_45_CTIM22},
    {IO_33, CTIMER_B5_OUT2, AM_HAL_PIN_33_CTIM23},
    {IO_46, CTIMER_A6_OUT1, AM_HAL_PIN_46_CTIM24},
    {IO_47, CTIMER_B6_OUT1, AM_HAL_PIN_47_CTIM26},
    {IO_35, CTIMER_B6_OUT2, AM_HAL_PIN_35_CTIM27},

    // Different from normal mapping since output selection 2 doesn't give a unique timer on this pin
    {IO_39, CTIMER_A6_OUT2, AM_HAL_PIN_39_CTIM25},

    // For these last four, we have to duplicate other timers, as CTIMER_x7 is
    // used for the us ticker.
    {IO_48, CTIMER_A3_OUT1, AM_HAL_PIN_48_CTIM28},
    {IO_37, CTIMER_A3_OUT2, AM_HAL_PIN_37_CTIM29},
    {IO_49, CTIMER_B3_OUT1, AM_HAL_PIN_49_CTIM30},
    {IO_11, CTIMER_B3_OUT2, AM_HAL_PIN_11_CTIM31},

    {NC, NC, 0}
};

/************ GPIO ***************/

// Note that this is only used for testing, so that the test knows what are valid GPIO pins.
// It's not used in normal usage.
// Also, only the "pin" field is significant here. Other fields are don't cares.

const PinMap PinMap_GPIO[] = {
    {IO_0, 0, 0},
    {IO_1, 0, 0},
    {IO_2, 0, 0},
    {IO_3, 0, 0},
    {IO_4, 0, 0},
    {IO_5, 0, 0},
    {IO_6, 0, 0},
    {IO_7, 0, 0},
    {IO_8, 0, 0},
    {IO_9, 0, 0},
    {IO_10, 0, 0},
    {IO_11, 0, 0},
    {IO_12, 0, 0},
    {IO_13, 0, 0},
    {IO_14, 0, 0},
    {IO_15, 0, 0},
    {IO_16, 0, 0},
    {IO_17, 0, 0},
    {IO_18, 0, 0},
    {IO_19, 0, 0},
    {IO_20, 0, 0},
    {IO_21, 0, 0},
    {IO_22, 0, 0},
    {IO_23, 0, 0},
    {IO_24, 0, 0},
    {IO_25, 0, 0},
    {IO_26, 0, 0},
    {IO_27, 0, 0},
    {IO_28, 0, 0},
    {IO_29, 0, 0},
    {IO_39, 0, 0},
    {IO_40, 0, 0},
    {IO_41, 0, 0},
    {IO_44, 0, 0},
    {IO_47, 0, 0},
    {IO_48, 0, 0},
    {IO_49, 0, 0},

    // Apollo3 I/O pins - BGA package only
    {IO_30, 0, 0},
    {IO_31, 0, 0},
    {IO_32, 0, 0},
    {IO_33, 0, 0},
    {IO_34, 0, 0},
    {IO_35, 0, 0},
    {IO_36, 0, 0},
    {IO_37, 0, 0},
    {IO_38, 0, 0},
    {IO_42, 0, 0},
    {IO_43, 0, 0},
    {IO_45, 0, 0},
    {IO_46, 0, 0},

    {NC, NC, 0}
};


