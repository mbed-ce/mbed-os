/*
 * Copyright (c) 2026 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"

using namespace utest::v1;

static BufferedSerial console(CONSOLE_TX, CONSOLE_RX, 115200);

FileHandle *mbed::mbed_override_console(int)
{
    return &console;
}

static void test_uart_rx_fifo_burst()
{
    char discarded;
    console.set_blocking(false);
    while (console.read(&discarded, 1) == 1) {
    }
    console.set_blocking(true);
    greentea_send_kv("r", "ready");
    console.sync();

    core_util_critical_section_enter();
    wait_us(100000);
    core_util_critical_section_exit();

    char received[32] = {};
    console.set_blocking(false);
    ThisThread::sleep_for(20ms);
    ssize_t count = console.read(received, sizeof(received) - 1);
    console.set_blocking(true);
    TEST_ASSERT_TRUE(count >= 13);
    TEST_ASSERT_NOT_NULL(strstr(received, "{{b;01234567}}"));
}

static void test_uart_rx_fifo_repeated()
{
    for (unsigned i = 0; i < 10; i++) {
        test_uart_rx_fifo_burst();
    }
}

static void test_uart_rx_fifo_reconfigure()
{
    console.sync();
    // sync() drains the software buffer, not the hardware TX FIFO.
    ThisThread::sleep_for(20ms);
    console.set_baud(57600);
    console.set_baud(115200);
    console.set_format(8, BufferedSerial::Even, 1);
    console.set_format(8, BufferedSerial::None, 1);
    test_uart_rx_fifo_burst();
}

static status_t setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "uart_rx_fifo");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("UART RX FIFO retains a burst while interrupts are disabled", test_uart_rx_fifo_burst),
    Case("UART RX FIFO retains repeated bursts", test_uart_rx_fifo_repeated),
    Case("UART RX FIFO survives baud and format changes", test_uart_rx_fifo_reconfigure),
};

Specification specification(setup, cases);

int main()
{
    return !Harness::run(specification);
}
