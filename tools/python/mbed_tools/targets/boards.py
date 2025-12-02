#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Interface to the Board Database."""

from __future__ import annotations

import json
from collections.abc import Set as AbstractSet
from dataclasses import asdict
from typing import Callable, Iterable, Iterator, Sequence

from typing_extensions import override

from mbed_tools.targets._internal import board_database
from mbed_tools.targets.board import Board
from mbed_tools.targets.exceptions import UnknownBoardError


class Boards(AbstractSet):
    """
    Interface to the Board Database.

    Boards is initialised with an Iterable[Board]. The classmethods
    can be used to construct Boards with data from either the online or offline database.
    """

    @classmethod
    def from_offline_database(cls) -> Boards:
        """
        Initialise with the offline board database.

        Raises:
            BoardDatabaseError: Could not retrieve data from the board database.
        """
        return cls(Board.from_offline_board_entry(b) for b in board_database.get_offline_board_data())

    def __init__(self, boards_data: Iterable[Board]) -> None:
        """
        Initialise with a list of boards.

        Args:
            boards_data: iterable of board data from a board database source.
        """
        self._boards_data: Sequence[Board] = tuple(boards_data)

    @override
    def __iter__(self) -> Iterator[Board]:
        """Yield an Board on each iteration."""
        yield from self._boards_data

    @override
    def __len__(self) -> int:
        """Return the number of boards."""
        return len(self._boards_data)

    @override
    def __contains__(self, board: object) -> bool:
        """
        Check if a board is in the collection of boards.

        Args:
            board: An instance of Board.
        """
        if not isinstance(board, Board):
            return False

        return any(x == board for x in self)

    def get_board(self, matching: Callable) -> Board:
        """
        Returns first Board for which `matching` returns True.

        Args:
            matching: A function which will be called for each board in database

        Raises:
            UnknownBoard: the given product code was not found in the board database.
        """
        try:
            return next(board for board in self if matching(board))
        except StopIteration:
            raise UnknownBoardError from None

    def json_dump(self) -> str:
        """Return the contents of the board database as a json string."""
        return json.dumps([asdict(b) for b in self], indent=4)
