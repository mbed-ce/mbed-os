/*
 * Copyright (c) 2013-2021, ARM Limited, All Rights Reserved
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

#ifndef GREENTEA_CLIENT_TEST_ENV_H_
#define GREENTEA_CLIENT_TEST_ENV_H_

#include <stddef.h>
#include "greentea-client/test_io.h"

#ifdef __cplusplus
#define GREENTEA_CLIENT_VERSION_STRING "1.3.0"

/**
 *  Auxilary macros
 */
#ifndef NL
#define NL "\n"
#endif
#ifndef RCNL
#define RCNL "\r\n"
#endif

/**
 * Ensure compatibility with utest
 */
#define TEST_ENV_TESTCASE_COUNT     GREENTEA_TEST_ENV_TESTCASE_COUNT
#define TEST_ENV_TESTCASE_START     GREENTEA_TEST_ENV_TESTCASE_START
#define TEST_ENV_TESTCASE_FINISH    GREENTEA_TEST_ENV_TESTCASE_FINISH
#define TEST_ENV_TESTCASE_SUMMARY   GREENTEA_TEST_ENV_TESTCASE_SUMMARY

/**
 *  Default length for UUID buffers (used during the sync process)
 */
#define GREENTEA_UUID_LENGTH        48

/**
 *  Generic test suite transport protocol keys
 */
extern const char *GREENTEA_TEST_ENV_END;
extern const char *GREENTEA_TEST_ENV_EXIT;
extern const char *GREENTEA_TEST_ENV_SYNC;
extern const char *GREENTEA_TEST_ENV_TIMEOUT;
extern const char *GREENTEA_TEST_ENV_HOST_TEST_NAME;
extern const char *GREENTEA_TEST_ENV_HOST_TEST_VERSION;

/**
 *  Test suite success code strings
 */
extern const char *GREENTEA_TEST_ENV_SUCCESS;
extern const char *GREENTEA_TEST_ENV_FAILURE;

/**
 *  Test case transport protocol start/finish keys
 */
extern const char *GREENTEA_TEST_ENV_TESTCASE_NAME;
extern const char *GREENTEA_TEST_ENV_TESTCASE_COUNT;
extern const char *GREENTEA_TEST_ENV_TESTCASE_START;
extern const char *GREENTEA_TEST_ENV_TESTCASE_FINISH;
extern const char *GREENTEA_TEST_ENV_TESTCASE_SUMMARY;

/**
 *  Code Coverage (LCOV)  transport protocol keys
 */
extern const char *GREENTEA_TEST_ENV_LCOV_START;

/**
 *  Greentea-client related API for communication with host side
 */

/**
 * Handshake with the host and send setup data (timeout and host test name). Allows you to preserve the sync UUID.
 *
 * @details This function will send a preamble to the host.
 *
 * @note After the host test name is received the host will invoke the relevant host test script
 *       and add the host test's callback handlers to the main event loop.
 * @note This function is blocking.
 *
 * @param timeout Maximum number of seconds allowed from the start to the end of the tests
 * @param host_test Name of the host test
 * @param buffer Buffer to save the UUID into
 * @param size Size of the buffer
 */
void GREENTEA_SETUP_UUID(const int timeout, const char *host_test_name, char *buffer, size_t size);

/**
 * Notify the host side that test suite execution was complete.
 *
 * @details This sends an __exit message.
 *
 * @note If __exit is not received by the host side it will assume TIMEOUT.
 *
 * @param result Test suite result
 */
void GREENTEA_TESTSUITE_RESULT(const int result);

/**
 *  Test Case support
 */

/**
 * Notify the host side that a test case started.
 *
 * @param test_case_name Test case name
 */
void GREENTEA_TESTCASE_START(const char *test_case_name);

/**
 * Notify the host side that a test case finished.
 *
 * @param test_case_name Test case name
 * @param passes Number of test passes
 * @param failures Number of test failures
 */
void GREENTEA_TESTCASE_FINISH(const char *test_case_name, const size_t passes, const size_t failures);

/**
 *  Test suite result related notification API
 */

