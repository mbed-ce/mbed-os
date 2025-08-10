"""
Functions for converting build system information from the CMake File API into SCons build targets.
"""

from __future__ import annotations

from SCons.Environment import Base as Environment
import pathlib

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

def prepare_build_envs(config: dict, default_env: Environment, debug_allowed=True):
    build_envs = []
    target_compile_groups = config.get("compileGroups", [])
    if not target_compile_groups:
        print("Warning! The `%s` component doesn't register any source files. "
              "Check if sources are set in component's CMakeLists.txt!" % config["name"]
              )

    is_build_type_debug = "debug" in default_env.GetBuildType() and debug_allowed
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
        compile_commands = cg.get("compileCommandFragments", [])
        build_env = default_env.Clone()
        build_env.SetOption("implicit_cache", 1)
        for cc in compile_commands:
            build_flags = cc.get("fragment", "").strip("\" ")
            if not build_flags.startswith("-D"):
                parsed_flags = build_env.ParseFlags(build_flags)
                build_env.AppendUnique(**parsed_flags)
                if cg.get("language", "") == "ASM":
                    build_env.AppendUnique(ASPPFLAGS=parsed_flags.get("CCFLAGS", []))
        build_env.AppendUnique(CPPDEFINES=defines, CPPPATH=includes)
        if sys_includes:
            build_env.Append(CCFLAGS=[("-isystem", inc) for inc in sys_includes])
        build_env.ProcessUnFlags(default_env.get("BUILD_UNFLAGS"))
        if is_build_type_debug:
            build_env.ConfigureDebugFlags()
        build_envs.append(build_env)

    return build_envs

def compile_source_files(
        config: dict, default_env: Environment, project_src_dir: pathlib.Path, prepend_dir: pathlib.Path | None=None, debug_allowed=True
):
    build_envs = prepare_build_envs(config, default_env, debug_allowed)
    objects = []
    for source in config.get("sources", []):
        if source["path"].endswith(".rule"):
            continue
        compile_group_idx = source.get("compileGroupIndex")
        if compile_group_idx is not None:
            src_path = pathlib.Path(source.get("path"))
            if not src_path.is_absolute():
                # For cases when sources are located near CMakeLists.txt
                src_path = project_src_dir / src_path

            obj_dir = pathlib.Path("$BUILD_DIR") / (prepend_dir or "")
            if not pathlib.Path(source["path"]).is_absolute():
                obj_path = obj_dir / source["path"]
            else:
                obj_path = obj_dir / src_path.name


            objects.append(
                build_envs[compile_group_idx].StaticObject(
                    target=str(obj_path.with_suffix(".o")),
                    source=str(src_path.resolve()),
                )
            )

    return objects

def build_library(
        default_env: Environment, lib_config: dict, project_src_dir: pathlib.Path, prepend_dir: pathlib.Path | None=None, debug_allowed=True
):
    lib_name = lib_config["nameOnDisk"]
    lib_path = lib_config["paths"]["build"]
    if prepend_dir:
        lib_path = prepend_dir / lib_path
    lib_objects = compile_source_files(
        lib_config, default_env, project_src_dir, prepend_dir, debug_allowed
    )
    return default_env.Library(
        target=str(pathlib.Path("$BUILD_DIR") / lib_path / lib_name), source=lib_objects
    )
