# Copyright (c) 2026 Jamie Smith
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
import subprocess

from .host_test_plugins import HostTestPluginBase


class HostTestPluginResetMethod_cmake(HostTestPluginBase):
    # Plugin interface
    name = "HostTestPluginResetMethod_cmake"
    type = "ResetMethod"
    capabilities = ["cmake"]
    required_parameters = ["build_dir"]
    stable = True

    def setup(self, *args, **kwargs):
        """! Configure plugin, this function should be called before plugin execute() method is used."""
        return True

    def execute(self, capability, *args, **kwargs):
        """! Executes capability by name

        @param capability Capability name
        @param args Additional arguments
        @param kwargs Additional arguments

        @details Each capability e.g. may directly just call some command line program or execute building pythonic function

        @return Capability call return value
        """
        result = False
        if self.check_parameters(capability, *args, **kwargs) is True:
            build_dir = kwargs["build_dir"]
            try:
                # call reset target provided by the upload method
                subprocess.check_call(["cmake", "--build", str(build_dir), "--target", "reset"])
                result = True
            except subprocess.CalledProcessError as ex:
                print("Reset failed: " + str(ex))

        return result


def load_plugin():
    """Returns plugin available in this module"""
    return HostTestPluginResetMethod_cmake()
