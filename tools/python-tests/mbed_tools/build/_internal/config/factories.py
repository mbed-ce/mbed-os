#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import factory

from mbed_tools.build._internal.config.config import Config, Option, Macro
from mbed_tools.build._internal.config.source import Source


class SourceFactory(factory.Factory):
    class Meta:
        model = Source

    human_name = factory.Faker("name")
    config = factory.Dict({})
    overrides = factory.Dict({})
    macros = factory.List([])


class OptionFactory(factory.Factory):
    class Meta:
        model = Option

    value = "0"
    macro_name = "MACRO_NAME"
    help_text = factory.Faker("text")
    set_by = "libname"
    key = "key"


class MacroFactory(factory.Factory):
    class Meta:
        model = Macro

    value = "0"
    name = "A_MACRO"
    set_by = "source"


class ConfigFactory(factory.Factory):
    class Meta:
        model = Config

    options = factory.Dict({})
    macros = factory.Dict({})
