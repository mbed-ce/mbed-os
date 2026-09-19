"""
Copyright (c) 2026 Isabella Wu
SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import time

from mbed_host_tests import BaseHostTest


class UartRxFifoTest(BaseHostTest):
    def setup(self):
        self.register_callback("r", self.send_burst)

    def send_burst(self, key, value, timestamp):
        time.sleep(0.02)
        self.send_kv("b", "01234567")
