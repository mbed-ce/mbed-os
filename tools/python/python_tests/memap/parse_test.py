"""
Copyright (c) 2018-2019 ARM Limited. All rights reserved.

SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations 
"""

import sys
from io import open
from os import sep
from os.path import isfile, join, dirname
from collections import defaultdict

import pytest

from memap.memap import MemapParser, _GccParser
from copy import deepcopy


PARSED_GCC_DATA = {
    "startup/startup.o": defaultdict(int, {".text": 0xc0}),
    "[lib]/d16M_tlf.a/__main.o": defaultdict(int, {".text": 8}),
    "[lib]/misc/foo.o": defaultdict(int, {".text": 8}),
    "irqs/irqs.o": defaultdict(int, {".text": 0x98}),
    "data/data.o":defaultdict(int,  {".data": 0x18, ".bss": 0x198}),
    "main.o": defaultdict(int, {".text": 0x36}),
}

def test_parse_gcc():
    memap = MemapParser()

    this_script_dir = dirname(__file__)
    memap.parse(join(this_script_dir, "gcc.map"), "GCC_ARM", join(this_script_dir, "test_memory_banks.json"))

    parsed_data_os_agnostic = dict()
    for k in PARSED_GCC_DATA:
        parsed_data_os_agnostic[k.replace('/', sep)] = PARSED_GCC_DATA[k]

    # Sum of everything in .text and .data
    assert memap.memory_banks["ROM"][0].used_size == 0x1B6

    # Sum of everything in .bss and .data
    assert memap.memory_banks["RAM"][0].used_size == 0x1B0

    assert memap.modules == parsed_data_os_agnostic


def test_add_symbol_missing_info():
    memap = _GccParser()
    old_symbols = deepcopy(memap.modules)
    memap.add_symbol(".data.some_func", "", 8, 10, ".data", 1000)
    assert(old_symbols == memap.modules)
    memap.add_symbol(".data.some_func", "foo.o", 8, 0, ".data", 1000)
    assert(old_symbols == memap.modules)


def test_add_full_module():
    memap = _GccParser()
    old_modules = deepcopy(memap.modules)
    memap.add_symbol(".data.foo", "main.o", 5, 8, ".data", 1000)
    assert(old_modules != memap.modules)
    assert("main.o" in memap.modules)
    assert(".data" in memap.modules["main.o"])
    assert(memap.modules["main.o"][".data"] == 8)
