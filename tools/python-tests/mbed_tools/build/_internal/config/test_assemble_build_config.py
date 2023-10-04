#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import json
from pathlib import Path
from tempfile import TemporaryDirectory

from mbed_tools.build._internal.config.assemble_build_config import _assemble_config_from_sources, assemble_config
from mbed_tools.build._internal.config.config import Config
from mbed_tools.build._internal.find_files import find_files
from mbed_tools.build._internal.config.source import prepare
from mbed_tools.lib.json_helpers import decode_json_file


def create_files(directory, files):
    created_files = []
    for file in files:
        path = Path(directory, file["path"])
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps(file["json_contents"]))
        created_files.append(path)
    return created_files


class TestAssembleConfigFromSourcesAndLibFiles:
    def test_assembles_config_using_all_relevant_files(self):
        target = {
            "config": {"foo": {"value": None}},
            "macros": [],
            "labels": ["A"],
            "extra_labels": [],
            "features": ["RED"],
            "components": [],
            "c_lib": "std",
            "printf_lib": "minimal-printf",
        }
        mbed_lib_files = [
            {
                "path": Path("subdir", "FEATURE_RED", "mbed_lib.json"),
                "json_contents": {
                    "name": "red",
                    "config": {"bool": {"value": False}},
                    "target_overrides": {
                        "A": {"bool": True, "target.features_add": ["BLUE"], "target.components_add": ["LEG"]}
                    },
                    "macros": ["RED_MACRO"],
                },
            },
            {
                "path": Path("TARGET_A", "mbed_lib.json"),
                "json_contents": {
                    "name": "a",
                    "config": {"number": {"value": 123}},
                    "target_overrides": {"*": {"target.features_add": ["RED"]}},
                },
            },
            {
                "path": Path("COMPONENT_LEG", "mbed_lib.json"),
                "json_contents": {
                    "name": "leg",
                    "config": {"number-of-fingers": {"value": 5}},
                    "macros": ["LEG_MACRO"],
                },
            },
        ]
        unused_mbed_lib_file = {
            "path": Path("subdir", "FEATURE_BROWN", "mbed_lib.json"),
            "json_contents": {
                "name": "brown",
                "target_overrides": {"*": {"red.bool": "DON'T USE ME"}},
                "macros": ["DONT_USE_THIS_MACRO"],
            },
        }
        mbed_app_file = {
            "path": Path("mbed_app.json"),
            "json_contents": {"target_overrides": {"*": {"target.foo": "bar"}}},
        }

        with TemporaryDirectory() as directory:
            created_mbed_lib_files = create_files(directory, mbed_lib_files)
            created_mbed_app_file = create_files(directory, [mbed_app_file])[0]
            create_files(directory, [unused_mbed_lib_file])

            subject = _assemble_config_from_sources(
                target, find_files("mbed_lib.json", Path(directory)), created_mbed_app_file
            )

            mbed_lib_sources = [
                prepare(decode_json_file(Path(directory, file)), target_filters=["A"])
                for file in created_mbed_lib_files
            ]
            mbed_app_source = prepare(decode_json_file(Path(directory, created_mbed_app_file)), target_filters=["A"])
            expected_config = Config(prepare(target, source_name="target"))
            for source in mbed_lib_sources + [mbed_app_source]:
                expected_config.update(source)

            subject["config"].sort(key=lambda x: x.name)
            expected_config["config"].sort(key=lambda x: x.name)
            assert subject == expected_config

    def test_updates_target_labels_from_config(self):
        target = {
            "config": {"foo": None},
            "macros": {"NO"},
            "labels": {"A"},
            "extra_labels": set(),
            "features": set(),
            "components": set(),
            "c_lib": "std",
            "printf_lib": "minimal-printf",
        }
        mbed_lib_files = [
            {
                "path": Path("TARGET_A", "mbed_lib.json"),
                "json_contents": {"name": "a", "target_overrides": {"*": {"target.features_add": ["RED"]}}},
            },
            {
                "path": Path("subdir", "FEATURE_RED", "mbed_lib.json"),
                "json_contents": {
                    "name": "red",
                    "target_overrides": {
                        "A": {
                            "target.macros_remove": ["NO"],
                            "target.features_add": ["BLUE"],
                            "target.components_add": ["LEG"],
                        }
                    },
                    "macros": ["RED_MACRO"],
                },
            },
        ]
        mbed_app_file = {
            "path": Path("mbed_app.json"),
            "json_contents": {
                "target_overrides": {
                    "*": {
                        "target.foo": "bar",
                        "target.labels_add": ["PICKLE"],
                        "target.extra_labels_add": ["EXTRA_HOT"],
                        "target.macros_add": ["TICKER"],
                    }
                }
            },
        }

        with TemporaryDirectory() as directory:
            _ = create_files(directory, mbed_lib_files)
            created_mbed_app_file = create_files(directory, [mbed_app_file])[0]

            config = _assemble_config_from_sources(
                target, find_files("mbed_lib.json", Path(directory)), created_mbed_app_file
            )

            assert config["features"] == {"RED", "BLUE"}
            assert config["components"] == {"LEG"}
            assert config["extra_labels"] == {"EXTRA_HOT"}
            assert config["labels"] == {"A", "PICKLE"}
            assert config["macros"] == {"TICKER", "RED_MACRO"}

    def test_ignores_duplicate_paths_to_lib_files(self, tmp_path, monkeypatch):
        target = {
            "labels": {"A"},
        }
        mbed_lib_files = [
            {
                "path": Path("mbed-os", "TARGET_A", "mbed_lib.json"),
                "json_contents": {"name": "a", "config": {"a": {"value": 4}}},
            },
        ]
        _ = create_files(tmp_path, mbed_lib_files)
        monkeypatch.chdir(tmp_path)

        config = assemble_config(target, [tmp_path, Path("mbed-os")], None)

        assert config["config"][0].name == "a"
        assert config["config"][0].value == 4

    def test_does_not_search_symlinks_in_proj_dir_twice(self, tmp_path, monkeypatch):
        target = {
            "labels": {"A"},
        }
        mbed_lib_files = [
            {
                "path": Path("mbed-os", "TARGET_A", "mbed_lib.json"),
                "json_contents": {"name": "a", "config": {"a": {"value": 4}}},
            },
        ]
        project_dir = tmp_path / "project"
        project_dir.mkdir()

        mbed_os_dir = tmp_path / "other" / "mbed-os"
        mbed_os_dir.mkdir(parents=True)
        _ = create_files(mbed_os_dir, mbed_lib_files)

        monkeypatch.chdir(project_dir)
        mbed_symlink = Path("mbed-os")
        mbed_symlink.symlink_to(mbed_os_dir, target_is_directory=True)

        config = assemble_config(target, [project_dir, mbed_symlink], None)

        assert config["config"][0].name == "a"
        assert config["config"][0].value == 4
