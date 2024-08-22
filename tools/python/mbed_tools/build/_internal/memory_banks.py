#
# Copyright (c) 2024 Jamie Smith
# SPDX-License-Identifier: Apache-2.0
#

from typing import Dict, Any, Set
import pathlib
import copy
import json
import logging

from mbed_tools.lib.json_helpers import decode_json_file
from mbed_tools.project import MbedProgram

from mbed_tools.build.exceptions import MbedBuildError
from mbed_tools.build._internal.config.config import Config

logger = logging.getLogger(__name__)


# Deprecated memory configuration properties from old (Mbed CLI 1) configuration system
DEPRECATED_MEM_CONFIG_PROPERTIES = {
    "mbed_rom_start",
    "mbed_rom_size",
    "mbed_ram_start",
    "mbed_ram_size",
    "mbed_rom1_start",
    "mbed_rom1_size",
    "mbed_ram1_start",
    "mbed_ram1_size",
}


def incorporate_memory_bank_data_from_cmsis(target_attributes: Dict[str, Any],
                                            program: MbedProgram) -> None:
    """
    Incorporate the memory bank information from the CMSIS JSON file into
    the target attributes.

    :param target_attributes: Merged targets.json content for this target
    """

    if "device_name" not in target_attributes:
        # No CMSIS device name for this target
        return

    cmsis_mcu_descriptions = decode_json_file(program.mbed_os.cmsis_mcu_descriptions_json_file)

    if target_attributes["device_name"] not in cmsis_mcu_descriptions:
        raise MbedBuildError(
f"""Target specifies device_name {target_attributes["device_name"]} but this device is not
listed in {program.mbed_os.cmsis_mcu_descriptions_json_file}.  Perhaps you need to use
the 'python -m mbed_tools.cli.main cmsis-mcu-descr fetch-missing' command to download
the missing MCU description?""")

    mcu_description = cmsis_mcu_descriptions[target_attributes["device_name"]]
    mcu_memory_description: Dict[str, Dict[str, Any]] = mcu_description["memories"]

    # If a memory bank is not already described in targets.json, import its description from the CMSIS
    # MCU description.
    target_memory_banks_section = target_attributes.get("memory_banks", {})
    for memory_bank_name, memory_bank in mcu_memory_description.items():
        if memory_bank_name not in target_memory_banks_section:
            target_memory_banks_section[memory_bank_name] = memory_bank
    target_attributes["memory_banks"] = target_memory_banks_section


def _pretty_print_size(size: int):
    """
    Pretty-print a memory size as MiB/KiB/B
    """
    if size >= 1024*1024 and (size % (1024*1024)) == 0:
        return f"{size//(1024*1024)} MiB"
    elif size >= 1024 and (size % 1024) == 0:
        return f"{size//1024} kiB"
    else:
        return f"{size} B"


