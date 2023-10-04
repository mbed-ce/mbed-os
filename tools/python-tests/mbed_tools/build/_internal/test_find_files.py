#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import contextlib
from pathlib import Path
from tempfile import TemporaryDirectory
from unittest import TestCase
from typing import Iterable


from mbed_tools.build._internal.find_files import find_files, filter_files, MbedignoreFilter, LabelFilter, _find_files


@contextlib.contextmanager
def create_files(files: Iterable[Path]):
    with TemporaryDirectory() as temp_directory:
        temp_directory = Path(temp_directory)
        for file in files:
            file_path = temp_directory / file
            file_directory = file_path.parent
            file_directory.mkdir(parents=True, exist_ok=True)
            file_path.touch()
        yield temp_directory


class TestFindFiles(TestCase):
    def test_finds_files_by_name(self):
        matching_paths = [
            Path("file.txt"),
            Path("sub_directory", "file.txt"),
        ]
        excluded_paths = [
            Path("not_interested.txt"),
            Path("sub_directory", "not_interested.txt"),
        ]

        with create_files(matching_paths + excluded_paths) as directory:
            subject = find_files("file.txt", directory)

        self.assertEqual(len(subject), len(matching_paths))
        for path in matching_paths:
            self.assertIn(Path(directory, path), subject)

    def test_respects_mbedignore(self):
        matching_paths = [
            Path("file.txt"),
        ]
        excluded_paths = [
            Path("foo", "file.txt"),
            Path("bar", "file.txt"),
        ]
        with create_files(matching_paths + excluded_paths) as directory:
            Path(directory, ".mbedignore").write_text("foo/*")
            Path(directory, "bar", ".mbedignore").write_text("*")

            subject = find_files("file.txt", directory)

        self.assertEqual(len(subject), len(matching_paths))
        for path in matching_paths:
            self.assertIn(Path(directory, path), subject)

    def test_respects_legacy_filters(self):
        matching_paths = [
            Path("file.txt"),
        ]
        excluded_paths = [
            Path("TESTS", "file.txt"),
            Path("bar", "TEST_APPS" "file.txt"),
        ]
        with create_files(matching_paths + excluded_paths) as directory:
            subject = find_files("file.txt", directory)

        self.assertEqual(len(subject), len(matching_paths))
        for path in matching_paths:
            self.assertIn(Path(directory, path), subject)

    def test_finds_all_with_no_filters(self):
        matching_paths = [
            Path("file.txt"),
            Path("bar", "file.txt"),
        ]
        with create_files(matching_paths) as directory:
            subject = _find_files("file.txt", directory)

        self.assertEqual(len(subject), len(matching_paths))
        for path in matching_paths:
            self.assertIn(Path(directory, path), subject)


class TestFilterFiles(TestCase):
    def test_respects_given_filters(self):
        matching_paths = [
            Path("foo", "file.txt"),
            Path("hello", "world", "file.txt"),
        ]
        excluded_paths = [
            Path("bar", "file.txt"),
            Path("foo", "foo-bar", "file.txt"),
        ]

        def my_filter(path):
            return "bar" not in str(path)

        subject = filter_files((matching_paths + excluded_paths), [my_filter])

        self.assertEqual(len(subject), len(matching_paths))
        for path in matching_paths:
            self.assertIn(path, subject)


class TestLabelFilter(TestCase):
    def test_matches_paths_not_following_label_rules(self):
        subject = LabelFilter("TARGET", ["BAR", "BAZ"])

        self.assertFalse(subject(Path("mbed-os", "TARGET_FOO", "some_file.c")))
        self.assertFalse(subject(Path("mbed-os", "TARGET_BAR", "TARGET_FOO", "other_file.c")))

    def test_does_not_match_paths_following_label_rules(self):
        subject = LabelFilter("TARGET", ["BAR", "BAZ"])

        self.assertTrue(subject(Path("mbed-os", "TARGET_BAR", "some_file.c")))
        self.assertTrue(subject(Path("mbed-os", "COMPONENT_X", "header.h")))
        self.assertTrue(subject(Path("mbed-os", "COMPONENT_X", "TARGET_BAZ", "some_file.c")))
        self.assertTrue(subject(Path("README.md")))


class TestMbedignoreFilter(TestCase):
    def test_matches_files_by_name(self):
        subject = MbedignoreFilter(("*.py",))

        self.assertFalse(subject("file.py"))
        self.assertFalse(subject("nested/file.py"))
        self.assertTrue(subject("file.txt"))

    def test_matches_wildcards(self):
        subject = MbedignoreFilter(("*/test/*",))

        self.assertFalse(subject("foo/test/bar.txt"))
        self.assertFalse(subject("bar/test/other/file.py"))
        self.assertTrue(subject("file.txt"))

    def test_from_file(self):
        with TemporaryDirectory() as temp_directory:
            mbedignore = Path(temp_directory, ".mbedignore")
            mbedignore.write_text(
                """
# Comment

foo/*.txt
*.py
"""
            )

            subject = MbedignoreFilter.from_file(mbedignore)

            self.assertEqual(
                subject._patterns, (str(Path(temp_directory, "foo/*.txt")), str(Path(temp_directory, "*.py")),)
            )
