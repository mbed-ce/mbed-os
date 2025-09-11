"""
Functions for converting build system information from the CMake File API into SCons build targets.

Copyright (c) 2025 Jamie Smith
SPDX-License-Identifier: Apache-2.0
"""

from __future__ import annotations

import collections

from SCons.Environment import Base as Environment
import pathlib
import click

def extract_defines(compile_group: dict) -> list[tuple[str, str]]:
    def _normalize_define(define_string):
        define_string = define_string.strip()
        if "=" in define_string:
            define, value = define_string.split("=", maxsplit=1)
            if any(char in value for char in (' ', '<', '>')):
                value = f'"{value}"'
            elif '"' in value and not value.startswith("\\"):
                value = value.replace('"', '\\"')
            return define, value
        return define_string

    result = [
        _normalize_define(d.get("define", ""))
        for d in compile_group.get("defines", []) if d
    ]

    for f in compile_group.get("compileCommandFragments", []):
        fragment = f.get("fragment", "").strip()
        if fragment.startswith('"'):
            fragment = fragment.strip('"')
        if fragment.startswith("-D"):
            result.append(_normalize_define(fragment[2:]))

    return result

def prepare_build_envs(target_json: dict, default_env: Environment) -> list[Environment]:
    """
    Creates the Scons Environment(s) needed to build the source files in a CMake target
    """
    build_envs = []
    target_compile_groups = target_json.get("compileGroups", [])
    if not target_compile_groups:
        print("Warning! The `%s` component doesn't register any source files. "
              "Check if sources are set in component's CMakeLists.txt!" % target_json["name"]
              )

    for cg in target_compile_groups:
        includes = []
        sys_includes = []
        for inc in cg.get("includes", []):
            inc_path = inc["path"]
            if inc.get("isSystem", False):
                sys_includes.append(inc_path)
            else:
                includes.append(inc_path)

        defines = extract_defines(cg)
        flags = extract_flags(target_json)
        build_env = default_env.Clone()
        build_env.SetOption("implicit_cache", 1)
        build_env.MergeFlags(flags)
        build_env.AppendUnique(CPPDEFINES=defines, CPPPATH=includes)
        if sys_includes:
            build_env.Append(CCFLAGS=[("-isystem", inc) for inc in sys_includes])
        build_env.ProcessUnFlags(default_env.get("BUILD_UNFLAGS"))
        build_envs.append(build_env)

    return build_envs

def compile_source_files(
        config: dict, default_env: Environment, project_src_dir: pathlib.Path, framework_dir: pathlib.Path, framework_obj_dir: pathlib.Path) -> list:
    """
    Generates SCons rules to compile the source files in a target.
    Returns list of object files to build.

    :param framework_dir: Path to the Mbed CE framework source
    :param framework_obj_dir: Path to the directory where object files for Mbed CE will be saved.
    """
    build_envs = prepare_build_envs(config, default_env)
    objects = []
    for source in config.get("sources", []):
        if source["path"].endswith(".rule"):
            continue
        compile_group_idx = source.get("compileGroupIndex")
        if compile_group_idx is not None:

            # Get absolute path to source, resolving relative to source dir if needed
            src_path = pathlib.Path(source.get("path"))
            if not src_path.is_absolute():
                src_path = project_src_dir / src_path

            # Figure out object path
            if src_path.is_relative_to(project_src_dir):
                obj_path = (pathlib.Path("$BUILD_DIR") / src_path.relative_to(project_src_dir)).with_suffix(".o")
            elif src_path.is_relative_to(framework_dir):
                obj_path = (framework_obj_dir / src_path.relative_to(framework_dir)).with_suffix(".o")
            else:
                raise RuntimeError(f"Source path {src_path!s} outside of project source dir and framework dir, don't know where to save object file!")

            env = build_envs[compile_group_idx]

            objects.append(env.StaticObject(target=str(obj_path), source=str(src_path)))

            # SCons isn't smart enough to add a dependency based on the "-include" compiler flag, so
            # manually add one.
            for included_file in find_included_files(env):
                env.Depends(str(obj_path), included_file)


    return objects

def build_library(
        default_env: Environment, lib_config: dict, project_src_dir: pathlib.Path, framework_dir: pathlib.Path, framework_obj_dir: pathlib.Path
):
    lib_name = lib_config["nameOnDisk"]
    lib_path = lib_config["paths"]["build"]
    lib_objects = compile_source_files(
        lib_config, default_env, project_src_dir, framework_dir, framework_obj_dir
    )

    #print(f"Created build rule for " + str(pathlib.Path("$BUILD_DIR") / lib_path / lib_name))

    return default_env.Library(
        target=str(pathlib.Path("$BUILD_DIR") / lib_path / lib_name), source=lib_objects
    )

def _get_flags_for_compile_group(compile_group_json: dict) -> list[str]:
    """
    Extract the flags from a CMake compile group.
    """
    flags = []
    for ccfragment in compile_group_json["compileCommandFragments"]:
        fragment = ccfragment.get("fragment", "").strip("\" ")
        if not fragment or fragment.startswith("-D"):
            continue
        flags.extend(
            click.parser.split_arg_string(fragment.strip())
        )
    return flags

def extract_flags(target_json: dict) -> dict[str, list[str]]:
    """
    Returns a dictionary with flags for SCons based on a given CMake target
    """
    default_flags = collections.defaultdict(list)
    for cg in target_json["compileGroups"]:
        default_flags[cg["language"]].extend(_get_flags_for_compile_group(cg))

    # Flags are sorted because CMake randomly populates build flags in code model
    return {
        "ASPPFLAGS": default_flags.get("ASM"),
        "CFLAGS": default_flags.get("C"),
        "CXXFLAGS": default_flags.get("CXX"),
    }

def find_included_files(environment: Environment) -> set[str]:
    """
    Process a list of flags produced by extract_flags() to find files manually included by '-include'
    """
    result = set()
    for flag_var in ["CFLAGS", "CXXFLAGS", "CCFLAGS"]:
        language_flags = environment.get(flag_var)
        for index in range(0, len(language_flags)):
            if language_flags[index] == "-include" and index < len(language_flags) - 1:
                result.add(language_flags[index + 1])
    return result

def extract_includes(target_json: dict) -> dict[str, list[str]]:
    """
    Extract the includes from a CMake target and return an SCons-style dict
    """
    plain_includes = []
    sys_includes = []
    cg = target_json["compileGroups"][0]
    for inc in cg.get("includes", []):
        inc_path = inc["path"]
        if inc.get("isSystem", False):
            sys_includes.append(inc_path)
        else:
            plain_includes.append(inc_path)

    return {"plain_includes": plain_includes, "sys_includes": sys_includes}

def extract_link_args(target_json: dict) -> list[str]:
    """
    Extract the linker flags from a CMake target
    """

    result = []

    for f in target_json.get("link", {}).get("commandFragments", []):
        fragment = f.get("fragment", "").strip()
        fragment_role = f.get("role", "").strip()
        if not fragment or not fragment_role:
            continue
        args = click.parser.split_arg_string(fragment)
        if fragment_role == "flags":
            result.extend(args)

    return result