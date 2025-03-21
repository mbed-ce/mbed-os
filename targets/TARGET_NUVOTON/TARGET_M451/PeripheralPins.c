/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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

// =====
// Note: Commented lines are alternative possibilities which are not used per default.
//       If you change them, you will have also to modify the corresponding xxx_api.c file
//       for pwmout, analogin, analogout, ...
// =====

#if 0
//*** GPIO ***
const PinMap PinMap_GPIO[] = {
    // GPIO A MFPL
    {PA_0, GPIO_A, SYS_GPA_MFPL_PA0MFP_GPIO},
    {PA_1, GPIO_A, SYS_GPA_MFPL_PA1MFP_GPIO},
    {PA_2, GPIO_A, SYS_GPA_MFPL_PA2MFP_GPIO},
    {PA_3, GPIO_A, SYS_GPA_MFPL_PA3MFP_GPIO},
    {PA_4, GPIO_A, SYS_GPA_MFPL_PA4MFP_GPIO},
    {PA_5, GPIO_A, SYS_GPA_MFPL_PA5MFP_GPIO},
    {PA_6, GPIO_A, SYS_GPA_MFPL_PA6MFP_GPIO},
    {PA_7, GPIO_A, SYS_GPA_MFPL_PA7MFP_GPIO},
    // GPIO A MFPH
    {PA_8, GPIO_A, SYS_GPA_MFPH_PA8MFP_GPIO},
    {PA_9, GPIO_A, SYS_GPA_MFPH_PA9MFP_GPIO},
    {PA_10, GPIO_A, SYS_GPA_MFPH_PA10MFP_GPIO},
    {PA_11, GPIO_A, SYS_GPA_MFPH_PA11MFP_GPIO},
    {PA_12, GPIO_A, SYS_GPA_MFPH_PA12MFP_GPIO},
    {PA_13, GPIO_A, SYS_GPA_MFPH_PA13MFP_GPIO},
    {PA_14, GPIO_A, SYS_GPA_MFPH_PA14MFP_GPIO},
    {PA_15, GPIO_A, SYS_GPA_MFPH_PA15MFP_GPIO},
    
    // GPIO B MFPL
    {PB_0, GPIO_B, SYS_GPB_MFPL_PB0MFP_GPIO},
    {PB_1, GPIO_B, SYS_GPB_MFPL_PB1MFP_GPIO},
    {PB_2, GPIO_B, SYS_GPB_MFPL_PB2MFP_GPIO},
    {PB_3, GPIO_B, SYS_GPB_MFPL_PB3MFP_GPIO},
    {PB_4, GPIO_B, SYS_GPB_MFPL_PB4MFP_GPIO},
    {PB_5, GPIO_B, SYS_GPB_MFPL_PB5MFP_GPIO},
    {PB_6, GPIO_B, SYS_GPB_MFPL_PB6MFP_GPIO},
    {PB_7, GPIO_B, SYS_GPB_MFPL_PB7MFP_GPIO},
    // GPIO B MFPH
    {PB_8, GPIO_B, SYS_GPB_MFPH_PB8MFP_GPIO},
    {PB_9, GPIO_B, SYS_GPB_MFPH_PB9MFP_GPIO},
    {PB_10, GPIO_B, SYS_GPB_MFPH_PB10MFP_GPIO},
    {PB_11, GPIO_B, SYS_GPB_MFPH_PB11MFP_GPIO},
    {PB_12, GPIO_B, SYS_GPB_MFPH_PB12MFP_GPIO},
    {PB_13, GPIO_B, SYS_GPB_MFPH_PB13MFP_GPIO},
    {PB_14, GPIO_B, SYS_GPB_MFPH_PB14MFP_GPIO},
    {PB_15, GPIO_B, SYS_GPB_MFPH_PB15MFP_GPIO},
    
    // GPIO C MFPL
    {PC_0, GPIO_C, SYS_GPC_MFPL_PC0MFP_GPIO},
    {PC_1, GPIO_C, SYS_GPC_MFPL_PC1MFP_GPIO},
    {PC_2, GPIO_C, SYS_GPC_MFPL_PC2MFP_GPIO},
    {PC_3, GPIO_C, SYS_GPC_MFPL_PC3MFP_GPIO},
    {PC_4, GPIO_C, SYS_GPC_MFPL_PC4MFP_GPIO},
    {PC_5, GPIO_C, SYS_GPC_MFPL_PC5MFP_GPIO},
    {PC_6, GPIO_C, SYS_GPC_MFPL_PC6MFP_GPIO},
    {PC_7, GPIO_C, SYS_GPC_MFPL_PC7MFP_GPIO},
    // GPIO C MFPH
    {PC_8, GPIO_C, SYS_GPC_MFPH_PC8MFP_GPIO},
    {PC_9, GPIO_C, SYS_GPC_MFPH_PC9MFP_GPIO},
    {PC_10, GPIO_C, SYS_GPC_MFPH_PC10MFP_GPIO},
    {PC_11, GPIO_C, SYS_GPC_MFPH_PC11MFP_GPIO},
    {PC_12, GPIO_C, SYS_GPC_MFPH_PC12MFP_GPIO},
    {PC_13, GPIO_C, SYS_GPC_MFPH_PC13MFP_GPIO},
    {PC_14, GPIO_C, SYS_GPC_MFPH_PC14MFP_GPIO},
    {PC_15, GPIO_C, SYS_GPC_MFPH_PC15MFP_GPIO},
    
    // GPIO D MFPL
    {PD_0, GPIO_D, SYS_GPD_MFPL_PD0MFP_GPIO},
    {PD_1, GPIO_D, SYS_GPD_MFPL_PD1MFP_GPIO},
    {PD_2, GPIO_D, SYS_GPD_MFPL_PD2MFP_GPIO},
    {PD_3, GPIO_D, SYS_GPD_MFPL_PD3MFP_GPIO},
    {PD_4, GPIO_D, SYS_GPD_MFPL_PD4MFP_GPIO},
    {PD_5, GPIO_D, SYS_GPD_MFPL_PD5MFP_GPIO},
    {PD_6, GPIO_D, SYS_GPD_MFPL_PD6MFP_GPIO},
    {PD_7, GPIO_D, SYS_GPD_MFPL_PD7MFP_GPIO},
    // GPIO D MFPH
    {PD_8, GPIO_D, SYS_GPD_MFPH_PD8MFP_GPIO},
    {PD_9, GPIO_D, SYS_GPD_MFPH_PD9MFP_GPIO},
    {PD_10, GPIO_D, SYS_GPD_MFPH_PD10MFP_GPIO},
    {PD_11, GPIO_D, SYS_GPD_MFPH_PD11MFP_GPIO},
    {PD_12, GPIO_D, SYS_GPD_MFPH_PD12MFP_GPIO},
    {PD_13, GPIO_D, SYS_GPD_MFPH_PD13MFP_GPIO},
    {PD_14, GPIO_D, SYS_GPD_MFPH_PD14MFP_GPIO},
    {PD_15, GPIO_D, SYS_GPD_MFPH_PD15MFP_GPIO},
    
    // GPIO E MFPL
    {PE_0, GPIO_E, SYS_GPE_MFPL_PE0MFP_GPIO},
    {PE_1, GPIO_E, SYS_GPE_MFPL_PE1MFP_GPIO},
    {PE_2, GPIO_E, SYS_GPE_MFPL_PE2MFP_GPIO},
    {PE_3, GPIO_E, SYS_GPE_MFPL_PE3MFP_GPIO},
    {PE_4, GPIO_E, SYS_GPE_MFPL_PE4MFP_GPIO},
    {PE_5, GPIO_E, SYS_GPE_MFPL_PE5MFP_GPIO},
    {PE_6, GPIO_E, SYS_GPE_MFPL_PE6MFP_GPIO},
    {PE_7, GPIO_E, SYS_GPE_MFPL_PE7MFP_GPIO},
    // GPIO E MFPH
    {PE_8, GPIO_E, SYS_GPE_MFPH_PE8MFP_GPIO},
    {PE_9, GPIO_E, SYS_GPE_MFPH_PE9MFP_GPIO},
    {PE_10, GPIO_E, SYS_GPE_MFPH_PE10MFP_GPIO},
    {PE_11, GPIO_E, SYS_GPE_MFPH_PE11MFP_GPIO},
    {PE_12, GPIO_E, SYS_GPE_MFPH_PE12MFP_GPIO},
    {PE_13, GPIO_E, SYS_GPE_MFPH_PE13MFP_GPIO},
    {PE_14, GPIO_E, SYS_GPE_MFPH_PE14MFP_GPIO},
    
    // GPIO F MFPL
    {PF_0, GPIO_F, SYS_GPF_MFPL_PF0MFP_GPIO},
    {PF_1, GPIO_F, SYS_GPF_MFPL_PF1MFP_GPIO},
    {PF_2, GPIO_F, SYS_GPF_MFPL_PF2MFP_GPIO},
    {PF_3, GPIO_F, SYS_GPF_MFPL_PF3MFP_GPIO},
    {PF_4, GPIO_F, SYS_GPF_MFPL_PF4MFP_GPIO},
    {PF_5, GPIO_F, SYS_GPF_MFPL_PF5MFP_GPIO},
    {PF_6, GPIO_F, SYS_GPF_MFPL_PF6MFP_GPIO},
    {PF_7, GPIO_F, SYS_GPF_MFPL_PF7MFP_GPIO},
};
#endif

