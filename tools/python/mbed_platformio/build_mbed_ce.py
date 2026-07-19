"""
PlatformIO build file for Mbed OS Community Edition.

This script acts as an SCons buildfile which gets configuration from PlatformIO, configures Mbed (if needed),
and returns information about the configuration to the PIO build system.

Copyright (c) 2025 Jamie Smith
SPDX-License-Identifier: Apache-2.0
"""

from __future__ import annotations

import json
import pathlib
import sys
import typing
from pathlib import Path
from typing import TYPE_CHECKING, Any

from click.parser import split_arg_string
from platformio.proc import exec_command
from SCons.Script import ARGUMENTS, DefaultEnvironment

if TYPE_CHECKING:
    from SCons.Environment import Base as Environment
    from SCons.Node import FS as SconsFS

env: Environment = DefaultEnvironment()
platform = env.PioPlatform()
board = env.BoardConfig()

# Directories
FRAMEWORK_DIR = Path(platform.get_package_dir("framework-mbed-ce"))
BUILD_DIR = Path(typing.cast(str, env.subst("$BUILD_DIR")))
PROJECT_DIR = Path(typing.cast(str, env.subst("$PROJECT_DIR")))
PROJECT_SRC_DIR = Path(typing.cast(str, env.subst("$PROJECT_SRC_DIR")))
CMAKE_API_DIR = BUILD_DIR / ".cmake" / "api" / "v1"
CMAKE_API_QUERY_DIR = CMAKE_API_DIR / "query"
CMAKE_API_REPLY_DIR = CMAKE_API_DIR / "reply"

PROJECT_CMAKELISTS_TXT = FRAMEWORK_DIR / "tools" / "python" / "mbed_platformio" / "CMakeLists.txt"
PROJECT_MBED_APP_JSON5 = PROJECT_DIR / "mbed_app.json5"
PROJECT_TARGET_CONFIG_H = BUILD_DIR / "mbed-os" / "generated-headers" / "mbed-target-config.h"

CMAKE_BUILD_TYPE = "Debug" if ("debug" in env.GetBuildType()) else "Release"

NINJA_PATH = pathlib.Path(platform.get_package_dir("tool-ninja")) / "ninja"
CMAKE_PATH = pathlib.Path(platform.get_package_dir("tool-cmake")) / "bin" / "cmake"

# Add mbed-os/tools/python dir to PYTHONPATH so we can import from it.
# This script is run by SCons so it does not have access to any other Python modules by default.
sys.path.append(str(FRAMEWORK_DIR / "tools" / "python"))

from mbed_platformio.cmake_to_scons_converter import (
    build_library,
    extract_defines,
    extract_flags,
    extract_includes,
    extract_link_args,
    extract_link_libraries,
    find_included_files, build_executable,
)
from mbed_platformio.pio_variants import PIO_VARIANT_TO_MBED_TARGET


def get_mbed_target() -> str:
    board_type = typing.cast(str, env.subst("$BOARD"))
    variant = PIO_VARIANT_TO_MBED_TARGET[board_type] if board_type in PIO_VARIANT_TO_MBED_TARGET else board_type.upper()
    return board.get("build.mbed_variant", variant)


def is_proper_mbed_ce_project() -> bool:
    return all(path.is_file() for path in (PROJECT_MBED_APP_JSON5,))


def create_default_project_files() -> None:
    if not PROJECT_MBED_APP_JSON5.exists():
        _ = PROJECT_MBED_APP_JSON5.write_text(
            """
{
   "target_overrides": {
      "*": {
         "platform.stdio-baud-rate": 9600, // matches PlatformIO default
         "platform.stdio-buffered-serial": 1,

         // Uncomment to use mbed-baremetal instead of mbed-os
         // "target.application-profile": "bare-metal"
      }
   }
}
"""
        )


