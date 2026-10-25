"""
Basic license checker script for Mbed.

This script:
- Filters passed files based on a list of directories and extensions that should have their licenses checked
- Finds 'SPDX-License-Identifier:' in each file
- Validates that the license identifier names a valid license and is permissive (using the license_expression package)
- Prints a report of any violating licenses
"""

"""
SPDX-License-Identifier: Apache-2.0

Copyright (c) 2026 Jamie Smith. All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations
"""
import fnmatch
import pathlib
import re
import sys
from collections.abc import Iterable
from typing import cast

import license_expression
import prettytable

# Top-level Mbed OS root dir
MBED_OS_ROOT_DIR = pathlib.Path(__file__).resolve().parent.parent.parent.parent

SPDX_LICENSE_REGEX = re.compile(r"SPDX-License-Identifier: (.+)")


class LicenseCheckFailedError(RuntimeError):
    """
    License check failure exception
    """


# Get JSON representation of license database
license_json = license_expression.get_license_index()

# Add some additional licenses to reflect code currently in Mbed.
license_json.append(
    {
        "license_key": "nordic-5-clause",
        "category": "Permissive",
        "spdx_license_key": "LicenseRef-Nordic-5-Clause",
        "is_exception": False,
        "is_deprecated": False,
    },
)

# Additional license keys to consider as permissive
# This mainly exists for licenses which say a specific body of code can only be used on MCUs from
# a specific manufacturer.
OVERRIDE_PERMISSIVE_LICENSES = {"LicenseRef-scancode-arm-cortex-mx", "LicenseRef-scancode-pbl-1.0"}

# Main license_expression library instance
licensing = license_expression.build_spdx_licensing(license_json)


def get_permissive_license_mapping() -> dict[license_expression.LicenseSymbol, bool]:
    """
    Build mapping of whether a license symbol is permissive.

    Oddly this information is contained within the license index JSON file inside `license_expression`,
    but is NOT made public by the API.
    """
    # Convert JSON into a map of LicenseSymbol() entries.
    # This is similar to the logic in license_expression.build_spdx_licensing()
    return {
        license_expression.LicenseSymbol(
            key=license_entry["spdx_license_key"],
            aliases=license_entry.get("other_spdx_license_keys", []),
            is_deprecated=license_entry.get("is_deprecated", False),
            is_exception=license_entry.get("is_exception", False),
        ): license_entry.get("category", None) == "Permissive"
        or license_entry["spdx_license_key"] in OVERRIDE_PERMISSIVE_LICENSES
        for license_entry in license_json
        if license_entry.get("spdx_license_key", None) is not None
    }


permissive_license_mapping = get_permissive_license_mapping()


def find_license_in_file(file: pathlib.Path) -> license_expression.LicenseExpression:
    """
    Parse a file to detect its license expression.

    This requires a string like "SPDX-License-Identifier:" to be in the first 10k of the file.

    :raises LicenseCheckFailedError: on error
    """
    # Read the first 10kB of the file. That allows for the license expression to not be at the
    # exact start of the file but should be faster than reading the entire thing.
    with open(file, encoding="utf-8") as opened_file:  # noqa: PTH123
        try:
            first_10k = opened_file.read(10000)
        except UnicodeDecodeError as ex:
            raise LicenseCheckFailedError("Failed to read file (not UTF-8 encoded?): " + str(ex)) from ex

    # Find the first instance of an SPDX license identifier
    match = SPDX_LICENSE_REGEX.search(first_10k)
    if match is None:
        msg = "No SPDX-License-Identifier found."
        raise LicenseCheckFailedError(msg)

    # Try to parse as a license identifier
    try:
        return cast(license_expression.LicenseExpression, licensing.parse(match.group(1), validate=True, strict=True))
    except license_expression.ExpressionError as ex:
        message = f"Failed to parse license expression in {file!s}: '{match.group(1)}': {ex!s}"
        raise LicenseCheckFailedError(message) from ex


