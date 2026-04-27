/* mbed Microcontroller Library
 * Copyright (c) 2019, Arm Limited and affiliates.
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


#include "../../../hal/include/hal/pinmap.h"
#include "objects.h"
#include "PeripheralPins.h"

const PinMap PinMap_FULL[] = {
    {IO_0,  0, 0},
    {IO_1,  0, 0},
    {IO_2,  0, 0},
    {IO_3,  0, 0},
    {IO_4,  0, 0},
    {IO_5,  0, 0},
    {IO_6,  0, 0},
    {IO_7,  0, 0},
    {IO_8,  0, 0},
    {IO_9,  0, 0},
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

#if !PICO_RP2350A
    {IO_30, 0, 0},
    {IO_31, 0, 0},
    {IO_32, 0, 0},
    {IO_33, 0, 0},
    {IO_34, 0, 0},
    {IO_35, 0, 0},
    {IO_36, 0, 0},
    {IO_37, 0, 0},
    {IO_38, 0, 0},
    {IO_39, 0, 0},
    {IO_40, 0, 0},
    {IO_41, 0, 0},
    {IO_42, 0, 0},
    {IO_43, 0, 0},
    {IO_44, 0, 0},
    {IO_45, 0, 0},
    {IO_46, 0, 0},
    {IO_47, 0, 0},
#endif

    {NC, NC, 0}
};


/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {IO_0,  UART_0, (uint32_t) uart0},
    {IO_4,  UART_1, (uint32_t) uart1},
    {IO_8,  UART_1, (uint32_t) uart1},
    {IO_12, UART_0, (uint32_t) uart0},
    {IO_16, UART_0, (uint32_t) uart0},
    {IO_20, UART_1, (uint32_t) uart1},
    {IO_24, UART_1, (uint32_t) uart1},
    {IO_28, UART_0, (uint32_t) uart0},
#if !PICO_RP2350A
    {IO_32, UART_0, (uint32_t) uart0},
    {IO_36, UART_1, (uint32_t) uart1},
    {IO_40, UART_1, (uint32_t) uart1},
    {IO_44, UART_0, (uint32_t) uart0},
#endif
    {NC, NC, 0}
};

const PinMap PinMap_UART_RX[] = {
    {IO_1,  UART_0, (uint32_t) uart0},
    {IO_5,  UART_1, (uint32_t) uart1},
    {IO_9,  UART_1, (uint32_t) uart1},
    {IO_13, UART_0, (uint32_t) uart0},
    {IO_17, UART_0, (uint32_t) uart0},
    {IO_21, UART_1, (uint32_t) uart1},
    {IO_25, UART_1, (uint32_t) uart1},
    {IO_29, UART_0, (uint32_t) uart0},
#if !PICO_RP2350A
    {IO_33, UART_0, (uint32_t) uart0},
    {IO_37, UART_1, (uint32_t) uart1},
    {IO_41, UART_1, (uint32_t) uart1},
    {IO_45, UART_0, (uint32_t) uart0},
#endif
    {NC, NC, 0}
};

const PinMap PinMap_UART_CTS[] = {
    {IO_2,  UART_0, (uint32_t) uart0},
    {IO_6,  UART_1, (uint32_t) uart1},
    {IO_10, UART_1, (uint32_t) uart1},
    {IO_14, UART_0, (uint32_t) uart0},
    {IO_18, UART_0, (uint32_t) uart0},
    {IO_22, UART_1, (uint32_t) uart1},
    {IO_26, UART_1, (uint32_t) uart1},
#if !PICO_RP2350A
    {IO_30, UART_0, (uint32_t) uart0},
    {IO_34, UART_0, (uint32_t) uart0},
    {IO_38, UART_1, (uint32_t) uart1},
    {IO_42, UART_1, (uint32_t) uart1},
#endif
    {NC, NC, 0}
};

const PinMap PinMap_UART_RTS[] = {
    {IO_3,  UART_0, (uint32_t) uart0},
    {IO_7,  UART_1, (uint32_t) uart1},
    {IO_11, UART_1, (uint32_t) uart1},
    {IO_15, UART_0, (uint32_t) uart0},
    {IO_19, UART_0, (uint32_t) uart0},
    {IO_23, UART_1, (uint32_t) uart1},
    {IO_27, UART_1, (uint32_t) uart1},
#if !PICO_RP2350A
    {IO_31, UART_0, (uint32_t) uart0},
    {IO_35, UART_0, (uint32_t) uart0},
    {IO_39, UART_1, (uint32_t) uart1},
    {IO_43, UART_1, (uint32_t) uart1},
#endif
    {NC, NC, 0}
};

/************PWM***************/
const PinMap PinMap_PWM_OUT[] = {
    {IO_0,  PWM_0, 0},
    {IO_1,  PWM_0, 0},
    {IO_2,  PWM_1, 0},
    {IO_3,  PWM_1, 0},
    {IO_4,  PWM_2, 0},
    {IO_5,  PWM_2, 0},
    {IO_6,  PWM_3, 0},
    {IO_7,  PWM_3, 0},
    {IO_8,  PWM_4, 0},
    {IO_9,  PWM_4, 0},
    {IO_10, PWM_5, 0},
    {IO_11, PWM_5, 0},
    {IO_12, PWM_6, 0},
    {IO_13, PWM_6, 0},
    {IO_14, PWM_7, 0},
    {IO_15, PWM_7, 0},
    {IO_16, PWM_0, 0},
    {IO_17, PWM_0, 0},
    {IO_18, PWM_1, 0},
    {IO_19, PWM_1, 0},
    {IO_20, PWM_2, 0},
    {IO_21, PWM_2, 0},
    {IO_22, PWM_3, 0},
    {IO_23, PWM_3, 0},
    {IO_24, PWM_4, 0},
    {IO_25, PWM_4, 0},
    {IO_26, PWM_5, 0},
    {IO_27, PWM_5, 0},
    {IO_28, PWM_6, 0},
    {IO_29, PWM_6, 0},
#if !PICO_RP2350A
    {IO_30, PWM_7, 0},
    {IO_31, PWM_7, 0},
    {IO_32, PWM_8, 0},
    {IO_33, PWM_8, 0},
    {IO_34, PWM_9, 0},
    {IO_35, PWM_9, 0},
    {IO_36, PWM_10, 0},
    {IO_37, PWM_10, 0},
    {IO_38, PWM_11, 0},
    {IO_39, PWM_11, 0},
    {IO_40, PWM_8, 0},
    {IO_41, PWM_8, 0},
    {IO_42, PWM_9, 0},
    {IO_43, PWM_9, 0},
    {IO_44, PWM_10, 0},
    {IO_45, PWM_10, 0},
    {IO_46, PWM_11, 0},
    {IO_47, PWM_11, 0},
#endif
    {NC, NC, 0}
};

