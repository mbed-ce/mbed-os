#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import pathlib
import tempfile
from unittest import TestCase

from mbed_tools.build._internal.write_files import write_file
from mbed_tools.build.exceptions import InvalidExportOutputDirectory


class TestWriteFile(TestCase):
    def test_writes_content_to_file(self):
        with tempfile.TemporaryDirectory() as directory:
            content = "Some rendered content"
            export_path = pathlib.Path(directory, "output", "some_file.txt")

            write_file(export_path, content)

            created_file = pathlib.Path(export_path)
            self.assertEqual(created_file.read_text(), content)

    def test_output_dir_is_file(self):
        with tempfile.TemporaryDirectory() as directory:
            bad_export_dir = pathlib.Path(directory, "some_file.txt", ".txt")
            bad_export_dir.parent.touch()
            with self.assertRaises(InvalidExportOutputDirectory):
                write_file(bad_export_dir, "some contents")
