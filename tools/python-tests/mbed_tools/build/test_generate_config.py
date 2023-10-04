#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import json

import os
import pytest

from mbed_tools.project import MbedProgram
from mbed_tools.build import generate_config
from mbed_tools.build.config import CMAKE_CONFIG_FILE, MBEDIGNORE_FILE
from mbed_tools.lib.exceptions import ToolsError


TARGETS = ["K64F", "NUCLEO_F401RE"]

TARGET_DATA = {
    "bootloader_supported": True,
    "c_lib": "std",
    "components": ["FLASHIAP", "SD"],
    "config": {
        "xip-enable": {
            "help": "Enable Execute In Place (XIP) on this "
            "target. Value is only significant if the "
            "board has executable external storage such "
            "as QSPIF. If this is enabled, customize "
            "the linker file to choose what text "
            "segments are placed on external storage",
            "value": False,
        }
    },
    "core": "Cortex-M4F",
    "default_toolchain": "ARM",
    "detect_code": ["0240"],
    "device_has": ["TRNG"],
    "device_name": "MK64FN1M0xxx12",
    "extra_labels": ["FRDM", "Freescale"],
    "features": ["PSA"],
    "is_disk_virtual": True,
    "labels": ["CORTEX", "CORTEX_M"],
    "macros": ["CPU_MK64FN1M0VMD12", "FSL_RTOS_MBED", "MBED_SPLIT_HEAP", "MBED_TICKLESS"],
    "printf_lib": "minimal-printf",
    "release_versions": ["5"],
    "static_memory_defines": True,
    "supported_application_profiles": ["full", "bare-metal"],
    "supported_c_libs": {"arm": ["std", "small"], "gcc_arm": ["std", "small"], "iar": ["std"]},
    "supported_form_factors": ["ARDUINO"],
    "supported_toolchains": ["ARM", "GCC_ARM", "IAR"],
    "trustzone": False,
    "mbed_ram_start": "0",
    "mbed_ram_size": "0",
    "mbed_rom_start": "0",
    "mbed_rom_size": "0",
}


def create_mbed_lib_json(lib_json_path, name, **kwargs):
    lib_json_path.parent.mkdir(parents=True, exist_ok=True)
    lib_json_path.write_text(json.dumps({"name": name, **kwargs}))


def create_mbed_app_json(root, **kwargs):
    (root / "mbed_app.json").write_text(json.dumps(kwargs))


@pytest.fixture
def program(tmp_path):
    prog = MbedProgram.from_new(tmp_path / "test-prog")
    # Overwrite the default mbed_app.json so it doesn't mess with our test env
    prog.files.app_config_file.write_text(json.dumps({"": ""}))
    # Create program mbed-os directory and fake targets.json
    prog.mbed_os.root.mkdir(parents=True)
    prog.mbed_os.targets_json_file.parent.mkdir(exist_ok=True, parents=True)
    prog.mbed_os.targets_json_file.write_text(json.dumps({target: TARGET_DATA for target in TARGETS}))
    return prog


@pytest.fixture
def program_in_mbed_os_subdir(tmp_path):
    mbed_os_path = tmp_path / "mbed-os"
    targets_json = mbed_os_path / "targets" / "targets.json"
    program_root = mbed_os_path / "test-prog"
    build_subdir = program_root / "__build" / "k64f"
    program_root.mkdir(parents=True, exist_ok=True)
    # Create program mbed-os directory and fake targets.json
    targets_json.parent.mkdir(exist_ok=True, parents=True)
    targets_json.write_text(json.dumps({target: TARGET_DATA for target in TARGETS}))
    return MbedProgram.from_existing(program_root, build_subdir, mbed_os_path=mbed_os_path)


@pytest.fixture(
    params=[(TARGETS[0], TARGETS[0]), (TARGETS[1], TARGETS[1]), (TARGETS[0], "*")],
    ids=lambda fixture_val: f"target: {fixture_val[0]}, filter: {fixture_val[1]}",
)
def matching_target_and_filter(request):
    return request.param


def test_mbedignore_generated(program):
    target = "K64F"
    toolchain = "GCC_ARM"

    generate_config(target, toolchain, program)

    mbedignore_file = (program.files.cmake_build_dir / MBEDIGNORE_FILE)

    assert os.path.isfile(mbedignore_file)


