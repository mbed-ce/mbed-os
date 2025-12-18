#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Configuration assembly algorithm."""

from __future__ import annotations

import logging
from dataclasses import dataclass
from typing import TYPE_CHECKING, Iterable, List, Optional, Set

import pydantic

from mbed_tools.build._internal.config import source
from mbed_tools.build._internal.config.config import Config
from mbed_tools.build._internal.config.schemas import MbedAppJSON
from mbed_tools.build._internal.find_files import LabelFilter, filter_files, find_files
from mbed_tools.lib.json_helpers import decode_json_file

if TYPE_CHECKING:
    from pathlib import Path

    from mbed_tools.project import MbedProgram

logger = logging.getLogger(__name__)


def assemble_config(target_attributes: dict, program: MbedProgram) -> Config:
    """
    Assemble config for given target and program directory.

    Mbed library and application specific config parameters are parsed from mbed_lib.json and mbed_app.json files
    located in the project source tree.
    The config files contain sets of "labels" which correspond to directory names in the mbed-os source tree. These
    labels are used to determine which mbed_lib.json files to include in the final configuration.

    The mbed_app.json config overrides must be applied after all the mbed_lib.json files have been parsed.
    Unfortunately, mbed_app.json may also contain filter labels to tell us which mbed libs we're depending on.
    This means we have to collect the filter labels from mbed_app.json before parsing any other config files.
    Then we parse all the required mbed_lib config and finally apply the app overrides.

    Args:
        target_attributes: Mapping of target specific config parameters.
        program: MbedProgram to build the config for
    """
    mbed_lib_files: Set[Path] = set()

    for path in [program.root, program.mbed_os.root]:
        mbed_lib_files.update(find_files("mbed_lib.json", path.absolute().resolve()))
        mbed_lib_files.update(find_files("mbed_lib.json5", path.absolute().resolve()))

    config, used_mbed_lib_files = _assemble_config_from_sources(
        target_attributes, list(mbed_lib_files), program.files.app_config_file
    )

    # Set up the config source path list using the path to every JSON
    config.json_sources.extend(used_mbed_lib_files)
    if program.files.app_config_file is not None:
        config.json_sources.append(program.files.app_config_file)
    config.json_sources.append(program.mbed_os.targets_json_file)
    config.json_sources.append(program.mbed_os.cmsis_mcu_descriptions_json_file)
    if program.files.custom_targets_json.exists():
        config.json_sources.append(program.files.custom_targets_json)

    # Make all JSON sources relative paths to the program root
    def make_relative_if_possible(path: Path) -> Path:
        # Sadly, Pathlib did not gain a better way to do this until newer python versions.
        try:
            return path.relative_to(program.root)
        except ValueError:
            return path

    config.json_sources = [make_relative_if_possible(json_source) for json_source in config.json_sources]

    return config


def _assemble_config_from_sources(
    target_attributes: dict, mbed_lib_files: List[Path], mbed_app_file: Optional[Path] = None
) -> tuple[Config, list[Path]]:
    config = Config(**source.prepare("merged target JSON", target_attributes, source_name="target"))

    # Process mbed_lib.json files according to the filter.
    filter_data = FileFilterData.from_config(config)
    filtered_files = list(_filter_files(mbed_lib_files, filter_data))
    for config_file in filtered_files:
        mbed_lib_config_source = source.from_mbed_lib_json_file(config_file, target_filters=filter_data.labels)
        config.update(mbed_lib_config_source)
        # Remove any mbed_lib files we've already visited from the list so we don't parse them multiple times.
        mbed_lib_files.remove(config_file)

    # Apply mbed_app.json data last so config parameters are overridden in the correct order.
    if mbed_app_file:
        mbed_app_json_dict = decode_json_file(mbed_app_file)

        # For right now, we check that mbed_app.json does validate against the schema, but we don't fail configuration
        # if it does not pass the schema. This provides compatibility with older projects, as mbed_app.json has
        # historically been a total wild west where any internal Mbed state could potentially be overridden.
        try:
            _ = MbedAppJSON.model_validate(mbed_app_json_dict, strict=True)
        except pydantic.ValidationError as ex:
            logger.warning(
                "mbed_app.json5 failed to validate against the schema. This likely means it contains deprecated attributes, misspelled attributes, or overrides for things that should not be set in mbed_app.json5. This version of mbed-os still allows this, but this will change in the future."
            )
            logger.warning("Error was: %s", str(ex))

        app_data = source.prepare(
            "mbed_app.json5", mbed_app_json_dict, source_name="app", target_filters=filter_data.labels
        )
        config.update(app_data)

    return config, filtered_files


@dataclass(frozen=True)
class FileFilterData:
    """Data used to navigate mbed-os directories for config files."""

    labels: Set[str]
    features: Set[str]
    components: Set[str]
    requires: Set[str]

    @classmethod
    def from_config(cls, config: Config) -> FileFilterData:
        """Extract file filters from a Config object."""
        return cls(
            labels=config.get("labels", set()) | config.get("extra_labels", set()),
            features=set(config.get("features", set())),
            components=set(config.get("components", set())),
            requires=set(config.get("requires", set())),
        )


def _filter_files(files: Iterable[Path], filter_data: FileFilterData) -> Iterable[Path]:
    filters = (
        LabelFilter("TARGET", filter_data.labels),
        LabelFilter("FEATURE", filter_data.features),
        LabelFilter("COMPONENT", filter_data.components),
    )
    return filter_files(files, filters)
