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

#include <cctype>
#include <cstdio>
#include <cstring>
#include "greentea-client/test_env.h"

/**
 *   Generic test suite transport protocol keys
 */
const char *GREENTEA_TEST_ENV_END = "end";
const char *GREENTEA_TEST_ENV_EXIT = "__exit";
const char *GREENTEA_TEST_ENV_SYNC = "__sync";
const char *GREENTEA_TEST_ENV_TIMEOUT = "__timeout";
const char *GREENTEA_TEST_ENV_HOST_TEST_NAME = "__host_test_name";
const char *GREENTEA_TEST_ENV_HOST_TEST_VERSION = "__version";

/**
 *   Test suite success code strings
 */
const char *GREENTEA_TEST_ENV_SUCCESS = "success";
const char *GREENTEA_TEST_ENV_FAILURE = "failure";

/**
 *   Test case transport protocol start/finish keys
 */
const char *GREENTEA_TEST_ENV_TESTCASE_NAME = "__testcase_name";
const char *GREENTEA_TEST_ENV_TESTCASE_COUNT = "__testcase_count";
const char *GREENTEA_TEST_ENV_TESTCASE_START = "__testcase_start";
const char *GREENTEA_TEST_ENV_TESTCASE_FINISH = "__testcase_finish";
const char *GREENTEA_TEST_ENV_TESTCASE_SUMMARY = "__testcase_summary";
// Code Coverage (LCOV)  transport protocol keys
const char *GREENTEA_TEST_ENV_LCOV_START = "__coverage_start";

/**
 *   Auxilary functions
 */
static void greentea_notify_timeout(const int);
static void greentea_notify_hosttest(const char *);
static void greentea_notify_completion(const int);
static void greentea_notify_version();

/**
 * Handle the handshake with the host.
 *
 * @details This function contains the shared handshake functionality that is used between
 *          GREENTEA_SETUP and GREENTEA_SETUP_UUID.
 *
 * @note This function is blocking.
 */
static void _GREENTEA_SETUP_COMMON(const int timeout, const char *host_test_name, char *buffer, size_t size)
{
    // Key-value protocol handshake function. Waits for {{__sync;...}} message
    // Sync preamble: "{{__sync;0dad4a9d-59a3-4aec-810d-d5fb09d852c1}}"
    // Example value of sync_uuid == "0dad4a9d-59a3-4aec-810d-d5fb09d852c1"

    char _key[8] = {0};

    while (1) {
        greentea_parse_kv(_key, buffer, sizeof(_key), size);
        greentea_write_string("mbedmbedmbedmbedmbedmbedmbedmbed\r\n");
        if (strcmp(_key, GREENTEA_TEST_ENV_SYNC) == 0) {
            // Found correct __sync message
            greentea_send_kv(_key, buffer);
            break;
        }
    }

    greentea_notify_version();
    greentea_notify_timeout(timeout);
    greentea_notify_hosttest(host_test_name);
}

extern "C" void GREENTEA_SETUP(const int timeout, const char *host_test_name)
{
#if ! defined(NO_GREENTEA)
    char _value[GREENTEA_UUID_LENGTH] = {0};
    _GREENTEA_SETUP_COMMON(timeout, host_test_name, _value, GREENTEA_UUID_LENGTH);
#endif
}

void GREENTEA_SETUP_UUID(const int timeout, const char *host_test_name, char *buffer, size_t size)
{
    _GREENTEA_SETUP_COMMON(timeout, host_test_name, buffer, size);
}

void GREENTEA_TESTSUITE_RESULT(const int result)
{
    greentea_notify_completion(result);
}

void GREENTEA_TESTCASE_START(const char *test_case_name)
{
    greentea_send_kv(GREENTEA_TEST_ENV_TESTCASE_START, test_case_name);
}

void GREENTEA_TESTCASE_FINISH(const char *test_case_name, const size_t passes, const size_t failed)
{
    greentea_send_kv(GREENTEA_TEST_ENV_TESTCASE_FINISH, test_case_name, passes, failed);
}

/**
 *****************************************************************************
 *  Auxilary functions and key-value protocol support
 *****************************************************************************
 */


/**
 *****************************************************************************
 *  LCOV support
 *****************************************************************************
 */
#ifdef GREENTEA_CLIENT_COVERAGE_REPORT_NOTIFY
extern "C" void __gcov_flush(void);
extern bool coverage_report;

void greentea_notify_coverage_start(const char *path)
{
    printf("{{%s;%s;", GREENTEA_TEST_ENV_LCOV_START, path);
}

