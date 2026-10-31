"""
Test suite for test_run_parser.

Copyright (c) 2026 Jamie Smith
SPDX-License-Identifier: Apache-2.0
"""

from collections import OrderedDict

from mbed_ci_scripts import test_run_parser
from mbed_ci_scripts.mbed_test_database import MbedTestDatabase, TestResult
import pathlib

THIS_SCRIPT_DIR = pathlib.Path(__file__).parent


def test_parse_regular_test_suite_run():
    """
    Parse a regular test run (from running the test shield UART test) and extract the failed, skipped, and passed
    test cases.
    """

    test_suite_name = "test-testshield-uart"
    test_suite_text = (THIS_SCRIPT_DIR / "regular_test_suite_run.txt").read_text()
    test_suite_result = TestResult.FAILED

    # Create a database for test use with one target.
    # Note that this target needs to be its own MCU family to satisfy foreign key constraints
    db = MbedTestDatabase()
    db.create_database()
    db.add_target("TEST_TARGET", is_mcu_family=True, mcu_family_target="TEST_TARGET")

    # Satisfy foreign key constraint by adding the test to the tests table
    db.add_test_record(test_suite_name, "TEST_TARGET", 0, test_suite_result, test_suite_text)

    # Run test runner
    test_run_parser._parse_test_suite(db, "TEST_TARGET", "test-testshield-uart", test_suite_text, test_suite_result)

    # Check data in database
    assert db.get_tests() == [test_suite_name]
    assert db.get_test_results() == {test_suite_name: {"TEST_TARGET": TestResult.FAILED}}

    expected_test_cases = {
        "Send test string from MCU once (1200 baud)": {"TEST_TARGET": TestResult.PASSED},
        "Receive test string from PC once (1200 baud)": {"TEST_TARGET": TestResult.FAILED},
        "Send test string from MCU once (9600 baud)": {"TEST_TARGET": TestResult.PASSED},
        "Receive test string from PC once (9600 baud)": {"TEST_TARGET": TestResult.PASSED},
        "Send test string from MCU once (115200 baud)": {"TEST_TARGET": TestResult.PASSED},
        "Receive test string from PC once (115200 baud)": {"TEST_TARGET": TestResult.PASSED},
        "Send test string from MCU once (921600 baud)": {"TEST_TARGET": TestResult.PASSED},
        "Receive test string from PC once (921600 baud)": {"TEST_TARGET": TestResult.FAILED},
        "Send test string from MCU once (3000000 baud)": {"TEST_TARGET": TestResult.SKIPPED},
        "Receive test string from PC once (3000000 baud)": {"TEST_TARGET": TestResult.SKIPPED},
    }
    actual_test_cases = db.get_test_details("test-testshield-uart")
    assert expected_test_cases == actual_test_cases

    # Verify that the right test results got returned in the right order
    assert OrderedDict(expected_test_cases) == OrderedDict(actual_test_cases), "Order incorrect!"

    # Spot check that we extracted the right test case output
    assert (
        db.get_test_case_run_output(
            "test-testshield-uart", "Receive test string from PC once (1200 baud)", "TEST_TARGET"
        )
        == """{{__testcase_start;Receive test string from PC once (1200 baud)}}, queued...
[+7870ms][CONN][INF] found KV pair in stream: {{setup_port_at_baud;1200}}, queued...
[+7886ms][SERI][TXD] {{setup_port_at_baud;complete}}
[+7898ms][CONN][INF] found KV pair in stream: {{send_test_string;1}}, queued...
[+7910ms][SERI][TXD] {{send_test_string;started}}
[+8344ms][CONN][RXD] <greentea test suite>:156::FAIL: Expected 'The quick brown fox jumps over the lazy dog.\\n' Was 'xThe quick brown fox jumps over the lazy dog.'
[+8345ms][CONN][INF] found KV pair in stream: {{__testcase_finish;Receive test string from PC once (1200 baud);0;1}}"""
    )
