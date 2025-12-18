#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Tests for parsing the attributes for targets in targets.json that override."""

from unittest import TestCase, mock

from mbed_tools.build._internal.config.schemas import TargetJSON
from mbed_tools.targets._internal.targets_json_parsers.overriding_attribute_parser import (
    get_overriding_attributes_for_target,
    get_labels_for_target,
    _targets_override_hierarchy,
    _determine_overridden_attributes,
    _remove_unwanted_attributes,
    MERGING_ATTRIBUTES,
)
from mbed_tools.targets._internal.targets_json_parsers.accumulating_attribute_parser import ALL_ACCUMULATING_ATTRIBUTES


class TestGetOverridingAttributes(TestCase):
    @mock.patch(
        "mbed_tools.targets._internal.targets_json_parsers.overriding_attribute_parser._targets_override_hierarchy"
    )
    @mock.patch(
        "mbed_tools.targets._internal.targets_json_parsers.overriding_attribute_parser._determine_overridden_attributes"
    )
    def test_correctly_calls(self, _determine_overridden_attributes, _targets_override_hierarchy):
        target_name = "Target_Name"
        all_targets_data = {target_name: {"attribute_1": ["something"]}}
        result = get_overriding_attributes_for_target(all_targets_data, target_name)

        _targets_override_hierarchy.assert_called_once_with(all_targets_data, target_name)
        _determine_overridden_attributes.assert_called_once_with(_targets_override_hierarchy.return_value)
        self.assertEqual(result, _determine_overridden_attributes.return_value)


class TestParseHierarchy(TestCase):
    def test_overwrite_hierarchy_single_inheritance(self):
        all_targets_data = {
            "D": TargetJSON(),
            "C": TargetJSON(inherits=["D"]),
            "B": TargetJSON(),
            "A": TargetJSON(inherits=["C"]),
        }
        result = _targets_override_hierarchy(all_targets_data, "A")

        self.assertEqual(
            result,
            [
                all_targets_data["A"].model_dump(exclude_unset=True),
                all_targets_data["C"].model_dump(exclude_unset=True),
                all_targets_data["D"].model_dump(exclude_unset=True),
            ],
        )

    def test_overwrite_hierarchy_multiple_inheritance(self):
        all_targets_data = {
            "F": TargetJSON(),
            "E": TargetJSON(macros=["foo"]),  # Set an attribute so that it does not compare equal to target F
            "D": TargetJSON(inherits=["F"]),
            "C": TargetJSON(inherits=["E"]),
            "B": TargetJSON(inherits=["C", "D"]),
            "A": TargetJSON(inherits=["B"]),
        }
        result = _targets_override_hierarchy(all_targets_data, "A")

        self.assertEqual(
            result,
            [
                all_targets_data["A"].model_dump(exclude_unset=True),
                all_targets_data["B"].model_dump(exclude_unset=True),
                all_targets_data["C"].model_dump(exclude_unset=True),
                all_targets_data["E"].model_dump(exclude_unset=True),
                all_targets_data["D"].model_dump(exclude_unset=True),
                all_targets_data["F"].model_dump(exclude_unset=True),
            ],
        )


class TestOverridingAttributes(TestCase):
    def test_determine_overwritten_attributes(self):
        override_order = [
            {"attribute_1": "1"},
            {"attribute_1": "I should be overridden", "attribute_2": "2"},
            {"attribute_3": "3"},
        ]
        expected_attributes = {"attribute_1": "1", "attribute_2": "2", "attribute_3": "3"}

        result = _determine_overridden_attributes(override_order)
        self.assertEqual(result, expected_attributes)

    def test_remove_accumulating_attributes(self):
        override_order = [
            {ALL_ACCUMULATING_ATTRIBUTES[0]: "1"},
            {"attribute": "Normal override attribute"},
            {ALL_ACCUMULATING_ATTRIBUTES[1]: "3"},
        ]
        expected_attributes = {"attribute": "Normal override attribute"}

        result = _determine_overridden_attributes(override_order)
        self.assertEqual(result, expected_attributes)

    def test_merging_attributes(self):
        override_order = [
            {MERGING_ATTRIBUTES[0]: {"FOO": "I should also remain"}},
            {MERGING_ATTRIBUTES[0]: {"FOO": "I should not remain"}},
            {MERGING_ATTRIBUTES[0]: {"BAR": "I should remain"}},
        ]
        expected_attributes = {MERGING_ATTRIBUTES[0]: {"BAR": "I should remain", "FOO": "I should also remain"}}

        result = _determine_overridden_attributes(override_order)
        self.assertEqual(result, expected_attributes)


class TestGetLabelsForTarget(TestCase):
    def test_linear_inheritance(self):
        all_targets_data = {
            "C": TargetJSON(),
            "B": TargetJSON(inherits=["C"]),
            "A": TargetJSON(inherits=["B"]),
        }

        target_name = "A"
        expected_result = {"A", "B", "C"}
        result = get_labels_for_target(all_targets_data, target_name)

        self.assertSetEqual(result, expected_result)

    def test_multiple_inheritance(self):
        all_targets_data = {
            "E": TargetJSON(),
            "D": TargetJSON(macros=["foo"]),  # Set an attribute so that it does not compare equal to target E
            "C": TargetJSON(inherits=["E"]),
            "B": TargetJSON(inherits=["C", "D"]),
            "A": TargetJSON(inherits=["B"]),
        }

        target_name = "A"
        expected_result = {"A", "B", "C", "D", "E"}
        result = get_labels_for_target(all_targets_data, target_name)

        self.assertSetEqual(result, expected_result)

    def test_no_inheritance(self):
        all_targets_data = {"A": TargetJSON()}
        target_name = "A"
        expected_result = {"A"}
        result = get_labels_for_target(all_targets_data, target_name)

        self.assertSetEqual(result, expected_result)


class TestRemoveUnwantedAttributes(TestCase):
    def test_removes_accumulating_public_and_inherits(self):
        target_attributes = {
            ALL_ACCUMULATING_ATTRIBUTES[0]: "1",
            "public": False,
            "inherits": "SomeOtherBoard",
            "attribute": "I should remain",
        }
        expected_result = {"attribute": "I should remain"}

        result = _remove_unwanted_attributes(target_attributes)
        self.assertEqual(result, expected_result)
