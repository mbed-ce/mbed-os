#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Tests for `mbed_tools.targets.target_attributes`."""

from unittest import TestCase, mock

from mbed_tools.build._internal.config.schemas import TargetJSON
from mbed_tools.targets._internal.target_attributes import (
    TargetNotFoundError,
    get_target_attributes,
    _extract_full_target_definition,
    _extract_core_labels,
    _apply_config_overrides,
)


class TestExtractTargetAttributes(TestCase):
    def test_no_target_found(self):
        all_targets_data = {"Target_1": TargetJSON(), "Target_2": TargetJSON()}
        with self.assertRaises(TargetNotFoundError):
            _extract_full_target_definition(all_targets_data, "Unlisted_Target", False)

    def test_target_found(self):
        all_targets_data = {
            "Target_1": TargetJSON(macros=["some_macro"]),
            "Target_2": TargetJSON(macros=["other_macro"]),
        }
        # When not explicitly included public is assumed to be True
        self.assertEqual(
            _extract_full_target_definition(all_targets_data, "Target_1", False),
            all_targets_data["Target_1"],
        )

    def test_target_public(self):
        all_targets_data = {
            "Target_1": TargetJSON(macros=["some_macro"], public=True),
            "Target_2": TargetJSON(macros=["other_macro"]),
        }
        # The public attribute affects visibility and should still be in the result since it was manually set.
        result_target_json = _extract_full_target_definition(all_targets_data, "Target_1", False)
        self.assertEqual(result_target_json, all_targets_data["Target_1"])

    def test_target_private(self):
        all_targets_data = {
            "Target_1": TargetJSON(macros=["some_macro"], public=False),
            "Target_2": TargetJSON(macros=["other_macro"]),
        }
        with self.assertRaises(TargetNotFoundError):
            _extract_full_target_definition(all_targets_data, "Target_1", False)

        # Should be able to get it if we pass the allow non public flag
        self.assertEqual(
            _extract_full_target_definition(all_targets_data, "Target_1", True),
            all_targets_data["Target_1"],
        )


class TestExtractCoreLabels(TestCase):
    @mock.patch("mbed_tools.targets._internal.target_attributes.decode_json_file")
    def test_extract_core(self, read_json_file):
        core_labels = ["FOO", "BAR"]
        metadata = {"CORE_LABELS": {"core_name": core_labels}}
        read_json_file.return_value = metadata
        target_core = "core_name"

        result = _extract_core_labels(target_core)

        self.assertEqual(result, set(core_labels))

    def test_no_core(self):
        result = _extract_core_labels(None)
        self.assertEqual(result, set())

    @mock.patch("mbed_tools.targets._internal.target_attributes.decode_json_file")
    def test_no_labels(self, read_json_file):
        metadata = {"CORE_LABELS": {"not_the_same_core": []}}
        read_json_file.return_value = metadata

        result = _extract_core_labels("core_name")

        self.assertEqual(result, set())


class TestApplyConfigOverrides(TestCase):
    def test_applies_overrides(self):
        config = {"foo": {"help": "Do a foo", "value": 0}}
        overrides = {"foo": 9}
        expected_result = {"foo": {"help": "Do a foo", "value": 9}}

        self.assertEqual(expected_result, _apply_config_overrides(config, overrides))

    def test_applies_no_overrides(self):
        config = {"foo": {"help": "Do a foo", "value": 0}}
        overrides = {}

        self.assertEqual(config, _apply_config_overrides(config, overrides))

    def test_warns_when_attempting_to_override_undefined_config_parameter(self):
        config = {"foo": {"help": "Do a foo", "value": 0}}
        overrides = {"bar": 9}

        with self.assertLogs(level="WARNING") as logger:
            _apply_config_overrides(config, overrides)

        self.assertIn("bar=9", logger.output[0])