void greentea_notify_coverage_end()
{
    printf("}}" NL);
}

#endif

/**
 *****************************************************************************
 *  Key-value protocol support
 *****************************************************************************
 */

/**
 * Write the preamble characters to the stream.
 *
 * @details This function writes the preamble "{{" which is required
 *          for key-value comunication between the target and the host.
 *          This uses greentea_putc which allows the direct writing of characters
 *          using the write() method.
 *          This suite of functions are provided to allow for serial communication
 *          to the host from within a thread/ISR.
 */
static void greentea_write_preamble()
{
    greentea_putc('{');
    greentea_putc('{');
}

/**
 * Write the postamble characters to the stream.
 *
 * @details This function writes the postamble "{{\r\n" which is required
 *          for key-value comunication between the target and the host.
 *          This uses greentea_putc which allows the direct writing of characters
 *          using the write() method.
 *          This suite of functions are provided to allow for serial communication
 *          to the host from within a thread/ISR.
 */
static void greentea_write_postamble()
{
    greentea_putc('}');
    greentea_putc('}');
    greentea_putc('\r');
    greentea_putc('\n');
}

/**
 * Write an int to the serial port.
 *
 * @details This function writes an integer value from the target
 *          to the host. The integer value is converted to a string and
 *          and then written character by character directly to the serial
 *          port using the console.
 *          The thread-safe sprintf() is used to convert the int to a string.
 *
 * @param val Integer value.
 */
#define MAX_INT_STRING_LEN 15
static void greentea_write_int(const int val)
{
    char intval[MAX_INT_STRING_LEN];
    unsigned int i = 0;
    sprintf(intval, "%d", val);
    while (intval[i] != '\0') {
        greentea_putc(intval[i]);
        i++;
    }
}

extern "C" void greentea_send_kv(const char *key, const char *val)
{
    if (key && val) {
        greentea_write_preamble();
        greentea_write_string(key);
        greentea_putc(';');
        greentea_write_string(val);
        greentea_write_postamble();
    }
}

void greentea_send_kv(const char *key, const int val)
{
    if (key) {
        greentea_write_preamble();
        greentea_write_string(key);
        greentea_putc(';');
        greentea_write_int(val);
        greentea_write_postamble();
    }
}

void greentea_send_kv(const char *key, const char *val, const int result)
{
    if (key) {
        greentea_write_preamble();
        greentea_write_string(key);
        greentea_putc(';');
        greentea_write_string(val);
        greentea_putc(';');
        greentea_write_int(result);
        greentea_write_postamble();

    }
}

void greentea_send_kv(const char *key, const char *val, const int passes, const int failures)
{
    if (key) {
        greentea_write_preamble();
        greentea_write_string(key);
        greentea_putc(';');
        greentea_write_string(val);
        greentea_putc(';');
        greentea_write_int(passes);
        greentea_putc(';');
        greentea_write_int(failures);
        greentea_write_postamble();
    }
}

void greentea_send_kv(const char *key, const int passes, const int failures)
{
    if (key) {
        greentea_write_preamble();
        greentea_write_string(key);
        greentea_putc(';');
        greentea_write_int(passes);
        greentea_putc(';');
        greentea_write_int(failures);
        greentea_write_postamble();
    }
}

/**
 * Send a message with timeout in seconds to the host.
 *
 * @details GREENTEA_TEST_ENV_TIMEOUT message is part of the preamble
 *          sent from the DUT (device under test) to the host during synchronisation
 *          (beginning of test suite execution). It notifies the host of the total test
 *          suite timeout. Test execution time is measured from the moment of
 *          GREENTEA_TEST_ENV_TIMEOUT reception by the host.
 *          If the timeout is reached, the host (and host test) will be stopped and
 *          control will return to Greentea.
 *
 * @param timeout Test suite timeout in seconds
 */
static void greentea_notify_timeout(const int timeout)
{
    greentea_send_kv(GREENTEA_TEST_ENV_TIMEOUT, timeout);
}

/**
 * Send a host test name to the host.
 *
 * @details GREENTEA_TEST_ENV_HOST_TEST_NAME message is part of the preamble
 *          sent from the DUT (device under test) to the host during synchronisation
 *          (beginning of test suite execution).
 *          The host test Python script implements host side callbacks
 *          for key-value events sent from the DUT (device under test) to the host.
 *          The host test's callbacks are registered after GREENTEA_TEST_ENV_HOST_TEST_NAME
 *          message reaches the host.
 *
 * @param host_test_name Host test name, host test will be loaded by mbedhtrun
 */
