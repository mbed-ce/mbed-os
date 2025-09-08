"""
PlatformIO build file for Mbed OS Community Edition.

This script acts as an SCons buildfile which gets configuration from PlatformIO, configures Mbed (if needed),
and returns information about the configuration to the PIO build system.

Copyright (c) 2025 Jamie Smith
SPDX-License-Identifier: Apache-2.0
"""
from __future__ import annotations

import pathlib
from pathlib import Path
import json
import sys

from SCons.Script import DefaultEnvironment, ARGUMENTS
from SCons.Environment import Base as Environment
from platformio.proc import exec_command
import click

env: Environment = DefaultEnvironment()
platform = env.PioPlatform()
board = env.BoardConfig()

# Directories
FRAMEWORK_DIR = Path(platform.get_package_dir("framework-mbed-ce"))
BUILD_DIR = Path(env.subst("$BUILD_DIR"))
PROJECT_DIR = Path(env.subst("$PROJECT_DIR"))
PROJECT_SRC_DIR = Path(env.subst("$PROJECT_SRC_DIR"))
CMAKE_API_DIR = BUILD_DIR / ".cmake" / "api" / "v1"
CMAKE_API_QUERY_DIR = CMAKE_API_DIR / "query"
CMAKE_API_REPLY_DIR = CMAKE_API_DIR / "reply"

PROJECT_CMAKELISTS_TXT = FRAMEWORK_DIR  / "tools" / "python" / "mbed_platformio" / "CMakeLists.txt"
PROJECT_MBED_APP_JSON5 = PROJECT_DIR / "mbed_app.json5"
PROJECT_TARGET_CONFIG_H = BUILD_DIR / "mbed-os" / "generated-headers" / "mbed-target-config.h"

CMAKE_BUILD_TYPE = "Debug" if ("debug" in env.GetBuildType()) else "Release"

NINJA_PATH = pathlib.Path(platform.get_package_dir("tool-ninja")) / "ninja"
CMAKE_PATH = pathlib.Path(platform.get_package_dir("tool-cmake")) / "bin" / "cmake"

# Add mbed-os/tools/python dir to PYTHONPATH so we can import from it.
# This script is run by SCons so it does not have access to any other Python modules by default.
sys.path.append(str(FRAMEWORK_DIR / "tools" / "python"))

from mbed_platformio.pio_variants import PIO_VARIANT_TO_MBED_TARGET
from mbed_platformio.cmake_to_scons_converter import build_library, extract_defines, extract_flags, extract_includes, extract_link_args, find_included_files

def get_mbed_target():
    board_type = env.subst("$BOARD")
    variant = (
        PIO_VARIANT_TO_MBED_TARGET[board_type]
        if board_type in PIO_VARIANT_TO_MBED_TARGET
        else board_type.upper()
    )
    return board.get("build.mbed_variant", variant)

def is_proper_mbed_ce_project():
    return all(
        path.is_file()
        for path in (
            PROJECT_MBED_APP_JSON5,
        )
    )

def create_default_project_files():
    print("Mbed CE: Creating default project files")
    if not PROJECT_MBED_APP_JSON5.exists():
        PROJECT_MBED_APP_JSON5.write_text(
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

def is_cmake_reconfigure_required():
    cmake_cache_file = BUILD_DIR / "CMakeCache.txt"
    cmake_config_files = [
        PROJECT_MBED_APP_JSON5,
        PROJECT_CMAKELISTS_TXT
    ]
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


def run_tool(command_and_args: list[str] | None = None):
    result = exec_command(command_and_args)
    if result["returncode"] != 0:
        sys.stderr.write(result["out"] + "\n")
        sys.stderr.write(result["err"] + "\n")
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
        sys.stderr.write("Error: Couldn't find CMake API response file\n")
        env.Exit(1)

    codemodel = {}
    for target in CMAKE_API_REPLY_DIR.iterdir():
        if target.name.startswith("codemodel-v2"):
            with open(target, "r") as fp:
                codemodel = json.load(fp)

    assert codemodel["version"]["major"] == 2
    return codemodel

def get_target_config(project_configs: dict, target_index):
    target_json = project_configs.get("targets")[target_index].get("jsonFile", "")
    target_config_file = CMAKE_API_REPLY_DIR / target_json
    if not target_config_file.is_file():
        sys.stderr.write("Error: Couldn't find target config %s\n" % target_json)
        env.Exit(1)

    with open(target_config_file) as fp:
        return json.load(fp)


def load_target_configurations(cmake_codemodel: dict) -> dict:
    configs = {}
    project_configs = cmake_codemodel.get("configurations")[0]
    for config in project_configs.get("projects", []):
        for target_index in config.get("targetIndexes", []):
            target_config = get_target_config(
                project_configs, target_index
            )
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
        "mbed-linker-script"
    ]
    run_tool(cmd)

    # Find the linker script. It gets saved in the build dir as
    # <target>.link-script.ld.
    return next(BUILD_DIR.glob("*.link_script.ld"))


