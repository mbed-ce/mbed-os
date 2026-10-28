"""
Schema for a JSON5 file listing allowed test failures
"""

import enum
import pathlib
import sys
from collections import defaultdict

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

    @staticmethod
    def extract_from_test_db(db: MbedTestDatabase) -> "AllowedTestFailures":
        """
        Build the list of allowed failures from an already-populated test database.

        Assumes that all tests which failed in this test run should be allowed.
        Mainly useful for initially creating the list for a new target.
        """
        tests_to_targets_to_results = db.get_test_results()

        failures_dict = defaultdict(dict)

        for test_name in tests_to_targets_to_results:
            all_results = tests_to_targets_to_results[test_name].values()
            if any(result == TestResult.FAILED for result in all_results):
                # This test failed for at least one target. Which one(s)?
                test_case_target_results = db.get_test_details(test_name)

                for test_case in test_case_target_results:
                    all_results = test_case_target_results[test_case].values()

                    if (
                        all(result in {TestResult.FAILED, TestResult.PRIOR_TEST_CASE_CRASHED} for result in all_results)
                        and len(all_results) > 1
                    ):
                        # Test case failed for all targets that it was run for.
                        failures_dict[test_name][test_case] = AllowedTestFailures.TestCaseInfo(
                            default=TestCaseStatus.FAILING,
                        )
                    else:
                        # Test case failed for specific targets
                        target_failures = {}
                        for target, result in test_case_target_results[test_case].items():
                            if result in {TestResult.FAILED, TestResult.PRIOR_TEST_CASE_CRASHED}:
                                target_failures[target] = TestCaseStatus.FAILING

                        failures_dict[test_name][test_case] = AllowedTestFailures.TestCaseInfo(
                            target_overrides=target_failures
                        )

        return AllowedTestFailures(allowed_failures=dict(sorted(failures_dict.items())))

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


def main() -> None:
    # Simple main function that processes a test database and generates an allowed-test-failures file.
    # Used for creating the initial allowed-test-failures document.
    db = MbedTestDatabase(pathlib.Path(sys.argv[1]))
    allowed_failures = AllowedTestFailures.extract_from_test_db(db)
    print(allowed_failures.model_dump_json(indent=2, exclude_defaults=True, exclude_none=True, exclude_unset=True))


if __name__ == "__main__":
    main()
