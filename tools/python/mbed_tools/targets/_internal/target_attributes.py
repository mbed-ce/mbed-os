#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""
Internal helper to retrieve target attribute information.

This information is parsed from the targets.json configuration file
found in the mbed-os repo.
"""

from __future__ import annotations

import logging
import pathlib
from typing import Any, Dict, Optional, Set

from mbed_tools.build._internal.config.schemas import TargetJSON
from mbed_tools.lib.exceptions import ToolsError
from mbed_tools.lib.json_helpers import decode_json_file
from mbed_tools.targets._internal.targets_json_parsers.accumulating_attribute_parser import (
    get_accumulating_attributes_for_target,
)
from mbed_tools.targets._internal.targets_json_parsers.overriding_attribute_parser import (
    get_labels_for_target,
    get_overriding_attributes_for_target,
    NON_INHERITED_ATTRIBUTES,
)

INTERNAL_PACKAGE_DIR = pathlib.Path(__file__).parent
MBED_OS_METADATA_FILE = pathlib.Path(INTERNAL_PACKAGE_DIR, "data", "targets_metadata.json")

logger = logging.getLogger(__name__)


class TargetAttributesError(ToolsError):
    """Target attributes error."""


class ParsingTargetsJSONError(TargetAttributesError):
    """targets.json parsing failed."""


class TargetNotFoundError(TargetAttributesError):
    """Target definition not found in targets.json."""


def get_target_attributes(
    targets_json_data: dict[str, TargetJSON], target_name: str, allow_non_public_targets: bool = False
) -> dict:
    """
    Retrieves attribute data taken from targets.json for a single target.

    Args:
        targets_json_data: target definitions from targets.json
        target_name: the name of the target (often a Board's board_type).
        allow_non_public_targets: If set to True, attributes can be gotten even for non-public targets

    Returns:
        A dictionary representation of the attributes for the target.

    Raises:
        ParsingTargetJSONError: error parsing targets.json
        TargetNotFoundError: there is no target attribute data found for that target.
    """

    target_definition = _extract_full_target_definition(targets_json_data, target_name, allow_non_public_targets)

    # At this point, for now, we stop using the schema and just convert into a dict, as this is where
    # lots of additional, often undocumented attributes start getting added by the config system.
    # Someday this may be refactored more to remove the use of dicts entirely...
    target_attributes = target_definition.model_dump(exclude_unset=True)

    target_attributes["labels"] = get_labels_for_target(targets_json_data, target_name).union(
        _extract_core_labels(target_attributes.get("core", None))
    )
    target_attributes["extra_labels"] = set(target_attributes.get("extra_labels", []))
    target_attributes["features"] = set(target_attributes.get("features", []))
    target_attributes["components"] = set(target_attributes.get("components", []))
    target_attributes["macros"] = set(target_attributes.get("macros", []))
    target_attributes["config"] = _apply_config_overrides(
        target_attributes.get("config", {}), target_attributes.get("overrides", {})
    )
    return target_attributes


def _extract_full_target_definition(
    all_targets_data: Dict[str, TargetJSON], target_name: str, allow_non_public_targets: bool
) -> TargetJSON:
    """
    Extracts the definition for a particular target from all the targets in targets.json, processing the inheritance hierarchy.

    Args:
        all_targets_data: a dictionary representation of the raw targets.json data.
        target_name: the name of the target.
        allow_non_public_targets: If set to True, attributes can be gotten even for non-public targets

    Returns:
        A dictionary representation the target definition.

    Raises:
        TargetNotFoundError: no target definition found in targets.json.
    """
    if target_name not in all_targets_data:
        msg = f"Target attributes for {target_name} not found."
        raise TargetNotFoundError(msg)

    # All target definitions are assumed to be public unless specifically set as public=false
    if not all_targets_data[target_name].public and not allow_non_public_targets:
        msg = f"Cannot get attributes for {target_name} because it is marked non-public in targets JSON.  This likely means you set MBED_TARGET to the name of the MCU rather than the name of the board."
        raise TargetNotFoundError(msg)

    target_attributes = get_overriding_attributes_for_target(all_targets_data, target_name)
    accumulated_attributes = get_accumulating_attributes_for_target(all_targets_data, target_name)
    target_attributes.update(accumulated_attributes)

    # Lastly, manually move over non-inherited attributes to the final target, except for "inherits"
    # which we would like to discard (it has already served its purpose).
    target_data_as_dict = all_targets_data[target_name].model_dump(exclude_unset=True)
    for attr_name in NON_INHERITED_ATTRIBUTES:
        if attr_name != "inherits":
            if attr_name in target_data_as_dict:
                target_attributes[attr_name] = target_data_as_dict[attr_name]

    # Once we have combined together all the target attributes, we can combine them into one target JSON
    # model again.
    return TargetJSON.model_validate(target_attributes)


def _extract_core_labels(target_core: Optional[str]) -> Set[str]:
    """
    Find the labels associated with the target's core.

    Args:
        target_core: the target core, set as a build attribute

    Returns:
        A list of labels associated with the target's core, or an empty set
        if either core is undefined or no labels found for the core.
    """
    if target_core:
        mbed_os_metadata = decode_json_file(MBED_OS_METADATA_FILE)
        return set(mbed_os_metadata["CORE_LABELS"].get(target_core, []))
    return set()


def _apply_config_overrides(config: Dict[str, Any], overrides: Dict[str, Any]) -> Dict[str, Any]:
    """
    Returns the config attribute with any overrides applied.

    Args:
        config: the cumulative config settings defined for a target
        overrides: the values that need to be changed in the config settings for this target

    Returns:
        The config settings with the overrides applied.

    Raises:
        TargetsJsonConfigurationError: overrides can't be applied to config settings that aren't already defined
    """
    config = config.copy()
    for key, override in overrides.items():
        try:
            config[key]["value"] = override
        except KeyError:
            logger.warning(
                f"Cannot apply override {key}={override}, there is no config setting defined matching that name."
            )
    return config
