#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import pytest

from mbed_tools.build._internal.config.config import Config
from mbed_tools.build._internal.config.source import prepare, ConfigSetting, Override


class TestConfig:
    def test_config_updated(self):
        conf = Config()

        conf.update(prepare({"config": {"param": {"value": 0}}}, source_name="lib"))
        conf.update(prepare({"config": {"param2": {"value": 0}}}, source_name="lib2"))

        assert conf["config"][0].name == "param"
        assert conf["config"][1].name == "param2"

    def test_raises_when_trying_to_add_duplicate_config_setting(self):
        conf = Config(prepare({"config": {"param": {"value": 0}}}, source_name="lib"))

        with pytest.raises(ValueError, match="lib.param already defined"):
            conf.update(prepare({"config": {"param": {"value": 0}}}, source_name="lib"))

    def test_target_overrides_handled(self):
        conf = Config(
            {
                "config": [
                    ConfigSetting(namespace="target", name="network-default-interface-type", help_text="", value="WIFI")
                ],
                "device_has": ["TEST"],
            }
        )

        conf.update(
            {
                "overrides": [
                    Override(namespace="target", name="network-default-interface-type", value="ETHERNET"),
                    Override(namespace="target", name="device_has", value={"OVERRIDDEN"}),
                ]
            }
        )

        network_iface, *_ = conf["config"]
        assert network_iface.value == "ETHERNET"
        assert conf["device_has"] == {"OVERRIDDEN"}

    def test_target_overrides_separate_namespace(self):
        conf = Config(
            {
                "config": [
                    ConfigSetting(
                        namespace="dontchangeme", name="network-default-interface-type", help_text="", value="WIFI"
                    ),
                    ConfigSetting(
                        namespace="changeme", name="network-default-interface-type", help_text="", value="WIFI"
                    ),
                ]
            }
        )

        conf.update(
            {"overrides": [Override(namespace="changeme", name="network-default-interface-type", value="ETHERNET")]}
        )

        dontchangeme, changeme, *_ = conf["config"]
        assert changeme.namespace == "changeme"
        assert changeme.value == "ETHERNET"
        assert dontchangeme.namespace == "dontchangeme"
        assert dontchangeme.value == "WIFI"

    def test_lib_overrides_handled(self):
        conf = Config(
            {
                "config": [
                    ConfigSetting(namespace="lib", name="network-default-interface-type", help_text="", value="WIFI")
                ],
            }
        )

        conf.update({"overrides": [Override(namespace="lib", name="network-default-interface-type", value="ETHERNET")]})

        network_iface, *_ = conf["config"]
        assert network_iface.value == "ETHERNET"

    def test_cumulative_fields_can_be_modified(self):
        conf = Config({"device_has": {"FLASHING_LIGHTS"}, "macros": {"A"}})

        conf.update(
            {
                "overrides": [
                    Override(namespace="target", name="device_has", modifier="add", value={"OTHER_STUFF"}),
                    Override(namespace="target", name="device_has", modifier="remove", value={"FLASHING_LIGHTS"}),
                    Override(namespace="lib", name="macros", modifier="remove", value={"A"}),
                    Override(namespace="target", name="macros", modifier="add", value={"B"}),
                ]
            }
        )

        conf.update({"overrides": [Override(namespace="target", name="macros", modifier="add", value={"B"})]})
        assert conf["device_has"] == {"OTHER_STUFF"}
        assert conf["macros"] == {"B"}

    def test_macros_are_appended_to(self):
        conf = Config({"macros": {"A"}})

        conf.update({"macros": {"B"}})
        conf.update({"macros": {"B"}})

        assert conf["macros"] == {"A", "B"}

    def test_requires_are_appended_to(self):
        conf = Config({"requires": {"A"}})

        conf.update({"requires": {"B"}})
        conf.update({"requires": {"C"}})

        assert conf["requires"] == {"A", "B", "C"}

    def test_warns_and_skips_override_for_undefined_config_parameter(self, caplog):
        conf = Config()
        override_name = "this-does-not-exist"
        conf.update(prepare({"target_overrides": {"*": {override_name: ""}}}))
        assert override_name in caplog.text
        assert not conf

    def test_ignores_present_option(self):
        source = prepare({"name": "mbed_component", "config": {"present": {"help": "Mbed Component", "value": True}}})

        config = Config(source)

        assert not config["config"]
