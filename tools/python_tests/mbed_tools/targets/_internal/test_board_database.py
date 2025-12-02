#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Tests for `mbed_tools.targets._internal.board_database`."""

from unittest import mock

import logging
import pytest

import mbed_tools.targets._internal.board_database as board_database
from mbed_tools.targets.env import env


class TestGetOfflineTargetData:
    """Tests for the method get_offline_target_data."""

    def test_local_target_database_file_found(self):
        """Test local database is found and loaded."""
        data = board_database.get_offline_board_data()
        assert len(data), "Some data should be returned from local database file."

    @mock.patch("mbed_tools.targets._internal.board_database.get_board_database_path")
    def test_raises_on_invalid_json(self, mocked_get_file):
        """Test raises an error when the file contains invalid JSON."""
        invalid_json = "None"
        path_mock = mock.Mock()
        path_mock.read_text.return_value = invalid_json
        mocked_get_file.return_value = path_mock
        with pytest.raises(board_database.ResponseJSONError):
            board_database.get_offline_board_data()


class TestGetLocalTargetDatabaseFile:
    def test_returns_path_to_targets(self):
        path = board_database.get_board_database_path()
        assert path.exists(), "Path to boards should exist in the package data folder."
