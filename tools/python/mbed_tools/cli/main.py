#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Main cli entry point."""

import logging
import sys
from typing import Any, Union

import click
from pkg_resources import get_distribution

from mbed_tools.cli.cmsis_mcu_descr import cmsis_mcu_descr
from mbed_tools.cli.configure import configure
from mbed_tools.cli.list_connected_devices import list_connected_devices
from mbed_tools.cli.project_management import deploy, import_, new
from mbed_tools.cli.sterm import sterm
from mbed_tools.lib.logging import MbedToolsHandler, set_log_level

CONTEXT_SETTINGS = {"help_option_names": ["-h", "--help"]}
LOGGER = logging.getLogger(__name__)


class GroupWithExceptionHandling(click.Group):
    """A click.Group which handles ToolsErrors and logging."""

    def invoke(self, context: click.Context) -> None:
        """
        Invoke the command group.

        Args:
            context: The current click context.
        """
        # Use the context manager to ensure tools exceptions (expected behaviour) are shown as messages to the user,
        # but all other exceptions (unexpected behaviour) are shown as errors.
        with MbedToolsHandler(LOGGER, context.params["traceback"]) as handler:
            super().invoke(context)

        sys.exit(handler.exit_code)


def print_version(context: click.Context, _param: Union[click.Option, click.Parameter], value: bool) -> Any:
    """Print the version of mbed-tools."""
    if not value or context.resilient_parsing:
        return

    version_string = get_distribution("mbed-ce-tools").version
    click.echo(version_string)
    context.exit()


@click.group(cls=GroupWithExceptionHandling, context_settings=CONTEXT_SETTINGS)
@click.option(
    "--version",
    is_flag=True,
    callback=print_version,
    expose_value=False,
    is_eager=True,
    help="Display versions of all Mbed Tools packages.",
)
@click.option(
    "-v",
    "--verbose",
    default=0,
    count=True,
    help="Set the verbosity level, enter multiple times to increase verbosity.",
)
@click.option("-t", "--traceback", is_flag=True, show_default=True, help="Show a traceback when an error is raised.")
def cli(verbose: int, _traceback: bool) -> None:
    """Command line tool for interacting with Mbed OS."""
    # note: traceback parameter not used here but is accessed through
    # click.context.params in GroupWithExceptionHandling

    set_log_level(verbose)


cli.add_command(configure, "configure")
cli.add_command(list_connected_devices, "detect")
cli.add_command(new, "new")
cli.add_command(deploy, "deploy")
cli.add_command(import_, "import")
cli.add_command(sterm, "sterm")
cli.add_command(cmsis_mcu_descr)

if __name__ == "__main__":
    cli()
