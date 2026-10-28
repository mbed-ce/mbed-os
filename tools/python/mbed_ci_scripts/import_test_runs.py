"""
Import the JUnit test reports from Mbed testing in the given directory into the test database.

Files must be named as <any identifier>-MBED_TARGET_NAME.xml to identify the target in use
"""

import pathlib
import re
import sys

from . import mbed_test_database, test_run_parser

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <path to database to use> <path to directory containing JUnit XML files.>")
        sys.exit(1)

    db_path = pathlib.Path(sys.argv[1])
    junit_dir = pathlib.Path(sys.argv[2])

    database = mbed_test_database.MbedTestDatabase(db_path)

    for file in junit_dir.iterdir():
        if file.is_file() and file.suffix == ".xml":
            match_result = re.match("^.*-([^-]+).xml$", file.name)
            if match_result is None:
                print(f"Warning: {file.name} does not appear to contain the target name.")
            else:
                mbed_target = match_result.group(1)
                print(f">> Parsing {file.name} for target {mbed_target}")
                test_run_parser.parse_test_run(database, mbed_target, file)
                print(">> Done.")

    database.close()
