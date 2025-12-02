#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Exceptions internal to the package."""

from mbed_tools.lib.exceptions import ToolsError


class OperatingSystemError(ToolsError):
    """Exception with regards to the underlying operating system."""


class NoBoardForCandidateError(ToolsError):
    """Raised when board data cannot be determined for a candidate."""


class ResolveBoardError(ToolsError):
    """There was an error resolving the board for a device."""
