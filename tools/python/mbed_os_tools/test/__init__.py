# Copyright (c) 2018, Arm Limited and affiliates.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


"""! @package mbed-host-tests

Flash, reset and  perform host supervised tests on mbed platforms.
Write your own programs (import this package) or use 'mbedhtrun'
command line tool instead.

"""

import importlib
import sys
from argparse import ArgumentParser, SUPPRESS
from . import host_tests_plugins
from .host_tests_registry import HostRegistry  # noqa: F401
from .host_tests import BaseHostTest, event_callback  # noqa: F401

# Set the default baud rate
DEFAULT_BAUD_RATE = 9600

###############################################################################
# Functional interface for test supervisor registry
###############################################################################


def get_plugin_caps(methods=None):
    if not methods:
        methods = ["CopyMethod", "ResetMethod"]
    result = {}
    for method in methods:
        result[method] = host_tests_plugins.get_plugin_caps(method)
    return result


def init_host_test_cli_params():
    """! Function creates CLI parser object and returns populated options object.
    @return Function returns 'options' object returned from OptionParser class
    @details Options object later can be used to populate host test selector script.
    """
    parser = ArgumentParser()

    parser.add_argument(
        "-m",
        "--micro",
        dest="micro",
        help="Target microcontroller name",
        metavar="MICRO",
    )

    parser.add_argument(
        "-p", "--port", dest="port", help="Serial port of the target", metavar="PORT"
    )

    parser.add_argument(
        "-d",
        "--disk",
        dest="disk",
        help="Target disk (mount point) path",
        metavar="DISK_PATH",
    )

    parser.add_argument(
        "-t",
        "--target-id",
        dest="target_id",
        help="Unique Target Id or mbed platform",
        metavar="TARGET_ID",
    )

    parser.add_argument(
        "--sync",
        dest="sync_behavior",
        default=2,
        type=int,
        help=(
            "Define how many times __sync packet will be sent to device: 0: "
            "none; -1: forever; 1,2,3... - number of times (Default 2 time)"
        ),
        metavar="SYNC_BEHAVIOR",
    )

    parser.add_argument(
        "--sync-timeout",
        dest="sync_timeout",
        default=5,
        type=int,
        help="Define delay in seconds between __sync packet (Default is 5 seconds)",
        metavar="SYNC_TIMEOUT",
    )

    parser.add_argument(
        "-f",
        "--image-path",
        dest="image_path",
        help="Path with target's binary image",
        metavar="IMAGE_PATH",
    )

    copy_methods_str = "Plugin support: " + ", ".join(
        host_tests_plugins.get_plugin_caps("CopyMethod")
    )

    parser.add_argument(
        "-c",
        "--copy",
        dest="copy_method",
        help="Copy (flash the target) method selector. " + copy_methods_str,
        metavar="COPY_METHOD",
    )

    parser.add_argument(
        "--retry-copy",
        dest="retry_copy",
        default=3,
        type=int,
        help="Number of attempts to flash the target",
        metavar="RETRY_COPY",
    )

    parser.add_argument(
        "--tag-filters",
        dest="tag_filters",
        default="",
        type=str,
        help=(
            "Comma seperated list of device tags used when allocating a target "
            "to specify required hardware or attributes [--tag-filters tag1,tag2]"
        ),
        metavar="TAG_FILTERS",
    )

    reset_methods_str = "Plugin support: " + ", ".join(
        host_tests_plugins.get_plugin_caps("ResetMethod")
    )

    parser.add_argument(
        "-r",
        "--reset",
        dest="forced_reset_type",
        help="Forces different type of reset. " + reset_methods_str,
    )

    parser.add_argument(
        "-C",
        "--program_cycle_s",
        dest="program_cycle_s",
        default=4,
        help=(
            "Program cycle sleep. Define how many seconds you want wait after "
            "copying binary onto target (Default is 4 second)"
        ),
        type=float,
        metavar="PROGRAM_CYCLE_S",
    )

    parser.add_argument(
        "-R",
        "--reset-timeout",
        dest="forced_reset_timeout",
        default=1,
        metavar="NUMBER",
        type=float,
        help=(
            "When forcing a reset using option -r you can set up after reset "
            "idle delay in seconds (Default is 1 second)"
        ),
    )

    parser.add_argument(
        "--process-start-timeout",
        dest="process_start_timeout",
        default=60,
        metavar="NUMBER",
        type=float,
        help=(
            "This sets the maximum time in seconds to wait for an internal "
            "process to start. This mostly only affects machines under heavy "
            "load (Default is 60 seconds)"
        ),
    )

    parser.add_argument(
        "-e",
        "--enum-host-tests",
        dest="enum_host_tests",
        action="append",
        default=["./test/host_tests"],
        help="Define directory with local host tests",
    )

    parser.add_argument(
        "--test-cfg",
        dest="json_test_configuration",
        help="Pass to host test class data about host test configuration",
    )

    parser.add_argument(
        "--list",
        dest="list_reg_hts",
        default=False,
        action="store_true",
        help="Prints registered host test and exits",
    )

    parser.add_argument(
        "--plugins",
        dest="list_plugins",
        default=False,
        action="store_true",
        help="Prints registered plugins and exits",
    )

    parser.add_argument(
        "-g",
        "--grm",
        dest="global_resource_mgr",
        help=(
            'Global resource manager: "<remote mgr module>:<host url or IP address>'
            '[:<port>]", Ex. "module_name:10.2.123.43:3334", '
            'module_name:https://example.com"'
        ),
    )

    # Show --fm option only if "fm_agent" module installed
    fm_help=SUPPRESS
    try:
        if importlib.util.find_spec('fm_agent') is not None:
            fm_help='Fast Model connection, This option requires mbed-fastmodel-agent module installed, list CONFIGs via "mbedfm"'
    except ModuleNotFoundError:
        pass

    parser.add_argument(
        "--fm",
        dest="fast_model_connection",
        metavar="CONFIG",
        default=None,
        help=fm_help,
    )

    parser.add_argument(
        "--run",
        dest="run_binary",
        default=False,
        action="store_true",
        help="Runs binary image on target (workflow: flash, reset, output console)",
    )

    parser.add_argument(
        "--skip-flashing",
        dest="skip_flashing",
        default=False,
        action="store_true",
        help="Skips use of copy/flash plugin. Note: target will not be reflashed",
    )

    parser.add_argument(
        "--skip-reset",
        dest="skip_reset",
        default=False,
        action="store_true",
        help="Skips use of reset plugin. Note: target will not be reset",
    )

    parser.add_argument(
        "-P",
        "--polling-timeout",
        dest="polling_timeout",
        default=60,
        metavar="NUMBER",
        type=int,
        help=(
            "Timeout in sec for readiness of mount point and serial port of "
            "local or remote device. Default 60 sec"
        ),
    )

    parser.add_argument(
        "-b",
        "--send-break",
        dest="send_break_cmd",
        default=False,
        action="store_true",
        help=(
            "Send reset signal to board on specified port (-p PORT) and print "
            "serial output. You can combine this with (-r RESET_TYPE) switch"
        ),
    )

    parser.add_argument(
        "--baud-rate",
        dest="baud_rate",
        help=(
            "Baud rate of target, overrides values from mbed-ls, disk/mount "
            "point (-d, --disk-path), and serial port -p <port>:<baud rate>"
        ),
        metavar="BAUD_RATE",
    )

    parser.add_argument(
        "-v",
        "--verbose",
        dest="verbose",
        default=False,
        action="store_true",
        help="More verbose mode",
    )

    parser.add_argument(
        "--serial-output-file",
        dest="serial_output_file",
        default=None,
        help="Save target serial output to this file.",
    )

    parser.add_argument(
        "--compare-log",
        dest="compare_log",
        default=None,
        help="Log file to compare with the serial output from target.",
    )

    parser.add_argument(
        "--version",
        dest="version",
        default=False,
        action="store_true",
        help="Prints package version and exits",
    )

    parser.add_argument(
        "--format",
        dest="format",
        help="Image file format passed to pyocd (elf, bin, hex, axf...).",
    )

    parser.description = (
        """Flash, reset and perform host supervised tests on mbed platforms"""
    )
    parser.epilog = (
        """Example: mbedhtrun -d E: -p COM5 -f "test.bin" -C 4 -c shell -m K64F"""
    )

    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit()

    return parser.parse_args()
