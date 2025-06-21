#
# Copyright (c) 2024 Jamie Smith. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#

from mbed_tools.build._internal.memory_banks import process_memory_banks


class TestMemoryBankProcessing:
    def test_simple_memory_layout(self):
        """
        Test a simple memory layout to ensure we process it correctly.
        """

        # Note: process_memory_banks() accepts a Config object, but that's just a
        # subclass of Dict, so we can pass in a dict in the test and get away with it.
        config = {
            # Real snippet from cmsis_mcu_descriptions.json
            "memory_banks": {
                "IRAM1": {
                    "access": {
                        "execute": True,
                        "non_secure": False,
                        "non_secure_callable": False,
                        "peripheral": False,
                        "read": True,
                        "secure": False,
                        "write": True
                    },
                    "default": True,
                    "size": 0x100000,
                    "start": 0x08000000,
                    "startup": False
                },
                "IROM1": {
                    "access": {
                        "execute": True,
                        "non_secure": False,
                        "non_secure_callable": False,
                        "peripheral": False,
                        "read": True,
                        "secure": False,
                        "write": False
                    },
                    "default": True,
                    "size": 0x200000,
                    "start": 0x10000000,
                    "startup": True
                }
            }
        }

        process_memory_banks(config)

        assert config["memory_bank_macros"] == {
            # New style definitions (ROM)
            'MBED_ROM_BANK_IROM1_START=0x10000000',
            'MBED_ROM_BANK_IROM1_SIZE=0x200000',
            'MBED_CONFIGURED_ROM_BANK_IROM1_START=0x10000000',
            'MBED_CONFIGURED_ROM_BANK_IROM1_SIZE=0x200000',

            # Old style definitions (ROM)
            'MBED_ROM_START=0x10000000',
            'MBED_ROM_SIZE=0x200000',
            'MBED_CONFIGURED_ROM_START=0x10000000',
            'MBED_CONFIGURED_ROM_SIZE=0x200000',

            # New style definitions (RAM)
            'MBED_RAM_BANK_IRAM1_START=0x8000000',
            'MBED_RAM_BANK_IRAM1_SIZE=0x100000',
            'MBED_CONFIGURED_RAM_BANK_IRAM1_START=0x8000000',
            'MBED_CONFIGURED_RAM_BANK_IRAM1_SIZE=0x100000',

            # Old style definitions (RAM)
            'MBED_RAM_START=0x8000000',
            'MBED_RAM_SIZE=0x100000',
            'MBED_CONFIGURED_RAM_START=0x8000000',
            'MBED_CONFIGURED_RAM_SIZE=0x100000',
            }

    def test_memory_configuration(self):
        """
        Test configuring the size and address of a memory bank
        """

        # Note: process_memory_banks() accepts a Config object, but that's just a
        # subclass of Dict, so we can pass in a dict in the test and get away with it.
        config = {
            # Real snippet from cmsis_mcu_descriptions.json
            "memory_banks": {
                "IRAM1": {
                    "access": {
                        "execute": True,
                        "non_secure": False,
                        "non_secure_callable": False,
                        "peripheral": False,
                        "read": True,
                        "secure": False,
                        "write": True
                    },
                    "default": True,
                    "size": 0x100000,
                    "start": 0x08000000,
                    "startup": False
                },
                "IROM1": {
                    "access": {
                        "execute": True,
                        "non_secure": False,
                        "non_secure_callable": False,
                        "peripheral": False,
                        "read": True,
                        "secure": False,
                        "write": False
                    },
                    "default": True,
                    "size": 0x200000,
                    "start": 0x10000000,
                    "startup": True
                }
            },
            "memory_bank_config": {
                "IRAM1": {
                    # Configure size only
                    "size": 0xa0000,
                },
                "IROM1": {
                    # Configure size and address
                    "size": 0x1f0000,
                    "start": 0x10010000
                }
            }
        }

        process_memory_banks(config)

        assert config["memory_bank_macros"] == {
            # New style definitions (ROM)
            'MBED_ROM_BANK_IROM1_START=0x10000000',
            'MBED_ROM_BANK_IROM1_SIZE=0x200000',
            'MBED_CONFIGURED_ROM_BANK_IROM1_START=0x10010000',
            'MBED_CONFIGURED_ROM_BANK_IROM1_SIZE=0x1f0000',

            # Old style definitions (ROM)
            'MBED_ROM_START=0x10000000',
            'MBED_ROM_SIZE=0x200000',
            'MBED_CONFIGURED_ROM_START=0x10010000',
            'MBED_CONFIGURED_ROM_SIZE=0x1f0000',

            # New style definitions (RAM)
            'MBED_RAM_BANK_IRAM1_START=0x8000000',
            'MBED_RAM_BANK_IRAM1_SIZE=0x100000',
            'MBED_CONFIGURED_RAM_BANK_IRAM1_START=0x8000000',
            'MBED_CONFIGURED_RAM_BANK_IRAM1_SIZE=0xa0000',

            # Old style definitions (RAM)
            'MBED_RAM_START=0x8000000',
            'MBED_RAM_SIZE=0x100000',
            'MBED_CONFIGURED_RAM_START=0x8000000',
            'MBED_CONFIGURED_RAM_SIZE=0xa0000',
            }

    def test_two_ram_banks(self):
        """
        Test to see if two RAM banks are handled correctly.
        """

        # Note: process_memory_banks() accepts a Config object, but that's just a
        # subclass of Dict, so we can pass in a dict in the test and get away with it.
        config = {
            "memory_banks": {
                "IRAM1": {
                    "access": {
                        "execute": True,
                        "non_secure": False,
                        "non_secure_callable": False,
                        "peripheral": False,
                        "read": True,
                        "secure": False,
                        "write": True
                    },
                    "default": True,
                    "size": 0x100000,
                    "start": 0x08000000,
                    "startup": False
                },
                "IRAM2": {
                    "access": {
                        "execute": True,
                        "non_secure": False,
                        "non_secure_callable": False,
                        "peripheral": False,
                        "read": True,
                        "secure": False,
                        "write": True
                    },
                    "default": False,
                    "size": 0x400000,
                    "start": 0x08100000,
                    "startup": False
                },
                "IROM1": {
                    "access": {
                        "execute": True,
                        "non_secure": False,
                        "non_secure_callable": False,
                        "peripheral": False,
                        "read": True,
                        "secure": False,
                        "write": False
                    },
                    "default": True,
                    "size": 0x200000,
                    "start": 0x10000000,
                    "startup": True
                }
            }
        }

        process_memory_banks(config)

        # Note: IRAM2 should become MBED_RAM1 because it is listed second
        # in the dictionary
        assert config["memory_bank_macros"] == {
            # New style definitions (ROM)
            'MBED_ROM_BANK_IROM1_START=0x10000000',
            'MBED_ROM_BANK_IROM1_SIZE=0x200000',
            'MBED_CONFIGURED_ROM_BANK_IROM1_START=0x10000000',
            'MBED_CONFIGURED_ROM_BANK_IROM1_SIZE=0x200000',

            # Old style definitions (ROM)
            'MBED_ROM_START=0x10000000',
            'MBED_ROM_SIZE=0x200000',
            'MBED_CONFIGURED_ROM_START=0x10000000',
            'MBED_CONFIGURED_ROM_SIZE=0x200000',

            # New style definitions (RAM)
            'MBED_RAM_BANK_IRAM1_START=0x8000000',
            'MBED_RAM_BANK_IRAM1_SIZE=0x100000',
            'MBED_CONFIGURED_RAM_BANK_IRAM1_START=0x8000000',
            'MBED_CONFIGURED_RAM_BANK_IRAM1_SIZE=0x100000',

            # Old style definitions (RAM)
            'MBED_RAM_START=0x8000000',
            'MBED_RAM_SIZE=0x100000',
            'MBED_CONFIGURED_RAM_START=0x8000000',
            'MBED_CONFIGURED_RAM_SIZE=0x100000',

            # New style definitions (RAM 2)
            'MBED_RAM_BANK_IRAM2_START=0x8100000',
            'MBED_RAM_BANK_IRAM2_SIZE=0x400000',
            'MBED_CONFIGURED_RAM_BANK_IRAM2_START=0x8100000',
            'MBED_CONFIGURED_RAM_BANK_IRAM2_SIZE=0x400000',

            # Old style definitions (RAM 2)
            'MBED_RAM1_START=0x8100000',
            'MBED_RAM1_SIZE=0x400000',
            'MBED_CONFIGURED_RAM1_START=0x8100000',
            'MBED_CONFIGURED_RAM1_SIZE=0x400000',
            }
