#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Helpers for json related functions."""
import json
import pyjson5
import logging

from pathlib import Path
from typing import Any

logger = logging.getLogger(__name__)


def decode_json_file(path: Path) -> Any:
    """Return the contents of json file."""
    if path.suffix == '.json':
        try:
            logger.debug(f"Loading JSON file {path}")
            return json.loads(path.read_text())
        except json.JSONDecodeError:
            logger.error(f"Failed to decode JSON data in the file located at '{path}'")
            raise
    elif path.suffix == '.json5':
        try:
            logger.debug(f"Loading JSON file {path}")
            with path.open() as json_file:
                return pyjson5.decode_io(json_file)
        except ValueError:
            logger.error(f"Failed to decode JSON data in the file located at '{path}'")
            raise
    else:
        raise ValueError(f"Unknown JSON file extension {path.suffix}")
