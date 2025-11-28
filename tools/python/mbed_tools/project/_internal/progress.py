#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Progress bar for git operations."""

from __future__ import annotations

import sys
from typing import Any, Optional

from git import RemoteProgress
from tqdm import tqdm
from typing_extensions import override

from mbed_tools.project.exceptions import VersionControlError


class ProgressBar(tqdm):
    """tqdm progress bar that can be used in a callback context."""

    total: Any

    def update_progress(self, block_num: float = 1, block_size: float = 1, total_size: Optional[float] = None) -> None:
        """
        Update the progress bar.

        Args:
            block_num: Number of the current block.
            block_size: Size of the current block.
            total_size: Total size of all expected blocks.
        """
        if total_size is not None and self.total != total_size:
            self.total = total_size
        _ = self.update(block_num * block_size - self.n)


class ProgressReporter(RemoteProgress):
    """GitPython RemoteProgress subclass that displays a progress bar for git fetch and push operations."""

    bar: Optional[ProgressBar] = None

    def __init__(self, *args: Any, name: str = "", **kwargs: Any) -> None:
        """
        Initialiser.

        Args:
            name: The name of the git repository to report progress on.
        """
        self.name = name
        super().__init__(*args, **kwargs)

    @override
    def update(
        self, op_code: int, cur_count: str | float, max_count: str | float | None = None, message: str = ""
    ) -> None:
        """
        Called whenever the progress changes.

        Args:
            op_code: Integer describing the stage of the current operation.
            cur_count: Current item count.
            max_count: Maximum number of items expected.
            message: Message string describing the number of bytes transferred in the WRITING operation.
        """
        cur_count = float(cur_count)
        if max_count is not None:
            max_count = float(max_count)

        if self.BEGIN & op_code:
            self.bar = ProgressBar(total=max_count, file=sys.stderr, leave=False)

        if self.bar is None:
            err_message = "Did not get BEGIN opcode from git first!"
            raise VersionControlError(err_message)

        self.bar.desc = f"{self.name} {self._cur_line}"
        self.bar.update_progress(block_num=cur_count, total_size=max_count)

        if self.END & op_code:
            self.bar.close()
