#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Objects representing Mbed program and library data."""

import logging
from dataclasses import dataclass
from pathlib import Path
from typing import Optional

logger = logging.getLogger(__name__)

# Mbed program file names and constants.
APP_CONFIG_FILE_NAME_JSON = "mbed_app.json"
APP_CONFIG_FILE_NAME_JSON5 = "mbed_app.json5"
BUILD_DIR = "cmake_build"
CMAKELISTS_FILE_NAME = "CMakeLists.txt"
MAIN_CPP_FILE_NAME = "main.cpp"
MBED_OS_REFERENCE_FILE_NAME = "mbed-os.lib"
MBED_OS_DIR_NAME = "mbed-os"
TARGETS_JSON_FILE_PATH = Path("targets", "targets.json5")
CMSIS_MCU_DESCRIPTIONS_JSON_FILE_PATH = Path("targets", "cmsis_mcu_descriptions.json5")
CUSTOM_TARGETS_JSON_FILE_NAME = "custom_targets.json"
CUSTOM_TARGETS_JSON5_FILE_NAME = "custom_targets.json5"

# Information written to mbed-os.lib
MBED_OS_REFERENCE_URL = "https://github.com/ARMmbed/mbed-os"
MBED_OS_REFERENCE_ID = "master"

# For some reason Mbed OS expects the default mbed_app.json to contain some target_overrides
# for the K64F target. TODO: Find out why this wouldn't be defined in targets.json.
DEFAULT_APP_CONFIG = {"target_overrides": {"K64F": {"platform.stdio-baud-rate": 9600}}}


@dataclass
class MbedProgramFiles:
    """
    Files defining an MbedProgram.

    This object holds paths to the various files which define an MbedProgram.

    MbedPrograms must contain an mbed-os.lib reference file, defining Mbed OS as a program dependency. A program can
    optionally include an mbed_app.json file which defines application level config.

    Attributes:
        app_config_file: Path to mbed_app.json file. This can be `None` if the program doesn't set any custom config.
        mbed_os_ref: Library reference file for MbedOS. All programs require this file.
        cmakelists_file: A top-level CMakeLists.txt containing build definitions for the application.
        cmake_build_dir: The CMake build tree.
    """

    app_config_file: Optional[Path]
    mbed_os_ref: Path
    cmakelists_file: Path
    cmake_build_dir: Path
    custom_targets_json: Path

    @classmethod
    def from_existing(cls, root_path: Path, build_dir: Path) -> "MbedProgramFiles":
        """
        Create MbedProgramFiles from a directory containing an existing program.

        Args:
            root_path: The path containing the MbedProgramFiles.
            build_dir: The directory to use for CMake build.
        """
        app_config: Optional[Path] = None
        if (root_path / APP_CONFIG_FILE_NAME_JSON5).exists():
            app_config = root_path / APP_CONFIG_FILE_NAME_JSON5
        elif (root_path / APP_CONFIG_FILE_NAME_JSON).exists():
            app_config = root_path / APP_CONFIG_FILE_NAME_JSON

        # If there's already a custom_targets.json5, use that.
        # Otherwise, assume json.
        if (root_path / CUSTOM_TARGETS_JSON5_FILE_NAME).exists():
            custom_targets_json = root_path / CUSTOM_TARGETS_JSON5_FILE_NAME
        else:
            custom_targets_json = root_path / CUSTOM_TARGETS_JSON_FILE_NAME

        mbed_os_file = root_path / MBED_OS_REFERENCE_FILE_NAME

        cmakelists_file = root_path / CMAKELISTS_FILE_NAME
        if not cmakelists_file.exists():
            logger.warning("No CMakeLists.txt found in the program root.")

        return cls(
            app_config_file=app_config,
            mbed_os_ref=mbed_os_file,
            cmakelists_file=cmakelists_file,
            cmake_build_dir=build_dir,
            custom_targets_json=custom_targets_json,
        )


@dataclass
class MbedOS:
    """
    Metadata associated with a copy of MbedOS.

    This object holds information about MbedOS used by MbedProgram.

    Attributes:
        root: The root path of the MbedOS source tree.
        targets_json_file: Path to a targets.json file, which contains target data specific to MbedOS revision.
    """

    root: Path
    targets_json_file: Path
    cmsis_mcu_descriptions_json_file: Path

    @classmethod
    def from_existing(cls, root_path: Path, check_root_path_exists: bool = True) -> "MbedOS":
        """Create MbedOS from a directory containing an existing MbedOS installation."""
        targets_json_file = root_path / TARGETS_JSON_FILE_PATH
        cmsis_mcu_descriptions_json_file = root_path / CMSIS_MCU_DESCRIPTIONS_JSON_FILE_PATH

        if check_root_path_exists and not root_path.exists():
            msg = "The mbed-os directory does not exist."
            raise ValueError(msg)

        if root_path.exists() and not targets_json_file.exists():
            msg = f"This MbedOS copy does not contain a {TARGETS_JSON_FILE_PATH} file."
            raise ValueError(msg)

        if root_path.exists() and not cmsis_mcu_descriptions_json_file.exists():
            msg = f"This MbedOS copy does not contain a {CMSIS_MCU_DESCRIPTIONS_JSON_FILE_PATH.name} file."
            raise ValueError(msg)

        return cls(
            root=root_path,
            targets_json_file=targets_json_file,
            cmsis_mcu_descriptions_json_file=cmsis_mcu_descriptions_json_file,
        )

    @classmethod
    def from_new(cls, root_path: Path) -> "MbedOS":
        """Create MbedOS from an empty or new directory."""
        return cls(
            root=root_path,
            targets_json_file=root_path / TARGETS_JSON_FILE_PATH,
            cmsis_mcu_descriptions_json_file=root_path / CMSIS_MCU_DESCRIPTIONS_JSON_FILE_PATH,
        )
