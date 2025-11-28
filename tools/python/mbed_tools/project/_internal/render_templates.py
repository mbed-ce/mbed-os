#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Render jinja templates required by the project package."""

from pathlib import Path

import jinja2

TEMPLATES_DIRECTORY = Path("_internal", "templates")


def render_jinja_template(template_name: str, context: dict) -> str:
    """
    Render a jinja template.

    Args:
        template_name: The name of the template being rendered.
        context: Data to render into the jinja template.
    """
    env = jinja2.Environment(
        loader=jinja2.PackageLoader("mbed_tools.project", str(TEMPLATES_DIRECTORY)),
        autoescape=False,  # autoescape not needed because we are not rendering HTML
    )
    template = env.get_template(template_name)
    return template.render(context)