def is_cmake_reconfigure_required() -> bool:
    cmake_cache_file = BUILD_DIR / "CMakeCache.txt"
    cmake_config_files = [PROJECT_MBED_APP_JSON5, PROJECT_CMAKELISTS_TXT]
    ninja_buildfile = BUILD_DIR / "build.ninja"

    if not cmake_cache_file.exists():
        print("Mbed CE: Reconfigure required because CMake cache does not exist")
        return True
    if not CMAKE_API_REPLY_DIR.is_dir() or not any(CMAKE_API_REPLY_DIR.iterdir()):
        print("Mbed CE: Reconfigure required because CMake API reply dir is missing")
        return True
    if not ninja_buildfile.exists():
        print("Mbed CE: Reconfigure required because Ninja buildfile does not exist")
        return True

    # If the JSON files have 'Debug' in their names that means this project was previously configured as Debug.
    if not any(CMAKE_API_REPLY_DIR.glob(f"directory-*{CMAKE_BUILD_TYPE}*.json")):
        print("Mbed CE: Reconfigure required because build type (debug / release) changed.")
        return True

    cache_file_mtime = cmake_cache_file.stat().st_mtime
    for file in cmake_config_files:
        if file.stat().st_mtime > cache_file_mtime:
            print(f"Mbed CE: Reconfigure required because {file.name} was modified")
            return True

    return False


def run_tool(command_and_args: list[str] | None = None) -> None:
    result = exec_command(command_and_args)

    # Note: Pyright seems to think that this will always fail due to missing type annotations
    # for exec_command(), but I believe the actual code is fine.
    if result["returncode"] != 0:  # pyright: ignore[reportUnnecessaryComparison]
        print(result["out"], file=sys.stderr)
        print(result["err"], file=sys.stderr)
        env.Exit(1)

    if int(ARGUMENTS.get("PIOVERBOSE", 0)):
        print(result["out"])
        print(result["err"])


def get_cmake_code_model(cmake_args: list) -> dict:
    query_file = CMAKE_API_QUERY_DIR / "codemodel-v2"

    if not query_file.exists():
        query_file.parent.mkdir(parents=True, exist_ok=True)
        query_file.touch()

    if not is_proper_mbed_ce_project():
        create_default_project_files()

    if is_cmake_reconfigure_required():
        print("Mbed CE: Configuring CMake build system...")
        cmake_command = [str(CMAKE_PATH), *cmake_args]
        run_tool(cmake_command)

        # Seems like CMake doesn't update the timestamp on the cache file if nothing actually changed.
        # Ensure that the timestamp is updated so we won't reconfigure next time.
        (BUILD_DIR / "CMakeCache.txt").touch()

    if not CMAKE_API_REPLY_DIR.is_dir() or not any(CMAKE_API_REPLY_DIR.iterdir()):
        print("Error: Couldn't find CMake API response file", file=sys.stderr)
        env.Exit(1)

    codemodel = {}
    for target in CMAKE_API_REPLY_DIR.iterdir():
        if target.name.startswith("codemodel-v2"):
            with target.open(encoding="utf-8") as fp:
                codemodel = json.load(fp)

    if codemodel["version"]["major"] != 2:
        print("Warning: Unexpected CMake code model version, reading compilation data may fail!", file=sys.stderr)

    return codemodel


def get_target_config(project_configs: dict, target_index: int) -> dict[str, Any]:
    target_json = project_configs["targets"][target_index].get("jsonFile", "")
    target_config_file = CMAKE_API_REPLY_DIR / target_json
    if not target_config_file.is_file():
        print(f"Error: Couldn't find target config {target_json}", file=sys.stderr)
        env.Exit(1)

    with target_config_file.open(encoding="utf-8") as fp:
        return json.load(fp)


def load_target_configurations(cmake_codemodel: dict) -> dict:
    configs = {}
    project_configs = cmake_codemodel["configurations"][0]
    for config in project_configs.get("projects", []):
        for target_index in config.get("targetIndexes", []):
            target_config = get_target_config(project_configs, target_index)
            configs[target_config["name"]] = target_config

    return configs


def generate_project_ld_script() -> pathlib.Path:
    # Run Ninja to build the target which generates the linker script.
    # Note that we don't want to use CMake as running it has the side effect of redoing
    # the file API query.
    cmd = [
        str(pathlib.Path(platform.get_package_dir("tool-ninja")) / "ninja"),
        "-C",
        str(BUILD_DIR),
        "mbed-linker-script",
    ]
    run_tool(cmd)

    # Find the linker script. It gets saved in the build dir as
    # <target>.link-script.ld.
    return next(BUILD_DIR.glob("*.link_script.ld"))


