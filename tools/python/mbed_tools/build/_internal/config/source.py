#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Configuration source parser."""

from __future__ import annotations

import logging
import typing
from dataclasses import dataclass
from typing import Any, Iterable, List, Literal, Optional, Union

import pydantic

from mbed_tools.build.exceptions import InvalidConfigOverrideError
from mbed_tools.lib.json_helpers import decode_json_file
from mbed_tools.lib.python_helpers import flatten_nested

from . import schemas
from .schemas import ConfigSettingValue

if typing.TYPE_CHECKING:
    import pathlib

logger = logging.getLogger(__name__)


def from_file(
    config_source_file_path: pathlib.Path, target_filters: Iterable[str], default_name: Optional[str] = None
) -> dict:
    """Load a JSON config file and prepare the contents as a config source."""
    return prepare(
        "file " + str(config_source_file_path),
        decode_json_file(config_source_file_path),
        source_name=default_name,
        target_filters=target_filters,
    )


def prepare(
    context: str, input_data: dict, source_name: Optional[str] = None, target_filters: Optional[Iterable[str]] = None
) -> dict:
    """
    Prepare a config source for entry into the Config object.

    Extracts config and override settings from the source. Flattens these nested dictionaries out into
    lists of objects which are namespaced in the way the Mbed config system expects.

    Args:
        context String that will be printed when issuing warnings. Should be something like
            "file foo.json" or "target MCU_FOO".
        input_data: The raw config JSON object parsed from the config file.
        source_name: Optional default name to use for namespacing config settings. If the input_data contains a 'name'
            field, that field is used as the namespace.
        target_filters: List of filter string used when extracting data from target_overrides section of the config
            data.

    Returns:
        Prepared config source.
    """
    data = input_data.copy()
    namespace = data.pop("name", source_name)
    for key in data:
        data[key] = _sanitise_value(data[key])

    if "config" in data:
        data["config"] = _extract_config_settings(context, namespace, data["config"])

    if "overrides" in data:
        data["overrides"] = _extract_overrides(context, namespace, data["overrides"])

    if "target_overrides" in data:
        data["overrides"] = data.get("overrides", []) + _extract_target_overrides(
            context, namespace, data.pop("target_overrides"), target_filters if target_filters is not None else []
        )

    return data


def _to_canonical_json_setting_name(name: str) -> str:
    """
    Transform a JSON setting name to its "canonical" form by replacing underscores with dashes and lowercasing.

    This is used for backwards compatibility, so that "canonical" settings can be overridden by old, non-canonical
    names.
    """
    return name.replace("_", "-").lower()


def check_and_transform_config_name(context: str, config_namespace: str, config_name: str) -> tuple[str, str]:
    """
    Issue a warning if a config name is not recommended to be used, and transform the name into the recommended format.

    :param context: String that will be printed about where this warning comes from. Should be something like
        "file foo.json" or "target MCU_FOO".
    :param config_namespace: Namespace of this config item.
    :param config_name: Name of config

    :return: New namespace and name to use for the config.
    """
    if config_name.lower() != config_name:
        logger.warning(
            f"Config setting '{config_name}' in {context} contains uppercase letters. This style is not recommended and these will be replaced by lowercase letters."
        )
    if config_namespace.lower() != config_namespace:
        logger.warning(
            f"Config namespace '{config_namespace}' in {context} contains uppercase letters. This style is not recommended and these will be replaced by lowercase letters."
        )
    if "." in config_name:
        logger.warning(
            f"Config setting '{config_name}' in {context} contains a period. This style is not recommended as it may cause confusion with the config namespace name."
        )
    if "_" in config_name:
        logger.warning(
            f"Config setting '{config_name}' in {context} contains an underscore. This style is not recommended as it may cause confusion (config names should be in skewer-case). Underscores are replaced by hyphens when Mbed processes JSON settings."
        )
    if "_" in config_namespace:
        logger.warning(
            f"Config namespace '{config_namespace}' in {context} contains an underscore. This style is not recommended as it may cause confusion (config names should be in skewer-case). Underscores are replaced by hyphens when Mbed processes JSON settings."
        )

    return _to_canonical_json_setting_name(config_namespace), _to_canonical_json_setting_name(config_name)