//*** ADC ***

const PinMap PinMap_ADC[] = {
    {PB_0, ADC_0_0, SYS_GPB_MFPL_PB0MFP_EADC_CH0},
    {PB_1, ADC_0_1, SYS_GPB_MFPL_PB1MFP_EADC_CH1},
    {PB_2, ADC_0_2, SYS_GPB_MFPL_PB2MFP_EADC_CH2},
    {PB_3, ADC_0_3, SYS_GPB_MFPL_PB3MFP_EADC_CH3},
    {PB_4, ADC_0_4, SYS_GPB_MFPL_PB4MFP_EADC_CH4},
    {PB_5, ADC_0_13, SYS_GPB_MFPL_PB5MFP_EADC_CH13},
    {PB_6, ADC_0_14, SYS_GPB_MFPL_PB6MFP_EADC_CH14},
    {PB_7, ADC_0_15, SYS_GPB_MFPL_PB7MFP_EADC_CH15},
    {PB_8, ADC_0_5, SYS_GPB_MFPH_PB8MFP_EADC_CH5},
    {PB_9, ADC_0_6, SYS_GPB_MFPH_PB9MFP_EADC_CH6},
    {PB_10, ADC_0_7, SYS_GPB_MFPH_PB10MFP_EADC_CH7},
    {PB_11, ADC_0_8, SYS_GPB_MFPH_PB11MFP_EADC_CH8},
    {PB_12, ADC_0_9, SYS_GPB_MFPH_PB12MFP_EADC_CH9},
    {PB_13, ADC_0_10, SYS_GPB_MFPH_PB13MFP_EADC_CH10},
    {PB_14, ADC_0_11, SYS_GPB_MFPH_PB14MFP_EADC_CH11},
    {PB_15, ADC_0_12, SYS_GPB_MFPH_PB15MFP_EADC_CH12},
#if defined(TARGET_M45xD_M45xC)
    {PD_0, ADC_0_6, SYS_GPD_MFPL_PD0MFP_EADC_CH6},
    {PD_1, ADC_0_11, SYS_GPD_MFPL_PD1MFP_EADC_CH11},
    {PD_8, ADC_0_7, SYS_GPD_MFPH_PD8MFP_EADC_CH7},
    {PD_9, ADC_0_10, SYS_GPD_MFPH_PD9MFP_EADC_CH10},
#endif

    {NC,   NC,    0}
};

