#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import factory
import pathlib

from typing import List

from mbed_tools.devices.device import Device, ConnectedDevices
from mbed_tools.targets.board import Board


class BoardFactory(factory.Factory):
    class Meta:
        model = Board

    board_type = "TEST"
    board_name = ""
    product_code = ""
    target_type = ""
    slug = ""
    build_variant = []
    mbed_os_support = []
    mbed_enabled = []


class DeviceFactory(factory.Factory):
    class Meta:
        model = Device

    serial_port = ""
    serial_number = ""
    mount_points = [pathlib.Path(".")]
    mbed_board = BoardFactory()


class ConnectedDevicesFactory(factory.Factory):
    class Meta:
        model = ConnectedDevices

    identified_devices: List[Device]
    unidentified_devices: List[Device]
