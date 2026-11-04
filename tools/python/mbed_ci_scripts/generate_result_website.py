"""
Script to format the target, driver, & test data from an Mbed test database as an HTML website.

Copyright (c) 2026 Jamie Smith
SPDX-License-Identifier: Apache-2.0
"""

import pathlib
import sys

from . import mbed_test_database
from .website_generator import generate_tests_and_targets_website

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <path to database to use> <path to generate site at>")
        sys.exit(1)

    # Load database
    db_path = pathlib.Path(sys.argv[1])
    database = mbed_test_database.MbedTestDatabase(db_path)

    print(">> Generating Website...")
    html_gen_dir = pathlib.Path(sys.argv[2])
    generate_tests_and_targets_website(database, html_gen_dir)

    print("Done.")
