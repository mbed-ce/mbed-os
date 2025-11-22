#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""
Creation and management of Mbed OS projects.

* Creation of a new Mbed OS application.
* Cloning of an existing Mbed OS program.
* Deploy of a specific version of Mbed OS or library.
"""

from mbed_tools.project.mbed_program import MbedProgram as MbedProgram
from mbed_tools.project.project import (
    deploy_project as deploy_project,
)
from mbed_tools.project.project import (
    get_known_libs as get_known_libs,
)
from mbed_tools.project.project import (
    import_project as import_project,
)
from mbed_tools.project.project import (
    initialise_project as initialise_project,
)
