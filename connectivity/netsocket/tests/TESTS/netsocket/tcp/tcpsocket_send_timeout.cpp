/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined(MBED_CONF_RTOS_PRESENT)
#include "mbed.h"
#include "TCPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "tcp_tests.h"

using namespace utest::v1;

void TCPSOCKET_SEND_TIMEOUT()
{
    SKIP_IF_TCP_UNSUPPORTED();
    TCPSocket sock;
    if (tcpsocket_connect_to_discard_srv(sock) != NSAPI_ERROR_OK) {
        TEST_FAIL();
        return;
    }
    sock.set_blocking(false);

    int err;
    Timer timer;
    static const char tx_buffer[] = {'h', 'e', 'l', 'l', 'o'};
    for (int i = 0; i < 10; i++) {
        timer.reset();
        timer.start();
        err = sock.send(tx_buffer, sizeof(tx_buffer));
        timer.stop();
        if ((err == sizeof(tx_buffer) || err == NSAPI_ERROR_WOULD_BLOCK) &&
                (timer.elapsed_time() <= 800ms)) {
            continue;
        }
        tr_error("send: err %d, time %" PRIi64, err, std::chrono::duration_cast<chrono::milliseconds>(timer.elapsed_time()).count());
        TEST_FAIL();
        break;
    }

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
#endif // defined(MBED_CONF_RTOS_PRESENT)
