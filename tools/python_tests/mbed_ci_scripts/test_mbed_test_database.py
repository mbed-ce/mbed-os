"""
Schema for a JSON5 file listing allowed test failures

Copyright (c) 2026 Jamie Smith
SPDX-License-Identifier: Apache-2.0
"""

import warnings

from mbed_ci_scripts.mbed_test_database import MbedTestDatabase
from mbed_license_checker.license_checker import MBED_OS_ROOT_DIR


def test_smoke_creating_mbed_test_database() -> None:
    """
    Smoke test that verifies we can build an Mbed test database from the current source tree without errors
    """

    db = MbedTestDatabase()

    with warnings.catch_warnings():
        db.create_database()
        db.populate_targets_and_drivers(MBED_OS_ROOT_DIR)