/**
 * Encapsulate and send a key-value message from the DUT (device under test) to the host
 *
 * @param key Message key (message/event name)
 * @param value Message payload, integer value
 */
void greentea_send_kv(const char *key, const int value);

/**
 * Encapsulate and send a key-value-value message from the DUT (device under test) to the host
 *
 * @note Names of the parameters: This function is used to send numbers
 *       of passes and failures to the host. But it can be used to send any
 *       key-value-value (string-integer-integer) message to the host.
 *
 * @param key Message key (message/event name)
 * @param passes Send additional integer formatted data
 * @param failures Send additional integer formatted data
 */
void greentea_send_kv(const char *key, const int passes, const int failures);

/**
 * Encapsulate and send a key-value-value message from the DUT (device under test) to the host
 *
 * @note Names of the parameters: this function is used to send test case
 *       name with result to the host. But it can be used
 *       to send any key-value-value (string-string-integer).
 *
 * @param key Message key (message/event name)
 * @param value Message payload, string value
 * @param result Send additional integer formatted data
 */
void greentea_send_kv(const char *key, const char *value, const int result);


/**
 * Encapsulate and send a key-value-value-value message from the DUT (device under test) to the host
 *
 * @note Names of the parameters: this function is used to send test case
 *       name with number of passes and failures to the host. But it can be used
 *       to send any key-value-value-value (string-string-integer-integer).
 *
 * @param key Message key (message/event name)
 * @param value Message payload, string value
 * @param passes Send additional integer formatted data
 * @param failures Send additional integer formatted data
 */
void greentea_send_kv(const char *key, const char *value, const int passes, const int failures);

#ifdef GREENTEA_CLIENT_COVERAGE_REPORT_NOTIFY
/**
 *  Code Coverage API
 */

/**
 * Send a code coverage (gcov/LCOV) notification to the host.
 * Generates a preamble of message sent to notify the host about the code coverage data dump.
 *
 * @details This function is used to generate and send code coverage
 *          messages to the host. When the code coverage feature is enabled the client will
 *          print the code coverage data in the key-value protocol format.
 *          The code coverage data message will contain the message name and a path to a code
 *          coverage output file. The host will write the code coverage binary
 *          payload to the output file. The coverage payload is encoded as stream of ASCII coded bytes ("%02X").
 *
 * @param path Path to file with code coverage payload (set by gcov instrumentation)
 */
void greentea_notify_coverage_start(const char *path);

/**
 * Suffix for the code coverage message to the host (closing statement).
 *
 * @see Companion function greentea_notify_coverage_start() defines code coverage message structure.
 */
void greentea_notify_coverage_end();
#endif  // GREENTEA_CLIENT_COVERAGE_REPORT_NOTIFY

#endif  // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Greentea-client C API
 */

/**
 * Handshake with the host and send setup data
 *
 * @details This function will send a preamble to the host.
 *          After the host test name is received the host will invoke the host test script
 *          specified by the host_test parameter and add the host test's callback handlers
 *          to its main event loop.
 *
 * @note This function is blocking.
 *
 * @param timeout Maximum number of seconds allowed from the start to the end of the tests
 * @param host_test Name of the host test
 */
void GREENTEA_SETUP(const int timeout, const char *host_test);

/**
 * Encapsulate and send a key-value message from the DUT (device under test) to the host.
 *
 * @param key Message key (message/event name)
 * @param value Message payload, string value
 */
void greentea_send_kv(const char *key, const char *val);

/**
 * Parse input strings for key-value pairs: {{key;value}}
 *       This function should replace scanf() used to
 *       check for incoming messages from the host. All data
 *       parsed and rejected is discarded.
 *
 * @note This function blocks until the full key-value message is received.
 *
 * @param out_key Ouput data with key
 * @param out_value Ouput data with value
 * @param out_key_size out_key total size
 * @param out_value_size out_value total data
 *
 * @return !0 if key-value pair was found,
 *         0 if end of the stream was found
 */
int greentea_parse_kv(char *key, char *val,
                      const int key_len, const int val_len);

#ifdef __cplusplus
}
#endif

#endif  // GREENTEA_CLIENT_TEST_ENV_H_
