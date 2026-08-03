/* SPDX-License-Identifier: Apache-2.0 */
#include "PeripheralPins.h"
#include "mbed_toolchain.h"

MBED_WEAK const PinMap PinMap_GPIO[] = {
    {PA_0, 0, GPIO_NOPULL}, {PA_1, 0, GPIO_NOPULL},
    {PA_2, 0, GPIO_NOPULL}, {PA_3, 0, GPIO_NOPULL},
    {PA_8, 0, GPIO_NOPULL}, {PA_9, 0, GPIO_NOPULL},
    {PA_10, 0, GPIO_NOPULL}, {PA_11, 0, GPIO_NOPULL},
    {PB_0, 0, GPIO_NOPULL}, {PB_1, 0, GPIO_NOPULL},
    {PB_2, 0, GPIO_NOPULL}, {PB_3, 0, GPIO_NOPULL},
    {PB_4, 0, GPIO_NOPULL}, {PB_5, 0, GPIO_NOPULL},
    {PB_6, 0, GPIO_NOPULL}, {PB_7, 0, GPIO_NOPULL},
    {PB_12, 0, GPIO_NOPULL}, {PB_13, 0, GPIO_NOPULL},
    {PB_14, 0, GPIO_NOPULL}, {PB_15, 0, GPIO_NOPULL},
    {NC, NC, 0}
};
