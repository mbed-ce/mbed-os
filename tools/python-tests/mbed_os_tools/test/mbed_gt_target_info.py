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
import shutil
import tempfile
import unittest

from six import StringIO

from mock import patch
from mbed_os_tools.test import mbed_target_info


class GreenteaTargetInfo(unittest.TestCase):

    def setUp(self):
        self.YOTTA_SEARCH_SSL_ISSUE = """/Library/Python/2.7/site-packages/requests/packages/urllib3/util/ssl_.py:90: InsecurePlatformWarning: A true SSLContext object is not available. This prevents urllib3 from configuring SSL appropriately and may cause certain SSL connections to fail. For more information, see https://urllib3.readthedocs.org/en/latest/security.html#insecureplatformwarning.
  InsecurePlatformWarning
frdm-k64f-gcc 0.0.24: Official mbed build target for the mbed frdm-k64f development board.
frdm-k64f-armcc 0.0.16: Official mbed build target for the mbed frdm-k64f development board, using the armcc toolchain.
/Library/Python/2.7/site-packages/requests/packages/urllib3/util/ssl_.py:90: InsecurePlatformWarning: A true SSLContext object is not available. This prevents urllib3 from configuring SSL appropriately and may cause certain SSL connections to fail. For more information, see https://urllib3.readthedocs.org/en/latest/security.html#insecureplatformwarning.
  InsecurePlatformWarning
"""

    def tearDown(self):
        pass

    def test_parse_yotta_target_cmd_output_mixed_chars(self):
        self.assertIn("m", mbed_target_info.parse_yotta_target_cmd_output("m 0.0.0"))
        self.assertIn("m", mbed_target_info.parse_yotta_target_cmd_output(" m 0.0.0"))
        self.assertIn("aaaaaaaaaaaaa", mbed_target_info.parse_yotta_target_cmd_output("aaaaaaaaaaaaa 0.0.0"))
        self.assertIn("aaaa-bbbb-cccc", mbed_target_info.parse_yotta_target_cmd_output("aaaa-bbbb-cccc 0.0.0"))
        self.assertIn("aBc-DEF_hijkkk", mbed_target_info.parse_yotta_target_cmd_output("aBc-DEF_hijkkk 0.0.0"))

    def test_parse_yotta_target_cmd_output_mixed_version(self):
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 0.0.0"))
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 1.1.1"))
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 1.1.12"))
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 1.1.123"))
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 11.22.33"))
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 0.0.1"))
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 0.10.12"))
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 0.20.123"))
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 0.2.123"))
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 10.20.123"))
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 110.200.123"))

    def test_parse_yotta_target_cmd_output(self):
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 0.0.24"))
        self.assertIn("frdm-k64f-armcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-armcc 1.12.3"))
        self.assertIn("mbed-gcc", mbed_target_info.parse_yotta_target_cmd_output("mbed-gcc 0.0.14"))
        self.assertIn("stm32f429i-disco-gcc", mbed_target_info.parse_yotta_target_cmd_output("stm32f429i-disco-gcc 0.0.14"))

    def test_parse_yotta_target_cmd_output_mixed_whitechars(self):
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 0.0.24 "))
        self.assertIn("frdm-k64f-armcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-armcc 1.12.3 "))
        self.assertIn("mbed-gcc", mbed_target_info.parse_yotta_target_cmd_output("mbed-gcc 0.0.14 "))
        self.assertIn("stm32f429i-disco-gcc", mbed_target_info.parse_yotta_target_cmd_output("stm32f429i-disco-gcc 0.0.14 "))

    def test_parse_yotta_target_cmd_output_mixed_nl(self):
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 0.0.24\n"))
        self.assertIn("frdm-k64f-armcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-armcc 1.12.3\n"))
        self.assertIn("mbed-gcc", mbed_target_info.parse_yotta_target_cmd_output("mbed-gcc 0.0.14\n"))
        self.assertIn("stm32f429i-disco-gcc", mbed_target_info.parse_yotta_target_cmd_output("stm32f429i-disco-gcc 0.0.14\n"))

    def test_parse_yotta_target_cmd_output_mixed_rcnl(self):
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 0.0.24\r\n"))
        self.assertIn("frdm-k64f-armcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-armcc 1.12.3\r\n"))
        self.assertIn("mbed-gcc", mbed_target_info.parse_yotta_target_cmd_output("mbed-gcc 0.0.14\r\n"))
        self.assertIn("stm32f429i-disco-gcc", mbed_target_info.parse_yotta_target_cmd_output("stm32f429i-disco-gcc 11.222.333\r\n"))

    def test_parse_yotta_target_cmd_output_mixed_nl_whitechars(self):
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 0.0.24 \n"))
        self.assertIn("frdm-k64f-armcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-armcc 1.12.3 \n"))
        self.assertIn("mbed-gcc", mbed_target_info.parse_yotta_target_cmd_output("mbed-gcc 0.0.14 \n"))
        self.assertIn("stm32f429i-disco-gcc", mbed_target_info.parse_yotta_target_cmd_output("stm32f429i-disco-gcc 0.0.14 \n"))

    def test_parse_yotta_target_cmd_output_mixed_rcnl_whitechars(self):
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-gcc 0.0.24 \r\n"))
        self.assertIn("frdm-k64f-armcc", mbed_target_info.parse_yotta_target_cmd_output("frdm-k64f-armcc 1.12.3 \r\n"))
        self.assertIn("mbed-gcc", mbed_target_info.parse_yotta_target_cmd_output("mbed-gcc 0.0.14 \r\n"))
        self.assertIn("stm32f429i-disco-gcc", mbed_target_info.parse_yotta_target_cmd_output("stm32f429i-disco-gcc 0.0.14 \r\n"))

    def test_parse_yotta_target_cmd_output_fail(self):
        self.assertEqual(None, mbed_target_info.parse_yotta_target_cmd_output(""))
        self.assertEqual(None, mbed_target_info.parse_yotta_target_cmd_output("stm32f429i-disco-gcc 1"))
        self.assertEqual(None, mbed_target_info.parse_yotta_target_cmd_output("stm32f429i-disco-gcc 1."))
        self.assertEqual(None, mbed_target_info.parse_yotta_target_cmd_output("stm32f429i-disco-gcc 1.0"))
        self.assertEqual(None, mbed_target_info.parse_yotta_target_cmd_output("stm32f429i-disco-gcc 1.0."))
        self.assertEqual(None, mbed_target_info.parse_yotta_target_cmd_output("stm32f429i-disco-gcc 1.0.x"))

    def test_parse_yotta_search_cmd_output(self):
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_search_cmd_output("frdm-k64f-gcc 0.0.24: Official mbed build target for the mbed frdm-k64f development board."))
        self.assertIn("frdm-k64f-armcc", mbed_target_info.parse_yotta_search_cmd_output("frdm-k64f-armcc 0.0.16: Official mbed build target for the mbed frdm-k64f development board, using the armcc toolchain."))
        self.assertEqual(None, mbed_target_info.parse_yotta_search_cmd_output(""))
        self.assertEqual(None, mbed_target_info.parse_yotta_search_cmd_output("additional results from https://yotta-private.herokuapp.com:"))

    def test_parse_yotta_search_cmd_output_text(self):
        # Old style with new switch --short : 'yotta search ... --short'
        text = """frdm-k64f-gcc 0.1.4: Official mbed build target for the mbed frdm-k64f development board.
frdm-k64f-armcc 0.1.3: Official mbed build target for the mbed frdm-k64f development board, using the armcc toolchain.

additional results from https://yotta-private.herokuapp.com:
"""
        targets = []
        for line in text.splitlines():
            yotta_target_name = mbed_target_info.parse_yotta_search_cmd_output(line)
            if yotta_target_name:
                targets.append(yotta_target_name)
        self.assertIn("frdm-k64f-gcc", targets)
        self.assertIn("frdm-k64f-armcc", targets)
        self.assertEqual(2, len(targets))

    def test_parse_yotta_search_cmd_output_new_style(self):
        self.assertIn("frdm-k64f-gcc", mbed_target_info.parse_yotta_search_cmd_output("frdm-k64f-gcc 0.1.4"))
        self.assertIn("frdm-k64f-armcc", mbed_target_info.parse_yotta_search_cmd_output("frdm-k64f-armcc 0.1.4"))
        pass

    def test_parse_yotta_search_cmd_output_new_style_text(self):
        # New style of 'yotta search ...'
        text = """frdm-k64f-gcc 0.1.4
    Official mbed build target for the mbed frdm-k64f development board.
    mbed-target:k64f, mbed-official, k64f, frdm-k64f, gcc
frdm-k64f-armcc 0.1.4
    Official mbed build target for the mbed frdm-k64f development board, using the armcc toolchain.
    mbed-target:k64f, mbed-official, k64f, frdm-k64f, armcc

additional results from https://yotta-private.herokuapp.com:"""
        targets = []
        for line in text.splitlines():
            yotta_target_name = mbed_target_info.parse_yotta_search_cmd_output(line)
            if yotta_target_name:
                targets.append(yotta_target_name)
        self.assertIn("frdm-k64f-gcc", targets)
        self.assertIn("frdm-k64f-armcc", targets)
        self.assertEqual(2, len(targets))

    def test_parse_yotta_search_cmd_output_new_style_text_2(self):
        # New style of 'yotta search ...'
        text = """nrf51dk-gcc 0.0.3:
    Official mbed build target for the nRF51-DK 32KB platform.
    mbed-official, mbed-target:nrf51_dk, gcc

additional results from https://yotta-private.herokuapp.com:
  nrf51dk-gcc 0.0.3:
      Official mbed build target for the nRF51-DK 32KB platform.
      mbed-official, mbed-target:nrf51_dk, gcc
  nrf51dk-armcc 0.0.3:
      Official mbed build target for the nRF51-DK 32KB platform.
      mbed-official, mbed-target:nrf51_dk, armcc
"""
        targets = []
        for line in text.splitlines():
            yotta_target_name = mbed_target_info.parse_yotta_search_cmd_output(line)
            if yotta_target_name and yotta_target_name not in targets:
                targets.append(yotta_target_name)

        self.assertIn("nrf51dk-gcc", targets)
        self.assertIn("nrf51dk-armcc", targets)
        self.assertEqual(2, len(targets))

    def test_parse_yotta_search_cmd_output_with_ssl_errors(self):
        result = []
        for line in self.YOTTA_SEARCH_SSL_ISSUE.splitlines():
            yotta_target_name = mbed_target_info.parse_yotta_search_cmd_output(line)
            if yotta_target_name:
                result.append(yotta_target_name)
        self.assertIn("frdm-k64f-gcc", result)
        self.assertIn("frdm-k64f-armcc", result)
        self.assertEqual(2, len(result))

    def test_parse_mbed_target_from_target_json_no_keywords(self):
        target_json_data = {
            "name": "frdm-k64f-armcc",
            "version": "0.1.4",
        }

        self.assertIsNone(mbed_target_info.parse_mbed_target_from_target_json('k64f', target_json_data))
        self.assertIsNone(mbed_target_info.parse_mbed_target_from_target_json('K64F', target_json_data))

    def test_parse_mbed_target_from_target_json_no_name(self):
        target_json_data = {
            "version": "0.1.4",
            "keywords": [
                "mbed-target:k64f",
                "mbed-target::garbage",
                "mbed-official",
                "k64f",
                "frdm-k64f",
                "armcc"
            ],
        }

        self.assertIsNone(mbed_target_info.parse_mbed_target_from_target_json('k64f', target_json_data))
        self.assertIsNone(mbed_target_info.parse_mbed_target_from_target_json('K64F', target_json_data))

    def test_parse_mbed_target_from_target_json(self):
        target_json_data = {
            "name": "frdm-k64f-armcc",
            "version": "0.1.4",
            "keywords": [
                "mbed-target:k64f",
                "mbed-target::garbage",
                "mbed-official",
                "k64f",
                "frdm-k64f",
                "armcc"
            ],
        }

        # Positive tests
        self.assertEqual('frdm-k64f-armcc', mbed_target_info.parse_mbed_target_from_target_json('k64f', target_json_data))
        self.assertEqual('frdm-k64f-armcc', mbed_target_info.parse_mbed_target_from_target_json('K64F', target_json_data))

        # Except cases
        self.assertNotEqual('frdm-k64f-gcc', mbed_target_info.parse_mbed_target_from_target_json('k64f', target_json_data))
        self.assertNotEqual('frdm-k64f-gcc', mbed_target_info.parse_mbed_target_from_target_json('K64F', target_json_data))
        self.assertEqual(None, mbed_target_info.parse_mbed_target_from_target_json('_k64f_', target_json_data))
        self.assertEqual(None, mbed_target_info.parse_mbed_target_from_target_json('', target_json_data))
        self.assertEqual(None, mbed_target_info.parse_mbed_target_from_target_json('Some board name', target_json_data))

    def test_parse_mbed_target_from_target_json_multiple(self):
        target_json_data = {
          "name": "frdm-k64f-armcc",
          "version": "0.1.4",
          "keywords": [
            "mbed-target:k64f",
            "mbed-target:frdm-k64f",
            "mbed-official",
            "k64f",
            "frdm-k64f",
            "armcc"
          ],
        }

        # Positive tests
        self.assertEqual('frdm-k64f-armcc', mbed_target_info.parse_mbed_target_from_target_json('k64f', target_json_data))
        self.assertEqual('frdm-k64f-armcc', mbed_target_info.parse_mbed_target_from_target_json('frdm-k64f', target_json_data))
        self.assertEqual('frdm-k64f-armcc', mbed_target_info.parse_mbed_target_from_target_json('K64F', target_json_data))
        self.assertEqual('frdm-k64f-armcc', mbed_target_info.parse_mbed_target_from_target_json('FRDM-K64F', target_json_data))

        # Except cases
        self.assertNotEqual('frdm-k64f-gcc', mbed_target_info.parse_mbed_target_from_target_json('k64f', target_json_data))
        self.assertNotEqual('frdm-k64f-gcc', mbed_target_info.parse_mbed_target_from_target_json('frdm-k64f', target_json_data))
        self.assertNotEqual('frdm-k64f-gcc', mbed_target_info.parse_mbed_target_from_target_json('K64F', target_json_data))
        self.assertNotEqual('frdm-k64f-gcc', mbed_target_info.parse_mbed_target_from_target_json('FRDM-K64F', target_json_data))

    @patch('mbed_os_tools.test.mbed_target_info.get_mbed_target_call_yotta_target')
    def test_get_mbed_target_from_current_dir_ok(self, callYtTarget_mock):

        yotta_target_cmd = """frdm-k64f-gcc 2.0.0
kinetis-k64-gcc 2.0.2
mbed-gcc 1.1.0
"""

        callYtTarget_mock.return_value = ('', '',  0)
        r = mbed_target_info.get_mbed_target_from_current_dir()
        self.assertEqual(None, r)

        callYtTarget_mock.return_value = ('', '',  1)
        r = mbed_target_info.get_mbed_target_from_current_dir()
        self.assertEqual(None, r)

        callYtTarget_mock.return_value = (yotta_target_cmd, '',  1)
        r = mbed_target_info.get_mbed_target_from_current_dir()
        self.assertEqual(None, r)

        callYtTarget_mock.return_value = (yotta_target_cmd, '',  0)
        r = mbed_target_info.get_mbed_target_from_current_dir()
        self.assertEqual('frdm-k64f-gcc', r)

    def test_get_yotta_target_from_local_config_invalid_path(self):
        result = mbed_target_info.get_yotta_target_from_local_config("invalid_path.json")

        self.assertIsNone(result)

    def test_get_yotta_target_from_local_config_valid_path(self):
        payload = '{"build": { "target": "test_target"}}'
        handle, path = tempfile.mkstemp("test_file")

        with open(path, 'w+') as f:
            f.write(payload)

        result = mbed_target_info.get_yotta_target_from_local_config(path)

        self.assertIsNotNone(result)
        self.assertEqual(result, "test_target")

    def test_get_yotta_target_from_local_config_failed_open(self):
        handle, path = tempfile.mkstemp()

        with open(path, 'w+') as f:
            result = mbed_target_info.get_yotta_target_from_local_config(path)

            self.assertIsNone(result)

    def test_get_mbed_targets_from_yotta_local_module_invalid_path(self):
        result = mbed_target_info.get_mbed_targets_from_yotta_local_module("null", "invalid_path")
        self.assertEqual(result, [])

    def test_get_mbed_targets_from_yotta_local_module_invalid_target(self):
        base_path = tempfile.mkdtemp()
        targ_path = tempfile.mkdtemp("target-1", dir=base_path)
        handle, targ_file = tempfile.mkstemp("target.json", dir=targ_path)

        result = mbed_target_info.get_mbed_targets_from_yotta_local_module("null", base_path)

        self.assertEqual(result, [])

    def test_get_mbed_targets_from_yotta_local_module_valid(self):
        payload = '{"name": "test_name", "keywords": [ "mbed-target:k64f" ]}'
        base_path = tempfile.mkdtemp()
        tar1_path = tempfile.mkdtemp("target-1", dir=base_path)
        tar1_file = os.path.join(tar1_path, "target.json")

        with open(tar1_file, 'w+') as f:
            f.write(payload)

        result = mbed_target_info.get_mbed_targets_from_yotta_local_module("k64f", base_path)
        self.assertIsNot(result, None)
        self.assertEqual(result[0], "test_name")

        shutil.rmtree(base_path)

    def test_parse_mbed_target_from_target_json_missing_json_data(self):
        result = mbed_target_info.parse_mbed_target_from_target_json("null", "null")
        self.assertIsNone(result)

    def test_parse_mbed_target_from_target_json_missing_keywords(self):
        data = {}
        result = mbed_target_info.parse_mbed_target_from_target_json("null", data)
        self.assertIsNone(result)

    def test_parse_mbed_target_from_target_json_missing_target(self):
        data = {}
        data["keywords"] = {}
        result = mbed_target_info.parse_mbed_target_from_target_json("null", data)
        self.assertIsNone(result)

    def test_parse_mbed_target_from_target_json_missing_name(self):
        data = {}
        data["keywords"] = ["mbed-target:null"]
        result = mbed_target_info.parse_mbed_target_from_target_json("null", data)
        self.assertIsNone(result)

    # def test_get_mbed_targets_from_yotta(self):
    #     result = mbed_target_info.get_mbed_targets_from_yotta("k64f")

    def test_parse_add_target_info_mapping(self):
        result = mbed_target_info.add_target_info_mapping("null")


    def test_get_platform_property_from_targets_no_json(self):
        with patch("mbed_os_tools.test.mbed_target_info._find_targets_json") as _find:
            _find.return_value = iter([])
            result = mbed_target_info._get_platform_property_from_targets("not_a_platform", "not_a_property", "default")
            self.assertIsNone(result)

    def test_get_platform_property_from_targets_no_file(self):
        with patch("mbed_os_tools.test.mbed_target_info._find_targets_json") as _find,\
             patch("mbed_os_tools.test.mbed_target_info.open") as _open:
            _find.return_value = iter(["foo"])
            _open.side_effect = IOError
            result = mbed_target_info._get_platform_property_from_targets("not_a_platform", "not_a_property", "default")
            self.assertIsNone(result)

    def test_get_platform_property_from_targets_invalid_json(self):
        with patch("mbed_os_tools.test.mbed_target_info._find_targets_json") as _find,\
             patch("mbed_os_tools.test.mbed_target_info.open") as _open:
            _find.return_value = iter(["foo"])
            _open.return_value.__enter__.return_value = StringIO("{")
            result = mbed_target_info._get_platform_property_from_targets("not_a_platform", "not_a_property", "default")
            self.assertIsNone(result)

    def test_get_platform_property_from_targets_empty_json(self):
        with patch("mbed_os_tools.test.mbed_target_info._find_targets_json") as _find,\
             patch("mbed_os_tools.test.mbed_target_info.open") as _open:
            _find.return_value = iter(["foo"])
            _open.return_value.__enter__.return_value = StringIO("{}")
            result = mbed_target_info._get_platform_property_from_targets("not_a_platform", "not_a_property", "default")
            self.assertIsNone(result)

    def test_get_platform_property_from_targets_no_value(self):
        with patch("mbed_os_tools.test.mbed_target_info._find_targets_json") as _find,\
             patch("mbed_os_tools.test.mbed_target_info.open") as _open:
            _find.return_value = iter(["foo"])
            _open.return_value.__enter__.return_value = StringIO("{\"K64F\": {}}")
            result = mbed_target_info._get_platform_property_from_targets("K64F", "not_a_property", "default")
            self.assertEqual(result, "default")

    def test_get_platform_property_from_targets_in_json(self):
        with patch("mbed_os_tools.test.mbed_target_info._find_targets_json") as _find,\
             patch("mbed_os_tools.test.mbed_target_info.open") as _open:
            _find.return_value = iter(["foo"])
            _open.return_value.__enter__.return_value = StringIO("{\"K64F\": {\"copy_method\": \"cp\"}}")
            result = mbed_target_info._get_platform_property_from_targets("K64F", "copy_method", "default")
            self.assertEqual("cp", result)

    def test_find_targets_json(self):
        with patch("mbed_os_tools.test.mbed_target_info.walk") as _walk:
            _walk.return_value = iter([("", ["foo"], []), ("foo", [], ["targets.json"])])
            result = list(mbed_target_info._find_targets_json("bogus_path"))
            self.assertEqual(result, [os.path.join("foo", "targets.json")])

    def test_find_targets_json_ignored(self):
        with patch("mbed_os_tools.test.mbed_target_info.walk") as _walk:
            walk_result =[("", [".build"], [])]
            _walk.return_value = iter(walk_result)
            result = list(mbed_target_info._find_targets_json("bogus_path"))
            self.assertEqual(result, [])
            self.assertEqual(walk_result, [("", [], [])])

    def test_platform_property_from_targets_json_empty(self):
        result = mbed_target_info._platform_property_from_targets_json(
            {}, "not_a_target", "not_a_property", "default"
        )
        self.assertIsNone(result)

    def test_platform_property_from_targets_json_base_target(self):
        result = mbed_target_info._platform_property_from_targets_json(
            {"K64F": {"copy_method": "cp"}}, "K64F", "copy_method", "default"
        )
        self.assertEqual(result, "cp")

    def test_platform_property_from_targets_json_inherits(self):
        result = mbed_target_info._platform_property_from_targets_json(
            {"K64F": {"inherits": ["Target"]}, "Target": {"copy_method": "cp"}},
            "K64F", "copy_method", "default"
        )
        self.assertEqual(result, "cp")

    def test_platform_property_from_default_missing(self):
        result = mbed_target_info._get_platform_property_from_default("not_a_property")
        self.assertIsNone(result)

    def test_platform_property_from_default(self):
        result = mbed_target_info._get_platform_property_from_default("copy_method")
        self.assertEqual(result, "default")

    def test_platform_property_from_info_mapping_bad_platform(self):
        result = mbed_target_info._get_platform_property_from_info_mapping("not_a_platform", "not_a_property")
        self.assertIsNone(result)

    def test_platform_property_from_info_mapping_missing(self):
        result = mbed_target_info._get_platform_property_from_info_mapping("K64F", "not_a_property")
        self.assertIsNone(result)

    def test_platform_property_from_info_mapping(self):
        result = mbed_target_info._get_platform_property_from_info_mapping("K64F", "copy_method")
        self.assertEqual(result, "default")


    # The following test cases are taken from this table:
    #
    # Num | In targets.json | In yotta blob | In Default | property used
    # --- | --------------- | ------------- | ---------- | --------------
    # 1   | Yes             | No            | Yes        |`targets.json`
    # 2   | Yes             | Yes           | Yes        |`targets.json`
    # 3   | No              | Yes           | Yes        | yotta blob
    # 4   | No              | No            | Yes        | default
    # 5   | No              | No            | No         | None
    # 6   | Yes             | No            | No         |`targets.json`
    # 7   | Yes             | Yes           | No         |`targets.json`
    # 8   | No              | Yes           | No         | yotta blob
    def test_platform_property(self):
        """Test that platform_property picks the property value preserving
        the following priority relationship:
        targets.json > yotta blob > default
        """
        with patch("mbed_os_tools.test.mbed_target_info._get_platform_property_from_targets") as _targets,\
             patch("mbed_os_tools.test.mbed_target_info._get_platform_property_from_info_mapping") as _info_mapping,\
             patch("mbed_os_tools.test.mbed_target_info._get_platform_property_from_default") as _default:
            # 1
            _targets.return_value = "targets"
            _info_mapping.return_value = None
            _default.return_value = "default"
            self.assertEqual(
                mbed_target_info.get_platform_property("K64F", "copy_method"),
                "targets")
            # 2
            _info_mapping.return_value = "yotta"
            self.assertEqual(
                mbed_target_info.get_platform_property("K64F", "copy_method"),
                "targets")
            # 3
            _targets.return_value = None
            self.assertEqual(
                mbed_target_info.get_platform_property("K64F", "copy_method"),
                "yotta")
            # 4
            _info_mapping.return_value = None
            self.assertEqual(
                mbed_target_info.get_platform_property("K64F", "copy_method"),
                "default")
            # 5
            _default.return_value = None
            self.assertEqual(
                mbed_target_info.get_platform_property("K64F", "copy_method"),
                None)
            # 6
            _targets.return_value = "targets"
            self.assertEqual(
                mbed_target_info.get_platform_property("K64F", "copy_method"),
                "targets")
            # 7
            _info_mapping.return_value = "yotta"
            self.assertEqual(
                mbed_target_info.get_platform_property("K64F", "copy_method"),
                "targets")
            # 8
            _targets.return_value = None
            self.assertEqual(
                mbed_target_info.get_platform_property("K64F", "copy_method"),
                "yotta")


    def test_parse_yotta_json_for_build_name(self):
        self.assertEqual("", mbed_target_info.parse_yotta_json_for_build_name(
            {
                "build": {
                    "target": ""
                }
            }
        ))

        self.assertEqual(None, mbed_target_info.parse_yotta_json_for_build_name(
            {
                "build": {}
            }
        ))

        self.assertEqual('frdm-k64f-gcc', mbed_target_info.parse_yotta_json_for_build_name(
            {
              "build": {
                "target": "frdm-k64f-gcc,*",
                "targetSetExplicitly": True
              }
            }
        ))

        self.assertEqual('x86-linux-native', mbed_target_info.parse_yotta_json_for_build_name(
            {
              "build": {
                "target": "x86-linux-native,*",
                "targetSetExplicitly": True
              }
            }
        ))

        self.assertEqual('frdm-k64f-gcc', mbed_target_info.parse_yotta_json_for_build_name(
            {
              "build": {
                "target": "frdm-k64f-gcc,*"
              }
            }
        ))

        self.assertEqual('frdm-k64f-gcc', mbed_target_info.parse_yotta_json_for_build_name(
            {
              "build": {
                "target": "frdm-k64f-gcc"
              }
            }
        ))

        self.assertEqual(None, mbed_target_info.parse_yotta_json_for_build_name(
            {
                "build": {
                }
            }
        ))

        self.assertEqual(None, mbed_target_info.parse_yotta_json_for_build_name(
            {
              "BUILD": {
                  "target": "frdm-k64f-gcc,*",
                  "targetSetExplicitly": True
              }
            }
        ))


if __name__ == '__main__':
    unittest.main()