static void greentea_notify_hosttest(const char *host_test_name)
{
    greentea_send_kv(GREENTEA_TEST_ENV_HOST_TEST_NAME, host_test_name);
}

/**
 * Send to the host information that test suite finished its execution.
 *
 * @details GREENTEA_TEST_ENV_END and GREENTEA_TEST_ENV_EXIT messages
 *          are sent just before test suite execution finishes (nothing
 *          else to do). You can place it just before you return from your
 *          main() function.
 *
 * @notes Code coverage: If GREENTEA_CLIENT_COVERAGE_REPORT_NOTIFY is set in the
 *        project via build configuration, this function will output a series
 *        of code coverage messages GREENTEA_TEST_ENV_LCOV_START with code
 *        coverage binary data. This data is captured by Greentea and can
 *        be used to generate LCOV reports.
 *
 * @param result Test suite result from DUT (device under test) (0 - FAIl, !0 - SUCCESS)
 */
static void greentea_notify_completion(const int result)
{
    const char *val = result ? GREENTEA_TEST_ENV_SUCCESS : GREENTEA_TEST_ENV_FAILURE;
#ifdef GREENTEA_CLIENT_COVERAGE_REPORT_NOTIFY
    coverage_report = true;
    __gcov_flush();
    coverage_report = false;
#endif
    greentea_send_kv(GREENTEA_TEST_ENV_END, val);
    greentea_send_kv(GREENTEA_TEST_ENV_EXIT, 0);
}

/**
 * \brief Send to the host greentea-client's version.
 */
static void greentea_notify_version()
{
    greentea_send_kv(GREENTEA_TEST_ENV_HOST_TEST_VERSION, GREENTEA_CLIENT_VERSION_STRING);
}

/**
 *****************************************************************************
 *  Parse engine for KV values which replaces scanf
 *****************************************************************************
 *
 *  Example usage:
 *
 *  char key[10];
 *  char value[48];
 *
 *  greentea_parse_kv(key, value, 10, 48);
 *  greentea_parse_kv(key, value, 10, 48);
 *
 */


static int gettok(char *, const int);
static int getNextToken(char *, const int);
static int HandleKV(char *,  char *,  const int,  const int);
static int isstring(int);

/**
 * Current token of key-value protocol's tokenizer
 */
static int CurTok = 0;

/**
 * @enum Token enumeration for key-value protocol tokenizer
 *
 *       This enum is used by key-value protocol tokenizer
 *       to detect parts of protocol in stream.
 *
 *       tok_eof       ::= EOF (end of file)
 *       tok_open      ::= "{{"
 *       tok_close     ::= "}}"
 *       tok_semicolon ::= ";"
 *       tok_string    ::= [a-zA-Z0-9_-!@#$%^&*()]+    // See isstring() function
 */
enum Token {
    tok_eof = -1,
    tok_open = -2,
    tok_close = -3,
    tok_semicolon = -4,
    tok_string = -5
};

extern "C" int greentea_parse_kv(char *out_key,
                                 char *out_value,
                                 const int out_key_size,
                                 const int out_value_size)
{
    getNextToken(0, 0);
    while (1) {
        switch (CurTok) {
            case tok_eof:
                return 0;

            case tok_open:
                if (HandleKV(out_key, out_value, out_key_size, out_value_size)) {
                    // We've found {{ KEY ; VALUE }} expression
                    return 1;
                }
                break;

            default:
                // Load next token and pray...
                getNextToken(0, 0);
                break;
        }
    }
}

/**
 *  Get the next token from the stream.
 *
 *  Key-value TOKENIZER feature.
 *
 *  @details This function is used by the key-value parser to determine
 *           if the key-value message is embedded in the data stream.
 *
 *  @param str Output parameters to store token string value
 *  @param str_size Size of 'str' parameter in bytes (characters)
 */
static int getNextToken(char *str, const int str_size)
{
    return CurTok = gettok(str, str_size);
}

/**
 *  Check if a character is a punctuation.
 *
 *  Auxilary key-value TOKENIZER function.
 *
 *  @details Defines if character is in subset of allowed punctuation
 *           characters which can be part of a key or value string.
 *           Invalid punctuation characters are: ";{}"
 *
 *  @param c Input character to check
 *  @return Return 1 if character is allowed punctuation character, otherwise return 0
 *
 */
