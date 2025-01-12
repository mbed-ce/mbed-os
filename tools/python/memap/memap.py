#!/usr/bin/env python

"""
Copyright (c) 2016-2019 ARM Limited. All rights reserved.

SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""
from __future__ import annotations

"""
 memap term glossary, for code reviewers and for developers working on this script
 --------------------------------------------------------------------------------------------

 - Module: In this script, a module refers to the code library (i.e. the .o file) where an object came from.
 - Symbol: Any entity declared in the program that has a global address.  Generally this means any global
     variables and all functions.  Note that symbol names have to be alphanumeric, so C++ implemented
     "mangling" to encode class and function names as valid symbol names.  This means that C++ symbols will look
     like "_ZN4mbed8AnalogIn6_mutexE" to the linker.  You can use the "c++filt" tool to convert those back
     into a human readable name like "mbed::AnalogIn::_mutex."
 - Section: A logical region of an elf of object file.  Each section has a specific name and occupies contiguous memory.
     It's a vague term.
 - Input section: The section in the object (.o/.obj) file that a symbol comes from.  It generally has a specific name,
     e.g. a function could be from the .text.my_function input section.
 - Output section: The section in the linked application file (.elf) that a symbol has been put into.  The output
     section *might* match the input section, but not always! A linker script can happily put stuff from
     any input section into any output section if so desired.
 - VMA (Virtual Memory Address): The address that an output section will have when the application runs.
     Note that this name is something of a misnomer as it is inherited from desktop Linux.  There is no virtual
     memory on microcontrollers!
 - LMA (Load Memory Address): The address that an output section is loaded from in flash when the program boots.
 - .bss: Output section for global variables which have zero values at boot.  This region of RAM is zeroed at boot.
 - .data: Output section for global variables with nonzero default values.  This region is copied, as a single block
    of data, from the LMA to the VMA at boot.
 - .text: Output section for code and constant data (e.g. the values of constant arrays).  This region
    is mapped directly into flash and does not need to be copied at runtime.
