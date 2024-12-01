#
# Copyright (c) 2024 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#

"""
Subcommands to allow managing the list of CMSIS MCU descriptions that comes with Mbed.
The MCU description list is used both for generating docs, and for providing information to the code
about the memory banks present on a device.

MCU descriptions are kept in mbed-os/targets/cmsis_mcu_descriptions.json.  Unlike targets.json5,
this is a json file as it is updated automatically by code.  MCU descriptions are sourced initially
from the CMSIS pack index (a resource hosted by ARM), but can also be edited manually after being downloaded.
This is needed since the index is missing certain MCUs and has wrong information about a few others.
"""
from mbed_tools.lib.json_helpers import decode_json_file

import click
import cmsis_pack_manager
import humanize

import pathlib
import os
import datetime
import logging
import json
import sys
import re
import argparse
from typing import Set, Dict, Any

LOGGER = logging.getLogger(__name__)

# Calculate path to Mbed OS JSON files
THIS_SCRIPT_DIR = pathlib.Path(os.path.dirname(__file__))
PROJECT_ROOT = THIS_SCRIPT_DIR.parent.parent.parent.parent.parent
MBED_OS_DIR = THIS_SCRIPT_DIR.parent.parent.parent.parent
TARGETS_JSON5_PATH = MBED_OS_DIR / "targets" / "targets.json5"
CMSIS_MCU_DESCRIPTIONS_JSON_PATH = MBED_OS_DIR / "targets" / "cmsis_mcu_descriptions.json5"

# Top-level command
@click.group(
    name="cmsis-mcu-descr",
    help="Manage CMSIS MCU description JSON file"
)
def cmsis_mcu_descr():

    # Set up logger defaults
    LOGGER.setLevel(logging.INFO)


def open_cmsis_cache(*, must_exist: bool = True) -> cmsis_pack_manager.Cache:
    """
    Open an accessor to the CMSIS cache.  Also prints how old the cache is.
    """

    cmsis_cache = cmsis_pack_manager.Cache(False, False)

    index_file_path = pathlib.Path(cmsis_cache.index_path)
    if not index_file_path.exists() and must_exist:
        raise RuntimeError("CMSIS device descriptor cache does not exist!  Run 'python -m mbed_tools.cli.main cmsis-mcu-descr reload-cache' to populate it!")

    if index_file_path.exists():
        # Check how old the index file is
        index_file_modified_time = datetime.datetime.fromtimestamp(index_file_path.stat().st_mtime)
        index_age = humanize.naturaltime(index_file_modified_time)
        LOGGER.info("CMSIS MCU description cache was last updated: %s", index_age)

    return cmsis_cache


def find_json_files(root_dir, exclude_dirs=[], file_pattern=r".*\.(json|json5)"):
    """
    Recursively searches for files matching the specified pattern in a given directory, excluding specified directories.

    Args:
        root_dir (str): The root directory to search.
        exclude_dirs (list): A list of directory names to exclude.
        file_pattern (str): A regular expression pattern to match file names.

    Returns:
        A list of paths to found files.
    """
    json_files = []

    for root, dirs, files in os.walk(root_dir):
        # Exclude specified directories
        for exclude_dir in exclude_dirs:
            if exclude_dir in dirs:
                dirs.remove(exclude_dir)

        for file in files:
            if re.match(file_pattern, file):
                json_files.append(pathlib.Path(os.path.join(root, file)))

    return json_files


def get_mcu_names_used_by_targets_json5() -> Set[str]:
    """
    Accumulate set of all `device_name` properties used by all targets defined in targets.json5 and custom_targets.json/json5.
    """

    # Search for files starting with "custom_targets" of type .json or .json5. Also exclude some folders like build and mbed-os
    exclude_dirs = ["build", "mbed-os", ".git"]
    file_pattern = r"custom_targets\.(json|json5)"  
    custom_targets_file = find_json_files(PROJECT_ROOT, exclude_dirs, file_pattern)

    custom_targets_json_path = {}
    for file in custom_targets_file:
        if os.path.exists(file):
            custom_targets_json_path = file
            LOGGER.info(f"Custom_targets file detected - {custom_targets_json_path}")


    used_mcu_names = set()
    LOGGER.info("Scanning targets.json5 for used MCU names...")
    json_contents = decode_json_file(TARGETS_JSON5_PATH)
    if custom_targets_file:
        LOGGER.info("Scanning custom_targets.json/json5. for used MCU names...")
        json_contents.update(decode_json_file(custom_targets_json_path))

    for target_details in json_contents.values():
        if "device_name" in target_details:
            used_mcu_names.add(target_details["device_name"])
    return used_mcu_names


