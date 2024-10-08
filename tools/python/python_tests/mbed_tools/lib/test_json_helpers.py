#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import json
from pyjson5 import pyjson5

import pytest

from mbed_tools.lib.json_helpers import decode_json_file


def test_invalid_json(tmp_path):
    lib_json_path = tmp_path / "mbed_lib.json"
    lib_json_path.write_text("name")

    with pytest.raises(json.JSONDecodeError):
        decode_json_file(lib_json_path)


def test_invalid_json5(tmp_path):
    lib_json_path = tmp_path / "mbed_lib.json5"
    lib_json_path.write_text("name")

    with pytest.raises(pyjson5.Json5Exception):
        decode_json_file(lib_json_path)
