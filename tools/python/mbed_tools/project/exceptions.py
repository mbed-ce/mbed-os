#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Public exceptions exposed by the package."""

from mbed_tools.lib.exceptions import ToolsError


class MbedProjectError(ToolsError):
    """Base exception for mbed-project."""


class VersionControlError(MbedProjectError):
    """Raised when a source control management operation failed."""


class ExistingProgramError(MbedProjectError):
    """Raised when a program already exists at a given path."""


class ProgramNotFoundError(MbedProjectError):
    """Raised when an expected program is not found."""


class MbedOSNotFoundError(MbedProjectError):
    """A valid copy of MbedOS was not found."""
