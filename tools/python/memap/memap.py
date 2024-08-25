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
from __future__ import absolute_import

import dataclasses
from typing import Tuple, Optional, Dict, TextIO, List
from abc import abstractmethod, ABC
from sys import stdout, exit, argv, path
from os import sep
from os.path import (basename, dirname, join, relpath, abspath, commonprefix,
                     splitext)
import re
import csv
import json
from argparse import ArgumentParser
from copy import deepcopy
from collections import defaultdict
from prettytable import PrettyTable, HEADER
from jinja2 import FileSystemLoader, StrictUndefined
from jinja2.environment import Environment
from future.utils import with_metaclass


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
    # Name of the bank, from cmsis_mcu_descriptions.json
    name: str

    # Start address of memory bank
    start_addr: int

    # Total size of the memory bank in bytes
    total_size: int

    # Size used in the memory bank in bytes
    used_size: int = 0


class _Parser(ABC):
    """Internal interface for parsing"""
    SECTIONS = ('.text', '.data', '.bss', '.heap', '.stack')
    MISC_FLASH_SECTIONS = ('.interrupts', '.flash_config')
    OTHER_SECTIONS = ('.interrupts_ram', '.init', '.ARM.extab',
                      '.ARM.exidx', '.ARM.attributes', '.eh_frame',
                      '.init_array', '.fini_array', '.jcr', '.stab',
                      '.stabstr', '.ARM.exidx', '.ARM')

    def __init__(self):
        # Dict of object name to {section name, size}
        self.symbols: Dict[str, Dict[str, int]] = {}

        # Memory bank info, by type (RAM/ROM)
        self.memory_banks: Dict[str, List[MemoryBankInfo]] = {"RAM": [], "ROM": []}

    def add_symbol(self, symbol_name: str, object_name: str, start_addr: int, size: int, section: str):
        """ Adds information about a symbol (e.g. a function or global variable) to the data structures.

        Positional arguments:
        symbol_name - Descriptive name of the symbol, e.g. ".text.some_function"
        object_name - name of the object file containing the symbol
        start addr - start address of symbol
        size - the size of the symbol being added
        section - Name of the section, e.g. ".text".  Can also be "unknown".
        """
        if not object_name or not size:
            return

        if object_name in self.symbols:
            self.symbols[object_name].setdefault(section, 0)
            self.symbols[object_name][section] += size
            return

        obj_split = sep + basename(object_name)
        for module_path, contents in self.symbols.items():
            if module_path.endswith(obj_split) or module_path == object_name:
                contents.setdefault(section, 0)
                contents[section] += size
                return

        new_module = defaultdict(int)
        new_module[section] = size
        self.symbols[object_name] = new_module

    def load_memory_banks_info(self, memory_banks_json_file: TextIO) -> None:
        """
        Load the memory bank information from a memory_banks.json file
        """
        memory_banks_json = json.load(memory_banks_json_file)
        for bank_type, banks in memory_banks_json["configured_memory_banks"].items():
            for bank_name, bank_data in banks.items():
                self.memory_banks["bank_type"].append(MemoryBankInfo(
                    name=bank_name,
                    start_addr=bank_data["start"],
                    total_size=bank_data["size"]
                ))

    @abstractmethod
    def parse_mapfile(self, file_desc: TextIO) -> Dict[str, Dict[str, int]]:
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

    # Gets the input section name from the line, if it exists.
    # Input section names are always indented 1 space.
    RE_INPUT_SECTION_NAME = re.compile(r'^ (\.\w+\.?\w*\.?\w*)')  # Note: This allows up to 3 dots... hopefully that's enough...

    ALL_SECTIONS = (
        _Parser.SECTIONS
        + _Parser.OTHER_SECTIONS
        + _Parser.MISC_FLASH_SECTIONS
        + ('unknown', )
    )

    def check_new_section(self, line):
        """ Check whether a new section in a map file has been detected

        Positional arguments:
        line - the line to check for a new section

        return value - A section name, if a new section was found, None
                       otherwise
        """
        for i in self.ALL_SECTIONS:
            if line.startswith(i):
                return i
        if line.startswith('.'):
            return 'unknown'
        else:
            return None

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

    def parse_object_name(self, line) -> str:
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

    def parse_section(self, line: str) -> Tuple[str, int, int]:
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

    def parse_mapfile(self, file_desc: TextIO) -> Dict[str, Dict[str, int]]:
        """ Main logic to decode gcc map files

        Positional arguments:
        file_desc - a stream object to parse as a gcc map file
        """

        # GCC can put the section/symbol info on its own line or on the same line as the size and address.
        # So since this is a line oriented parser, we have to remember the most recently seen input & output
        # section name for later.
        # Note: section (AKA output section) is the section in the output application that a symbol is going into.
        # Examples: .text, .data, .bss
        # input_section is the section in the object file that a symbol comes from.  It is generally more specific,
        # e.g. a function could be from .text.my_function
        # The output section *might* be a prefix to the input section, but not always: a linker script can happily
        # put stuff from any input section into .text or .data if it wants to!
        current_section = 'unknown'
        current_input_section = 'unknown'

        with file_desc as infile:
            for line in infile:
                if line.startswith('Linker script and memory map'):
                    break

            for line in infile:
                if line.startswith("OUTPUT("):
                    # Done with memory map part of the map file
                    break

                next_section = self.check_new_section(line)
                if next_section is not None:
                    current_section = next_section

                next_input_section = self.check_input_section(line)
                if next_input_section is not None:
                    current_input_section = next_input_section

                symbol_name, symbol_start_addr, symbol_size = self.parse_section(line)

                # With GCC at least, the closest we can get to a descriptive symbol name is the input section
                # name.  Thanks to the -ffunction-sections and -fdata-sections options, the section names should
                # be unique for each symbol.
                self.add_symbol(current_input_section, symbol_name, symbol_start_addr, symbol_size, current_section)

        common_prefix = dirname(commonprefix([
            o for o in self.symbols.keys()
            if (
                    o.endswith(self.OBJECT_EXTENSIONS)
                    and not o.startswith("[lib]")
            )]))
        new_modules = {}
        for name, stats in self.symbols.items():
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

        return output

    toolchains = ["ARM", "ARM_STD", "ARM_MICRO", "GCC_ARM", "IAR"]

    def compute_report(self):
        """ Generates summary of memory usage for main areas
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

        self.mem_report = []
        if self.short_modules:
            for name, sizes in sorted(self.short_modules.items()):
                self.mem_report.append({
                    "module": name,
                    "size": {
                        k: sizes.get(k, 0) for k in (self.print_sections +
                                                     self.delta_sections)
                    }
                })

        self.mem_report.append({
            'summary': self.mem_summary
        })

    def parse(self, mapfile, toolchain):
        """ Parse and decode map file depending on the toolchain

        Positional arguments:
        mapfile - the file name of the memory map file
        toolchain - the toolchain used to create the file
        """
        self.tc_name = toolchain.title()
        if toolchain == "GCC_ARM":
            parser = _GccParser
        else:
            return False
        try:
            with open(mapfile, 'r') as file_input:
                self.modules = parser().parse_mapfile(file_input)
            try:
                with open("%s.old" % mapfile, 'r') as old_input:
                    self.old_modules = parser().parse_mapfile(old_input)
            except IOError:
                self.old_modules = None
            return True

        except IOError as error:
            print("I/O error({0}): {1}".format(error.errno, error.strerror))
            return False


def main():
    """Entry Point"""
    version = '0.4.0'

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

    # Parse/run command
    if len(argv) <= 1:
        parser.print_help()
        exit(1)

    args = parser.parse_args()

    # Create memap object
    memap = MemapParser()

    # Parse and decode a map file
    if args.file and args.toolchain:
        if memap.parse(args.file, args.toolchain) is False:
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
