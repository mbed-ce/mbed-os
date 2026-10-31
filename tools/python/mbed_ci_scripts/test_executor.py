"""
A test executor for Mbed CE.

Test Executor performs the following steps:
1. Read a config file for what boards are connected to the current machine and how to talk to them
2. Compile Mbed for each of the available boards. (compile jobs happen one at a time, since each one will use all the cores on the machine)
3. Run a CTest job for each of the boards. This happens in parallel.

Copyright (c) 2026 Jamie Smith
SPDX-License-Identifier: Apache-2.0
"""

import pathlib
import shutil
import subprocess
import sys

import pydantic

from mbed_tools.lib.json_helpers import decode_json_file

from . import mbed_test_database, test_run_parser
from .allowed_test_failures import AllowedTestFailures


class BoardConfiguration(pydantic.BaseModel):
    mbed_target: str
    """ Mbed target name of the board """

    upload_method: str
    """ Upload method to use for this board. """

    baremetal: bool
    """ Whether to build mbed-baremetal or mbed-os. """

    upload_serial_number: str | None = None
    """ Serial number that will be passed to MBED_UPLOAD_SERIAL_NUMBER """

    com_port: str | None = None
    """ COM port name or TTY path for this board. Defaults to '/dev/tty<mbed_target>' if unset. """

    extra_cmake_options: dict[str, str] = {}
    """ Additional options to pass to CMake, as k-v pairs """


class TestExecutorConfig(pydantic.BaseModel):
    """
    Top-level configuration for the test executor.
    """

    boards: list[BoardConfiguration]


def build_mbed_ce(source_dir: pathlib.Path, build_dir: pathlib.Path, configuration: BoardConfiguration) -> None:
    # Find executables
    cmake_executable = shutil.which("cmake")
    if cmake_executable is None:
        msg = "Missing `cmake` on PATH!"
        raise RuntimeError(msg)

    ninja_executable = shutil.which("ninja")
    if ninja_executable is None:
        msg = "Missing `ninja` on PATH!"
        raise RuntimeError(msg)

    # Create build dir if it doesn't exist
    build_dir.mkdir(parents=True, exist_ok=True)

    # Build CMake args
    cmake_command: list[str] = [
        cmake_executable,
        str(source_dir.resolve()),
        "-GNinja",
        "-DMBED_TARGET=" + configuration.mbed_target,
        "-DUPLOAD_METHOD=" + configuration.upload_method,
        "-DMBED_BUILD_GREENTEA_TESTS=TRUE",
        # Use Release build as it generates the smallest build dirs and ensures that we find bugs caused
        # by optimization in testing. (though the cost is that we can't use a debugger with code built
        # by this script).
        "-DCMAKE_BUILD_TYPE=Release",
    ]
    if configuration.upload_serial_number is not None:
        cmake_command.append("-DMBED_UPLOAD_SERIAL_NUMBER=" + configuration.upload_serial_number)
    if configuration.com_port is not None:
        cmake_command.append("-DMBED_GREENTEA_SERIAL_PORT=" + configuration.com_port)
    else:
        cmake_command.append("-DMBED_GREENTEA_SERIAL_PORT=/dev/tty" + configuration.mbed_target)
    app_json_file_name = "greentea_baremetal.json5" if configuration.baremetal else "greentea_full.json5"
    cmake_command.append(f"-DMBED_APP_JSON_PATH={source_dir.resolve()!s}/TESTS/configs/{app_json_file_name}")
    cmake_command.extend(f"-D{var}={val}" for var, val in configuration.extra_cmake_options.items())

    print(f">> Configuring Mbed for {configuration.mbed_target}...")
    cmake_result = subprocess.run(cmake_command, check=False, cwd=str(build_dir))
    if cmake_result.returncode != 0:
        print("CMake configuration failed.")
        sys.exit(1)
    print(f">> Configuring Mbed for {configuration.mbed_target} -- done")

    print(f">> Building Mbed for {configuration.mbed_target}...")
    build_result = subprocess.run(
        [ninja_executable],
        check=False,
        cwd=str(build_dir),
    )
    if build_result.returncode != 0:
        print("Build failed.")
        sys.exit(1)
    print(f">> Building Mbed for {configuration.mbed_target} -- done")


