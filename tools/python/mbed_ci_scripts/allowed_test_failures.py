"""
Schema for a JSON5 file listing allowed test failures

Copyright (c) 2026 Jamie Smith
SPDX-License-Identifier: Apache-2.0
"""

import enum

import pydantic

from .mbed_test_database import MbedTestDatabase, TestResult


class TestCaseStatus(str, enum.Enum):
    """
    Represents the status of a given test case for a given target.
    """

    PASSING = "passing"
    "The default state of a test case. If the test case runs for the target, it must pass"

    FLAKY = "flaky"
    "The test case may pass or fail for this target."

    FAILING = "failing"
    "The test case is expected to always fail (or not run) for the target."


class AllowedTestFailures(pydantic.BaseModel):
    """
    Represents the allowed failures for the Mbed OS test suite
    """

    class TestCaseInfo(pydantic.BaseModel):
        """
        Allowed failure information for a given test case
        """

        default: TestCaseStatus = TestCaseStatus.PASSING
        "State of this test case for targets not explicitly called out."

        target_overrides: dict[str, TestCaseStatus] = pydantic.Field(default_factory=dict)
        "Overrides state of this test case based on target name or other labels"

    allowed_failures: dict[str, dict[str, TestCaseInfo]]
    "Allowed failures, by test name and then test case name."

    def check_against_test_db(self, db: MbedTestDatabase) -> tuple[bool, str]:
        """
        Check a set of test runs against this set of allowed failures.

        :returns: Tuple of (whether this test run passed, report on specific test failures)
        """
        success = True
        report = ""

        tests_to_targets_to_results = db.get_test_results()
        for test_name in tests_to_targets_to_results:
            test_cases_to_targets_to_results = db.get_test_details(test_name)
            for test_case, test_case_targets_to_results in test_cases_to_targets_to_results.items():
                test_allowed_failures = self.allowed_failures.get(test_name, None)
                allowed_failure_info = (
                    None if test_allowed_failures is None else test_allowed_failures.get(test_case, None)
                )
                for target_name, test_result in test_case_targets_to_results.items():
                    # Is this test case allowed to fail?
                    case_status = (
                        TestCaseStatus.PASSING
                        if allowed_failure_info is None
                        else allowed_failure_info.target_overrides.get(target_name, allowed_failure_info.default)
                    )

                    # Check result
                    if case_status == TestCaseStatus.PASSING and test_result not in {
                        TestResult.PASSED,
                        TestResult.SKIPPED,
                    }:
                        success = False
                        report += f"-- Test {test_name} case {test_case} does not have an allowed failure for {target_name} but had result {test_result.name}.\n"
                        report += "-- Output from this test follows:\n"
                        report += db.get_test_case_run_output(test_name, test_case, target_name) + "\n"
                        report += "-- This case either needs to be fixed, or needs to be set to FAILING/FLAKY in targets/allowed_test_failures.json5.\n"

                    if case_status == TestCaseStatus.FAILING and test_result == TestResult.PASSED:
                        success = False
                        report += (
                            f"-- Test {test_name} case {test_case} is set to failing for {target_name} but passed!\n"
                        )
                        report += "-- If this test only fails sometimes, it should be set to FLAKY in targets/allowed_test_failures.json5.\n"

                    # Note: if the test case is set to FLAKY then we allow any test result.

        return success, report