def process_memory_banks(config: Config, mem_banks_json_file: pathlib.Path) -> None:
    """
    Process memory bank information in the config.  Reads the 'memory_banks' and
    'memory_bank_config' sections and adds the memory_bank_macros section accordingly.

    :param config: Config structure containing merged data from every JSON file (app, lib, and targets)
    :param mem_banks_json_file: Memory banks JSON file is written here
    """

    memory_banks = config.get("memory_banks", {})

    # Check for deprecated properties
    for property_name in DEPRECATED_MEM_CONFIG_PROPERTIES:
        if property_name in config:
            logger.warning(f"Configuration uses old-style memory bank configuration property '{property_name}'.  "
                           f"This is deprecated and is not processed anymore, replace it with a "
                           f"'memory_bank_config' section.  See here for more: "
                           f"https://github.com/mbed-ce/mbed-os/wiki/Mbed-Memory-Bank-Information")

    # Check attributes, sort into rom and ram
    banks_by_type: Dict[str, Dict[str, Dict[str, Any]]] = {"ROM": {}, "RAM": {}}
    for bank_name, bank_data in memory_banks.items():
        if "access" not in bank_data or "start" not in bank_data or "size" not in bank_data:
            raise MbedBuildError(f"Memory bank '{bank_name}' must contain 'access', 'size', and 'start' elements")
        if not isinstance(bank_data["size"], int) or not isinstance(bank_data["start"], int):
            raise MbedBuildError(f"Memory bank '{bank_name}': start and size must be integers")

        if bank_data["access"]["read"] and bank_data["access"]["write"]:
            banks_by_type["RAM"][bank_name] = bank_data
        elif bank_data["access"]["read"] and bank_data["access"]["execute"]:
            banks_by_type["ROM"][bank_name] = bank_data

    # Create configured memory bank structure
    memory_bank_config = config.get("memory_bank_config", {})
    configured_memory_banks = copy.deepcopy(banks_by_type)

    for bank_name, bank_data in memory_bank_config.items():

        if bank_name not in configured_memory_banks["RAM"] and bank_name not in configured_memory_banks["ROM"]:
            raise MbedBuildError(f"Attempt to configure memory bank {bank_name} which does not exist for this device.""")
        bank_type = "RAM" if bank_name in configured_memory_banks["RAM"] else "ROM"

        if len(set(bank_data.keys()) - {"size", "start"}):
            raise MbedBuildError("Only the size and start properties of a memory bank can be "
                                 "configured in memory_bank_config")

        for property_name, property_value in bank_data.items():
            if not isinstance(property_value, int):
                raise MbedBuildError(f"Memory bank '{bank_name}': configured {property_name} must be an integer")

            configured_memory_banks[bank_type][bank_name][property_name] = property_value

    # Print summary
    print("Summary of available memory banks:")
    for bank_type, banks in banks_by_type.items():

        if len(banks) == 0:
            logger.warning("No %s banks are known to the Mbed configuration system!  This can cause problems with "
                           "features like Mbed Stats and FlashIAPBlockDevice!  To fix this, define a 'device_name'"
                           " property or specify 'memory_banks' in your target JSON.", bank_type)
            continue

        print(f"Target {bank_type} banks: -----------------------------------------------------------")

        bank_index = 0
        for bank_name, bank_data in banks.items():

            bank_size = bank_data["size"]
            bank_start = bank_data["start"]

            configured_size = configured_memory_banks[bank_type][bank_name]["size"]
            configured_start_addr = configured_memory_banks[bank_type][bank_name]["start"]

            # If the configured sizes are different, add info to the summary
            configured_size_str = ""
            configured_start_addr_str = ""
            if configured_size != bank_size:
                configured_size_str = f" (configured to {_pretty_print_size(configured_size)})"
            if configured_start_addr != bank_start:
                configured_start_addr_str = f" (configured to 0x{configured_start_addr:08x})"

            print(f"{bank_index}. {bank_name}, start addr 0x{bank_start:08x}{configured_start_addr_str}, size {_pretty_print_size(bank_size)}{configured_size_str}")

            bank_index += 1

        print("")

    # Define macros
    all_macros: Set[str] = set()

    for bank_type, banks in banks_by_type.items():

        bank_index = 0
        for bank_name, bank_data in banks.items():

            bank_number_str = "" if bank_index == 0 else str(bank_index)

            configured_bank_data = configured_memory_banks[bank_type][bank_name]

            # Legacy numbered definitions
            all_macros.add(f"MBED_{bank_type}{bank_number_str}_START=0x{bank_data['start']:x}")
            all_macros.add(f"MBED_{bank_type}{bank_number_str}_SIZE=0x{bank_data['size']:x}")

            # New style named definitions
            all_macros.add(f"MBED_{bank_type}_BANK_{bank_name}_START=0x{bank_data['start']:x}")
            all_macros.add(f"MBED_{bank_type}_BANK_{bank_name}_SIZE=0x{bank_data['size']:x}")

            # Same as above but for configured bank
            all_macros.add(f"MBED_CONFIGURED_{bank_type}{bank_number_str}_START=0x{configured_bank_data['start']:x}")
            all_macros.add(f"MBED_CONFIGURED_{bank_type}{bank_number_str}_SIZE=0x{configured_bank_data['size']:x}")
            all_macros.add(f"MBED_CONFIGURED_{bank_type}_BANK_{bank_name}_START=0x{configured_bank_data['start']:x}")
            all_macros.add(f"MBED_CONFIGURED_{bank_type}_BANK_{bank_name}_SIZE=0x{configured_bank_data['size']:x}")

            bank_index += 1

    # Save macros into configuration
    config["memory_bank_macros"] = all_macros

    # Write out JSON file
    memory_banks_json_content = {
        "memory_banks": banks_by_type,
        "configured_memory_banks": configured_memory_banks
    }
    mem_banks_json_file.parent.mkdir(parents=True, exist_ok=True)
    mem_banks_json_file.write_text(json.dumps(memory_banks_json_content, indent=4))
