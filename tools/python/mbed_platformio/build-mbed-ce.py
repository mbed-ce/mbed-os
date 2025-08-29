"""
PlatformIO build file for Mbed OS Community Edition.

This script acts as an SCons buildfile which gets configuration from PlatformIO, configures Mbed (if needed),
and returns information about the configuration to the PIO build system.
"""
from __future__ import annotations

import pathlib
from pathlib import Path
import json
import sys
import os

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

# Add mbed-os/tools/python dir to PYTHONPATH so we can import from it.
# This script is run by SCons so it does not have access to any other Python modules by default.
sys.path.append(str(FRAMEWORK_DIR / "tools" / "python"))

from mbed_platformio.pio_variants import PIO_VARIANT_TO_MBED_TARGET
from mbed_platformio.cmake_to_scons_converter import build_library, extract_defines, extract_flags, extract_includes, extract_link_args

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
         "platform.stdio-baud-rate": 115200,
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

    cache_file_mtime = cmake_cache_file.stat().st_mtime
    for file in cmake_config_files:
        if file.stat().st_mtime > cache_file_mtime:
            print(f"Mbed CE: Reconfigure required because {file.name} was modified")
            return True

    return False


def run_cmake(extra_args: list[str] | None = None):
    cmd = [
        str(pathlib.Path(platform.get_package_dir("tool-cmake")) / "bin" / "cmake"),
    ]

    if extra_args:
        cmd.extend(extra_args)

    result = exec_command(cmd)
    if result["returncode"] != 0:
        sys.stderr.write(result["out"] + "\n")
        sys.stderr.write(result["err"] + "\n")
        env.Exit(1)

    if int(ARGUMENTS.get("PIOVERBOSE", 0)):
        print(result["out"])
        print(result["err"])


def get_cmake_code_model(cmake_args=None) -> dict:

    query_file = CMAKE_API_QUERY_DIR / "codemodel-v2"

    if not query_file.exists():
        query_file.parent.mkdir(parents=True, exist_ok=True)
        query_file.touch()

    if not is_proper_mbed_ce_project():
        create_default_project_files()

    if is_cmake_reconfigure_required():
        print("Mbed CE: Configuring CMake build system...")
        run_cmake(cmake_args)

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

    # Run CMake to build the target which generates the linker script
    run_cmake([
        "--build",
        BUILD_DIR,
        "--target",
        "mbed-linker-script"
    ])

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
        env: Environment, components_map: dict, project_src_dir: pathlib.Path, prepend_dir: pathlib.Path | None = None, debug_allowed=True
):
    for k, v in components_map.items():
        components_map[k]["lib"] = build_library(
            env, v["config"], project_src_dir, prepend_dir, debug_allowed
        )

def get_app_defines(app_config: dict):
    return extract_defines(app_config["compileGroups"][0])

project_codemodel = get_cmake_code_model(
    [
        "-S",
        PROJECT_CMAKELISTS_TXT.parent,
        "-B",
        BUILD_DIR,
        "-G",
        "Ninja",
        "-DPLATFORMIO_MBED_OS_PATH=" + str(FRAMEWORK_DIR),
        "-DMBED_TARGET=" + get_mbed_target(),
        "-DUPLOAD_METHOD=NONE", # Disable Mbed CE upload method system as PlatformIO has its own
    ]
    + click.parser.split_arg_string(board.get("build.cmake_extra_args", "")),
)

if not project_codemodel:
    sys.stderr.write("Error: Couldn't find code model generated by CMake\n")
    env.Exit(1)

print("Generating linker script...")
project_ld_script = generate_project_ld_script()
env.Depends("$BUILD_DIR/$PROGNAME$PROGSUFFIX", str(project_ld_script))

print("Reading CMake configuration...")
target_configs = load_target_configurations(project_codemodel)

framework_components_map = get_components_map(
    target_configs,
    ["STATIC_LIBRARY", "OBJECT_LIBRARY"],
    [],
)

build_components(env, framework_components_map, PROJECT_DIR)

mbed_os_lib_target_json = target_configs.get("mbed-os", {})
app_target_json = target_configs.get("PIODummyExecutable", {})
project_defines = get_app_defines(app_target_json)
project_flags = extract_flags(app_target_json)
link_args = extract_link_args(app_target_json)
app_includes = extract_includes(app_target_json)
print(f"link_args={link_args!r}")

# The CMake build system adds a flag in mbed_set_post_build() to output a map file.
# We need to do that here.
link_args["LINKFLAGS"].append(f"-Wl,-Map={str(BUILD_DIR / 'firmware.map')}")

#
# Process main parts of the framework
#

# libs = find_lib_deps(
#     framework_components_map, elf_config, link_args, [project_target_name]
# )

# # Extra flags which need to be explicitly specified in LINKFLAGS section because SCons
# # cannot merge them correctly
# extra_flags = filter_args(
#     link_args["LINKFLAGS"],
#     [
#         "-T",
#         "-u",
#         "-Wl,--start-group",
#         "-Wl,--end-group",
#         "-Wl,--whole-archive",
#         "-Wl,--no-whole-archive",
#     ],
# )
# link_args["LINKFLAGS"] = sorted(list(set(link_args["LINKFLAGS"]) - set(extra_flags)))

#
# Main environment configuration
#

project_flags.update(link_args)
env.MergeFlags(project_flags)
env.Prepend(
    CPPPATH=app_includes["plain_includes"],
    CPPDEFINES=project_defines,
    #LINKFLAGS=extra_flags,
    #LIBS=libs
)