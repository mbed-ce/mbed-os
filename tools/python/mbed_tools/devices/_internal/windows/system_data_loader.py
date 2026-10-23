#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Loads system data in parallel and all at once in order to improve performance."""

from __future__ import annotations

from concurrent.futures import ThreadPoolExecutor
from typing import TYPE_CHECKING, Generic, cast

from typing_extensions import TypeVar

from mbed_tools.devices._internal.windows.component_descriptor import ComponentDescriptor, ComponentDescriptorWrapper
from mbed_tools.devices._internal.windows.disk_drive import DiskDrive
from mbed_tools.devices._internal.windows.disk_partition import DiskPartition
from mbed_tools.devices._internal.windows.disk_partition_logical_disk_relationships import (
    DiskPartitionLogicalDiskRelationship,
)
from mbed_tools.devices._internal.windows.logical_disk import LogicalDisk
from mbed_tools.devices._internal.windows.serial_port import SerialPort
from mbed_tools.devices._internal.windows.usb_controller import UsbController
from mbed_tools.devices._internal.windows.usb_hub import UsbHub

if TYPE_CHECKING:
    from collections.abc import Generator

# All Windows system data of interest in order to retrieve the information for DeviceCandidate.
SYSTEM_DATA_TYPES = [
    UsbHub,
    UsbController,
    DiskDrive,
    DiskPartition,
    LogicalDisk,
    DiskPartitionLogicalDiskRelationship,
    SerialPort,
]


def load_all(cls: type) -> tuple[type, list[ComponentDescriptor]]:
    """Loads all elements present in the system referring to a specific type."""
    return (cls, list(ComponentDescriptorWrapper(cls).element_generator()))


ComponentT = TypeVar("ComponentT", bound=ComponentDescriptor)


class SystemDataLoader:
    """
    Object in charge of loading all system data with regards to Usb, Disk or serial port.

    It loads all the data in parallel and all at once in order to improve performance.
    """

    def __init__(self) -> None:
        """Initialiser."""
        self._system_data: dict[type, list[ComponentDescriptor]] | None = None

    def _load(self) -> None:
        """Loads all system data in parallel."""
        with ThreadPoolExecutor() as executor:
            results = executor.map(load_all, SYSTEM_DATA_TYPES)
        self._system_data = dict(results)

    @property
    def system_data(self) -> dict[type, list[ComponentDescriptor]]:
        """Gets all system data."""
        if not self._system_data:
            self._load()
        return cast(dict[type, list[ComponentDescriptor]], self._system_data)

    def get_system_data(self, cls: type[ComponentT]) -> list[ComponentT]:
        """Gets the system data for a particular type."""
        return cast(list[ComponentT], self.system_data.get(cls, []))


class ComponentsLoader(Generic[ComponentT]):
    """Loads system components."""

    def __init__(self, data_loader: SystemDataLoader, cls: type[ComponentT]) -> None:
        """initialiser."""
        self._cls = cls
        self._data_loader = data_loader

    def element_generator(self) -> Generator[ComponentT, None, None]:
        """Gets a generator over all elements currently registered in the system."""
        elements = self._data_loader.get_system_data(self._cls)
        yield from elements
