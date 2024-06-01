"""
Subcommands to allow managing the list of CMSIS MCU descriptions that comes with Mbed.
The MCU description list is used both for generating docs, and for providing information to the code
about the memory banks present on a device.

MCU descriptions are kept in mbed-os/targets/cmsis_mcu_descriptions.json.  Unlike targets.json5,
this is a json file as it is updated automatically by code.  MCU descriptions are sourced initially
from the CMSIS pack index (a resource hosted by ARM), but can also be edited manually after being downloaded.
This is needed since the index is missing certain MCUs and has wrong information about a few others.
"""
import json

from mbed_tools.lib.json_helpers import decode_json_file

import click
import cmsis_pack_manager
import humanize

import pathlib
import os
import datetime
import logging

LOGGER = logging.getLogger(__name__)

# Calculate path to Mbed OS JSON files
THIS_SCRIPT_DIR = pathlib.Path(os.path.dirname(__file__))
MBED_OS_DIR = THIS_SCRIPT_DIR.parent.parent.parent.parent
TARGETS_JSON5_PATH = MBED_OS_DIR / "targets" / "targets.json5"
CMSIS_MCU_DESCRIPTIONS_JSON_PATH = MBED_OS_DIR / "targets" / "cmsis_mcu_descriptions.json"


# Top-level command
@click.group(
    name="cmsis-mcu-descr",
    help="Manage CMSIS MCU description JSON file"
)
def cmsis_mcu_descr():

    # Set up logger defaults
    LOGGER.setLevel(logging.INFO)


def open_cmsis_cache() -> cmsis_pack_manager.Cache:
    """
    Open an accessor to the CMSIS cache.  Also prints how old the cache is.
    """

    cmsis_cache = cmsis_pack_manager.Cache(False, False)

    # Check how old the index file is
    index_file_path = pathlib.Path(cmsis_cache.index_path)
    index_age = humanize.naturaltime(datetime.datetime.fromtimestamp(index_file_path.stat().st_mtime))
    LOGGER.info("CMSIS MCU description cache was last updated " + index_age)

    return cmsis_cache


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
    cmsis_cache = open_cmsis_cache()

    LOGGER.info("Cleaning and redownloading CMSIS device descriptions, this may take some time...")
    cmsis_cache.cache_clean()
    cmsis_cache.cache_descriptors()

@cmsis_mcu_descr.command(
    short_help="Remove MCU descriptions that are not used by targets.json5."
)
def prune():
    """
    Remove MCU descriptions that are not used by targets.json5.
    Use this command after removing targets from Mbed to clean up old MCU definitions.
    """
    # Accumulate set of all `device_name` properties used by all targets
    LOGGER.info("Scanning targets.json5 for used MCU names...")
    used_mcu_names = set()
    targets_json5_contents = decode_json_file(TARGETS_JSON5_PATH)
    for mbed_target, target_details in targets_json5_contents.items():
        if "device_name" in target_details:
            used_mcu_names.add(target_details["device_name"])

    # Accumulate set of all keys in cmsis_mcu_descriptions.json
    LOGGER.info("Scanning cmsis_mcu_descriptions.json for MCUs to be pruned...")
    cmsis_mcu_descriptions_json_contents = decode_json_file(CMSIS_MCU_DESCRIPTIONS_JSON_PATH)
    available_mcu_names = set(cmsis_mcu_descriptions_json_contents.keys())

    # Figure out which MCUs can be removed
    prunable_mcus = sorted(available_mcu_names - used_mcu_names)

    if len(prunable_mcus) == 0:
        print("No MCU descriptions can be pruned, all are used.")
        return

    # Ask the user before pruning
    print("The following MCU descriptions are not used and will be pruned from cmsis_mcu_descriptions.json")
    print("\n".join(prunable_mcus))
    response = input("Continue? (y/n) ")
    if response.lower() != "y":
        print("Canceled.")
        return

    for mcu in prunable_mcus:
        del cmsis_mcu_descriptions_json_contents[mcu]

    # Write out JSON file again
    with open(CMSIS_MCU_DESCRIPTIONS_JSON_PATH, "w") as cmsis_mcu_descriptions_file:
        json.dump(cmsis_mcu_descriptions_json_contents, cmsis_mcu_descriptions_file, indent=4)