def get_targets_by_type(target_configs: dict, target_types: list[str], ignore_targets: list[str] | None = None) -> list[dict[str, Any]]:
    """
    Get the CMake exported JSON for all targets of the given type
    """
    ignore_targets = ignore_targets or []
    result = []
    for target_config in target_configs.values():
        if target_config["type"] in target_types and target_config["name"] not in ignore_targets:
            result.append(target_config)

    return result


def get_targets_map(
    target_configs: dict, target_types: list[str], ignore_targets: list[str] | None = None
) -> dict[str, dict[str, Any]]:
    """
    Get map of target name to target JSON for each selected target
    """
    result = {}
    for config in get_targets_by_type(target_configs, target_types, ignore_targets):
        if "nameOnDisk" not in config:
            config["nameOnDisk"] = "lib{}.a".format(config["name"])
        result[config["name"]] = {"config": config}

    return result


def build_targets(env: Environment, targets_map: dict[str, Any], project_src_dir: pathlib.Path) -> None:
    for target_data in targets_map.values():
        if target_data["config"]["type"] == "EXECUTABLE":
            target_data["exe"] = build_executable(
                env, target_data["config"], project_src_dir, FRAMEWORK_DIR, pathlib.Path("$BUILD_DIR/mbed-os")
            )
        else:
            target_data["lib"] = build_library(
                env, target_data["config"], project_src_dir, FRAMEWORK_DIR, pathlib.Path("$BUILD_DIR/mbed-os")
            )


def get_app_defines(app_config: dict) -> list[tuple[str, str]]:
    return extract_defines(app_config["compileGroups"][0])

def rp2xxx_bootloader_custom_commands(env: Environment, framework_targets_map: dict[str, Any]) -> None:
    """
    Custom logic for generated bootloader source file for RP2xxx.

    Sadly this is needed because CMake does not export custom commands in any way, shape, or form via the API.
    So, we have to replicate the custom command logic here.
    """

    if "mbed-mcu-rp2-boot-stage-2" not in framework_targets_map:
        # Not rp2xxx
        return

    generated_src_file = None
    for source in framework_targets_map["mbed-os"]["config"]["sources"]:
        source_path = pathlib.Path(source["path"])
        if source.get("isGenerated", False) and source_path.name == "mbed-mcu-rp2-boot-stage-2_padded_checksummed.S":
            generated_src_file = "$BUILD_DIR" / source_path.relative_to(BUILD_DIR)
    if generated_src_file is None:
        raise RuntimeError("Failed to find generated bootloader source file in source list")

    # First generate bin file from elf
    boot_stage_2_elf: SconsFS.File = env.GetBuildPath(framework_targets_map["mbed-mcu-rp2-boot-stage-2"]["exe"])[0]
    boot_stage_2_bin = pathlib.Path(boot_stage_2_elf).with_suffix(".bin")
    env.Command(target=str(boot_stage_2_bin),
                source=boot_stage_2_elf,
                action=f"$OBJCOPY -O binary {boot_stage_2_elf} {boot_stage_2_bin!s}")

    # Now use the appropriate script to generate the ASM source file containing the padded and checksummed contents
    # of the source file. There are two versions of the script, one for RP2040 and one for RP2350+, so we need to figure
    # out which to use.
    use_rp2040_script = "pico-sdk/src/rp2040/boot_stage2" in framework_targets_map["mbed-mcu-rp2-boot-stage-2"]["config"]["sources"][0]["path"]
    script_path = f"{FRAMEWORK_DIR}/targets/TARGET_RASPBERRYPI/pico-sdk/src/{'rp2040' if use_rp2040_script else 'rp2350'}/boot_stage2/pad_checksum"

    env.Command(
        target=generated_src_file,
        source=str(boot_stage_2_bin),
        action=f"{sys.executable} {script_path} -s 0xffffffff {boot_stage_2_bin!s} {generated_src_file}"
    )

## CMake configuration -------------------------------------------------------------------------------------------------

