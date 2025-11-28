#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Defines the public API of the package."""

import logging
import pathlib
from typing import Any, List

from mbed_tools.project._internal import git_utils
from mbed_tools.project._internal.libraries import LibraryReferences
from mbed_tools.project.mbed_program import parse_url

logger = logging.getLogger(__name__)


def import_project(url: str, dst_path: Any = None, recursive: bool = False) -> pathlib.Path:
    """
    Clones an Mbed project from a remote repository.

    Args:
        url: URL of the repository to clone.
        dst_path: Destination path for the repository.
        recursive: Recursively clone all project dependencies.

    Returns:
        The path the project was cloned to.
    """
    git_data = parse_url(url)
    url = git_data["url"]
    if not dst_path:
        dst_path = pathlib.Path(git_data["dst_path"])

    _ = git_utils.clone(url, dst_path)
    if recursive:
        libs = LibraryReferences(root=dst_path, ignore_paths=["mbed-os"])
        libs.fetch()

    return dst_path


def deploy_project(path: pathlib.Path, force: bool = False) -> None:
    """
    Deploy a specific revision of the current Mbed project.

    This function also resolves and syncs all library dependencies to the revision specified in the library reference
    files.

    Args:
        path: Path to the Mbed project.
        force: Force overwrite uncommitted changes. If False, the deploy will fail if there are uncommitted local
               changes.
    """
    libs = LibraryReferences(path, ignore_paths=["mbed-os"])
    libs.checkout(force=force)
    if list(libs.iter_unresolved()):
        logger.info("Unresolved libraries detected, downloading library source code.")
        libs.fetch()


def get_known_libs(path: pathlib.Path) -> List:
    """
    List all resolved library dependencies.

    This function will not resolve dependencies. This will only generate a list of resolved dependencies.

    Args:
        path: Path to the Mbed project.

    Returns:
        A list of known dependencies.
    """
    libs = LibraryReferences(path, ignore_paths=["mbed-os"])
    return sorted(libs.iter_resolved())
