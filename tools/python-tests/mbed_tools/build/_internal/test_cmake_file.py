#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import pytest

from mbed_tools.build._internal.cmake_file import render_mbed_config_cmake_template
from mbed_tools.build._internal.config.config import Config
from mbed_tools.build._internal.config.source import ConfigSetting, prepare


TOOLCHAIN_NAME = "gcc"


@pytest.fixture()
def fake_target():
    return {
        "labels": ["foo"],
        "extra_labels": ["morefoo"],
        "features": ["bar"],
        "components": ["baz"],
        "macros": ["macbaz"],
        "device_has": ["stuff"],
        "c_lib": ["c_lib"],
        "core": ["core"],
        "printf_lib": ["printf_lib"],
        "supported_form_factors": ["arduino"],
        "supported_c_libs": {TOOLCHAIN_NAME: ["ginormous"]},
        "supported_application_profiles": ["full", "bare-metal"],
    }


class TestRendersCMakeListsFile:
    def test_returns_rendered_content(self, fake_target):
        config = Config(prepare(fake_target))
        result = render_mbed_config_cmake_template(config, TOOLCHAIN_NAME, "target_name")

        for label in fake_target["labels"] + fake_target["extra_labels"]:
            assert label in result

        for macro in fake_target["features"] + fake_target["components"] + [TOOLCHAIN_NAME]:
            assert macro in result

        for toolchain in fake_target["supported_c_libs"]:
            assert toolchain in result
            for supported_c_libs in toolchain:
                assert supported_c_libs in result

        for supported_application_profiles in fake_target["supported_application_profiles"]:
            assert supported_application_profiles in result

    def test_returns_quoted_content(self, fake_target):
        config = Config(prepare(fake_target))

        # Add an option whose value contains quotes to the config.
        config["config"] = [
            ConfigSetting(
                name="mqtt-host", namespace="iotc", help_text="", value='{"mqtt.2030.ltsapis.goog", IOTC_MQTT_PORT}',
            )
        ]

        result = render_mbed_config_cmake_template(config, TOOLCHAIN_NAME, "target_name")
        assert '"MBED_CONF_IOTC_MQTT_HOST={\\"mqtt.2030.ltsapis.goog\\", IOTC_MQTT_PORT}"' in result
