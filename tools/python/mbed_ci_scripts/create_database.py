"""
Script to create a database for storing Mbed testing information and initially populate it with data.

If the output file path already exists it will be deleted and recreated.

Copyright (c) 2026 Jamie Smith
SPDX-License-Identifier: Apache-2.0
"""

import pathlib
import sys

from . import mbed_test_database

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <path to Mbed OS> <path to database to initialize>")
        sys.exit(1)

    # Create database
    db_path = pathlib.Path(sys.argv[2])
    db_path.unlink(missing_ok=True)
    database = mbed_test_database.MbedTestDatabase(db_path)
    print(">> Creating Database...")
    database.create_database()
    print(">> Populating Target and Driver Info into Database...")
    database.populate_targets_and_drivers(pathlib.Path(sys.argv[1]))

    database.close()
    print("Done.")