def get_targets_by_type(target_configs: dict, target_types: list[str], ignore_targets: list[str] | None=None) -> list:
    ignore_targets = ignore_targets or []
    result = []
    for target_config in target_configs.values():
        if (
                target_config["type"] in target_types
                and target_config["name"] not in ignore_targets
        ):
            result.append(target_config)

    return result

def get_components_map(target_configs: dict, target_types: list[str], ignore_components: list[str] | None=None) -> dict:
    result = {}
    for config in get_targets_by_type(target_configs, target_types, ignore_components):
        if "nameOnDisk" not in config:
            config["nameOnDisk"] = "lib%s.a" % config["name"]
        result[config["id"]] = {"config": config}

    return result


def build_components(
        env: Environment, components_map: dict, project_src_dir: pathlib.Path
):
    for k, v in components_map.items():
        components_map[k]["lib"] = build_library(
            env, v["config"], project_src_dir, FRAMEWORK_DIR, pathlib.Path("$BUILD_DIR/mbed-os")
        )

def get_app_defines(app_config: dict):
    return extract_defines(app_config["compileGroups"][0])

## CMake configuration -------------------------------------------------------------------------------------------------

project_codemodel = get_cmake_code_model(
    [
        "-S",
        PROJECT_CMAKELISTS_TXT.parent,
        "-B",
        BUILD_DIR,
        "-G",
        "Ninja",
        "-DCMAKE_MAKE_PROGRAM=" + str(NINJA_PATH.as_posix()), # Note: CMake prefers to be passed paths with forward slashes, so use as_posix()
        "-DCMAKE_BUILD_TYPE=" + CMAKE_BUILD_TYPE,
        "-DPLATFORMIO_MBED_OS_PATH=" + str(FRAMEWORK_DIR.as_posix()),
        "-DPLATFORMIO_PROJECT_PATH=" + str(PROJECT_DIR.as_posix()),
        "-DMBED_TARGET=" + get_mbed_target(),
        "-DUPLOAD_METHOD=NONE", # Disable Mbed CE upload method system as PlatformIO has its own
    ]
    + click.parser.split_arg_string(board.get("build.cmake_extra_args", "")),
)

if not project_codemodel:
    sys.stderr.write("Error: Couldn't find code model generated by CMake\n")
    env.Exit(1)

print("Mbed CE: Reading CMake configuration...")
target_configs = load_target_configurations(project_codemodel)

framework_components_map = get_components_map(
    target_configs,
    ["STATIC_LIBRARY", "OBJECT_LIBRARY"],
    [],
)

## Convert targets & flags from CMake to SCons -------------------------------------------------------------------------

build_components(env, framework_components_map, PROJECT_DIR)

mbed_os_lib_target_json = target_configs.get("mbed-os", {})
app_target_json = target_configs.get("PIODummyExecutable", {})
project_defines = get_app_defines(app_target_json)
project_flags = extract_flags(app_target_json)
app_includes = extract_includes(app_target_json)

## Linker flags --------------------------------------------------------------------------------------------------------

# Link the main Mbed OS library using -Wl,--whole-archive. This is needed for the resolution of weak symbols
# within this archive.
link_args = ["-Wl,--whole-archive", "$BUILD_DIR\\mbed-os\\libmbed-os.a", "-Wl,--no-whole-archive"]
env.Depends("$BUILD_DIR/$PROGNAME$PROGSUFFIX", "$BUILD_DIR\\mbed-os\\libmbed-os.a")

# Get other linker flags from Mbed. We want these to appear after the application objects and Mbed libraries
# because they contain the C/C++ library link flags.
link_args.extend(extract_link_args(app_target_json))

# The CMake build system adds a flag in mbed_set_post_build() to output a map file.
# We need to do that here.
map_file = BUILD_DIR / 'firmware.map'
link_args.append(f"-Wl,-Map={str(map_file)}")

## Build environment configuration -------------------------------------------------------------------------------------

env.MergeFlags(project_flags)
env.Prepend(
    CPPPATH=app_includes["plain_includes"],
    CPPDEFINES=project_defines,
)
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
env.Depends("$BUILD_DIR/$PROGNAME$PROGSUFFIX", str(project_ld_script))
env.Append(LDSCRIPT_PATH=str(project_ld_script))

print("Mbed CE: Build environment configured.")