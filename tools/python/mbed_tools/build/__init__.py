#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""
Provides the core Python parts of the build system for Mbed OS.

This module contains the configuration generation functionality, which processes JSON files into a set of target labels,
flags, and options.
"""

from mbed_tools.build.config import generate_config as generate_config
from mbed_tools.build.flash import flash_binary as flash_binary
