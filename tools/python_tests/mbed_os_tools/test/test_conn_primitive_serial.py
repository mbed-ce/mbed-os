# Copyright (c) 2018-2021, Arm Limited and affiliates.
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

import unittest
import mock

from mbed_os_tools.test.host_tests.base_host_test import HostTestConfig
from mbed_os_tools.test.host_tests_conn_proxy.conn_primitive_serial import SerialConnectorPrimitive
from mbed_os_tools.test.host_tests_conn_proxy.conn_primitive import ConnectorPrimitiveException


@mock.patch("mbed_os_tools.test.host_tests_conn_proxy.conn_primitive_serial.Serial")
@mock.patch("mbed_os_tools.test.host_tests_plugins.host_test_plugins.detect")
class ConnPrimitiveSerialTestCase(unittest.TestCase):
    def test_provided_serial_port_used_with_target_id(self, mock_detect, mock_serial):
        platform_name = "irrelevant"
        target_id = "1234"
        port = "COM256"
        baudrate = "9600"

        # The mock list_mbeds() call needs to return a list of dictionaries,
        # and each dictionary must have a "serial_port", or else the
        # check_serial_port_ready() function we are testing will sleep waiting
        # for the serial port to become ready.
        mock_detect.create().list_mbeds.return_value = [
            {"target_id": target_id, "serial_port": port, "platform_name": platform_name}
        ]

        # Set skip_reset to avoid the use of a physical serial port.
        config = HostTestConfig(
            port=port,
            baudrate=int(baudrate),
            mbed_target=platform_name,
            skip_reset=True,
            sync_behavior=2,
            sync_timeout=5,
            test_name="test-something",
            polling_timeout=60,
            post_reset_delay=1
        )
        connector = SerialConnectorPrimitive("SERI", port, baudrate, config=config)

        mock_detect.create().list_mbeds.assert_not_called()


if __name__ == "__main__":
    unittest.main()
