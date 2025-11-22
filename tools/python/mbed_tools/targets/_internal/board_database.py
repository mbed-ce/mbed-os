#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Internal helper to retrieve target information from the online database."""

import json
import logging
import pathlib
from json.decoder import JSONDecodeError
from typing import Any

from mbed_tools.targets._internal.exceptions import ResponseJSONError

INTERNAL_PACKAGE_DIR = pathlib.Path(__file__).parent
SNAPSHOT_FILENAME = "board_database_snapshot.json"

logger = logging.getLogger(__name__)


def get_board_database_path() -> pathlib.Path:
    """Return the path to the offline board database."""
    return pathlib.Path(INTERNAL_PACKAGE_DIR, "data", SNAPSHOT_FILENAME)


_BOARD_API = "https://os.mbed.com/api/v4/targets"


def get_offline_board_data() -> Any:
    """
    Loads board data from JSON stored in offline snapshot.

    Returns:
        The board database as retrieved from the local database snapshot.

    Raises:
        ResponseJSONError: error decoding the local database JSON.
    """
    boards_snapshot_path = get_board_database_path()
    try:
        return json.loads(boards_snapshot_path.read_text())
    except JSONDecodeError as json_err:
        msg = f"Invalid JSON received from '{boards_snapshot_path}'."
        raise ResponseJSONError(msg) from json_err
