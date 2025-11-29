#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
from mbed_tools.project._internal.project_data import (
    CMAKELISTS_FILE_NAME,
    APP_CONFIG_FILE_NAME_JSON5,
    MBED_OS_REFERENCE_FILE_NAME,
)


def make_mbed_program_files(root, config_file_name=APP_CONFIG_FILE_NAME_JSON5):
    if not root.exists():
        root.mkdir()

    (root / MBED_OS_REFERENCE_FILE_NAME).touch()
    (root / config_file_name).touch()
    (root / CMAKELISTS_FILE_NAME).touch()


def make_mbed_os_files(root):
    if not root.exists():
        root.mkdir()

    targets_dir = root / "targets"
    targets_dir.mkdir()
    (targets_dir / "targets.json5").touch()
    (targets_dir / "cmsis_mcu_descriptions.json5").touch()
