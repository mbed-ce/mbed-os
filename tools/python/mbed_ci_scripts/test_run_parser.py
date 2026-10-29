"""
Script to parse the XML resulting from an Mbed test run and add the results to a given Mbed test database.

Copyright (c) 2026 Jamie Smith
SPDX-License-Identifier: Apache-2.0
"""

import pathlib
import re
from typing import cast

from junitparser import JUnitXml

from . import mbed_test_database
from .mbed_test_database import TestResult

# Regexes for parsing Greentea output
# ------------------------------------------------------------------------

# Matches one line of the test case list sent at the start of each test
GREENTEA_TESTCASE_NAME_RE = re.compile(r"\{\{__testcase_name;([^}]+)}}")

# Matches a test case which completes (either successfully or not) and allows extracting the output
GREENTEA_TESTCASE_OUTPUT_RE = re.compile(
    r"(\{\{__testcase_start;[^|]+?}}.+?\{\{__testcase_finish;[^|]+?;(\d);\d}})", re.DOTALL
)

# Matches if a test was marked as 'skipped' via TEST_SKIP() or TEST_SKIP_MESSAGE()
TEST_SKIPPED_RE = re.compile(r"<greentea test suite>:[0-9]+::SKIP")


def _parse_test_suite(
    database: mbed_test_database.MbedTestDatabase,
    mbed_target: str,
    test_suite_name: str,
    test_suite_output: str,
    test_suite_result: TestResult,
) -> None:
    """
    Parse the output of one Greentea test suite and add it to the database.
    """
    # First use a regex to extract the list of test cases...
    test_case_names = re.findall(GREENTEA_TESTCASE_NAME_RE, test_suite_output)

    # Next, we need some special handling for tests which reset.  These tests print out the list of
    # test cases multiple times, which causes the previous operation to return duplicate results.  Remove those
    # while preserving the test case order.
    test_case_names = list(dict.fromkeys(test_case_names))

    test_case_records: list[tuple[str, str]]

    if len(test_case_names) > 0:
        # This is a "normal" test with test cases.  Parse them.
        # Regex returns tuple of (output, passed/failed indicator)
        test_case_records = re.findall(GREENTEA_TESTCASE_OUTPUT_RE, test_suite_output)

        if len(test_case_records) < len(test_case_names):
            # Did one test case crash the test?
            # See if we can find the start of this test case but no end.
            crashing_test_name = test_case_names[len(test_case_records)]
            crash_re = re.compile(
                r"\{\{__testcase_start;" + crashing_test_name + r"}}(.+?)teardown\(\) finished", re.DOTALL
            )
            test_case_crash_output = re.search(crash_re, test_suite_output)

            if test_case_crash_output is not None:
                print(
                    f"Note: Test case '{crashing_test_name}' in test {test_suite_name} for target {mbed_target} appears to have crashed and prevented {len(test_case_names) - len(test_case_records) - 1} subsequent tests from running"
                )
                test_case_records.append((test_case_crash_output.group(0), "0"))
            else:
                # Otherwise the test simply didn't run the remaining test cases.
                pass

        for test_case_idx, test_case_name in enumerate(test_case_names):
            # If the test actually was run, save its output
            if test_case_idx < len(test_case_records):
                test_case_output = test_case_records[test_case_idx][0]
                if len(re.findall(TEST_SKIPPED_RE, test_case_output)) > 0:
                    result = TestResult.SKIPPED
                elif test_case_records[test_case_idx][1] == "1":
                    result = TestResult.PASSED
                else:
                    result = TestResult.FAILED

                database.add_test_case_record(
                    test_suite_name, test_case_name, test_case_idx, mbed_target, result, test_case_output
                )

            # Otherwise, mark it as prior crashed
            else:
                database.add_test_case_record(
                    test_suite_name, test_case_name, test_case_idx, mbed_target, TestResult.PRIOR_TEST_CASE_CRASHED, ""
                )
    # However, there are some tests (e.g. test-mbed-drivers-dev-null) which don't use the greentea
    # system in a standard way and therefore can't be divided evenly into test cases.  These tests need special
    # handling.
    else:
        # print(f"This test has non-standard output. Treating the entire test as one test case")

        is_skipped = len(re.findall(TEST_SKIPPED_RE, test_suite_output)) > 0

        database.add_test_case_record(
            test_suite_name,
            test_suite_name,
            0,
            mbed_target,
            TestResult.SKIPPED if is_skipped else test_suite_result,
            test_suite_output,
        )


def parse_test_run(
    database: mbed_test_database.MbedTestDatabase, mbed_target: str, junit_xml_path: pathlib.Path
) -> None:
    """
    Parse a JUnit file containing a test run and add/update the test information within into the database.
    """
    junit_report = JUnitXml.fromfile(str(junit_xml_path))

    for test_suite in junit_report:
        for test_report in test_suite:
            # First record info about the larger test.  We can do this entirely using the data recorded by CTest.
            if test_report.is_passed:
                test_suite_result = TestResult.PASSED
            elif test_report.is_skipped:
                test_suite_result = TestResult.SKIPPED
            else:
                test_suite_result = TestResult.FAILED
            test_name = cast(str, test_report.classname)

            database.add_test_record(
                test_name,
                mbed_target,
                cast(float, test_report.time),
                test_suite_result,
                cast(str, test_report.system_out),
            )

            if test_suite_result != TestResult.SKIPPED:
                # Now things get a bit more complicated as we have to parse Greentea's output directly to determine
                # the list of tests.
                _parse_test_suite(
                    database, mbed_target, test_name, cast(str, test_report.system_out), test_suite_result
                )