/************SPI***************/
const PinMap PinMap_SPI_MISO[] = {
    {IO_0,  SPI_0, (uint32_t) spi0},
    {IO_4,  SPI_0, (uint32_t) spi0},
    {IO_8,  SPI_1, (uint32_t) spi1},
    {IO_12, SPI_1, (uint32_t) spi1},
    {IO_16, SPI_0, (uint32_t) spi0},
    {IO_20, SPI_0, (uint32_t) spi0},
    {IO_24, SPI_1, (uint32_t) spi1},
    {IO_28, SPI_1, (uint32_t) spi1},
#if !PICO_RP2350A
    {IO_32, SPI_0, (uint32_t) spi0},
    {IO_36, SPI_0, (uint32_t) spi0},
    {IO_40, SPI_1, (uint32_t) spi1},
    {IO_44, SPI_1, (uint32_t) spi1},
#endif
    {NC, NC, 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {IO_1,  SPI_0, (uint32_t) spi0},
    {IO_5,  SPI_0, (uint32_t) spi0},
    {IO_9,  SPI_1, (uint32_t) spi1},
    {IO_13, SPI_1, (uint32_t) spi1},
    {IO_17, SPI_0, (uint32_t) spi0},
    {IO_21, SPI_0, (uint32_t) spi0},
    {IO_25, SPI_1, (uint32_t) spi1},
    {IO_29, SPI_1, (uint32_t) spi1},
#if !PICO_RP2350A
    {IO_33, SPI_0, (uint32_t) spi0},
    {IO_37, SPI_0, (uint32_t) spi0},
    {IO_41, SPI_1, (uint32_t) spi1},
    {IO_45, SPI_1, (uint32_t) spi1},
#endif
    {NC, NC, 0}
};

const PinMap PinMap_SPI_SCLK[] = {
    {IO_2,  SPI_0, (uint32_t) spi0},
    {IO_6,  SPI_0, (uint32_t) spi0},
    {IO_10, SPI_1, (uint32_t) spi1},
    {IO_14, SPI_1, (uint32_t) spi1},
    {IO_18, SPI_0, (uint32_t) spi0},
    {IO_22, SPI_0, (uint32_t) spi0},
    {IO_26, SPI_1, (uint32_t) spi1},
#if !PICO_RP2350A
    {IO_30, SPI_1, (uint32_t) spi1},
    {IO_34, SPI_0, (uint32_t) spi0},
    {IO_38, SPI_0, (uint32_t) spi0},
    {IO_42, SPI_1, (uint32_t) spi1},
#endif
    {NC, NC, 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {IO_3,  SPI_0, (uint32_t) spi0},
    {IO_7,  SPI_0, (uint32_t) spi0},
    {IO_11, SPI_1, (uint32_t) spi1},
    {IO_15, SPI_1, (uint32_t) spi1},
    {IO_19, SPI_0, (uint32_t) spi0},
    {IO_23, SPI_0, (uint32_t) spi0},
    {IO_27, SPI_1, (uint32_t) spi1},
#if !PICO_RP2350A
    {IO_31, SPI_1, (uint32_t) spi1},
    {IO_35, SPI_0, (uint32_t) spi0},
    {IO_39, SPI_0, (uint32_t) spi0},
    {IO_43, SPI_1, (uint32_t) spi1},
#endif
    {NC, NC, 0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {IO_0,  I2C_0, (uint32_t) i2c0},
    {IO_2,  I2C_1, (uint32_t) i2c1},
    {IO_4,  I2C_0, (uint32_t) i2c0},
    {IO_6,  I2C_1, (uint32_t) i2c1},
    {IO_8,  I2C_0, (uint32_t) i2c0},
    {IO_10, I2C_1, (uint32_t) i2c1},
    {IO_12, I2C_0, (uint32_t) i2c0},
    {IO_14, I2C_1, (uint32_t) i2c1},
    {IO_16, I2C_0, (uint32_t) i2c0},
    {IO_18, I2C_1, (uint32_t) i2c1},
    {IO_20, I2C_0, (uint32_t) i2c0},
    {IO_22, I2C_1, (uint32_t) i2c1},
    {IO_24, I2C_0, (uint32_t) i2c0},
    {IO_26, I2C_1, (uint32_t) i2c1},
    {IO_28, I2C_0, (uint32_t) i2c0},
#if !PICO_RP2350A
    {IO_30, I2C_1, (uint32_t) i2c1},
    {IO_32, I2C_0, (uint32_t) i2c0},
    {IO_34, I2C_1, (uint32_t) i2c1},
    {IO_36, I2C_0, (uint32_t) i2c0},
    {IO_38, I2C_1, (uint32_t) i2c1},
    {IO_40, I2C_0, (uint32_t) i2c0},
    {IO_42, I2C_1, (uint32_t) i2c1},
    {IO_44, I2C_0, (uint32_t) i2c0},
    {IO_46, I2C_1, (uint32_t) i2c1},
#endif
    {NC, NC, 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {IO_1,  I2C_0, (uint32_t) i2c0},
    {IO_3,  I2C_1, (uint32_t) i2c1},
    {IO_5,  I2C_0, (uint32_t) i2c0},
    {IO_7,  I2C_1, (uint32_t) i2c1},
    {IO_9,  I2C_0, (uint32_t) i2c0},
    {IO_11, I2C_1, (uint32_t) i2c1},
    {IO_13, I2C_0, (uint32_t) i2c0},
    {IO_15, I2C_1, (uint32_t) i2c1},
    {IO_17, I2C_0, (uint32_t) i2c0},
    {IO_19, I2C_1, (uint32_t) i2c1},
    {IO_21, I2C_0, (uint32_t) i2c0},
    {IO_23, I2C_1, (uint32_t) i2c1},
    {IO_25, I2C_0, (uint32_t) i2c0},
    {IO_27, I2C_1, (uint32_t) i2c1},
    {IO_29, I2C_0, (uint32_t) i2c0},
#if !PICO_RP2350A
    {IO_31, I2C_1, (uint32_t) i2c1},
    {IO_33, I2C_0, (uint32_t) i2c0},
    {IO_35, I2C_1, (uint32_t) i2c1},
    {IO_37, I2C_0, (uint32_t) i2c0},
    {IO_39, I2C_1, (uint32_t) i2c1},
    {IO_41, I2C_0, (uint32_t) i2c0},
    {IO_43, I2C_1, (uint32_t) i2c1},
    {IO_45, I2C_0, (uint32_t) i2c0},
    {IO_47, I2C_1, (uint32_t) i2c1},
#endif
    {NC, NC, 0}
};

/************ADC***************/
/* ADC inputs 0-3 are GPIOs 26-29, ADC input 4
 * is the onboard temperature sensor.
 */
const PinMap PinMap_ADC[] = {
#if PICO_RP2350 && !PICO_RP2350A
    { IO_40,       ADC0, 0},
    { IO_41,       ADC0, 1},
    { IO_42,       ADC0, 2},
    { IO_43,       ADC0, 3},
    { IO_44,       ADC0, 4},
    { IO_45,       ADC0, 5},
    { IO_46,       ADC0, 6},
    { IO_47,       ADC0, 7},
    { ADC_TEMP,    ADC0, 8},
#else
    { IO_26,       ADC0, 0},
    { IO_27,       ADC0, 1},
    { IO_28,       ADC0, 2},
    { IO_29,       ADC0, 3},
    { ADC_TEMP,    ADC0, 4},
#endif
    { NC,       NC,   0}
};

