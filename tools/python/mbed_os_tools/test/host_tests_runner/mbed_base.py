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

import json
import os
from time import sleep
from .. import host_tests_plugins as ht_plugins
from ... import detect
from .. import DEFAULT_BAUD_RATE
from ..host_tests_logger import HtrunLogger


class Mbed:
    """! Base class for a host driven test
    @details This class stores information about things like disk, port, serial speed etc.
             Class is also responsible for manipulation of serial port between host and mbed device
    """

    def __init__(self, options):
        """ctor"""
        # For compatibility with old mbed. We can use command line options for Mbed object
        # or we can pass options directly from .
        self.options = options
        self.logger = HtrunLogger("MBED")
        # Options related to copy / reset mbed device
        self.port = self.options.port
        self.mcu = self.options.micro
        self.disk = self.options.disk
        self.target_id = self.options.target_id
        self.image_path = self.options.image_path.strip('"') if self.options.image_path is not None else ""
        self.copy_method = self.options.copy_method
        self.retry_copy = self.options.retry_copy
        self.program_cycle_s = float(self.options.program_cycle_s if self.options.program_cycle_s is not None else 2.0)
        self.polling_timeout = self.options.polling_timeout

        # Serial port settings
        self.serial_baud = DEFAULT_BAUD_RATE
        self.serial_timeout = 1

        # Users can use command to pass port speeds together with port name. E.g. COM4:115200:1
        # Format if PORT:SPEED:TIMEOUT
        port_config = self.port.split(":") if self.port else ""
        if len(port_config) == 2:
            # -p COM4:115200
            self.port = port_config[0]
            self.serial_baud = int(port_config[1])
        elif len(port_config) == 3:
            # -p COM4:115200:0.5
            self.port = port_config[0]
            self.serial_baud = int(port_config[1])
            self.serial_timeout = float(port_config[2])

        # Overriding baud rate value with command line specified value
        self.serial_baud = self.options.baud_rate if self.options.baud_rate else self.serial_baud

        # Test configuration in JSON format
        self.test_cfg = None
        if self.options.json_test_configuration is not None:
            # We need to normalize path before we open file
            json_test_configuration_path = self.options.json_test_configuration.strip("\"'")
            try:
                self.logger.prn_inf("Loading test configuration from '%s'..." % json_test_configuration_path)
                with open(json_test_configuration_path) as data_file:
                    self.test_cfg = json.load(data_file)
            except IOError as e:
                self.logger.prn_err(
                    "Test configuration JSON file '{0}' I/O error({1}): {2}".format(
                        json_test_configuration_path, e.errno, e.strerror
                    )
                )
            except:
                self.logger.prn_err("Test configuration JSON Unexpected error:", str(e))
                raise

    def hw_reset(self):
        """
        Performs hardware reset of target ned device.

        :return:
        """
        device_info = {}
        result = ht_plugins.call_plugin(
            "ResetMethod", "power_cycle", target_id=self.target_id, device_info=device_info, format=self.options.format
        )
        if result:
            self.port = device_info["serial_port"]
            self.disk = device_info["mount_point"]
        return result
