#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Tests for `mbed_tools.targets.get_board`."""

import pytest

from unittest import mock

from mbed_tools.targets._internal.exceptions import BoardAPIError

# Import from top level as this is the expected interface for users
from mbed_tools.targets import get_board_by_online_id, get_board_by_product_code, get_board_by_jlink_slug
from mbed_tools.targets.get_board import get_board
from mbed_tools.targets.env import env
from mbed_tools.targets.exceptions import UnknownBoardError, UnsupportedModeError
from python_tests.mbed_tools.targets.factories import make_board


@pytest.fixture
def mock_get_board():
    with mock.patch("mbed_tools.targets.get_board.get_board", autospec=True) as gbp:
        yield gbp


@pytest.fixture
def mocked_boards():
    with mock.patch("mbed_tools.targets.get_board.Boards", autospec=True) as gbp:
        yield gbp


class TestGetBoard:
    def test_offline_mode(self, mocked_boards):
        fn = mock.Mock()

        subject = get_board(fn)

        assert subject == mocked_boards.from_offline_database().get_board.return_value
        mocked_boards.from_offline_database().get_board.assert_called_once_with(fn)


class TestGetBoardByProductCode:
    def test_matches_boards_by_product_code(self, mock_get_board):
        product_code = "swag"

        assert get_board_by_product_code(product_code) == mock_get_board.return_value

        # Test callable matches correct boards
        fn = mock_get_board.call_args[0][0]

        matching_board = make_board(product_code=product_code)
        not_matching_board = make_board(product_code="whatever")

        assert fn(matching_board)
        assert not fn(not_matching_board)


class TestGetBoardByOnlineId:
    def test_matches_boards_by_online_id(self, mock_get_board):
        target_type = "platform"

        assert get_board_by_online_id(slug="slug", target_type=target_type) == mock_get_board.return_value

        # Test callable matches correct boards
        fn = mock_get_board.call_args[0][0]

        matching_board_1 = make_board(target_type=target_type, slug="slug")
        matching_board_2 = make_board(target_type=target_type, slug="SlUg")
        not_matching_board = make_board(target_type=target_type, slug="whatever")

        assert fn(matching_board_1)
        assert fn(matching_board_2)
        assert not fn(not_matching_board)


class TestGetBoardByJlinkSlug:
    def test_matches_boards_by_online_id(self, mock_get_board):
        assert get_board_by_jlink_slug(slug="slug") == mock_get_board.return_value

        # Test callable matches correct boards
        fn = mock_get_board.call_args[0][0]

        matching_board_1 = make_board(slug="slug")
        matching_board_2 = make_board(board_type="slug")
        matching_board_3 = make_board(board_name="slug")
        not_matching_board = make_board()

        assert fn(matching_board_1)
        assert fn(matching_board_2)
        assert fn(matching_board_3)
        assert not fn(not_matching_board)