//*** DAC ***

const PinMap PinMap_DAC[] = {
    {PB_0, DAC_0_0, SYS_GPB_MFPL_PB0MFP_DAC},

    {NC,   NC,    0}
};

//*** I2C ***

const PinMap PinMap_I2C_SDA[] = {
    {PA_2,  I2C_0, SYS_GPA_MFPL_PA2MFP_I2C0_SDA},
    {PD_4,  I2C_0, SYS_GPD_MFPL_PD4MFP_I2C0_SDA},
    {PE_0, I2C_1, SYS_GPE_MFPL_PE0MFP_I2C1_SDA},
    {PE_5, I2C_1, SYS_GPE_MFPL_PE5MFP_I2C1_SDA},
    {PE_9, I2C_1, SYS_GPE_MFPH_PE9MFP_I2C1_SDA},
#if defined(TARGET_M45xD_M45xC)
    {PE_11, I2C_1, SYS_GPE_MFPH_PE11MFP_I2C1_SDA},
#endif
    {PE_13, I2C_0, SYS_GPE_MFPH_PE13MFP_I2C0_SDA},
    {PF_4, I2C_1, SYS_GPF_MFPL_PF4MFP_I2C1_SDA},
    
    {NC,    NC,    0}
};

const PinMap PinMap_I2C_SCL[] = {
    {PA_3, I2C_0, SYS_GPA_MFPL_PA3MFP_I2C0_SCL},
    {PC_4, I2C_1, SYS_GPC_MFPL_PC4MFP_I2C1_SCL},
    {PD_5, I2C_0, SYS_GPD_MFPL_PD5MFP_I2C0_SCL},
    {PE_4, I2C_1, SYS_GPE_MFPL_PE4MFP_I2C1_SCL},
    {PE_8, I2C_1, SYS_GPE_MFPH_PE8MFP_I2C1_SCL},
#if defined(TARGET_M45xD_M45xC)
    {PE_10, I2C_1, SYS_GPE_MFPH_PE10MFP_I2C1_SCL},
#endif
    {PE_12, I2C_0, SYS_GPE_MFPH_PE12MFP_I2C0_SCL},
    {PF_3, I2C_1, SYS_GPF_MFPL_PF3MFP_I2C1_SCL},
   
    
    {NC,    NC,    0}
};