"""

import dataclasses
from typing import Optional, TextIO
from abc import abstractmethod, ABC
from sys import stdout, exit, argv, path
import sys
from os import sep
from os.path import (basename, dirname, join, relpath, abspath, commonprefix,
                     splitext)
import re
import csv
import json
from argparse import ArgumentParser
from copy import deepcopy
from collections import defaultdict
from prettytable import PrettyTable

# prettytable moved this constant into an enum in the Python 3.9 release.
if sys.version_info >= (3, 9):
    from prettytable import HRuleStyle
    HEADER = HRuleStyle.HEADER
else:
    from prettytable import HEADER

from jinja2 import FileSystemLoader, StrictUndefined
from jinja2.environment import Environment


# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
path.insert(0, ROOT)

from .utils import (
    argparse_filestring_type,
    argparse_lowercase_hyphen_type,
    argparse_uppercase_type
)  # noqa: E402


@dataclasses.dataclass
class MemoryBankInfo:
    name: str
    """Name of the bank, from cmsis_mcu_descriptions.json"""

    start_addr: int
    """Start address of memory bank"""

    total_size: int
    """Total size of the memory bank in bytes"""

    used_size: int = 0
    """Size used in the memory bank in bytes (sum of the sizes of all symbols)"""

    def contains_addr(self, addr: int) -> bool:
        """
        :return: True if the given address is contained inside this memory bank
        """
        return addr >= self.start_addr and addr < self.start_addr + self.total_size


class _Parser(ABC):
    """Internal interface for parsing"""
    SECTIONS = ('.text', '.data', '.bss', '.heap', '.heap_0', '.stack')
    MISC_FLASH_SECTIONS = ('.interrupts', '.flash_config')
    OTHER_SECTIONS = ('.interrupts_ram', '.init', '.ARM.extab',
                      '.ARM.exidx', '.ARM.attributes', '.eh_frame',
                      '.init_array', '.fini_array', '.jcr', '.stab',
                      '.stabstr', '.ARM.exidx', '.ARM')

    def __init__(self):
        self.modules: dict[str, dict[str, int]] = {}
        """Dict of object name to {section name, size}"""

        self.memory_banks: dict[str, list[MemoryBankInfo]] = {"RAM": [], "ROM": []}
        """Memory bank info, by type (RAM/ROM)"""

    def _add_symbol_to_memory_banks(self, symbol_name: str, symbol_start_addr: int, size: int) -> None:
        """
        Update the memory banks structure to add the space used by a symbol.
        """

        if len(self.memory_banks["RAM"]) == 0 and len(self.memory_banks["ROM"]) == 0:
            # No memory banks loaded, skip
            return

        end_addr = symbol_start_addr + size
        for banks in self.memory_banks.values():
            for bank_info in banks:
                if bank_info.contains_addr(symbol_start_addr):
                    if bank_info.contains_addr(end_addr - 1): # end_addr is the first address past the end of the symbol so we subtract 1 here
                        # Symbol fully inside this memory bank
                        bank_info.used_size += size

                        # Uncomment to show debug info about each symbol
                        # print(f"Symbol {symbol_name} uses {size} bytes in {bank_info.name}")

                        return
                    print(f"Warning: Symbol {symbol_name} is only partially contained by memory bank {bank_info.name}")
                    first_addr_after_bank = bank_info.start_addr + bank_info.total_size
                    bank_info.used_size += first_addr_after_bank - symbol_start_addr

        print(f"Warning: Symbol {symbol_name} (at address 0x{symbol_start_addr:x}, size {size}) is not inside a "
              f"defined memory bank for this target.")

    def add_symbol(self, symbol_name: str, object_name: str, start_addr: int, size: int, section: str, vma_lma_offset: int) -> None:
        """ Adds information about a symbol (e.g. a function or global variable) to the data structures.

        Positional arguments:
        symbol_name - Descriptive name of the symbol, e.g. ".text.some_function" or "*fill*"
        object_name - name of the object file containing the symbol
        start addr - start address of symbol
        size - the size of the symbol being added
        section - Name of the output section, e.g. ".text".  Can also be "unknown".
        vma_lma_offset - Offset from where the output section exists in memory to where it's loaded from.  If nonzero,
           the initializer for this section will be considered too
        """
        if not object_name or not size:
            return

        # Don't count the heap output section for memory bank size tracking, because the linker scripts (almost always?)
        # configure that section to expand to fill the remaining amount of space
        if section not in {".heap", ".heap_0"}:
            self._add_symbol_to_memory_banks(symbol_name, start_addr, size)
            if vma_lma_offset != 0:
                self._add_symbol_to_memory_banks(f"<initializer for {symbol_name}>", start_addr + vma_lma_offset, size)

        if object_name in self.modules:
            self.modules[object_name].setdefault(section, 0)
            self.modules[object_name][section] += size
            return

        obj_split = sep + basename(object_name)
        for module_path, contents in self.modules.items():
            if module_path.endswith(obj_split) or module_path == object_name:
                contents.setdefault(section, 0)
                contents[section] += size
                return

        new_symbol = defaultdict(int)
        new_symbol[section] = size
        self.modules[object_name] = new_symbol

    def load_memory_banks_info(self, memory_banks_json_file: TextIO) -> None:
        """
        Load the memory bank information from a memory_banks.json file
        """
        memory_banks_json = json.load(memory_banks_json_file)
        for bank_type, banks in memory_banks_json["configured_memory_banks"].items():
            for bank_name, bank_data in banks.items():
                self.memory_banks[bank_type].append(MemoryBankInfo(
                    name=bank_name,
                    start_addr=bank_data["start"],
                    total_size=bank_data["size"]
                ))

    @abstractmethod
    def parse_mapfile(self, file_desc: TextIO) -> dict[str, dict[str, int]]:
        """Parse a given file object pointing to a map file

        Positional arguments:
        mapfile - an open file object that reads a map file

        return value - a dict mapping from object names to section dicts,
                       where a section dict maps from sections to sizes
        """
        raise NotImplemented


class _GccParser(_Parser):
    RE_OBJECT_FILE = re.compile(r'^(.+\/.+\.o(bj)?)$')
    RE_LIBRARY_OBJECT = re.compile(
        r'^.*' + r''.format(sep) + r'lib((.+\.a)\((.+\.o(bj)?)\))$'
    )
    RE_STD_SECTION = re.compile(r'^\s+.*0x(\w{8,16})\s+0x(\w+)\s(.+)$')
    RE_FILL_SECTION = re.compile(r'^\s*\*fill\*\s+0x(\w{8,16})\s+0x(\w+).*$')
    RE_TRANS_FILE = re.compile(r'^(.+\/|.+\.ltrans.o(bj)?)$')
    OBJECT_EXTENSIONS = (".o", ".obj")

    # Parses a line beginning a new output section in the map file that has a load address
    # Groups:
    # 1 = section name, including dot
    # 2 = in-memory address, hex, no 0x
    # 3 = section size
    # 4 = load address, i.e. where is the data for this section stored in flash
    RE_OUTPUT_SECTION_WITH_LOAD_ADDRESS = re.compile(r'^(.\w+) +0x([0-9a-f]+) +0x([0-9a-f]+) +load address +0x([0-9a-f]+)')

    # Parses a line beginning a new output section in the map file does not have a load address
    # Groups:
    # 1 = section name, including dot
    # 2 = in-memory address, hex, no 0x
    # 3 = section size
    # 4 = load address, i.e. where is the data for this section stored in flash
    RE_OUTPUT_SECTION_NO_LOAD_ADDRESS = re.compile(r'^(.\w+) +0x([0-9a-f]+) +0x([0-9a-f]+)')

    # Gets the input section name from the line, if it exists.
    # Input section names are always indented 1 space.
    # Note: This allows up to 3 dots... hopefully that's enough...
    # It can also capture "*fill*" instead of something that looks like a section name.
    RE_INPUT_SECTION_NAME = re.compile(r'^ ((?:\.\w+\.?\w*\.?\w*)|(?:\*fill\*))')

    ALL_SECTIONS = (
        _Parser.SECTIONS
        + _Parser.OTHER_SECTIONS
        + _Parser.MISC_FLASH_SECTIONS
        + ('unknown', )
    )

    def check_new_output_section(self, line: str) -> tuple[str, int] | None:
        """ Check whether a new output section in a map file has been detected

        Positional arguments:
        line - the line to check for a new section

        return value - Tuple of (name, vma to lma offset), if a new section was found, None
                       otherwise
        The vma to lma offset is the offset to be added to a memory address to get the
        address where it's loaded from.  If this is zero, the section is not loaded from flash to RAM at startup.
        """

        match = re.match(self.RE_OUTPUT_SECTION_WITH_LOAD_ADDRESS, line)
        if match:
            section_name = match.group(1)
            memory_address = int(match.group(2), 16)
            load_address = int(match.group(4), 16)
            load_addr_offset = load_address - memory_address
        else:
            match = re.match(self.RE_OUTPUT_SECTION_NO_LOAD_ADDRESS, line)
            if not match:
                return None
            section_name = match.group(1)
            load_addr_offset = 0

        # Ensure that this is a known section name, remove if not
        if section_name not in self.ALL_SECTIONS:
            section_name = "unknown"

        # Strangely, GCC still generates load address info for sections that are not loaded, such as .bss.
        # For now, suppress this for all sections other than .data.
        if section_name != ".data":
            load_addr_offset = 0

        return section_name, load_addr_offset

    def check_input_section(self, line) -> Optional[str]:
        """ Check whether a new input section in a map file has been detected.

        Positional arguments:
        line - the line to check for a new section

        return value - Input section name if found, None otherwise
        """
        match = re.match(self.RE_INPUT_SECTION_NAME, line)
        if not match:
            return None

        return match.group(1)

    def parse_object_name(self, line: str) -> str:
        """ Parse a path to object file

        Positional arguments:
        line - the path to parse the object and module name from

        return value - an object file name
        """
        if re.match(self.RE_TRANS_FILE, line):
            return '[misc]'

        test_re_mbed_os_name = re.match(self.RE_OBJECT_FILE, line)

        if test_re_mbed_os_name:
            object_name = test_re_mbed_os_name.group(1)

            # corner case: certain objects are provided by the GCC toolchain
            if 'arm-none-eabi' in line:
                return join('[lib]', 'misc', basename(object_name))
            return object_name

        else:
            test_re_obj_name = re.match(self.RE_LIBRARY_OBJECT, line)

            if test_re_obj_name:
                return join('[lib]', test_re_obj_name.group(2),
                            test_re_obj_name.group(3))
            else:
                if (
                    not line.startswith("LONG") and
                    not line.startswith("linker stubs")
                ):
                    print("Unknown object name found in GCC map file: %s"
                          % line)
                return '[misc]'

    def parse_section(self, line: str) -> tuple[str, int, int]:
        """ Parse data from a section of gcc map file describing one symbol in the code.

        examples:
                        0x00004308       0x7c ./BUILD/K64F/GCC_ARM/spi_api.o
         .text          0x00000608      0x198 ./BUILD/K64F/HAL_CM4.o

        Positional arguments:
        line - the line to parse a section from

        Returns tuple of (name, start addr, size)
        """
        is_fill = re.match(self.RE_FILL_SECTION, line)
        if is_fill:
            o_name = '[fill]'
            o_start_addr = int(is_fill.group(1), 16)
            o_size = int(is_fill.group(2), 16)
            return o_name, o_start_addr, o_size

        is_section = re.match(self.RE_STD_SECTION, line)
        if is_section:
            o_start_addr = int(is_section.group(1), 16)
            o_size = int(is_section.group(2), 16)
            if o_size:
                o_name = self.parse_object_name(is_section.group(3))
                return o_name, o_start_addr, o_size

        return "", 0, 0

    def parse_mapfile(self, file_desc: TextIO) -> dict[str, dict[str, int]]:
        """ Main logic to decode gcc map files

        Positional arguments:
        file_desc - a stream object to parse as a gcc map file
        """

        # GCC can put the section/symbol info on its own line or on the same line as the size and address.
        # So since this is a line oriented parser, we have to remember the most recently seen input & output
        # section name for later.
        current_output_section = 'unknown'
        current_output_section_addr_offset = 0
        current_input_section = 'unknown'

        with file_desc as infile:
            for line in infile:
                if line.startswith('Linker script and memory map'):
                    break

            for line in infile:
                if line.startswith("OUTPUT("):
                    # Done with memory map part of the map file
                    break

                next_section = self.check_new_output_section(line)
                if next_section is not None:
                    current_output_section, current_output_section_addr_offset = next_section

                next_input_section = self.check_input_section(line)
                if next_input_section is not None:
                    current_input_section = next_input_section

                symbol_name, symbol_start_addr, symbol_size = self.parse_section(line)

                # With GCC at least, the closest we can get to a descriptive symbol name is the input section
                # name.  Thanks to the -ffunction-sections and -fdata-sections options, the section names should
                # be unique for each symbol.
                self.add_symbol(current_input_section, symbol_name, symbol_start_addr, symbol_size, current_output_section, current_output_section_addr_offset)

        common_prefix = dirname(commonprefix([
            o for o in self.modules.keys()
            if (
                    o.endswith(self.OBJECT_EXTENSIONS)
                    and not o.startswith("[lib]")
            )]))
        new_modules = {}
        for name, stats in self.modules.items():
            if name.startswith("[lib]"):
                new_modules[name] = stats
            elif name.endswith(self.OBJECT_EXTENSIONS):
                new_modules[relpath(name, common_prefix)] = stats
            else:
                new_modules[name] = stats
        return new_modules


class MemapParser(object):
    """An object that represents parsed results, parses the memory map files,
    and writes out different file types of memory results
    """

    print_sections = ('.text', '.data', '.bss')
    delta_sections = ('.text-delta', '.data-delta', '.bss-delta')

    # sections to print info (generic for all toolchains)
    sections = _Parser.SECTIONS
    misc_flash_sections = _Parser.MISC_FLASH_SECTIONS
    other_sections = _Parser.OTHER_SECTIONS

    def __init__(self):
        # list of all modules and their sections
        # full list - doesn't change with depth
        self.modules = dict()
        self.old_modules = None
        # short version with specific depth
        self.short_modules = dict()

        # Memory report (sections + summary)
        self.mem_report = []

        # Memory summary
        self.mem_summary = dict()

        # Totals of ".text", ".data" and ".bss"
        self.subtotal = dict()

        # Flash no associated with a module
        self.misc_flash_mem = 0

        # Name of the toolchain, for better headings
        self.tc_name = None

    def reduce_depth(self, depth):
        """
        populates the short_modules attribute with a truncated module list

        (1) depth = 1:
        main.o
        mbed-os

        (2) depth = 2:
        main.o
        mbed-os/test.o
        mbed-os/drivers

        """
        if depth == 0 or depth is None:
            self.short_modules = deepcopy(self.modules)
        else:
            self.short_modules = dict()
            for module_name, v in self.modules.items():
                split_name = module_name.split(sep)
                if split_name[0] == '':
                    split_name = split_name[1:]
                new_name = join(*split_name[:depth])
                self.short_modules.setdefault(new_name, defaultdict(int))
                for section_idx, value in v.items():
                    self.short_modules[new_name][section_idx] += value
                    delta_name = section_idx + '-delta'
                    self.short_modules[new_name][delta_name] += value
            if self.old_modules:
                for module_name, v in self.old_modules.items():
                    split_name = module_name.split(sep)
                    if split_name[0] == '':
                        split_name = split_name[1:]
                    new_name = join(*split_name[:depth])
                    self.short_modules.setdefault(new_name, defaultdict(int))
                    for section_idx, value in v.items():
                        delta_name = section_idx + '-delta'
                        self.short_modules[new_name][delta_name] -= value

    export_formats = ["json", "csv-ci", "html", "table"]

    def generate_output(self, export_format, depth, file_output=None):
        """ Generates summary of memory map data

        Positional arguments:
        export_format - the format to dump

        Keyword arguments:
        file_desc - descriptor (either stdout or file)
        depth - directory depth on report

        Returns: generated string for the 'table' format, otherwise None
        """
        if depth is None or depth > 0:
            self.reduce_depth(depth)
        self.compute_report()
        try:
            if file_output:
                file_desc = open(file_output, 'w')
            else:
                file_desc = stdout
        except IOError as error:
            print("I/O error({0}): {1}".format(error.errno, error.strerror))
            return False

        to_call = {'json': self.generate_json,
                   'html': self.generate_html,
                   'csv-ci': self.generate_csv,
                   'table': self.generate_table}[export_format]
        output = to_call(file_desc)

        if file_desc is not stdout:
            file_desc.close()

        return output

    @staticmethod
    def _move_up_tree(tree, next_module):
        tree.setdefault("children", [])
        for child in tree["children"]:
            if child["name"] == next_module:
                return child
        else:
            new_module = {"name": next_module, "value": 0, "delta": 0}
            tree["children"].append(new_module)
            return new_module

    def generate_html(self, file_desc):
        """Generate a json file from a memory map for D3

        Positional arguments:
        file_desc - the file to write out the final report to
        """
        tree_text = {"name": ".text", "value": 0, "delta": 0}
        tree_bss = {"name": ".bss", "value": 0, "delta": 0}
        tree_data = {"name": ".data", "value": 0, "delta": 0}
        for name, dct in self.modules.items():
            cur_text = tree_text
            cur_bss = tree_bss
            cur_data = tree_data
            modules = name.split(sep)
            while True:
                try:
                    cur_text["value"] += dct['.text']
                    cur_text["delta"] += dct['.text']
                except KeyError:
                    pass
                try:
                    cur_bss["value"] += dct['.bss']
                    cur_bss["delta"] += dct['.bss']
                except KeyError:
                    pass
                try:
                    cur_data["value"] += dct['.data']
                    cur_data["delta"] += dct['.data']
                except KeyError:
                    pass
                if not modules:
                    break
                next_module = modules.pop(0)
                cur_text = self._move_up_tree(cur_text, next_module)
                cur_data = self._move_up_tree(cur_data, next_module)
                cur_bss = self._move_up_tree(cur_bss, next_module)
        if self.old_modules:
            for name, dct in self.old_modules.items():
                cur_text = tree_text
                cur_bss = tree_bss
                cur_data = tree_data
                modules = name.split(sep)
                while True:
                    try:
                        cur_text["delta"] -= dct['.text']
                    except KeyError:
                        pass
                    try:
                        cur_bss["delta"] -= dct['.bss']
                    except KeyError:
                        pass
                    try:
                        cur_data["delta"] -= dct['.data']
                    except KeyError:
                        pass
                    if not modules:
                        break
                    next_module = modules.pop(0)
                    if not any(
                        cld['name'] == next_module
                        for cld in cur_text['children']
                    ):
                        break
                    cur_text = self._move_up_tree(cur_text, next_module)
                    cur_data = self._move_up_tree(cur_data, next_module)
                    cur_bss = self._move_up_tree(cur_bss, next_module)

        tree_rom = {
            "name": "ROM",
            "value": tree_text["value"] + tree_data["value"],
            "delta": tree_text["delta"] + tree_data["delta"],
            "children": [tree_text, tree_data]
        }
        tree_ram = {
            "name": "RAM",
            "value": tree_bss["value"] + tree_data["value"],
            "delta": tree_bss["delta"] + tree_data["delta"],
            "children": [tree_bss, tree_data]
        }

        jinja_loader = FileSystemLoader(dirname(abspath(__file__)))
        jinja_environment = Environment(loader=jinja_loader,
                                        undefined=StrictUndefined)

        template = jinja_environment.get_template("memap_flamegraph.html")
        name, _ = splitext(basename(file_desc.name))
        if name.endswith("_map"):
            name = name[:-4]
        if self.tc_name:
            name = "%s %s" % (name, self.tc_name)
        data = {
            "name": name,
            "rom": json.dumps(tree_rom),
            "ram": json.dumps(tree_ram),
        }
        file_desc.write(template.render(data))
        return None

    def generate_json(self, file_desc):
        """Generate a json file from a memory map

        Positional arguments:
        file_desc - the file to write out the final report to
        """
        file_desc.write(json.dumps(self.mem_report, indent=4))
        file_desc.write('\n')
        return None

    RAM_FORMAT_STR = (
        "Total Static RAM memory (data + bss): {}({:+}) bytes\n"
    )

    ROM_FORMAT_STR = (
        "Total Flash memory (text + data): {}({:+}) bytes\n"
    )

    def generate_csv(self, file_desc: TextIO) -> None:
        """Generate a CSV file from a memoy map

        Positional arguments:
        file_desc - the file to write out the final report to
        """
        writer = csv.writer(file_desc, delimiter=',',
                            quoting=csv.QUOTE_MINIMAL)

        module_section = []
        sizes = []
        for i in sorted(self.short_modules):
            for k in self.print_sections + self.delta_sections:
                module_section.append((i + k))
                sizes += [self.short_modules[i][k]]

        module_section.append('static_ram')
        sizes.append(self.mem_summary['static_ram'])

        module_section.append('total_flash')
        sizes.append(self.mem_summary['total_flash'])

        writer.writerow(module_section)
        writer.writerow(sizes)
        return None

    def generate_table(self, file_desc):
        """Generate a table from a memoy map

        Returns: string of the generated table
        """
        # Create table
        columns = ['Module']
        columns.extend(self.print_sections)

        table = PrettyTable(columns, junction_char="|", hrules=HEADER)
        table.align["Module"] = "l"
        for col in self.print_sections:
            table.align[col] = 'r'

        for i in list(self.print_sections):
            table.align[i] = 'r'

        for i in sorted(self.short_modules):
            row = [i]

            for k in self.print_sections:
                row.append("{}({:+})".format(
                    self.short_modules[i][k],
                    self.short_modules[i][k + "-delta"]
                ))

            table.add_row(row)

        subtotal_row = ['Subtotals']
        for k in self.print_sections:
            subtotal_row.append("{}({:+})".format(
                self.subtotal[k], self.subtotal[k + '-delta']))

        table.add_row(subtotal_row)

        output = table.get_string()
        output += '\n'

        output += self.RAM_FORMAT_STR.format(
            self.mem_summary['static_ram'],
            self.mem_summary['static_ram_delta']
        )
        output += self.ROM_FORMAT_STR.format(
            self.mem_summary['total_flash'],
            self.mem_summary['total_flash_delta']
        )

        output += '\n'
        for bank_type, banks in self.memory_banks.items():
            for bank_info in banks:
                this_bank_deltas = self.memory_bank_summary[bank_type][bank_info.name]
                output += (f"{bank_type} Bank {bank_info.name}: {bank_info.used_size}({this_bank_deltas['delta_bytes_used']:+})/"
                           f"{bank_info.total_size} bytes used, "
                           f"{this_bank_deltas['percent_used']:.01f}% ({this_bank_deltas['delta_percent_used']:+.01f}%) used\n")

        return output

    toolchains = ["ARM", "ARM_STD", "ARM_MICRO", "GCC_ARM", "IAR"]

    def compute_report(self):
        """
        Generates summary of memory usage for main areas.  Result is put into the 'self.mem_report'
        dict, which is processed by tests and also dumped as JSON for the JSON output format.
        """
        self.subtotal = defaultdict(int)

        for mod in self.modules.values():
            for k in self.sections:
                self.subtotal[k] += mod[k]
                self.subtotal[k + '-delta'] += mod[k]
        if self.old_modules:
            for mod in self.old_modules.values():
                for k in self.sections:
                    self.subtotal[k + '-delta'] -= mod[k]

        self.mem_summary = {
            'static_ram': self.subtotal['.data'] + self.subtotal['.bss'],
            'static_ram_delta':
            self.subtotal['.data-delta'] + self.subtotal['.bss-delta'],
            'total_flash': (self.subtotal['.text'] + self.subtotal['.data']),
            'total_flash_delta':
            self.subtotal['.text-delta'] + self.subtotal['.data-delta'],
        }

        self.mem_report = {}
        modules = []
        if self.short_modules:
            for name, sizes in sorted(self.short_modules.items()):
                modules.append({
                    "module": name,
                    "size": {
                        k: sizes.get(k, 0) for k in (self.print_sections +
                                                     self.delta_sections)
                    }
                })
        self.mem_report["modules"] = modules

        self.mem_report["summary"] = self.mem_summary

        # Calculate the delta sizes for each memory bank in a couple different formats
        self.memory_bank_summary: dict[str, dict[str, dict[str, float|int]]] = {}
        for bank_type, banks in self.memory_banks.items():
            self.memory_bank_summary[bank_type] = {}
            for bank_info in banks:

                this_bank_info = {}

                # Find matching memory bank in old memory banks.  Compare by name as it would be possible
                # for the indices to change between builds if someone edited the memory bank definition
                old_bank_info = None
                if self.old_memory_banks is not None and bank_type in self.old_memory_banks:
                    for curr_old_bank_info in self.old_memory_banks[bank_type]:
                        if curr_old_bank_info.name == bank_info.name:
                            old_bank_info = curr_old_bank_info
                            break

                this_bank_info["bytes_used"] = bank_info.used_size
                this_bank_info["total_size"] = bank_info.total_size
                this_bank_info["delta_bytes_used"] = 0 if old_bank_info is None else bank_info.used_size - old_bank_info.used_size
                this_bank_info["percent_used"] = 100 * bank_info.used_size/bank_info.total_size
                this_bank_info["delta_percent_used"] = 100 * this_bank_info["delta_bytes_used"]/bank_info.total_size

                self.memory_bank_summary[bank_type][bank_info.name] = this_bank_info

        self.mem_report["memory_bank_usage"] = self.memory_bank_summary

    def parse(self, mapfile: str, toolchain: str, memory_banks_json_path: str | None) -> bool:
        """ Parse and decode map file depending on the toolchain

        Positional arguments:
        mapfile - the file name of the memory map file
        toolchain - the toolchain used to create the file
        """
        self.tc_name = toolchain.title()
        if toolchain == "GCC_ARM":
            parser_class = _GccParser
        else:
            return False
        parser = parser_class()
        old_map_parser = parser_class()

        if memory_banks_json_path is not None:
            with open(memory_banks_json_path, 'r') as memory_banks_json_file:
                parser.load_memory_banks_info(memory_banks_json_file)

        try:
            with open(mapfile, 'r') as file_input:
                self.modules = parser.parse_mapfile(file_input)
                self.memory_banks = parser.memory_banks
            try:
                with open("%s.old" % mapfile, 'r') as old_input:
                    self.old_modules = old_map_parser.parse_mapfile(old_input)
                    self.old_memory_banks = old_map_parser.memory_banks
            except IOError:
                self.old_modules = None
                self.old_memory_banks = None
            return True

        except IOError as error:
            print("I/O error({0}): {1}".format(error.errno, error.strerror))
            return False


def main():
    """Entry Point"""
    version = '1.0.0'

    # Parser handling
    parser = ArgumentParser(
        description="Memory Map File Analyser for ARM mbed\nversion %s" %
        version)

    parser.add_argument(
        'file', type=argparse_filestring_type, help='memory map file')

    parser.add_argument(
        '-t', '--toolchain', dest='toolchain',
        help='select a toolchain used to build the memory map file (%s)' %
        ", ".join(MemapParser.toolchains),
        required=True,
        type=argparse_uppercase_type(MemapParser.toolchains, "toolchain"))

    parser.add_argument(
        '-d', '--depth', dest='depth', type=int,
        help='specify directory depth level to display report', required=False)

    parser.add_argument(
        '-o', '--output', help='output file name', required=False)

    parser.add_argument(
        '-e', '--export', dest='export', required=False, default='table',
        type=argparse_lowercase_hyphen_type(MemapParser.export_formats,
                                            'export format'),
        help="export format (examples: %s: default)" %
        ", ".join(MemapParser.export_formats))

    parser.add_argument('-v', '--version', action='version', version=version)

    parser.add_argument(
        '-m', '--memory-banks-json',
        type=argparse_filestring_type,
        help='Path to memory bank JSON file.  If passed, memap will track the used space in each memory bank.')

    # Parse/run command
    if len(argv) <= 1:
        parser.print_help()
        exit(1)

    args = parser.parse_args()

    # Create memap object
    memap = MemapParser()

    # Parse and decode a map file
    if args.file and args.toolchain:
        if memap.parse(args.file, args.toolchain, args.memory_banks_json) is False:
            exit(0)

    if args.depth is None:
        depth = 2  # default depth level
    else:
        depth = args.depth

    returned_string = None
    # Write output in file
    if args.output is not None:
        returned_string = memap.generate_output(
            args.export,
            depth,
            args.output
        )
    else:  # Write output in screen
        returned_string = memap.generate_output(args.export, depth)

    if args.export == 'table' and returned_string:
        print(returned_string)

    exit(0)


if __name__ == "__main__":
    main()