static int ispunctuation(int c)
{
    static const char punctuation[] = "_-!@#$%^&*()=+:<>,./?\\\"'";  // No ";{}"
    for (size_t i = 0; i < sizeof(punctuation); ++i) {
        if (c == punctuation[i]) {
            return 1;
        }
    }
    return 0;
}

/**
 *  Check if character is string token character.
 *
 *  Auxilary key-value TOKENIZER function.
 *
 *  @details Defines if character is in subset of allowed string
 *           token characters.
 *           String defines set of characters which can be a key or value string.
 *
 *           Allowed subset includes:
 *           - Alphanumerical characters
 *           - Digits
 *           - White spaces and
 *           - subset of punctuation characters.
 *
 *  @param c Input character to check
 *  @return Return 1 if character is allowed punctuation character, otherwise return false
 *
 */
static int isstring(int c)
{
    return (isalpha(c) ||
            isdigit(c) ||
            isspace(c) ||
            ispunctuation(c));
}

/**
 *  TOKENIZER of key-value protocol.
 *
 *  Actual key-value TOKENIZER engine.
 *
 *  @details TOKENIZER defines #Token enum to map recognized tokens to integer values.
 *
 *           <TOK_EOF>       ::= EOF (end of file)
 *           <TOK_OPEN>      ::= "{{"
 *           <TOK_CLOSE>     ::= "}}"
 *           <TOK_SEMICOLON> ::= ";"
 *           <TOK_STRING>    ::= [a-zA-Z0-9_-!@#$%^&*()]+    // See isstring() function *
 *
 *  @param out_str Output string with parsed token (string)
 *  @param str_size Size of str buffer we can use
 *
 *  @return Return #Token enum value used by parser to check for key-value occurrences
 *
 */
static int gettok(char *out_str, const int str_size)
{
    static int LastChar = '!';
    static int str_idx = 0;

    // whitespace ::=
    while (isspace(LastChar)) {
        LastChar = greentea_getc();
    }

    // string ::= [a-zA-Z0-9_-!@#$%^&*()]+
    if (isstring(LastChar)) {
        str_idx = 0;
        if (out_str && str_idx < str_size - 1) {
            out_str[str_idx++] = LastChar;
        }

        while (isstring((LastChar = greentea_getc())))
            if (out_str && str_idx < str_size - 1) {
                out_str[str_idx++] = LastChar;
            }
        if (out_str && str_idx < str_size) {
            out_str[str_idx] = '\0';
        }

        return tok_string;
    }

    // semicolon ::= ';'
    if (LastChar == ';') {
        LastChar = greentea_getc();
        return tok_semicolon;
    }

    // open ::= '{{'
    if (LastChar == '{') {
        LastChar = greentea_getc();
        if (LastChar == '{') {
            LastChar = greentea_getc();
            return tok_open;
        }
    }

    // close ::= '}'
    if (LastChar == '}') {
        LastChar = greentea_getc();
        if (LastChar == '}') {
            greentea_getc(); //offset the extra '\n' send by Greentea python tool
            LastChar = '!';
            return tok_close;
        }
    }

    if (LastChar == EOF) {
        return tok_eof;
    }

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = greentea_getc();
    return ThisChar;
}

/**
 *  Key-value parser.
 *
 *  @details Key-value message grammar.
 *
 *           <MESSAGE>: <TOK_OPEN> <TOK_STRING> <TOK_SEMICOLON> <TOK_STRING> <TOK_CLOSE>
 *
 *           Examples:
 *           message:     "{{__timeout; 1000}}"
 *                        "{{__sync; 12345678-1234-5678-1234-567812345678}}"
 *
 *  @param out_key Output buffer to store key string value
 *  @param out_value Output buffer to store value string value
 *  @param out_key_size Buffer 'out_key' buffer size
 *  @param out_value_size Buffer 'out_value_size' buffer size
 *
 *  @return Returns 1 if key-value message was parsed successfully in stream of tokens from tokenizer
 */
static int HandleKV(char *out_key,
                    char *out_value,
                    const int out_key_size,
                    const int out_value_size)
{
    // We already started with <open>
    if (getNextToken(out_key, out_key_size) == tok_string) {
        if (getNextToken(0, 0) == tok_semicolon) {
            if (getNextToken(out_value, out_value_size) == tok_string) {
                if (getNextToken(0, 0) == tok_close) {
                    // <open> <string> <semicolon> <string> <close>
                    // Found "{{KEY;VALUE}}" expression
                    return 1;
                }
            }
        }
    }
    getNextToken(0, 0);
    return 0;
}