def run_ctest(
    build_dir: pathlib.Path,
    test_result_dir: pathlib.Path,
    configuration: BoardConfiguration,
    test_name_match_string: str | None,
) -> tuple[subprocess.Popen, pathlib.Path]:
    ctest_executable = shutil.which("ctest")
    if ctest_executable is None:
        msg = "Missing `ctest` on PATH!"
        raise RuntimeError(msg)

    result_junit_path = test_result_dir / f"mbed-tests-{configuration.mbed_target}.xml"
    ctest_command = [
        ctest_executable,
        "--output-on-failure",
        "--output-junit",
        str(result_junit_path.resolve()),
        "--test-output-size-passed",
        "100000",
        "--test-output-size-failed",
        "100000",
    ]
    if test_name_match_string is None:
        print(f">> Running all greentea tests for {configuration.mbed_target}...")
    else:
        print(f'>> Running greentea tests matching "{test_name_match_string}" for {configuration.mbed_target}...')
        ctest_command.extend(["-R", test_name_match_string])

    return subprocess.Popen(ctest_command, cwd=str(build_dir)), result_junit_path


def test_flashing_code(top_build_dir: pathlib.Path, loaded_config: TestExecutorConfig) -> None:
    """
    Runs a basic test to verify that we can successfully flash code to all targets.

    This makes sure we fail the test run early if any target is obviously inoperative.
    """
    print(">> Verifying that all targets can be flashed...")
    flash_test_processes = {}
    for board in loaded_config.boards:
        ninja_command = ["ninja", "flash-test-mbed-hal-stack-size-unification"]
        flash_test_processes[board.mbed_target] = subprocess.Popen(
            ninja_command,
            cwd=str(top_build_dir / board.mbed_target),
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
        )
    for mbed_target, process in flash_test_processes.items():
        flash_stdout, _ = process.communicate()
        if process.returncode != 0:
            print(f">> Target {mbed_target} cannot be flashed! Output follows:")
            print(flash_stdout)
            sys.exit(1)
    print(">> All targets appear functional.")


def main() -> None:
    if len(sys.argv) < 5 or len(sys.argv) > 6:
        print(
            f"Usage: {sys.argv[0]} <config_file> <source dir path> <top build dir> <test result dir> [substring to match test names against]"
        )
        sys.exit(1)

    config_file = pathlib.Path(sys.argv[1])
    source_dir = pathlib.Path(sys.argv[2])
    top_build_dir = pathlib.Path(sys.argv[3])
    test_result_dir = pathlib.Path(sys.argv[4])
    test_name_match_string = None if len(sys.argv) == 5 else sys.argv[5]

    # Load configuration
    config_deserialized_json = decode_json_file(config_file)
    loaded_config = TestExecutorConfig.model_validate(config_deserialized_json)
    allowed_test_failures_json = pathlib.Path(__file__).parent / "allowed_test_failures.json5"
    allowed_test_failures = AllowedTestFailures.model_validate(decode_json_file(allowed_test_failures_json))

    # Configure and build
    for board in loaded_config.boards:
        build_mbed_ce(source_dir, top_build_dir / board.mbed_target, board)

    # Verify that we can flash code
    test_flashing_code(top_build_dir, loaded_config)

    # Run tests
    ctest_processes = []
    target_junit_results = {}
    test_result_dir.mkdir(parents=True, exist_ok=True)
    for board in loaded_config.boards:
        process, junit_result = run_ctest(
            top_build_dir / board.mbed_target, test_result_dir, board, test_name_match_string
        )
        ctest_processes.append(process)
        target_junit_results[board.mbed_target] = junit_result

    # Wait for tests to complete
    try:
        for process in ctest_processes:
            process.wait()
        print(">> Test run complete.")
    except KeyboardInterrupt:
        for process in ctest_processes:
            process.kill()
        print(">> Test run aborted.")

    # Create database
    print(">> Creating blank test run database...")
    db_path = test_result_dir / "test_results.db"
    db_path.unlink(missing_ok=True)
    database = mbed_test_database.MbedTestDatabase(db_path)
    database.create_database()
    print(">> Populating target and driver info into database...")
    database.populate_targets_and_drivers(source_dir)

    # Import data
    print(">> Importing test runs into database...")
    for mbed_target, junit_result in target_junit_results.items():
        print(f">> Parsing {junit_result.name} for target {mbed_target}")
        test_run_parser.parse_test_run(database, mbed_target, junit_result)

    # Check results
    print(">> Checking test successes/failures...")
    success, report = allowed_test_failures.check_against_test_db(database)

    if success:
        print("All (expected) tests passed! :D :D")
    else:
        _ = (test_result_dir / "report.txt").write_text(report, encoding="utf-8")
        print("Some tests failed :( Report has been written to report.txt in the test result dir.")
        sys.exit(1)


if __name__ == "__main__":
    main()