def from_mbed_lib_json_file(
    mbed_lib_json_path: pathlib.Path, target_filters: Iterable[str]
) -> dict[str, Union[list, str, dict]]:
    """
    Prepare a config source for entry into the Config object.

    Extracts config and override settings from an mbed_lib.json file.

    Args:
        mbed_lib_json_path: Path to mbed_lib.json file
        target_filters: List of filter string used when extracting data from target_overrides section of the config
            data.

    Returns:
        Prepared config source.
    """
    context = "file " + str(mbed_lib_json_path)

    # Load JSON file using schema
    try:
        mbed_lib = schemas.MbedLibJSON.model_validate(decode_json_file(mbed_lib_json_path), strict=True)
    except pydantic.ValidationError:
        logger.exception(f"{context} did not validate against the schema for mbed_lib.json5!")
        raise

    config_source = {}

    # Process config settings
    if len(mbed_lib.config) > 0:
        settings = []
        for config_name, item in mbed_lib.config.items():
            # If the config item is about a certain component or feature
            # being present, avoid adding it to the mbed_config.cmake
            # configuration file. Instead, applications should depend on
            # the feature or component with target_link_libraries() and the
            # component's CMake file (in the Mbed OS repo) will create
            # any necessary macros or definitions.
            if config_name == "present":
                logger.warning(
                    f"Legacy 'present' entry in config file {mbed_lib_json_path}. This was for the Mbed CLI 1 "
                    f"build system only and is now ignored."
                )
                continue

            # Remove all underscores in the setting name and replace with hyphens, as this makes settings harder to get wrong
            transformed_config_namespace, transformed_config_name = check_and_transform_config_name(context, mbed_lib.name, config_name)

            logger.debug("Extracting config setting from '%s': '%s'='%s'", mbed_lib.name, config_name, item)
            if isinstance(item, schemas.ConfigEntryDetails):
                setting = ConfigSetting(
                    namespace=transformed_config_namespace,
                    name=transformed_config_name,
                    macro_name=item.macro_name,
                    help_text=item.help,
                    value=item.value,
                    accepted_values=set(item.accepted_values) if item.accepted_values is not None else None,
                    value_max=item.value_max,
                    value_min=item.value_min,
                )
            else:
                setting = ConfigSetting(
                    namespace=transformed_config_namespace,
                    name=transformed_config_name,
                    macro_name=None,
                    help_text=None,
                    value=item,
                    accepted_values=None,
                    value_max=None,
                    value_min=None,
                )
            settings.append(setting)

        # Note: I believe this needs to be before 'overrides' in the dict for the logic in
        # config.Config to work
        config_source["config"] = settings

    # Process macros
    if mbed_lib.macros is not None and len(mbed_lib.macros) > 0:
        config_source["macros"] = set(mbed_lib.macros)

    # Process overrides
    if len(mbed_lib.overrides) > 0:
        overrides = typing.cast(dict[str, ConfigSettingValue], mbed_lib.overrides)
        config_source["overrides"] = _extract_overrides(context, mbed_lib.name, overrides)

    # Process target overrides
    if len(mbed_lib.target_overrides) > 0:
        target_overrides = _extract_target_overrides(context, mbed_lib.name, mbed_lib.target_overrides, target_filters)

        config_source["overrides"] = config_source.get("overrides", []) + target_overrides

    return config_source


@dataclass
class ConfigSetting:
    """
    Representation of a config setting.

    Auto converts any list values to sets for faster operations and de-duplication of values.
    """

    namespace: str
    name: str
    value: schemas.ConfigSettingValue
    help_text: Optional[str] = None
    macro_name: Optional[str] = None
    accepted_values: set[schemas.ConfigSettingValue] | None = None
    value_max: int | float | None = None
    value_min: int | float | None = None

    def __post_init__(self) -> None:
        """Convert the value to a set if applicable."""
        self.value = _sanitise_value(self.value)
        self.check_value()

    def check_value(self) -> None:
        """Issue a warning if the value does not appear to be valid."""
        if self.accepted_values is not None:
            if self.value not in self.accepted_values:
                logger.warning(
                    f"Value set for {self.namespace}.{self.name} ({self.value}) does not appear to be valid. Valid values are {self.accepted_values!r}"
                )
        if self.value_max is not None:
            if not isinstance(self.value, float) and not isinstance(self.value, int):
                logger.warning(
                    f"Configuration setting {self.namespace}.{self.name} has a value_max attribute but is not of integer type (value = {self.value})."
                )
            elif self.value > self.value_max:
                logger.warning(
                    f"Value set for {self.namespace}.{self.name} ({self.value}) does not appear to be valid. Cannot be greater than {self.value_max}"
                )
        if self.value_min is not None:
            if not isinstance(self.value, float) and not isinstance(self.value, int):
                logger.warning(
                    f"Configuration setting {self.namespace}.{self.name} has a value_min attribute but is not of integer type (value = {self.value})."
                )
            elif self.value < self.value_min:
                logger.warning(
                    f"Value set for {self.namespace}.{self.name} ({self.value}) does not appear to be valid. Cannot be less than {self.value_min}"
                )


