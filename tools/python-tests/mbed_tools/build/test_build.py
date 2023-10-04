#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import subprocess

from unittest import mock

import pytest

from mbed_tools.build.build import build_project, generate_build_system
from mbed_tools.build.exceptions import MbedBuildError


@pytest.fixture
def subprocess_run():
    with mock.patch("mbed_tools.build.build.subprocess.run", autospec=True) as subproc:
        yield subproc


class TestBuildProject:
    def test_invokes_cmake_with_correct_args(self, subprocess_run):
        build_project(build_dir="cmake_build", target="install")

        subprocess_run.assert_called_with(["cmake", "--build", "cmake_build", "--target", "install"], check=True)

    def test_invokes_cmake_with_correct_args_if_no_target_passed(self, subprocess_run):
        build_project(build_dir="cmake_build")

        subprocess_run.assert_called_with(["cmake", "--build", "cmake_build"], check=True)

    def test_raises_build_error_if_cmake_invocation_fails(self, subprocess_run):
        subprocess_run.side_effect = (None, subprocess.CalledProcessError(1, ""))

        with pytest.raises(MbedBuildError, match="CMake invocation failed"):
            build_project(build_dir="cmake_build")


class TestConfigureProject:
    def test_invokes_cmake_with_correct_args(self, subprocess_run):
        source_dir = "source_dir"
        build_dir = "cmake_build"
        profile = "debug"

        generate_build_system(source_dir, build_dir, profile)

        subprocess_run.assert_called_with(
            ["cmake", "-S", source_dir, "-B", build_dir, "-GNinja", f"-DCMAKE_BUILD_TYPE={profile}"], check=True
        )

    def test_raises_when_ninja_cannot_be_found(self, subprocess_run):
        subprocess_run.side_effect = FileNotFoundError

        with pytest.raises(MbedBuildError, match="Ninja"):
            generate_build_system("", "", "")

    def test_raises_when_cmake_cannot_be_found(self, subprocess_run):
        subprocess_run.side_effect = (None, FileNotFoundError)

        with pytest.raises(MbedBuildError, match="Could not find CMake"):
            generate_build_system("", "", "")
