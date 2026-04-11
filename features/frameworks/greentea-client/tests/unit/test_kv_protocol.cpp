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
#include <string>

#include <gtest/gtest.h>

#include "fake_console_io.h"
#include "greentea-client/test_env.h"

class KiViProtocolTest: public testing::Test {
public:
    Console fake_console;

protected:
    KiViProtocolTest() {}
    virtual ~KiViProtocolTest() {}
    virtual void TearDown() override
    {
        fake_console = {};
    }
};

TEST_F(KiViProtocolTest, SendStringValue)
{
    const std::string key = "hello";
    const std::string value = "99";
    const std::string output = "{{" + key + ";" + value + "}}\r\n";
    greentea_send_kv(key.c_str(), value.c_str());

    const std::string console = fake_console.get_stdout();
    ASSERT_EQ(console, output);
}

TEST_F(KiViProtocolTest, SendIntValue)
{
    const std::string key = "hello";
    const int value = 99;
    const std::string output = "{{" + key + ";" + std::to_string(value) + "}}\r\n";

    greentea_send_kv(key.c_str(), value);

    const std::string console = fake_console.get_stdout();
    ASSERT_EQ(console, output);
}

TEST_F(KiViProtocolTest, SendIntPassFailCount)
{
    const std::string key = "hello";
    const int passes = 99;
    const int failures = 19;
    const std::string output = "{{" + key + ";" + std::to_string(passes) + ";" + std::to_string(failures) + "}}\r\n";

    greentea_send_kv(key.c_str(), passes, failures);

    const std::string console = fake_console.get_stdout();
    ASSERT_EQ(console, output);
}

TEST_F(KiViProtocolTest, SendStringValueAndResultCount)
{
    const std::string key = "hello";
    const std::string value = "hey";
    const int result = 99;
    const std::string output = "{{" + key + ";" + value + ";" + std::to_string(result) + "}}\r\n";

    greentea_send_kv(key.c_str(), value.c_str(), result);

    const std::string console = fake_console.get_stdout();
    ASSERT_EQ(console, output);
}

TEST_F(KiViProtocolTest, SendStringValueAndPassFailCount)
{
    const std::string key = "hello";
    const std::string value = "hey";
    const int passes = 1;
    const int failures = 99;
    const std::string output = "{{" + key + ";" + value + ";" + std::to_string(passes) + ";" + std::to_string(failures) + "}}\r\n";

    greentea_send_kv(key.c_str(), value.c_str(), passes, failures);

    const std::string console = fake_console.get_stdout();
    ASSERT_EQ(console, output);
}

TEST_F(KiViProtocolTest, PerformsSetupHandshake)
{
    const int timeout = 99;
    const std::string test_name = "test";
    const std::string sync_msg = "{{" + std::string(GREENTEA_TEST_ENV_SYNC) + ";" + "0}}";
    fake_console.set_stdin(sync_msg.c_str());

    GREENTEA_SETUP(timeout, test_name.c_str());

    const std::string console = fake_console.get_stdout();
    const std::string::size_type sync_msg_pos = console.find(sync_msg);
    const std::string::size_type version_string_pos = console.find(GREENTEA_CLIENT_VERSION_STRING);
    const std::string::size_type timeout_pos = console.find(std::to_string(timeout));
    const std::string::size_type test_name_pos = console.find(test_name);

    ASSERT_NE(sync_msg_pos, std::string::npos);
    ASSERT_TRUE(version_string_pos != std::string::npos && version_string_pos > sync_msg_pos);
    ASSERT_TRUE(timeout_pos != std::string::npos && timeout_pos > version_string_pos);
    ASSERT_TRUE(test_name_pos != std::string::npos && test_name_pos > timeout_pos);
}

TEST_F(KiViProtocolTest, SendsStartTestcaseMessage)
{
    const std::string test_name = "test";

    GREENTEA_TESTCASE_START(test_name.c_str());

    const std::string console = fake_console.get_stdout();
    const std::string::size_type testcase_start_pos = console.find(GREENTEA_TEST_ENV_TESTCASE_START);
    const std::string::size_type test_name_pos = console.find(test_name);

    ASSERT_NE(testcase_start_pos, std::string::npos);
    ASSERT_TRUE(test_name_pos != std::string::npos && test_name_pos > testcase_start_pos);
}

TEST_F(KiViProtocolTest, SendsFinishTestcaseMessage)
{
    const std::string test_name = "test";
    const int passes = 11;
    const int failures = 99;

    GREENTEA_TESTCASE_FINISH(test_name.c_str(), passes, failures);

    const std::string console = fake_console.get_stdout();
    const std::string::size_type testcase_finish_pos = console.find(GREENTEA_TEST_ENV_TESTCASE_FINISH);
    const std::string::size_type test_name_pos = console.find(test_name);
    const std::string::size_type passes_pos = console.find(std::to_string(passes));
    const std::string::size_type failures_pos = console.find(std::to_string(failures));

    ASSERT_NE(testcase_finish_pos, std::string::npos);
    ASSERT_TRUE(test_name_pos != std::string::npos && test_name_pos > testcase_finish_pos);
    ASSERT_TRUE(passes_pos != std::string::npos && passes_pos > test_name_pos);
    ASSERT_TRUE(failures_pos != std::string::npos && failures_pos > passes_pos);
}

TEST_F(KiViProtocolTest, SendsTestSuiteResultMessage)
{
    const int result = 1;

    GREENTEA_TESTSUITE_RESULT(result);

    const std::string console = fake_console.get_stdout();
    const std::string::size_type testenv_end_pos = console.find(GREENTEA_TEST_ENV_END);
    const std::string::size_type testenv_success_pos = console.find(GREENTEA_TEST_ENV_SUCCESS);
    const std::string::size_type testenv_exit_pos = console.find(GREENTEA_TEST_ENV_EXIT);

    ASSERT_NE(testenv_end_pos, std::string::npos);
    ASSERT_TRUE(testenv_success_pos != std::string::npos && testenv_success_pos > testenv_end_pos);
    ASSERT_TRUE(testenv_exit_pos != std::string::npos && testenv_exit_pos > testenv_success_pos);
}