@dataclass
class Override:
    """
    Representation of a config override.

    Checks for _add or _remove modifiers and splits them from the name.
    """

    context: str
    namespace: str
    name: str
    value: schemas.ConfigSettingValue
    modifier: Optional[Literal["add", "remove"]] = None

    def __post_init__(self) -> None:
        """Parse modifiers and convert list values to sets."""
        if self.name.endswith("_add"):
            self.modifier = "add"
            self.name = self.name.removesuffix("_add")
        elif self.name.endswith("_remove"):
            self.modifier = "remove"
            self.name = self.name.removesuffix("_remove")

        self.value = _sanitise_value(self.value)


def _extract_config_settings(context: str, namespace: str, config_data: dict) -> List[ConfigSetting]:
    settings = []
    for name, item in config_data.items():
        logger.debug("Extracting config setting from '%s': '%s'='%s'", namespace, name, item)
        if isinstance(item, dict):
            macro_name = item.get("macro_name")
            help_text = item.get("help")
            value = item.get("value")
        else:
            macro_name = None
            help_text = None
            value = item

        transformed_namespace, transformed_name = check_and_transform_config_name(context, namespace, name)

        setting = ConfigSetting(
            namespace=transformed_namespace, name=transformed_name, macro_name=macro_name, help_text=help_text, value=value
        )
        # If the config item is about a certain component or feature
        # being present, avoid adding it to the mbed_config.cmake
        # configuration file. Instead, applications should depend on
        # the feature or component with target_link_libraries() and the
        # component's CMake file (in the Mbed OS repo) will create
        # any necessary macros or definitions.
        if setting.name == "present":
            continue

        settings.append(setting)

    return settings


def _extract_target_overrides(
    context: str,
    namespace: str,
    target_override_data: dict[str, dict[str, schemas.ConfigSettingValue]],
    allowed_target_labels: Iterable[str],
) -> List[Override]:
    valid_target_data = {}
    for target_type, override in target_override_data.items():
        if target_type == "*" or target_type in allowed_target_labels:
            valid_target_data.update(override)

    return _extract_overrides(context, namespace, valid_target_data)


def _extract_overrides(
    context: str, namespace: str, override_data: dict[str, schemas.ConfigSettingValue]
) -> List[Override]:
    overrides = []
    for name, value in override_data.items():
        split_results = name.split(".", maxsplit=1)
        if len(split_results) == 2:
            override_namespace, override_name = split_results
        else:
            override_namespace = namespace
            override_name = name

        # Make the override names canonical, to match the transforms done to the actual settings
        override_name = _to_canonical_json_setting_name(override_name)
        override_namespace = _to_canonical_json_setting_name(override_namespace)

        if override_namespace not in {namespace, "target"} and namespace != "app":
            msg = (
                "mbed_lib.json files may only override their own settings and settings from targets.json5."
                f"An override was defined by {context} that attempts to override "
                f"`{override_namespace}.{override_name}`, which is not allowed."
            )

            raise InvalidConfigOverrideError(msg)

        overrides.append(Override(context=context, namespace=override_namespace, name=override_name, value=value))

    return overrides


def _sanitise_value(val: Any) -> Any:
    """
    Convert list values to sets and return scalar values and strings unchanged.

    For whatever reason, we allowed config settings to have values of any type available in the JSON spec.
    The value type can be a list, nested list, str, int, you name it.

    When we process the config, we want to use sets instead of lists, this is for two reasons:
    * To take advantage of set operations when we deal with "cumulative" settings.
    * To prevent any duplicate settings ending up in the final config.
    """
    if isinstance(val, list):
        return set(flatten_nested(val))

    return val