def test_target_and_toolchain_collected(program):
    target = "K64F"
    toolchain = "GCC_ARM"

    generate_config(target, toolchain, program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert target in config_text
    assert toolchain in config_text


def test_custom_targets_data_found(program):
    target = "IMAGINARYBOARD"
    toolchain = "GCC_ARM"

    program.files.custom_targets_json.write_text(json.dumps({target: TARGET_DATA}))

    generate_config(target, toolchain, program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert target in config_text


def test_raises_error_when_attempting_to_customize_existing_target(program):
    program.files.custom_targets_json.write_text(json.dumps({TARGETS[0]: TARGET_DATA}))
    target = TARGETS[0]
    toolchain = "GCC_ARM"

    with pytest.raises(ToolsError):
        generate_config(target, toolchain, program)


def test_config_param_from_lib_processed_with_default_name_mangling(program):
    create_mbed_lib_json(
        program.mbed_os.root / "platform" / "mbed_lib.json",
        "platform",
        config={
            "stdio-convert-newlines": {
                "help": "Enable conversion to standard newlines on stdin/stdout/stderr",
                "value": True,
            }
        },
    )

    generate_config("K64F", "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert "MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES" in config_text


def test_config_param_from_lib_processed_with_user_set_name(program):
    create_mbed_lib_json(
        program.mbed_os.root / "platform" / "mbed_lib.json",
        "platform",
        config={
            "stdio-convert-newlines": {
                "help": "Enable conversion to standard newlines on stdin/stdout/stderr",
                "value": True,
                "macro_name": "ENABLE_NEWLINES",
            }
        },
    )

    generate_config("K64F", "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert "ENABLE_NEWLINES" in config_text


def test_config_param_from_app_processed_with_default_name_mangling(program):
    create_mbed_app_json(
        program.root,
        config={
            "stdio-convert-newlines": {
                "help": "Enable conversion to standard newlines on stdin/stdout/stderr",
                "value": True,
            }
        },
    )

    generate_config("K64F", "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert "MBED_CONF_APP_STDIO_CONVERT_NEWLINES" in config_text


def test_config_param_from_target_processed_with_default_name_mangling(program):
    generate_config("K64F", "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert "MBED_CONF_TARGET_XIP_ENABLE=0" in config_text


@pytest.mark.parametrize(
    "macros",
    [["NS_USE_EXTERNAL_MBED_TLS"], ["NS_USE_EXTERNAL_MBED_TLS", "MBED_ENABLE_ERROR"]],
    ids=["single", "multiple"],
)
def test_macros_from_lib_collected(macros, program):
    create_mbed_lib_json(program.mbed_os.root / "connectivity" / "mbed_lib.json", "nanostack", macros=macros)

    generate_config("K64F", "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    for macro in macros:
        assert macro in config_text


@pytest.mark.parametrize(
    "macros",
    [["NS_USE_EXTERNAL_MBED_TLS"], ["NS_USE_EXTERNAL_MBED_TLS", "MBED_ENABLE_ERROR"]],
    ids=["single", "multiple"],
)
def test_macros_from_app_collected(macros, program):
    create_mbed_app_json(program.root, macros=macros)

    generate_config("K64F", "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    for macro in macros:
        assert macro in config_text


def test_macros_from_target_collected(program):
    generate_config("K64F", "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    for macro in TARGET_DATA["macros"]:
        assert macro in config_text


def test_target_labels_collected_as_defines(program):
    generate_config("K64F", "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    for label in TARGET_DATA["labels"] + TARGET_DATA["extra_labels"]:
        assert f"TARGET_{label}" in config_text

    for feature in TARGET_DATA["features"]:
        assert f"FEATURE_{feature}=1" in config_text

    for component in TARGET_DATA["components"]:
        assert f"COMPONENT_{component}=1" in config_text

    for device in TARGET_DATA["device_has"]:
        assert f"DEVICE_{device}=1" in config_text

    for form_factor in TARGET_DATA["supported_form_factors"]:
        assert f"TARGET_FF_{form_factor}" in config_text


def test_overrides_lib_config_param_from_app(matching_target_and_filter, program):
    target, target_filter = matching_target_and_filter
    create_mbed_lib_json(
        program.mbed_os.root / "mbed_lib.json", "platform", config={"stdio-baud-rate": {"value": 9600}},
    )

    create_mbed_app_json(program.root, target_overrides={target_filter: {"platform.stdio-baud-rate": 115200}})
    generate_config(target, "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert "MBED_CONF_PLATFORM_STDIO_BAUD_RATE=115200" in config_text


def test_overrides_target_config_param_from_app(matching_target_and_filter, program):
    target, target_filter = matching_target_and_filter
    create_mbed_app_json(program.root, target_overrides={target_filter: {"target.xip-enable": True}})

    generate_config(target, "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert "MBED_CONF_TARGET_XIP_ENABLE=1" in config_text


@pytest.mark.parametrize(
    "config_param, config_param_value, expected_output",
    [
        ("target.c_lib", "super", 'MBED_C_LIB "super"'),
        ("target.printf_lib", "maximal-printf", 'MBED_PRINTF_LIB "maximal-printf"'),
        ("target.extra_labels", ["NEW_LABELS"], "TARGET_NEW_LABELS"),
        ("target.supported_form_factors", ["BEAGLEBONE"], "TARGET_FF_BEAGLEBONE"),
        ("target.components", ["WARP_DRIVE"], "COMPONENT_WARP_DRIVE"),
        ("target.macros", ["DEFINE"], "DEFINE"),
        ("target.device_has", ["NOTHING"], "DEVICE_NOTHING"),
        ("target.features", ["ELECTRICITY"], "FEATURE_ELECTRICITY"),
        ("target.mbed_rom_start", "99", "MBED_ROM_START=0x63"),
        ("target.mbed_rom_size", "1010", "MBED_ROM_SIZE=0x3f2"),
        ("target.mbed_ram_start", "99", "MBED_RAM_START=0x63"),
        ("target.mbed_ram_size", "1010", "MBED_RAM_SIZE=0x3f2"),
    ],
)
def test_overrides_target_non_config_params_from_app(
    matching_target_and_filter, config_param, config_param_value, expected_output, program
):
    target, target_filter = matching_target_and_filter
    create_mbed_app_json(program.root, target_overrides={target_filter: {config_param: config_param_value}})

    generate_config(target, "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert expected_output in config_text


def test_overrides_target_config_param_from_lib(matching_target_and_filter, program):
    target, target_filter = matching_target_and_filter
    create_mbed_lib_json(
        program.root / "platform" / "mbed_lib.json",
        "platform",
        target_overrides={target_filter: {"target.xip-enable": True}},
    )

    generate_config(target, "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert "MBED_CONF_TARGET_XIP_ENABLE=1" in config_text


def test_overrides_lib_config_param_from_same_lib(matching_target_and_filter, program):
    target, target_filter = matching_target_and_filter
    create_mbed_lib_json(
        program.mbed_os.root / "mbed_lib.json",
        "platform",
        config={"stdio-baud-rate": {"value": 9600}},
        target_overrides={target_filter: {"stdio-baud-rate": 115200}},
    )

    generate_config(target, "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert "MBED_CONF_PLATFORM_STDIO_BAUD_RATE=115200" in config_text


def test_raises_when_attempting_to_override_lib_config_param_from_other_lib(matching_target_and_filter, program):
    target, target_filter = matching_target_and_filter
    create_mbed_lib_json(
        program.mbed_os.root / "platform" / "mbed_lib.json", "platform", config={"stdio-baud-rate": {"value": 9600}},
    )
    create_mbed_lib_json(
        program.mbed_os.root / "filesystem" / "mbed_lib.json",
        "filesystem",
        target_overrides={target_filter: {"platform.stdio-baud-rate": 115200}},
    )

    with pytest.raises(ToolsError):
        generate_config(target, "GCC_ARM", program)


@pytest.mark.parametrize(
    "config_param, config_param_value, expected_output",
    [
        ("target.macros_add", ["ENABLE_BOBBINS"], TARGET_DATA["macros"] + ["ENABLE_BOBBINS"]),
        ("target.extra_labels_add", ["EXTRA_LABEL_BOBBINS"], TARGET_DATA["extra_labels"] + ["EXTRA_LABEL_BOBBINS"]),
        ("target.features_add", ["FEATURE_BOBBINS"], TARGET_DATA["features"] + ["FEATURE_BOBBINS"]),
        ("target.components_add", ["COMPONENT_BOBBINS"], TARGET_DATA["components"] + ["COMPONENT_BOBBINS"]),
    ],
)
def test_target_list_params_can_be_added_to(
    matching_target_and_filter, config_param, config_param_value, expected_output, program
):
    target, target_filter = matching_target_and_filter
    create_mbed_app_json(
        program.root, target_overrides={target_filter: {config_param: config_param_value}},
    )

    generate_config(target, "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    for expected in expected_output:
        assert expected in config_text


@pytest.mark.parametrize(
    "config_param, config_param_value, expected_output",
    [
        ("target.macros_remove", [TARGET_DATA["macros"][0]], TARGET_DATA["macros"][0]),
        ("target.extra_labels_remove", [TARGET_DATA["extra_labels"][0]], TARGET_DATA["extra_labels"][0]),
        ("target.features_remove", [TARGET_DATA["features"][0]], TARGET_DATA["features"][0]),
        ("target.components_remove", [TARGET_DATA["components"][0]], TARGET_DATA["components"][0]),
    ],
)
def test_target_list_params_can_be_removed(
    matching_target_and_filter, config_param, config_param_value, expected_output, program
):
    target, target_filter = matching_target_and_filter
    create_mbed_app_json(
        program.root, target_overrides={target_filter: {config_param: config_param_value}},
    )

    generate_config(target, "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert expected_output not in config_text


def test_warns_when_attempting_to_override_nonexistent_param(matching_target_and_filter, program, caplog):
    target, target_filter = matching_target_and_filter
    override_name = "target.some-nonexistent-config-param"
    create_mbed_app_json(program.root, target_overrides={target_filter: {override_name: 999999}})

    generate_config(target, "GCC_ARM", program)

    assert override_name in caplog.text


def test_settings_from_multiple_libs_included(matching_target_and_filter, program):
    target, target_filter = matching_target_and_filter
    create_mbed_lib_json(
        program.mbed_os.root / "mbed_lib.json", "platform", config={"stdio-baud-rate": {"value": 9600}},
    )
    create_mbed_lib_json(
        program.mbed_os.root / "storage" / "mbed_lib.json",
        "filesystem",
        config={"read_size": {"macro_name": "MBED_LFS_READ_SIZE", "value": 64}},
    )

    generate_config(target, "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert "MBED_CONF_PLATFORM_STDIO_BAUD_RATE=9600" in config_text
    assert "MBED_LFS_READ_SIZE=64" in config_text


def test_requires_config_option(program):
    create_mbed_app_json(program.root, requires=["bare-metal"])
    create_mbed_lib_json(program.mbed_os.root / "bare-metal" / "mbed_lib.json", "bare-metal", requires=["platform"])
    create_mbed_lib_json(
        program.mbed_os.root / "platform" / "mbed_lib.json", "platform", config={"stdio-baud-rate": {"value": 9600}},
    )
    create_mbed_lib_json(
        program.mbed_os.root / "storage" / "mbed_lib.json",
        "filesystem",
        config={"read_size": {"macro_name": "MBED_LFS_READ_SIZE", "value": 64}},
    )

    generate_config("K64F", "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert "MBED_CONF_PLATFORM_STDIO_BAUD_RATE=9600" in config_text
    assert "MBED_LFS_READ_SIZE=64" not in config_text


def test_config_parsed_when_mbed_os_outside_project_root(program_in_mbed_os_subdir, matching_target_and_filter):
    program = program_in_mbed_os_subdir
    target, target_filter = matching_target_and_filter
    create_mbed_lib_json(
        program.mbed_os.root / "mbed_lib.json", "platform", config={"stdio-baud-rate": {"value": 9600}},
    )
    create_mbed_lib_json(
        program.mbed_os.root / "storage" / "mbed_lib.json",
        "filesystem",
        config={"read_size": {"macro_name": "MBED_LFS_READ_SIZE", "value": 64}},
    )

    generate_config("K64F", "GCC_ARM", program)

    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert "MBED_CONF_PLATFORM_STDIO_BAUD_RATE=9600" in config_text
    assert "MBED_LFS_READ_SIZE=64" in config_text


def test_output_ext_unspecified(program):
    generate_config("K64F", "GCC_ARM", program)
    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert 'MBED_OUTPUT_EXT "hex"' not in config_text


def test_output_ext_bin(program):
    target = "IMAGINARYBOARD"
    toolchain = "GCC_ARM"

    target_data = TARGET_DATA.copy()
    target_data["OUTPUT_EXT"] = "bin"
    program.files.custom_targets_json.write_text(json.dumps({target: target_data}))

    generate_config(target, toolchain, program)
    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert 'MBED_OUTPUT_EXT "hex"' not in config_text


def test_output_ext_hex(program):
    target = "IMAGINARYBOARD"
    toolchain = "GCC_ARM"

    target_data = TARGET_DATA.copy()
    target_data["OUTPUT_EXT"] = "hex"
    program.files.custom_targets_json.write_text(json.dumps({target: target_data}))

    generate_config(target, toolchain, program)
    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert 'MBED_OUTPUT_EXT "hex"' in config_text


def test_forced_reset_timeout_unspecified(program):
    generate_config("K64F", "GCC_ARM", program)
    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert 'MBED_GREENTEA_TEST_RESET_TIMEOUT ""' in config_text


def test_forced_reset_timeout_set(program):
    target = "IMAGINARYBOARD"
    toolchain = "GCC_ARM"

    target_data = TARGET_DATA.copy()
    target_data["forced_reset_timeout"] = 20
    program.files.custom_targets_json.write_text(json.dumps({target: target_data}))

    generate_config(target, toolchain, program)
    config_text = (program.files.cmake_build_dir / CMAKE_CONFIG_FILE).read_text()

    assert 'MBED_GREENTEA_TEST_RESET_TIMEOUT "20"' in config_text
