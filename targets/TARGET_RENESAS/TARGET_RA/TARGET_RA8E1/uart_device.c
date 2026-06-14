#include "PeripheralNames.h"
#include "hal_data.h"
#include "device.h"

const uart_instance_t * const g_uart_instances[UART_COUNT] = {
    &g_uart0,
    &g_uart1,
    &g_uart2,
    &g_uart3,
    &g_uart4,
    &g_uart9,
};
