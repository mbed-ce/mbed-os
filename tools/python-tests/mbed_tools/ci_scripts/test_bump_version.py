#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import pathlib

from unittest import mock

import pytest

from ci_scripts.bump_version import bump_version_from_news_files, news_ext_to_version_type, get_current_version, main


@pytest.mark.parametrize(
    "files,expected_version",
    (
        ((pathlib.Path("blah.feature"), pathlib.Path("blah.misc"), pathlib.Path("blah.bugfix")), "0.1.0"),
        ((pathlib.Path("blah.major"), pathlib.Path("blah.feature"), pathlib.Path("blah.bugfix")), "1.0.0"),
        ((pathlib.Path("blah.doc"), pathlib.Path("blah.misc"), pathlib.Path("blah.bugfix")), "0.0.1"),
    ),
)
def test_bumps_version_according_to_news_file_extension(files, expected_version):
    ver = bump_version_from_news_files("0.0.0", files)

    assert ver == expected_version


@pytest.mark.parametrize(
    "files",
    (
        tuple(),
        (pathlib.Path("blah.doc"), pathlib.Path("blah.removal"), pathlib.Path("blah.misc")),
        (pathlib.Path("blah.md"),),
    ),
)
def test_raises_when_significant_news_file_not_found(files):
    with pytest.raises(ValueError):
        bump_version_from_news_files("0.0.0", files)


@pytest.mark.parametrize(
    "news_exts,expected",
    (((".feature", ".major", ".bugfix"), "major"), ((".feature", ".bugfix"), "minor"), ((".misc", ".bugfix"), "patch")),
)
def test_converts_news_ext_to_version_type(news_exts, expected):
    version_type = news_ext_to_version_type(news_exts)

    assert version_type == expected


@mock.patch("ci_scripts.bump_version.git.Repo")
def test_get_version_returns_version_from_git(mock_repo):
    mock_version = "0.0.0"
    mock_repo().__enter__().git.describe.return_value = mock_version

    version_str = get_current_version()

    assert version_str == mock_version


@mock.patch("ci_scripts.bump_version.set_log_level")
@mock.patch("ci_scripts.bump_version.bump_version_from_news_files")
@mock.patch("ci_scripts.bump_version.get_current_version")
def test_main_execution_context(get_current_version, bump_version_from_news_files, set_log_level, tmp_path):
    class Args:
        verbosity = 0
        news_file_directory = pathlib.Path(tmp_path, "fake", "news")

    fake_version = "0.0.0"
    fake_args = Args()
    fake_args.news_file_directory.mkdir(parents=True)
    get_current_version.return_value = fake_version

    ret = main(fake_args)

    set_log_level.assert_called_once_with(fake_args.verbosity)
    bump_version_from_news_files.assert_called_once_with(fake_version, list(fake_args.news_file_directory.iterdir()))
    assert ret == 0
