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
#include <algorithm>

#include "greentea-client/test_io.h"
#include "fake_console_io.h"

static std::string _stdout;
static std::string _stdin;

Console::Console()
{
}

std::string Console::get_stdout() const
{
    return _stdout;
}

void Console::set_stdin(const std::string &str)
{
    _stdin.append(str);
    std::reverse(_stdin.begin(), _stdin.end());
}

Console::~Console()
{
    _stdout = {};
    _stdin = {};
}

int greentea_getc()
{
    int out = EOF;
    if (!_stdin.empty()) {
        out = _stdin.back();
        _stdin.pop_back();
    }
    return out;
}

void greentea_putc(int c)
{
    _stdout.push_back(c);
}

void greentea_write_string(const char *str)
{
    _stdout.append(str);
}
