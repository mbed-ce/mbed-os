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
#ifndef _FAKE_CONSOLE_IO
#define _FAKE_CONSOLE_IO

#include <string>

struct Console {
    Console();
    ~Console();

    void set_stdin(const std::string &);
    std::string get_stdout() const;
};

#endif // _FAKE_CONSOLE_IO
