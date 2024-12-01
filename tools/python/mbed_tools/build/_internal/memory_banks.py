#
# Copyright (c) 2024 Jamie Smith
# SPDX-License-Identifier: Apache-2.0
#

from __future__ import annotations

from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, Any, Set, TypedDict, NotRequired

import copy
import logging

import humanize

from mbed_tools.lib.json_helpers import decode_json_file
from mbed_tools.project import MbedProgram

from mbed_tools.build.exceptions import MbedBuildError
from mbed_tools.build._internal.config.config import Config

logger = logging.getLogger(__name__)


if TYPE_CHECKING:
    # Type hints for memory bank config
    class MemoryBankInfo(TypedDict):
        """
        Info about one memory bank
        """
        size: int
        start: int
        default: NotRequired[bool]
        startup: NotRequired[bool]
        access: Dict[str, bool]


    class BanksByType(TypedDict):
        """
        Info about all memory banks, ROM and RAM
        """
        ROM: Dict[str, MemoryBankInfo]
        RAM: Dict[str, MemoryBankInfo]


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


BANK_TYPES = ("RAM", "ROM")


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


def _apply_configured_overrides(banks_by_type: BanksByType, bank_config: Dict[str, Dict[str, int]]) -> BanksByType:

    """
    Apply overrides from configuration to the physical memory bank information, producing the configured
    memory bank information.
    :param bank_config: memory_bank_config element from target JSON
    :param banks_by_type: Physical memory bank information
    """

    configured_memory_banks = copy.deepcopy(banks_by_type)

    for bank_name, bank_data in bank_config.items():

        if bank_name not in configured_memory_banks["RAM"] and bank_name not in configured_memory_banks["ROM"]:
            raise MbedBuildError(f"Attempt to configure memory bank {bank_name} which does not exist for this device.")
        bank_type = "RAM" if bank_name in configured_memory_banks["RAM"] else "ROM"

        if len(set(bank_data.keys()) - {"size", "start"}):
            raise MbedBuildError("Only the size and start properties of a memory bank can be "
                                 "configured in memory_bank_config")

        for property_name, property_value in bank_data.items():
            if not isinstance(property_value, int):
                raise MbedBuildError(f"Memory bank '{bank_name}': configured {property_name} must be an integer")

            configured_memory_banks[bank_type][bank_name][property_name] = property_value

    return configured_memory_banks


def _print_mem_bank_summary(banks_by_type: BanksByType, configured_banks_by_type: BanksByType) -> None:

    """
    Print a summary of the memory banks to the console
    :param banks_by_type: Physical memory bank information
    :param configured_banks_by_type: Configured memory bank information
    """

    print("Summary of available memory banks:")
    for bank_type in BANK_TYPES:
        banks = banks_by_type[bank_type]

        if len(banks) == 0:
            logger.warning("No %s banks are known to the Mbed configuration system!  This can cause problems with "
                           "features like Mbed Stats and FlashIAPBlockDevice!  To fix this, define a 'device_name'"
                           " property or specify 'memory_banks' in your target JSON.", bank_type)
            continue

        print(f"Target {bank_type} banks: -----------------------------------------------------------")

        for bank_index, (bank_name, bank_data) in enumerate(banks.items()):

            bank_size = bank_data["size"]
            bank_start = bank_data["start"]

            configured_size = configured_banks_by_type[bank_type][bank_name]["size"]
            configured_start_addr = configured_banks_by_type[bank_type][bank_name]["start"]

            # If the configured sizes are different, add info to the summary
            configured_size_str = ""
            configured_start_addr_str = ""
            if configured_size != bank_size:
                configured_size_str = f" (configured to {humanize.naturalsize(configured_size, binary=True)})"
            if configured_start_addr != bank_start:
                configured_start_addr_str = f" (configured to 0x{configured_start_addr:08x})"

            print(f"{bank_index}. {bank_name}, "
                  f"start addr 0x{bank_start:08x}{configured_start_addr_str}, "
                  f"size {humanize.naturalsize(bank_size, binary=True)}{configured_size_str}")

        print()


def _generate_macros_for_memory_banks(banks_by_type: BanksByType,
                                      configured_banks_by_type: BanksByType) -> Set[str]:

    """
    Generate a set of macros to define to pass the memory bank information into Mbed.
    :param banks_by_type: Physical memory bank information
    :param configured_banks_by_type: Configured memory bank information
    """
    all_macros: Set[str] = set()

    for bank_type in BANK_TYPES:
        banks = banks_by_type[bank_type]

        for bank_index, (bank_name, bank_data) in enumerate(banks.items()):

            bank_number_str = "" if bank_index == 0 else str(bank_index)

            configured_bank_data = configured_banks_by_type[bank_type][bank_name]

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

    return all_macros


def process_memory_banks(config: Config) -> Dict[str, BanksByType]:
    """
    Process memory bank information in the config.  Reads the 'memory_banks' and
    'memory_bank_config' sections and adds the memory_bank_macros section accordingly.

    :param config: Config structure containing merged data from every JSON file (app, lib, and targets)
    :return: Memory bank information structure that shall be written to memory_banks.json
    """

    memory_banks = config.get("memory_banks", {})

    # Check for deprecated properties
    for property_name in DEPRECATED_MEM_CONFIG_PROPERTIES:
        if property_name in config:
            logger.warning("Configuration uses old-style memory bank configuration property %s. "
                           "This is deprecated and is not processed anymore, replace it with a "
                           "'memory_bank_config' section.  See here for more: "
                           "https://github.com/mbed-ce/mbed-os/wiki/Mbed-Memory-Bank-Information", property_name)

    # Check attributes, sort into rom and ram
    banks_by_type: BanksByType = {"ROM": {}, "RAM": {}}
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
    configured_banks_by_type = _apply_configured_overrides(banks_by_type, memory_bank_config)

    # Print summary
    _print_mem_bank_summary(banks_by_type, configured_banks_by_type)

    # Generate define macros
    config["memory_bank_macros"] = _generate_macros_for_memory_banks(banks_by_type, configured_banks_by_type)

    # Write out JSON file
    return {
        "memory_banks": banks_by_type,
        "configured_memory_banks": configured_banks_by_type
    }

