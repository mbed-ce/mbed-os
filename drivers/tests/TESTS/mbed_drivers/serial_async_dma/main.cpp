/*
 * Copyright (c) 2026 Isabella Wu
 * SPDX-License-Identifier: Apache-2.0
 */

#if !DEVICE_SERIAL_ASYNCH
#error [NOT_SUPPORTED] asynchronous serial communication not supported for this target
#else

#include "greentea-client/test_env.h"
#include "hal/us_ticker_api.h"
#include "mbed.h"
#include "unity/unity.h"
#include "utest/utest.h"

using namespace utest::v1;
using namespace mbed;

#define MSG_KEY_ECHO_MESSAGE "echo_message"
#define MSG_VALUE_HELLO_WORLD "Hello, world!"
#define EXPECTED_ECHOED_STRING "{{" MSG_KEY_ECHO_MESSAGE ";" MSG_VALUE_HELLO_WORLD "}}"

class AsyncSerial : public SerialBase {
public:
    AsyncSerial(PinName tx, PinName rx, int baud) : SerialBase(tx, rx, baud)
    {
    }

    using SerialBase::read;
    using SerialBase::set_dma_usage_rx;
};

static volatile int received_event;

static void receive_complete(int event)
{
    received_event = event;
}

static void test_async_dma_receive()
{
    AsyncSerial serial(CONSOLE_TX, CONSOLE_RX, MBED_CONF_PLATFORM_STDIO_BAUD_RATE);
    uint8_t received[sizeof(EXPECTED_ECHOED_STRING)] = {};

    TEST_ASSERT_EQUAL_INT(0, serial.set_dma_usage_rx(DMA_USAGE_ALWAYS));
    TEST_ASSERT_EQUAL_INT(0, serial.read(received, sizeof(received) - 1,
                                         callback(receive_complete), SERIAL_EVENT_RX_COMPLETE));

    greentea_send_kv(MSG_KEY_ECHO_MESSAGE, MSG_VALUE_HELLO_WORLD);

    const us_timestamp_t timeout = ticker_read_us(get_us_ticker_data()) + 5000000;
    while ((received_event == 0) && (ticker_read_us(get_us_ticker_data()) < timeout)) {
    }

    TEST_ASSERT_BITS(SERIAL_EVENT_RX_COMPLETE, SERIAL_EVENT_RX_COMPLETE, received_event);
    TEST_ASSERT_EQUAL_STRING(EXPECTED_ECHOED_STRING, received);
}

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "serial_comms");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Asynchronous serial reception with DMA hint", test_async_dma_receive),
};

Specification specification(greentea_setup, cases, greentea_test_teardown_handler);

int main()
{
    return !Harness::run(specification);
}

#endif
