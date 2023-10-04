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
import six
import sys
import tempfile
import unittest

from lockfile import LockFile
from mock import patch
from mbed_os_tools.test import mbed_greentea_dlm

home_dir = tempfile.mkdtemp()
mbed_greentea_dlm.HOME_DIR = home_dir
mbed_greentea_dlm.GREENTEA_HOME_DIR = ".mbed-greentea"
mbed_greentea_dlm.GREENTEA_GLOBAL_LOCK = "glock.lock"
mbed_greentea_dlm.GREENTEA_KETTLE = "kettle.json" # active Greentea instances
mbed_greentea_dlm.GREENTEA_KETTLE_PATH = os.path.join(mbed_greentea_dlm.HOME_DIR, mbed_greentea_dlm.GREENTEA_HOME_DIR, mbed_greentea_dlm.GREENTEA_KETTLE)


class GreenteaDlmFunctionality(unittest.TestCase):
    def setUp(self):
        pass

    def tearDown(self):
        pass

    def test_greentea_home_dir_init(self):
        mbed_greentea_dlm.greentea_home_dir_init()

        path = os.path.join(mbed_greentea_dlm.HOME_DIR, mbed_greentea_dlm.GREENTEA_HOME_DIR)
        self.assertTrue(os.path.exists(path))

    def test_greentea_get_app_sem(self):
        sem, name, uuid = mbed_greentea_dlm.greentea_get_app_sem()
        self.assertIsNotNone(sem)
        self.assertIsNotNone(name)
        self.assertIsNotNone(uuid)

    def test_greentea_get_target_lock(self):
        lock = mbed_greentea_dlm.greentea_get_target_lock("target-id-2")
        path = os.path.join(mbed_greentea_dlm.HOME_DIR, mbed_greentea_dlm.GREENTEA_HOME_DIR, "target-id-2")
        self.assertIsNotNone(lock)
        self.assertEqual(path, lock.path)

    def test_greentea_get_global_lock(self):
        lock = mbed_greentea_dlm.greentea_get_global_lock()
        path = os.path.join(mbed_greentea_dlm.HOME_DIR, mbed_greentea_dlm.GREENTEA_HOME_DIR, "glock.lock")
        self.assertIsNotNone(lock)
        self.assertEqual(path, lock.path)

    def test_get_json_data_from_file_invalid_file(self):
        result = mbed_greentea_dlm.get_json_data_from_file("null_file")
        self.assertIsNone(result)

    def test_get_json_data_from_file_invalid_json(self):
        path = os.path.join(mbed_greentea_dlm.HOME_DIR, "test")

        with open(path, "w") as f:
            f.write("invalid json")

        result = mbed_greentea_dlm.get_json_data_from_file(path)
        self.assertEqual(result, None)

        os.remove(path)

    def test_get_json_data_from_file_valid_file(self):
        path = os.path.join(mbed_greentea_dlm.HOME_DIR, "test")

        with open(path, "w") as f:
            f.write("{}")

        result = mbed_greentea_dlm.get_json_data_from_file(path)
        self.assertEqual(result, {})

        os.remove(path)

    def test_greentea_update_kettle(self):
        uuid = "001"
        mbed_greentea_dlm.greentea_update_kettle(uuid)

        data = mbed_greentea_dlm.get_json_data_from_file(mbed_greentea_dlm.GREENTEA_KETTLE_PATH)
        self.assertIsNotNone(data)
        self.assertIn("start_time", data[uuid])
        self.assertIn("cwd", data[uuid])
        self.assertIn("locks", data[uuid])

        self.assertEqual(data[uuid]["cwd"], os.getcwd())
        self.assertEqual(data[uuid]["locks"], [])

        # Check greentea_kettle_info()
        output = mbed_greentea_dlm.greentea_kettle_info().splitlines()
        line   = output[3]
        self.assertIn(os.getcwd(), line)
        self.assertIn(uuid, line)

        # Test greentea_acquire_target_id
        target_id = "999"
        mbed_greentea_dlm.greentea_acquire_target_id(target_id, uuid)
        data = mbed_greentea_dlm.get_json_data_from_file(mbed_greentea_dlm.GREENTEA_KETTLE_PATH)
        self.assertIn(uuid, data)
        self.assertIn("locks", data[uuid])
        self.assertIn(target_id, data[uuid]["locks"])

        # Test greentea_release_target_id
        mbed_greentea_dlm.greentea_release_target_id(target_id, uuid)
        data = mbed_greentea_dlm.get_json_data_from_file(mbed_greentea_dlm.GREENTEA_KETTLE_PATH)
        self.assertIn(uuid, data)
        self.assertIn("locks", data[uuid])
        self.assertNotIn(target_id, data[uuid]["locks"])

        # Test greentea_acquire_target_id_from_list
        target_id = "999"
        result = mbed_greentea_dlm.greentea_acquire_target_id_from_list([target_id], uuid)
        data = mbed_greentea_dlm.get_json_data_from_file(mbed_greentea_dlm.GREENTEA_KETTLE_PATH)
        self.assertEqual(result, target_id)
        self.assertIn(uuid, data)
        self.assertIn("locks", data[uuid])
        self.assertIn(target_id, data[uuid]["locks"])

        # Check greentea_clean_kettle()
        mbed_greentea_dlm.greentea_clean_kettle(uuid)
        data = mbed_greentea_dlm.get_json_data_from_file(mbed_greentea_dlm.GREENTEA_KETTLE_PATH)
        self.assertEqual(data, {})
