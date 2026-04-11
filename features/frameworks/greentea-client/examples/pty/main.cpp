/*
 * Copyright (c) 2021, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstdio>
#include <cstring>
#include <unistd.h>

// For PTY (pseudo-terminals) which are available on most UNIX-like
// systems but for simplicity we only check macOS and Linux.
// Windows has an entirely different API for terminals which this
// demo does not support yet.
#if defined(__APPLE__)
#include <util.h>
#elif defined(__linux__)
#include <pty.h>
#else
#error "This example supports macOS and Linux only."
#endif

#include "greentea-client/test_env.h"

/* Implementation of test_io.h */

static int pty_master;
static int pty_slave;

int greentea_getc()
{
    char out;
    ssize_t bytes = read(pty_master, &out, sizeof(out));
    if (bytes == sizeof(char)) {
        return out;
    } else {
        printf("Error: greentea_getc failed\r\n");
        return EOF;
    }
}

void greentea_putc(int c)
{
    char in = c;
    ssize_t bytes = write(pty_master, &in, sizeof(in));
    if (bytes != sizeof(in)) {
        printf("Error: greentea_putc failed\r\n");
    }
}

void greentea_write_string(const char *str)
{
    size_t len = strlen(str);
    ssize_t bytes = write(pty_master, str, len);
    if (bytes != len) {
        printf("Error: greentea_write_string failed\r\n");
    }
}

/* Example */

int main()
{
    char tty_name[64];

    // Obtain pseudo-terminals for communication with htrun.
    int ret = openpty(&pty_master, &pty_slave, tty_name, nullptr, nullptr);
    if (ret != 0) {
        printf("Failed to obtain PTY: %d\r\n", ret);
        return ret;
    }

    printf("In another terminal, please go to the same repository and run:\r\n");
    printf("  mbedhtrun --skip-flashing --skip-reset -e ./examples/pty -p %s\r\n", tty_name);

    // Wait for htrun to connect.
    // The client will instruct htrun to use example_host.py and test is
    // expected to take less than 20 seconds AFTER connection is established.
    GREENTEA_SETUP(/* timeout */ 20, /* host_test */ "example_host");

    printf("Host connected\r\n");

    printf("Sending greetings to the host...\r\n");
    greentea_send_kv("device_greetings", "Hello from the device!");
    printf("Sent\r\n");

    printf("Expecting greetings from the host...\r\n");
    char key[64];
    char value[64];
    greentea_parse_kv(key, value, sizeof(key), sizeof(value));
    printf("Message received from the host: %s\r\n", value);

    printf("Notifying test success to the host\r\n");
    GREENTEA_TESTSUITE_RESULT(1);
    printf("Sent\r\n");

    // It takes time for the htrun to read the test result, so we can't close
    // the pseudo-terminals immediately. And htrun does not send any confirmation,
    // so we ask the user to end the demo with a key press.
    // By contrast, the debug serial of an embedded device is assumed always available.
    printf("Once mbedhtrun has reported results, press [Enter] to end this demo\r\n");
    getchar();
    close(pty_master);
    close(pty_slave);

    printf("End of the demo\r\n");

    return 0;
}
