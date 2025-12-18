#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Helpers for json related functions."""

import json
import logging
from pathlib import Path
from typing import Any

import json5
import pyjson5

logger = logging.getLogger(__name__)


def decode_json_file(path: Path) -> Any:
    """Return the contents of json file."""
    if path.suffix == ".json":
        try:
            logger.debug(f"Loading JSON file {path}")
            return json.loads(path.read_text())
        except json.JSONDecodeError:
            logger.exception(f"Failed to decode JSON data in the file located at '{path}'")
            raise
    elif path.suffix == ".json5":
        logger.debug(f"Loading JSON file {path}")
        try:
            with path.open() as json_file:
                return pyjson5.decode_io(json_file)  # pyright: ignore[reportArgumentType]
        except pyjson5.Json5Exception as ex:
            # Parsing failed. pyjson5 produces completely useless error messages, so reparse the file with
            # json5 for a better one (slow!).
            try:
                with path.open() as json_file:
                    parsed_file = json5.load(json_file)
                    logger.warning(
                        f"JSON5 file {path} could not be decoded with pyjson5 but was decodable with"
                        f"json5. Error from pyjson5 was: {ex!s}"
                    )
                    return parsed_file
            except ValueError as json5_ex:
                logger.exception(f"Failed to decode JSON5 data in the file located at '{path}'")
                raise json5_ex from None
    else:
        msg = f"Unknown JSON file extension {path.suffix}"
        raise ValueError(msg)