//*** PWM ***

const PinMap PinMap_PWM[] = {
    {PA_0, PWM_1_5, SYS_GPA_MFPL_PA0MFP_PWM1_CH5},
    {PA_1, PWM_1_4, SYS_GPA_MFPL_PA1MFP_PWM1_CH4},
    {PA_2, PWM_1_3, SYS_GPA_MFPL_PA2MFP_PWM1_CH3},
    {PA_3, PWM_1_2, SYS_GPA_MFPL_PA3MFP_PWM1_CH2},
    {PB_8, PWM_0_2, SYS_GPB_MFPH_PB8MFP_PWM0_CH2},
    {PC_0, PWM_0_0, SYS_GPC_MFPL_PC0MFP_PWM0_CH0},
    {PC_1, PWM_0_1, SYS_GPC_MFPL_PC1MFP_PWM0_CH1},
    {PC_2, PWM_0_2, SYS_GPC_MFPL_PC2MFP_PWM0_CH2},
    {PC_3, PWM_0_3, SYS_GPC_MFPL_PC3MFP_PWM0_CH3},
    {PC_4, PWM_0_4, SYS_GPC_MFPL_PC4MFP_PWM0_CH4},
    {PC_5, PWM_0_5, SYS_GPC_MFPL_PC5MFP_PWM0_CH5},
    {PC_6, PWM_1_0, SYS_GPC_MFPL_PC6MFP_PWM1_CH0},
    {PC_7, PWM_1_1, SYS_GPC_MFPL_PC7MFP_PWM1_CH1},
    {PC_9, PWM_1_0, SYS_GPC_MFPH_PC9MFP_PWM1_CH0},
    {PC_10, PWM_1_1, SYS_GPC_MFPH_PC10MFP_PWM1_CH1},
    {PC_11, PWM_1_2, SYS_GPC_MFPH_PC11MFP_PWM1_CH2},
    {PC_12, PWM_1_3, SYS_GPC_MFPH_PC12MFP_PWM1_CH3},
    {PC_13, PWM_1_4, SYS_GPC_MFPH_PC13MFP_PWM1_CH4},
    {PC_14, PWM_1_5, SYS_GPC_MFPH_PC14MFP_PWM1_CH5},
    {PC_15, PWM_1_0, SYS_GPC_MFPH_PC15MFP_PWM1_CH0},
    {PD_6, PWM_0_5, SYS_GPD_MFPL_PD6MFP_PWM0_CH5},
    {PD_7, PWM_0_5, SYS_GPD_MFPL_PD7MFP_PWM0_CH5},
    {PD_12, PWM_1_0, SYS_GPD_MFPH_PD12MFP_PWM1_CH0},
    {PD_13, PWM_1_1, SYS_GPD_MFPH_PD13MFP_PWM1_CH1},
    {PD_14, PWM_1_2, SYS_GPD_MFPH_PD14MFP_PWM1_CH2},
    {PD_15, PWM_1_3, SYS_GPD_MFPH_PD15MFP_PWM1_CH3},
    {PE_0, PWM_0_0, SYS_GPE_MFPL_PE0MFP_PWM0_CH0},
    {PE_1, PWM_0_1, SYS_GPE_MFPL_PE1MFP_PWM0_CH1},
    {PE_2, PWM_1_1, SYS_GPE_MFPL_PE2MFP_PWM1_CH1},
    {PE_3, PWM_0_3, SYS_GPE_MFPL_PE3MFP_PWM0_CH3},

    {NC,    NC,    0}
};

