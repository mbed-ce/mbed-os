#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Main cli entry point."""

import logging
import sys

import click
from typing_extensions import override

from mbed_tools.cli.cmsis_mcu_descr import cmsis_mcu_descr
from mbed_tools.cli.configure import configure
from mbed_tools.cli.list_connected_devices import list_connected_devices
from mbed_tools.cli.sterm import sterm
from mbed_tools.lib.logging import MbedToolsHandler, set_log_level

CONTEXT_SETTINGS = {"help_option_names": ["-h", "--help"]}
LOGGER = logging.getLogger(__name__)


class GroupWithExceptionHandling(click.Group):
    """A click.Group which handles ToolsErrors and logging."""

    @override
    def invoke(self, ctx: click.Context) -> None:
        """
        Invoke the command group.

        Args:
            ctx: The current click context.
        """
        # Use the context manager to ensure tools exceptions (expected behaviour) are shown as messages to the user,
        # but all other exceptions (unexpected behaviour) are shown as errors.
        with MbedToolsHandler(LOGGER, ctx.params["traceback"]) as handler:
            super().invoke(ctx)

        sys.exit(handler.exit_code)


@click.group(cls=GroupWithExceptionHandling, context_settings=CONTEXT_SETTINGS)
@click.option(
    "-v",
    "--verbose",
    default=0,
    count=True,
    help="Set the verbosity level, enter multiple times to increase verbosity.",
)
@click.option("-t", "--traceback", is_flag=True, show_default=True, help="Show a traceback when an error is raised.")
def cli(verbose: int, traceback: bool) -> None:  # noqa: ARG001 # pyright: ignore[reportUnusedParameter]
    """Command line tool for interacting with Mbed OS."""
    # note: traceback parameter not used here but is accessed through
    # click.context.params in GroupWithExceptionHandling

    set_log_level(verbose)


cli.add_command(configure, "configure")
cli.add_command(list_connected_devices, "detect")
cli.add_command(sterm, "sterm")
cli.add_command(cmsis_mcu_descr)

if __name__ == "__main__":
    cli()
