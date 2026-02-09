# Copyright (c) 2018, Arm Limited and affiliates.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
from builtins import super
from mock import MagicMock

from .posix import MockTestEnvironmentPosix


class MockTestEnvironmentLinux(MockTestEnvironmentPosix):
    def __init__(self, test_case, platform_info, image_path):
        super().__init__(test_case, platform_info, image_path)

        self.patch("os.uname", new=MagicMock(return_value=("Linux",)), create=True)

    def __exit__(self, type, value, traceback):
        super().__exit__(type, value, traceback)

        if value:
            return False