project_codemodel = get_cmake_code_model(
    [
        "-S",
        PROJECT_CMAKELISTS_TXT.parent,
        "-B",
        BUILD_DIR,
        "-G",
        "Ninja",
        "-DCMAKE_MAKE_PROGRAM="
        + str(NINJA_PATH.as_posix()),  # Note: CMake prefers to be passed paths with forward slashes, so use as_posix()
        "-DCMAKE_BUILD_TYPE=" + CMAKE_BUILD_TYPE,
        "-DPLATFORMIO_MBED_OS_PATH=" + str(FRAMEWORK_DIR.as_posix()),
        "-DPLATFORMIO_PROJECT_PATH=" + str(PROJECT_DIR.as_posix()),
        "-DMBED_TARGET=" + get_mbed_target(),
        "-DUPLOAD_METHOD=NONE",  # Disable Mbed CE upload method system as PlatformIO has its own
        "-DMBED_MANAGE_SUBMODULES=FALSE",  # Don't try to use git
        # Add in any extra options from higher layers
        *split_arg_string(board.get("build.cmake_extra_args", "")),
    ]
)

if not project_codemodel:
    print("Error: Couldn't find code model generated by CMake", file=sys.stderr)
    env.Exit(1)

print("Mbed CE: Reading CMake configuration...")
target_configs = load_target_configurations(project_codemodel)

# Scan targets information from CMake. Note that we DO want to load executable targets as they can
# be used as intermediary steps during the build, but we do NOT want to load the final executable target here.
framework_targets_map = get_targets_map(target_configs, ["STATIC_LIBRARY", "OBJECT_LIBRARY", "EXECUTABLE"], ["PIODummyExecutable"])

## Convert targets & flags from CMake to SCons -------------------------------------------------------------------------

build_targets(env, framework_targets_map, PROJECT_DIR)
rp2xxx_bootloader_custom_commands(env, framework_targets_map)

mbed_os_lib_target_json = target_configs.get("mbed-os", {})
app_target_json = target_configs.get("PIODummyExecutable", {})
project_defines = get_app_defines(app_target_json)
project_flags = extract_flags(app_target_json)
app_includes = extract_includes(app_target_json)
app_link_libraries = extract_link_libraries(app_target_json)

## Linker flags --------------------------------------------------------------------------------------------------------

# Link the main Mbed OS library using -Wl,--whole-archive. This is needed for the resolution of weak symbols
# within this archive.
mbed_ce_lib_path = pathlib.Path("$BUILD_DIR") / "mbed-os" / "libmbed-os.a"
link_args = ["-Wl,--whole-archive", '"' + str(mbed_ce_lib_path) + '"', "-Wl,--no-whole-archive"]
_ = env.Depends("$BUILD_DIR/$PROGNAME$PROGSUFFIX", str(mbed_ce_lib_path))

# Now link optional libraries. Note that according to normal link order rules, we'd link the optional libraries
# first since they reference libmbed-os.a. However, mbed-os can also reference the optional libraries
# (e.g. usb for USB serial support), and all of libmbed-os.a is already being considered for linking thanks
# to --whole-archive. So we actually want to link these second.
for lib in app_link_libraries:
    link_args.append(str(lib))
    _ = env.Depends("$BUILD_DIR/$PROGNAME$PROGSUFFIX", str(lib))

# Get other linker flags from Mbed. We want these to appear after the application objects and Mbed libraries
# because they contain the C/C++ library link flags.
link_args.extend(extract_link_args(app_target_json))

# The CMake build system adds a flag in mbed_set_post_build() to output a map file.
# We need to do that here.
map_file = BUILD_DIR / "firmware.map"
link_args.append(f"-Wl,-Map={map_file!s}")

## Build environment configuration -------------------------------------------------------------------------------------

env.MergeFlags(project_flags)
env.Prepend(CPPPATH=app_includes["plain_includes"], CPPDEFINES=project_defines)
env.Append(_LIBFLAGS=link_args)

# Set up a dependency between all application source files and mbed-target-config.h.
# This ensures that the app will be recompiled if the header changes.
env.Append(PIO_EXTRA_APP_SOURCE_DEPS=find_included_files(env))

## Linker script -------------------------------------------------------------------------------------------------------

# Run Ninja to produce the linker script.
# Note that this seems to execute CMake, causing the code model query to be re-done.
# So, we have to do this after we are done using the results of said query.
print("Mbed CE: Generating linker script...")
project_ld_script = generate_project_ld_script()
_ = env.Depends("$BUILD_DIR/$PROGNAME$PROGSUFFIX", str(project_ld_script))
env.Append(LDSCRIPT_PATH=str(project_ld_script))

print("Mbed CE: Build environment configured.")