def is_permissive_license(license_expr: license_expression.LicenseExpression) -> bool:
    """
    Detect whether a license expression allows permissive licensing
    """
    # Simple case: just one license
    if isinstance(license_expr, license_expression.LicenseSymbol):
        return permissive_license_mapping[license_expr]

    # The LicenseExpression class is really just a boolean expression from the `boolean` package.
    # Each license is represented as a symbol in the boolean expression.
    # So, all we gotta do is "substitute" the expression with actual boolean values
    # to find out if it is true (permissive) or false (not permissive)
    substituted_expr = license_expr.subs(
        {
            license_symbol: licensing.TRUE
            if permissive_license_mapping[cast(license_expression.LicenseSymbol, license_symbol)]
            else licensing.FALSE
            for license_symbol in license_expr.symbols
        }
    )

    # Simplify to true or false
    # Note: For some reason pyright seems to hit a false positive issue in CI on the following line.
    # It doesn't reproduce locally :((
    return cast(bool, substituted_expr.simplify()) # pyright: ignore [reportCallIssue]


def verify_permissive(path: pathlib.Path) -> None:
    """
    Verify that a path contains a permissive license.

    :raises LicenseCheckFailedError: if this is not the case
    """
    # Is the file empty? If so we don't need to check the license
    # (this is common in __init__.py's)
    if path.stat().st_size == 0:
        return

    file_license_expression = find_license_in_file(path)

    if not is_permissive_license(file_license_expression):
        message = f"Detected license {file_license_expression!s} which is not known to be permissive."
        raise LicenseCheckFailedError(message)


EXCLUDE_PATH_PATTERNS = [
    # As of 2024, STMicro stopped putting license declarations or SPDX identifiers
    # in its HAL drivers.  Instead they reference a license file in the repo root.
    # We don't want to have to modify all their code to add SPDX license IDs every time
    # it gets updated, so we ignore everything under STM32Cube_FW
    re.compile(r"TARGET_STM/.*STM32Cube_FW"),
    # There are also a few Pico SDK files missing headers
    re.compile(r"TARGET_RASPBERRYPI/pico-sdk"),
]

FILE_PATTERNS_TO_CHECK = ["*.c", "*.cpp", "*.h", "*.hpp", "*.py"]


def scan_licenses(root_paths: Iterable[pathlib.Path]) -> bool:
    """
    Scan the licenses of all files in a given list of files and directories. Then prints a report to stdout.

    Ignores files based on the configured ignore patterns.

    :param root_dirs: Files and directories to scan
    :return: True if all files were OK, false if there were some errors.
    """
    root_paths = [path.resolve() for path in root_paths]

    # Discover files to check matching the given patterns
    files_to_check: list[pathlib.Path] = []
    for root_path in root_paths:
        if root_path.is_file():
            for pattern in FILE_PATTERNS_TO_CHECK:
                if fnmatch.fnmatch(root_path.name, pattern):
                    files_to_check.append(root_path)
        else:
            # List all files
            for pattern in FILE_PATTERNS_TO_CHECK:
                files_to_check.extend(root_path.rglob(pattern))

    # Filter based on ignore rules.
    # Note: Operate on relative paths so we don't match the parent of the Mbed OS dir with an ignore rule.
    def is_not_ignored(path: pathlib.Path) -> bool:
        path_rel_to_root = path.relative_to(MBED_OS_ROOT_DIR)
        return not any(exclude_pattern.search(path_rel_to_root.as_posix()) for exclude_pattern in EXCLUDE_PATH_PATTERNS)

    not_ignored_files = filter(is_not_ignored, files_to_check)

    # Now check em!
    errors: list[tuple[pathlib.Path, str]] = []
    valid_files = 0
    for file in not_ignored_files:
        try:
            verify_permissive(file)
            valid_files += 1
        except LicenseCheckFailedError as ex:
            errors.append((file.relative_to(MBED_OS_ROOT_DIR), str(ex)))

    # Print report
    print(f"Scanned {valid_files} files, found {len(errors)} license errors.")
    if len(errors) == 0:
        return True

    errors = sorted(errors)

    error_table = prettytable.PrettyTable()
    error_table.align = "l"  # left justify
    error_table.field_names = ["File", "Error"]
    error_table.add_rows([[str(error[0]), error[1]] for error in errors])
    print(error_table)
    return False


def main() -> None:
    if len(sys.argv) == 1:
        print(f"Usage: {sys.argv[0]} <files and directories to scan>")
        sys.exit(1)

    if not scan_licenses(pathlib.Path(path) for path in sys.argv[1:]):
        sys.exit(1)


if __name__ == "__main__":
    main()
