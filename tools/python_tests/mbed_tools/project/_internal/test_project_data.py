#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Tests for project_data.py."""

import pathlib
import pytest

from unittest import mock

from mbed_tools.project._internal.project_data import (
    MbedProgramFiles,
    MbedOS,
    MAIN_CPP_FILE_NAME,
    APP_CONFIG_FILE_NAME_JSON,
)
from python_tests.mbed_tools.project.factories import (
    make_mbed_program_files,
    make_mbed_os_files,
)


class TestMbedProgramFiles:
    def test_from_existing_finds_existing_program_data(self, tmp_path):
        root = pathlib.Path(tmp_path, "foo")
        make_mbed_program_files(root)

        program = MbedProgramFiles.from_existing(root, pathlib.Path("K64F", "develop", "GCC_ARM"))

        assert program.app_config_file.exists()
        assert program.mbed_os_ref.exists()
        assert program.cmakelists_file.exists()

    def test_from_existing_finds_existing_program_data_app_json(self, tmp_path):
        """
        Same as test_from_existing_finds_existing_program_data() except the app config
        is json instead of json5
        """

        root = pathlib.Path(tmp_path, "foo")
        make_mbed_program_files(root, APP_CONFIG_FILE_NAME_JSON)

        program = MbedProgramFiles.from_existing(root, pathlib.Path("K64F", "develop", "GCC_ARM"))

        assert program.app_config_file.exists()
        assert program.mbed_os_ref.exists()
        assert program.cmakelists_file.exists()


class TestMbedOS:
    def test_from_existing_finds_existing_mbed_os_data(self, tmp_path):
        root_path = pathlib.Path(tmp_path, "my-version-of-mbed-os")
        make_mbed_os_files(root_path)

        mbed_os = MbedOS.from_existing(root_path)

        assert mbed_os.targets_json_file == root_path / "targets" / "targets.json5"

    def test_raises_if_files_missing(self, tmp_path):
        root_path = pathlib.Path(tmp_path, "my-version-of-mbed-os")
        root_path.mkdir()

        with pytest.raises(ValueError):
            MbedOS.from_existing(root_path)