@cmsis_mcu_descr.command(
    short_help="Reload the cache of CMSIS MCU descriptions.  This can take several minutes."
)
def reload_cache():
    """
    Reload the cache of CMSIS MCU descriptions.  This can take several minutes.
    Note that it's possible for various MCU vendors' CMSIS pack servers to be down, and
    cmsis-pack-manager does not report any errors in this case (augh whyyyyy).

    So, if the target you are looking for does not exist after running this command, you might
    just have to try again the next day.  It's happened to me several times...
    """
    cmsis_cache = open_cmsis_cache(must_exist=False)

    LOGGER.info("Cleaning and redownloading CMSIS device descriptions, this may take some time...")
    cmsis_cache.cache_clean()
    cmsis_cache.cache_descriptors()


@cmsis_mcu_descr.command(
    name="find-unused",
    short_help="Find MCU descriptions that are not used by targets.json5."
)
def find_unused():
    """
    Remove MCU descriptions that are not used by targets.json5.
    Use this command after removing targets from Mbed to clean up old MCU definitions.
    """
    used_mcu_names = get_mcu_names_used_by_targets_json5()

    # Accumulate set of all keys in cmsis_mcu_descriptions.json
    LOGGER.info("Scanning cmsis_mcu_descriptions.json for MCUs to be pruned...")
    cmsis_mcu_descriptions_json_contents: Dict[str, Any] = decode_json_file(CMSIS_MCU_DESCRIPTIONS_JSON_PATH)
    available_mcu_names = cmsis_mcu_descriptions_json_contents.keys()

    # Figure out which MCUs can be removed
    removable_mcus = sorted(available_mcu_names - used_mcu_names)

    if len(removable_mcus) == 0:
        print("No MCU descriptions can be pruned, all are used.")
        return

    print("The following MCU descriptions are not used and should be pruned from cmsis_mcu_descriptions.json5")
    print("\n".join(removable_mcus))
    sys.exit(1)


@cmsis_mcu_descr.command(
    name="check-missing",
    short_help="Check if there are any missing MCU descriptions used by targets.json5."
)
def check_missing():
    used_mcu_names = get_mcu_names_used_by_targets_json5()

    # Accumulate set of all keys in cmsis_mcu_descriptions.json
    LOGGER.info("Scanning cmsis_mcu_descriptions.json for missing MCUs...")
    cmsis_mcu_descriptions_json_contents: Dict[str, Any] = decode_json_file(CMSIS_MCU_DESCRIPTIONS_JSON_PATH)
    available_mcu_names = cmsis_mcu_descriptions_json_contents.keys()

    # Are there any missing?
    missing_mcu_names = used_mcu_names - available_mcu_names
    if len(missing_mcu_names) == 0:
        print("No missing MCUs, no work to do.")
        return

    print("The following MCU descriptions are used by targets.json5 and need to be added to"
          " cmsis_mcu_descriptions.json5:")
    print("\n".join(missing_mcu_names))
    sys.exit(1)


@cmsis_mcu_descr.command(
    name="fetch-missing",
    short_help="Fetch any missing MCU descriptions used by targets.json5 or custom_targets.json/json5.."
)
def fetch_missing():
    """
    Scans through cmsis_mcu_descriptions.json5 for any missing MCU descriptions that are referenced by 
    targets.json5 or custom_targets.json/json5. If any are found, they are imported from the CMSIS cache.

    Note that downloaded descriptions should be checked for accuracy before they are committed.
    """

    used_mcu_names = get_mcu_names_used_by_targets_json5()

    # Accumulate set of all keys in cmsis_mcu_descriptions.json
    LOGGER.info("Scanning cmsis_mcu_descriptions.json5 file for missing MCUs...")
    cmsis_mcu_descriptions_json_contents: Dict[str, Any] = decode_json_file(CMSIS_MCU_DESCRIPTIONS_JSON_PATH)
    available_mcu_names = cmsis_mcu_descriptions_json_contents.keys()

    # Are there any missing?
    missing_mcu_names = used_mcu_names - available_mcu_names
    if len(missing_mcu_names) == 0:
        LOGGER.info("No missing MCUs, no work to do.")
        return

    # Load CMSIS cache to access new MCUs
    cmsis_cache = open_cmsis_cache()

    missing_mcus_dict = {}

    for mcu in missing_mcu_names:
        if mcu not in cmsis_cache.index:
            raise RuntimeError(f"MCU {mcu} is not present in the CMSIS MCU index ({cmsis_cache.index_path}).  Maybe "
                               f"wrong part number, or this MCU simply doesn't exist in the CMSIS index and has "
                               f"to be added manually?")
        missing_mcus_dict[mcu] = cmsis_cache.index[mcu]
        
    LOGGER.info("In case of Custom target remove 'device_name' from your custom_targets.json5 file and add\n" +
                    "just the 'memories' section as 'memory_banks' section from content below.\n" +
                    f"Otherwise add the whole following entries to {CMSIS_MCU_DESCRIPTIONS_JSON_PATH}:")
    print(json.dumps(missing_mcus_dict, indent=4, sort_keys=True))
    sys.exit(1) 