//*** SERIAL ***

const PinMap PinMap_UART_TX[] = {
    {PA_0, UART_1, SYS_GPA_MFPL_PA0MFP_UART1_TXD},
    {PA_2, UART_0, SYS_GPA_MFPL_PA2MFP_UART0_TXD},
    {PA_8, UART_3, SYS_GPA_MFPH_PA8MFP_UART3_TXD},
    {PB_1, UART_2, SYS_GPB_MFPL_PB1MFP_UART2_TXD},
#if defined(TARGET_M45xD_M45xC)
    {PB_3, UART_1, SYS_GPB_MFPL_PB3MFP_UART1_TXD},
#endif
    {PB_3, UART_3, SYS_GPB_MFPL_PB3MFP_UART3_TXD},
#if defined(TARGET_M45xD_M45xC)
    {PB_4, UART_2, SYS_GPB_MFPL_PB4MFP_UART2_TXD},
    {PC_0, UART_3, SYS_GPC_MFPL_PC0MFP_UART3_TXD},
#endif
    {PC_2, UART_2, SYS_GPC_MFPL_PC2MFP_UART2_TXD},
#if defined(TARGET_M45xD_M45xC)
    {PC_6, UART_0, SYS_GPC_MFPL_PC6MFP_UART0_TXD},
#endif
    {PD_1, UART_0, SYS_GPD_MFPL_PD1MFP_UART0_TXD},
    {PD_12, UART_3, SYS_GPD_MFPH_PD12MFP_UART3_TXD},
    {PE_8, UART_1, SYS_GPE_MFPH_PE8MFP_UART1_TXD},
#if defined(TARGET_M45xD_M45xC)
    {PE_10, UART_3, SYS_GPE_MFPH_PE10MFP_UART3_TXD},
#endif
    {PE_12, UART_1, SYS_GPE_MFPH_PE12MFP_UART1_TXD},
    
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RX[] = {
    {PA_1, UART_1, SYS_GPA_MFPL_PA1MFP_UART1_RXD},
    {PA_3, UART_0, SYS_GPA_MFPL_PA3MFP_UART0_RXD},
    {PA_9, UART_3, SYS_GPA_MFPH_PA9MFP_UART3_RXD},
    {PB_0, UART_2, SYS_GPB_MFPL_PB0MFP_UART2_RXD},
#if defined(TARGET_M45xD_M45xC)
    {PB_2, UART_1, SYS_GPB_MFPL_PB2MFP_UART1_RXD},
#endif
    {PB_2, UART_3, SYS_GPB_MFPL_PB2MFP_UART3_RXD},
#if defined(TARGET_M45xD_M45xC)
    {PB_5, UART_2, SYS_GPB_MFPL_PB5MFP_UART2_RXD},
    {PC_1, UART_3, SYS_GPC_MFPL_PC1MFP_UART3_RXD},
#endif
    {PC_3, UART_2, SYS_GPC_MFPL_PC3MFP_UART2_RXD},
#if defined(TARGET_M45xD_M45xC)
    {PC_7, UART_0, (int) SYS_GPC_MFPL_PC7MFP_UART0_RXD},
#endif
    {PD_0, UART_0, SYS_GPD_MFPL_PD0MFP_UART0_RXD},
    {PD_6, UART_0, SYS_GPD_MFPL_PD6MFP_UART0_RXD},
    {PD_13, UART_3, SYS_GPD_MFPH_PD13MFP_UART3_RXD},
    {PE_9, UART_1, SYS_GPE_MFPH_PE9MFP_UART1_RXD},
#if defined(TARGET_M45xD_M45xC)
    {PE_11, UART_3, SYS_GPE_MFPH_PE11MFP_UART3_RXD},
#endif
    {PE_13, UART_1, SYS_GPE_MFPH_PE13MFP_UART1_RXD},
    
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RTS[] = {
    {PA_1, UART_1, SYS_GPA_MFPL_PA1MFP_UART1_nRTS},
    {PA_3, UART_0, SYS_GPA_MFPL_PA3MFP_UART0_nRTS},
    {PA_11, UART_3, SYS_GPA_MFPH_PA11MFP_UART3_nRTS},
    {PA_15, UART_2, SYS_GPA_MFPH_PA15MFP_UART2_nRTS},
    {PB_8, UART_1, SYS_GPB_MFPH_PB8MFP_UART1_nRTS},
    {PC_1, UART_2, SYS_GPC_MFPL_PC1MFP_UART2_nRTS},
    {PD_15, UART_3, SYS_GPD_MFPH_PD15MFP_UART3_nRTS},
    {PE_11, UART_1, SYS_GPE_MFPH_PE11MFP_UART1_nRTS},
    
    {NC,    NC,     0}
};

const PinMap PinMap_UART_CTS[] = {
    {PA_0, UART_1, SYS_GPA_MFPL_PA0MFP_UART1_nCTS},
    {PA_2, UART_0, SYS_GPA_MFPL_PA2MFP_UART0_nCTS},
    {PA_10, UART_3, SYS_GPA_MFPH_PA10MFP_UART3_nCTS},
    {PA_14, UART_2, SYS_GPA_MFPH_PA14MFP_UART2_nCTS},
    {PB_4, UART_1, SYS_GPB_MFPL_PB4MFP_UART1_nCTS},
    {PC_0, UART_2, SYS_GPC_MFPL_PC0MFP_UART2_nCTS},
    {PD_14, UART_3, SYS_GPD_MFPH_PD14MFP_UART3_nCTS},
    {PE_10, UART_1, SYS_GPE_MFPH_PE10MFP_UART1_nCTS},
    
    {NC,    NC,     0}
};

//*** SPI ***

const PinMap PinMap_SPI_MOSI[] = {
    {PA_5, SPI_1, SYS_GPA_MFPL_PA5MFP_SPI1_MOSI},
    {PB_5, SPI_0, SYS_GPB_MFPL_PB5MFP_SPI0_MOSI0},
    {PB_5, SPI_1, SYS_GPB_MFPL_PB5MFP_SPI1_MOSI},
    {PC_3, SPI_2, SYS_GPC_MFPL_PC3MFP_SPI2_MOSI},
    {PC_10, SPI_2, SYS_GPC_MFPH_PC10MFP_SPI2_MOSI},
    {PD_13, SPI_2, SYS_GPD_MFPH_PD13MFP_SPI2_MOSI},
    {PE_3, SPI_1, SYS_GPE_MFPL_PE3MFP_SPI1_MOSI},
    {PE_11, SPI_1, SYS_GPE_MFPH_PE11MFP_SPI1_MOSI},
    {PE_11, SPI_0, SYS_GPE_MFPH_PE11MFP_SPI0_MOSI0},
    
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_MISO[] = {
    {PA_6, SPI_1, SYS_GPA_MFPL_PA6MFP_SPI1_MISO},
    {PB_3, SPI_0, SYS_GPB_MFPL_PB3MFP_SPI0_MISO0},
    {PB_3, SPI_1, SYS_GPB_MFPL_PB3MFP_SPI1_MISO},
    {PB_6, SPI_0, SYS_GPB_MFPL_PB6MFP_SPI0_MISO0},
    {PB_6, SPI_1, SYS_GPB_MFPL_PB6MFP_SPI1_MISO},
    {PC_4, SPI_2, SYS_GPC_MFPL_PC4MFP_SPI2_MISO},
    {PC_11, SPI_2, SYS_GPC_MFPH_PC11MFP_SPI2_MISO},
    {PD_5, SPI_1, SYS_GPD_MFPL_PD5MFP_SPI1_MISO},
    {PD_14, SPI_2, SYS_GPD_MFPH_PD14MFP_SPI2_MISO},
    {PE_10, SPI_1, SYS_GPE_MFPH_PE10MFP_SPI1_MISO},
    {PE_10, SPI_0, SYS_GPE_MFPH_PE10MFP_SPI0_MISO0},
    
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_SCLK[] = {
    {PA_7, SPI_1, SYS_GPA_MFPL_PA7MFP_SPI1_CLK},
    {PB_2, SPI_0, SYS_GPB_MFPL_PB2MFP_SPI0_CLK},
    {PB_2, SPI_1, SYS_GPB_MFPL_PB2MFP_SPI1_CLK},
    {PB_7, SPI_0, SYS_GPB_MFPL_PB7MFP_SPI0_CLK},
    {PB_7, SPI_1, SYS_GPB_MFPL_PB7MFP_SPI1_CLK},
    {PC_0, SPI_2, SYS_GPC_MFPL_PC0MFP_SPI2_CLK},
    {PC_12, SPI_2, SYS_GPC_MFPH_PC12MFP_SPI2_CLK},
    {PD_4, SPI_1, SYS_GPD_MFPL_PD4MFP_SPI1_CLK},
    {PD_15, SPI_2, SYS_GPD_MFPH_PD15MFP_SPI2_CLK},
    {PE_0, SPI_2, SYS_GPE_MFPL_PE0MFP_SPI2_CLK},
    {PE_13, SPI_1, SYS_GPE_MFPH_PE13MFP_SPI1_CLK},
    {PE_13, SPI_0, SYS_GPE_MFPH_PE13MFP_SPI0_CLK},
    
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {PA_4, SPI_1, SYS_GPA_MFPL_PA4MFP_SPI1_SS},
    {PB_4, SPI_0, SYS_GPB_MFPL_PB4MFP_SPI0_SS},
    {PB_4, SPI_1, SYS_GPB_MFPL_PB4MFP_SPI1_SS},
    {PC_2, SPI_2, SYS_GPC_MFPL_PC2MFP_SPI2_SS},
    {PC_13, SPI_2, SYS_GPC_MFPH_PC13MFP_SPI2_SS},
    {PD_6, SPI_1, SYS_GPD_MFPL_PD6MFP_SPI1_SS},
    {PD_12, SPI_2, SYS_GPD_MFPH_PD12MFP_SPI2_SS},
    {PE_12, SPI_1, SYS_GPE_MFPH_PE12MFP_SPI1_SS},
    {PE_12, SPI_0, SYS_GPE_MFPH_PE12MFP_SPI0_SS},
    
    {NC,    NC,    0}
};

const PinMap PinMap_CAN_TD[] = {
    {PC_0, CAN_0, SYS_GPC_MFPL_PC0MFP_CAN0_TXD},
    {PA_1, CAN_0, SYS_GPA_MFPL_PA1MFP_CAN0_TXD},
    {PA_12, CAN_0, SYS_GPA_MFPH_PA12MFP_CAN0_TXD},
	
    {NC,    NC,     0}
};
    
const PinMap PinMap_CAN_RD[] = { 
    {PC_1, CAN_0, SYS_GPC_MFPL_PC1MFP_CAN0_RXD},
    {PA_0, CAN_0, SYS_GPA_MFPL_PA0MFP_CAN0_RXD},
    {PA_13, CAN_0, SYS_GPA_MFPH_PA13MFP_CAN0_RXD},
	
    {NC,    NC,    0}
};
