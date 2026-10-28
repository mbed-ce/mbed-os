"""
Module for creating and accessing an SQLite database of Mbed test results
"""

from __future__ import annotations

import collections
import dataclasses
import enum
import sqlite3
from typing import TYPE_CHECKING, Any

import graphviz

from mbed_tools.build._internal import memory_banks
from mbed_tools.build.config import load_raw_targets_data
from mbed_tools.lib.json_helpers import decode_json_file
from mbed_tools.project import MbedProgram
from mbed_tools.targets._internal.target_attributes import get_target_attributes

if TYPE_CHECKING:
    import pathlib


class TestResult(enum.IntEnum):
    """
    Enumeration of the possible results a test or test case can have
    """

    PASSED = 1  # Test ran and passed
    FAILED = 2  # Test ran and failed
    SKIPPED = 3  # Test was not run because it is not supported on this target
    PRIOR_TEST_CASE_CRASHED = (
        4  # Test case was not executed because a prior test case crashed. For test cases only, not tests.
    )


class DriverType(enum.Enum):
    """
    Enumeration of the possible types a driver can be
    """

    PERIPHERAL = "Peripheral"  # Peripheral present on this target and supported by Mbed CE
    FEATURE = "Feature"  # Larger Mbed OS optional feature supported for this target
    COMPONENT = "Component"  # External component present on this board


# String to set for the MCU target family when there is none
NO_MCU_TARGET_FAMILY = "NO_FAMILY"


class MbedTestDatabase:
    def __init__(self, database_path: pathlib.Path | None = None):
        """
        Create an MbedTestDatabase, passing the path to the database file.

        If the path is None, an in-memory database is created.
        """
        db_path = ":memory:" if database_path is None else str(database_path)
        self._database = sqlite3.connect(db_path)

        # Enable accessing rows by their name in cursors
        # https://stackoverflow.com/a/20042292/7083698
        self._database.row_factory = sqlite3.Row

        # turn off transaction management (we do that ourselves)
        self._database.isolation_level = None

        # turn on foreign keys
        _ = self._database.execute("PRAGMA foreign_keys = 1")

    def close(self) -> None:
        """
        Close the database file.
        """
        self._database.close()

    def create_database(self) -> None:
        """Create all the needed tables in an empty database"""
        _ = self._database.execute("BEGIN")

        # -- Targets table
        # Lists targets and their attibutes
        _ = self._database.execute(
            "CREATE TABLE Targets("
            "name TEXT PRIMARY KEY, "  # Name of the target
            "isPublic INTEGER, "  # 1 if the target is a public target (a board that can be built for).
            # 0 if not (the target is just used as a parent for other targets).
            "isMCUFamily INTEGER, "  # 1 if this is the top-level target for one family of MCUs and boards.
            # (for example, STM32L4 is the family target for all STM32L4 MCUs and boards).
            # Family targets are used to group together documentation and test results.
            # 0 otherwise (target is above or below the family level).
            "mcuVendorName TEXT, "  # Name of the vendor which the CPU on this target comes from.  From the CMSIS
            # database.  NULL if this target does not have a valid link to the CMSIS database.
            "mcuPartNumber TEXT NULL, "  # Part number of the MCU.  This is copied verbatim from the 'device_name'
            # property in target.json5.  May be NULL if there is no part number.
            "mcuFamilyTarget TEXT NOT NULL, "  # Name of the MCU family target which is a parent of this target.
            # If this target isn't part of an MCU family, this will contain NO_MCU_TARGET_FAMILY.
            # If isMCUFamily = 1, this will contain the target's own name.
            "imageURL TEXT NULL  "  # URL of the image that will be shown in the target table for this board, if set
            # in JSON.
            ")"
        )

        # -- Tests table
        # Holds details about tests for each target
        _ = self._database.execute(
            "CREATE TABLE Tests("
            "testName TEXT, "  # Name of the test
            "targetName TEXT REFERENCES Targets(name), "  # Name of the target it was ran for
            "executionTime REAL, "  # Time in seconds it took to run the test
            "result INTEGER,"  # TestResult of the test
            "output TEXT,"  # Output that the complete test printed (not divided into test cases)
            "UNIQUE(testName, targetName)"  # Combo of test name - target name must be unique
            ")"
        )

        # -- TestCases table
        # Holds the result of each test case for each target
        _ = self._database.execute(
            "CREATE TABLE TestCases("
            "testName TEXT NOT NULL, "  # Name of the test
            "testCaseName TEXT NOT NULL, "  # Name of the test case
            "testCaseIndex INTEGER NOT NULL, "  # 0-indexed order that this test case ran in
            "targetName TEXT NOT NULL REFERENCES Targets(name), "  # Name of the target it was ran for
            "result INTEGER NOT NULL,"  # TestResult of the test
            "output TEXT NOT NULL,"  # Output that this test case printed specifically, or empty string for skipped tests
            "FOREIGN KEY(testName, targetName) REFERENCES Tests(testName, targetName), "
            "UNIQUE(testName, testCaseName, targetName)"  # Combo of test name - test case name - target name must be unique
            ")"
        )

        # -- Drivers table
        # Lists target features
        _ = self._database.execute(
            "CREATE TABLE Drivers("
            "name TEXT PRIMARY KEY, "  # Driver name, matching how it's named in code.  This is a string like
            # DEVICE_SERIAL, FEATURE_BLE, or COMPONENT_SPIF
            "friendlyName TEXT, "  # Human readable name, like FEATURE_BLE would have "Bluetooth Low Energy"
            "description TEXT, "  # Description, if available
            "type TEXT, "  # Type, value of DriverType
            "hidden INTEGER"  # 1 if the feature is an internal one and should not be shown in docs, 0 otherwise
            ")"
        )

        # -- TargetGraph table
        # Contains target parent/child relationships
        _ = self._database.execute(
            "CREATE TABLE TargetGraph("
            "parentTarget TEXT REFERENCES Targets(name), "  # Name of the parent target
            "childTarget TEXT REFERENCES Targets(name), "  # Name of the child target
            "UNIQUE(parentTarget, childTarget)"
            ")"
        )

        # -- TargetDrivers table
        # Maps targets to the features they support
        _ = self._database.execute(
            "CREATE TABLE TargetDrivers("
            "targetName TEXT REFERENCES Targets(name), "  # Name of the target
            "driver TEXT REFERENCES Drivers(name),"  # Name of driver or component.
            "UNIQUE(targetName, driver)"  # Combo of target name - driver name must be unique
            ")"
        )

        # -- TargetMemories table
        # Stores information about the memory banks available on a target
        _ = self._database.execute(
            "CREATE TABLE TargetMemories("
            "targetName TEXT REFERENCES Targets(name), "  # Name of the target
            "bankName TEXT NOT NULL, "  # Name of memory.  Directly from the CMSIS json.
            "size INTEGER NOT NULL, "  # Size in bytes
            "isFlash INTEGER NOT NULL, "  # 1 if flash memory, 0 if RAM.  This comes from the "writeable"
            # attribute from the CMSIS JSON.
            "UNIQUE(targetName, bankName)"  # Combo of target name - bank name must be unique
            ")"
        )

        # -- TestDrivers table
        # Maps test cases to the Mbed drivers / components they test
        _ = self._database.execute(
            "CREATE TABLE DriverTests("
            "testName TEXT, "  # Name of the test
            "driverTested TEXT,"  # Name of driver or component that the test tests.
            "FOREIGN KEY(driverTested) REFERENCES Drivers(name)"
            ")"
        )

        # now commit the initial transaction
        self._database.commit()

    def add_target(
        self,
        target_name: str,
        *,
        is_public: bool = True,
        is_mcu_family: bool = False,
        mcu_family_target: str = NO_MCU_TARGET_FAMILY,
    ) -> None:
        """
        Manually add a target to the database.

        This is useful for tests that want to create a database with just one target.
        """
        _ = self._database.execute(
            "INSERT INTO Targets(name, isPublic, isMCUFamily, mcuFamilyTarget) VALUES(?, ?, ?, ?)",
            (
                target_name,
                1 if is_public else 0,
                1 if is_mcu_family else 0,
                mcu_family_target,
            ),
        )

    def populate_targets_and_drivers(self, mbed_os_path: pathlib.Path) -> None:  # noqa: C901, PLR0912
        """
        Populate Targets, Drivers, and related tables from a given Mbed OS path.

        CMSIS cache is used to get attributes like RAM sizes from CMSIS.
        """
        # Create a new "MbedProgram" object which can tell us where the important JSON files are
        mbed_program = MbedProgram.from_existing(
            mbed_os_path,
            mbed_os_path / "build",  # build dir arbitrary, not used
            mbed_os_path,
        )

        targets_data = load_raw_targets_data(mbed_program)

        drivers_json5_file = mbed_os_path / "targets" / "drivers.json5"
        drivers_data: dict[str, Any] = decode_json_file(drivers_json5_file)

        cmsis_mcu_description_data: dict[str, Any] = decode_json_file(
            mbed_program.mbed_os.cmsis_mcu_descriptions_json_file
        )

        # First assemble a list of all the drivers.
        # For this we want to process the JSON directly rather than dealing with target inheritance, because
        # we just want a list of all the drivers used everywhere.
        component_names: set[str] = set()
        feature_names: set[str] = set()
        peripheral_names: set[str] = set()

        for target_data in targets_data.values():
            # Note: The names are built matching the logic in mbed_tools/build/_internal/templates/mbed_config.tmpl
            # Also note that top level targets will define e.g. 'components' while child targets will define
            # e.g. 'components_add', so we have to check both attributes.
            peripheral_names.update("DEVICE_" + entry for entry in target_data.device_has)
            peripheral_names.update("DEVICE_" + entry for entry in target_data.device_has_add)
            feature_names.update("FEATURE_" + entry for entry in target_data.features)
            feature_names.update("FEATURE_" + entry for entry in target_data.features_add)
            component_names.update("COMPONENT_" + entry for entry in target_data.components)
            component_names.update("COMPONENT_" + entry for entry in target_data.components_add)

        # First add the targets
        # Note that we don't need to use get_target_attributes() here because none of the attributes we need
        # are inherited
        for target_name, target_data in targets_data.items():
            self.add_target(
                target_name,
                is_public=target_data.public,
                is_mcu_family=target_data.is_mcu_family_target,
                mcu_family_target=NO_MCU_TARGET_FAMILY,  # Default to no family unless it's set to one later
            )

            # Also add the parents for each target
            for parent in target_data.inherits:
                try:
                    _ = self._database.execute(
                        "INSERT INTO TargetGraph(parentTarget, childTarget) VALUES(?, ?)", (parent, target_name)
                    )
                except sqlite3.IntegrityError:
                    print(
                        f"Warning: Failed to add parent relationship from target {target_name} to parent '{parent}'. Perhaps there is an invalid \"inherits\" attribute in the targets JSON?"
                    )

        # Now add the drivers to the database
        _ = self._database.execute("BEGIN")
        types_and_drivers = (
            (DriverType.PERIPHERAL, peripheral_names),
            (DriverType.FEATURE, feature_names),
            (DriverType.COMPONENT, component_names),
        )
        for driver_type, driver_names in types_and_drivers:
            for driver_name in driver_names:
                # Look up driver friendly name, description, etc in json file
                if driver_name not in drivers_data[driver_type.value]:
                    message = f"drivers.json5 section '{driver_type.value}' is missing information on {driver_name}!"
                    raise RuntimeError(message)

                if "friendly_name" not in drivers_data[driver_type.value][driver_name]:
                    message = f"drivers.json5 section {driver_type.value}.{driver_name} is missing 'friendly_name'!"
                    raise RuntimeError(message)
                if "description" not in drivers_data[driver_type.value][driver_name]:
                    message = f"drivers.json5 section {driver_type.value}.{driver_name} is missing 'description'!"
                    raise RuntimeError(message)

                hidden = 0
                if "hidden_from_docs" in drivers_data[driver_type.value][driver_name]:
                    hidden = 1 if drivers_data[driver_type.value][driver_name]["hidden_from_docs"] else 0

                _ = self._database.execute(
                    "INSERT INTO Drivers(name, friendlyName, description, type, hidden) VALUES(?, ?, ?, ?, ?)",
                    (
                        driver_name,
                        drivers_data[driver_type.value][driver_name]["friendly_name"],
                        drivers_data[driver_type.value][driver_name]["description"],
                        driver_type.value,
                        hidden,
                    ),
                )

        for target_name in targets_data:
            # Next, add the drivers for each target
            target_attrs = get_target_attributes(targets_data, target_name, True)

            for feature_name in target_attrs["features"]:
                feature_full_name = "FEATURE_" + feature_name

                try:
                    _ = self._database.execute(
                        "INSERT INTO TargetDrivers(targetName, driver) VALUES(?, ?)", (target_name, feature_full_name)
                    )
                except sqlite3.IntegrityError as ex:
                    message = f"Failed to add usage of feature {feature_full_name} to target {target_name}. Maybe this feature is missing from drivers.json?"
                    raise RuntimeError(message) from ex

            for component_name in target_attrs["components"]:
                component_full_name = "COMPONENT_" + component_name
                _ = self._database.execute(
                    "INSERT INTO TargetDrivers(targetName, driver) VALUES(?, ?)", (target_name, component_full_name)
                )

            # Note: device_has can contain duplicates, so we have to wrap it in set()
            for peripheral_name in set(target_attrs.get("device_has", [])):
                peripheral_full_name = "DEVICE_" + peripheral_name
                _ = self._database.execute(
                    "INSERT INTO TargetDrivers(targetName, driver) VALUES(?, ?)", (target_name, peripheral_full_name)
                )

            # Set image URL into database
            image_url = target_attrs.get("image_url", None)
            if image_url is not None:
                _ = self._database.execute("UPDATE Targets SET imageURL = ? WHERE name == ?", (image_url, target_name))

            cmsis_mcu_part_number: str | None = target_attrs.get("device_name", None)

            if cmsis_mcu_part_number is not None:
                # Set MCU part number in the database
                _ = self._database.execute(
                    "UPDATE Targets SET mcuPartNumber = ? WHERE name == ?", (cmsis_mcu_part_number, target_name)
                )

                if cmsis_mcu_part_number not in cmsis_mcu_description_data:
                    message = (
                        f"Target {target_name} specifies CMSIS MCU part number {cmsis_mcu_part_number} which "
                        f"does not exist in CMSIS pack index. Error in 'device_name' targets.json5 "
                        f"attribute?"
                    )

                    raise RuntimeError(message)

                # Set vendor name in the database.
                cmsis_cpu_data = cmsis_mcu_description_data[cmsis_mcu_part_number]
                cpu_vendor_name = cmsis_cpu_data["vendor"]

                # Set vendor name in the database.
                # In the JSON file the vendor name has a colon then a number after it.  I think this is
                # some sort of vendor ID but can't find actual docs.
                cpu_vendor_name = cpu_vendor_name.split(":")[0]
                _ = self._database.execute(
                    "UPDATE Targets SET mcuVendorName = ? WHERE name == ?", (cpu_vendor_name, target_name)
                )

            # Compute memory banks
            memory_banks_json = target_attrs.get("memory_banks", {})
            if cmsis_mcu_part_number is not None:
                memory_banks_json = memory_banks.incorporate_memory_bank_data_from_cmsis_preparsed(
                    cmsis_mcu_part_number, cmsis_mcu_description_data, memory_banks_json
                )

            for bank_name, bank_data in memory_banks_json.items():
                _ = self._database.execute(
                    "INSERT INTO TargetMemories(targetName, bankName, size, isFlash) VALUES(?, ?, ?, ?)",
                    (target_name, bank_name, bank_data["size"], 0 if bank_data["access"]["write"] else 1),
                )

        # Match targets with their MCU family targets.
        for mcu_family_target in self.get_mcu_family_targets():
            mcu_family_targets = self.get_all_target_children(mcu_family_target)
            mcu_family_targets.add(mcu_family_target)
            for target in mcu_family_targets:
                _ = self._database.execute(
                    "UPDATE Targets SET mcuFamilyTarget = ? WHERE name = ?", (mcu_family_target, target)
                )

        self._database.commit()

    @dataclasses.dataclass(eq=True, frozen=True)  # note: eq and frozen must be enabled to make the dataclass hashable
    class TargetDriverInfo:
        """
        Convenience type for returning information about a given target driver
        """

        name: str
        friendly_name: str
        description: str
        type: DriverType

    def get_all_drivers(self, driver_type: DriverType | None = None) -> set[TargetDriverInfo]:
        """
        Get all target drivers, optionally filtering by type
        """
        cursor = self._database.execute(
            f"""
SELECT
    name,
    friendlyName,
    description,
    type
FROM
    Drivers
    INNER JOIN TargetDrivers ON Drivers.name = TargetDrivers.driver
WHERE
    Drivers.hidden == 0
    {"AND Drivers.type == ?" if driver_type is not None else ""}
GROUP BY Drivers.name
ORDER BY friendlyName ASC""",  # noqa: S608
            (driver_type.value,) if driver_type is not None else (),
        )

        result = set()
        for row in cursor:
            result.add(
                MbedTestDatabase.TargetDriverInfo(
                    row["name"], row["friendlyName"], row["description"], DriverType(row["type"])
                )
            )
        cursor.close()

        return result

    def get_target_drivers(self, target_name: str) -> set[TargetDriverInfo]:
        """
        Get a set of the drivers of all types that the given target has
        """
        cursor = self._database.execute(
            """
SELECT
    name,
    friendlyName,
    description,
    type
FROM
    Drivers
    INNER JOIN TargetDrivers ON Drivers.name = TargetDrivers.driver
WHERE
    Drivers.hidden == 0
    AND TargetDrivers.targetName == ?
""",
            (target_name,),
        )

        result = set()
        for row in cursor:
            result.add(
                MbedTestDatabase.TargetDriverInfo(
                    row["name"], row["friendlyName"], row["description"], DriverType(row["type"])
                )
            )
        cursor.close()

        return result

    def get_mcu_family_targets(self) -> list[str]:
        """
        Get all the targets that are MCU family targets and should have webpages generated for them.

        Ordering goes by vendor first, then alphabetically by name.
        """
        mcu_family_targets = []

        # Note: in the below query, we use a subquery to find any target in the given MCU family with a non-null mcuVendorName.
        # This gets the vendor name for ordering.
        # We can't query it directly because family targets usually don't have specific device_names associated with them
        # in targets.json5, so they won't have a vendor name set.
        cursor = self._database.execute("""
SELECT
    OuterTargets.name AS name,
	(SELECT max(InnerTargets.mcuVendorName) FROM Targets AS InnerTargets WHERE InnerTargets.mcuFamilyTarget == OuterTargets.name) AS aggregateVendorName
FROM Targets AS OuterTargets
WHERE isMCUFamily == 1
ORDER BY
	aggregateVendorName ASC,
	name ASC
""")

        for row in cursor:
            mcu_family_targets.append(row["name"])

        cursor.close()

        return mcu_family_targets

    def _get_target_parents(self, target_name: str) -> list[str]:
        """
        Get the parent(s) of a target
        """
        parents = []

        cursor = self._database.execute(
            """
SELECT parentTarget
FROM TargetGraph
WHERE childTarget == ?
ORDER BY parentTarget ASC
""",
            (target_name,),
        )

        for row in cursor:
            parents.append(row["parentTarget"])

        cursor.close()

        return parents

    def _get_target_children(self, target_name: str) -> list[str]:
        """
        Get the children of a target
        """
        parents = []

        cursor = self._database.execute(
            """
SELECT childTarget
FROM TargetGraph
WHERE parentTarget == ?
ORDER BY childTarget ASC
""",
            (target_name,),
        )

        for row in cursor:
            parents.append(row["childTarget"])

        cursor.close()

        return parents

    def get_inheritance_graph(self, target_name: str) -> graphviz.Digraph:
        """
        Get the inheritance graph for a target, showing its parents and children.
        """
        # Create the graph with a nice blue color for all nodes.
        # The "rankdir=BT" attr puts the parent node at the top.
        inheritance_graph = graphviz.Digraph(
            comment=f"Inheritance graph for {target_name}",
            node_attr={"fillcolor": "lightblue", "style": "filled"},
            graph_attr={"rankdir": "BT"},
        )

        # Give the MCU family target some special styling
        inheritance_graph.node(target_name, label=f"<<B>{target_name}</B>>", _attributes={"shape": "box"})

        # Note: There is probably some 400 IQ way to do this in one SQLite query, but I
        # think we can leave that to figure out later.  For now we just do BFS upwards and downwards.
        nodes_to_explore = collections.deque()
        nodes_to_explore.append(target_name)

        # BFS parent targets
        while len(nodes_to_explore) > 0:
            curr_target = nodes_to_explore.pop()
            parents = self._get_target_parents(curr_target)

            for parent_target in parents:
                inheritance_graph.node(parent_target)
                inheritance_graph.edge(curr_target, parent_target)
                nodes_to_explore.append(parent_target)

        # BFS child targets
        nodes_to_explore = collections.deque()
        nodes_to_explore.append(target_name)

        while len(nodes_to_explore) > 0:
            curr_target = nodes_to_explore.pop()
            children = self._get_target_children(curr_target)

            for child_target in children:
                inheritance_graph.node(child_target)
                inheritance_graph.edge(child_target, curr_target)
                nodes_to_explore.append(child_target)

        return inheritance_graph

    def get_all_target_children(self, target_name: str) -> set[str]:
        """
        Get all targets which inherit from the given target anywhere in the inheritance hierarchy
        """
        all_children: set[str] = set()

        # BFS child targets
        nodes_to_explore = collections.deque()
        nodes_to_explore.append(target_name)

        while len(nodes_to_explore) > 0:
            curr_target = nodes_to_explore.pop()

            this_target_children = self._get_target_children(curr_target)
            all_children.update(this_target_children)

            for child_target in this_target_children:
                nodes_to_explore.append(child_target)

        return all_children

    def get_all_boards_in_mcu_family(self, mcu_family_name: str) -> sqlite3.Cursor:
        """
        Get all boards (public targets) which are in an MCU family.

        Returns a cursor containing the name, image, and the CPU vendor name
        """
        return self._database.execute(
            "SELECT name, imageURL, mcuVendorName, mcuPartNumber "
            "FROM Targets "
            "WHERE "
            "isPublic == 1 AND "
            "mcuFamilyTarget == ?",
            (mcu_family_name,),
        )

    def get_target_memories(self, target_name: str) -> sqlite3.Cursor:
        """
        Get all memory banks for a target.

        Returns a cursor containing the name and the CPU vendor name
        """
        return self._database.execute(
            "SELECT bankName, size, isFlash FROM TargetMemories WHERE targetName == ?ORDER BY bankName ASC",
            (target_name,),
        )

    def get_targets_with_driver_by_family(self, driver_name: str) -> sqlite3.Cursor:
        """
        Get all the targets that have a given driver, grouped by MCU family.

        Returns a cursor containing the MCU family and grouped targets
        """
        # Note: In the query below, we use max(mcuVendorName) to select any non-null value for
        # mcuVendorName.

        return self._database.execute(
            """
        SELECT
            group_concat(TargetDrivers.targetName, ',') AS targetNames,
            Targets.mcuFamilyTarget AS mcuFamilyTarget,
            max(Targets.mcuVendorName) AS mcuVendorName
        FROM
            TargetDrivers
            INNER JOIN Targets ON TargetDrivers.targetName = Targets.name
        WHERE
            TargetDrivers.driver == ?
            AND Targets.isPublic == 1
        GROUP BY Targets.mcuFamilyTarget
        ORDER BY
            max(Targets.mcuVendorName) ASC,
            Targets.mcuFamilyTarget ASC
            """,
            (driver_name,),
        )

    def add_test_record(
        self, test_name: str, target_name: str, execution_time: float, result: TestResult, output: str
    ) -> None:
        """
        Add or update a record of a test to the Tests table.

        Replaces the record if it already exists
        """
        _ = self._database.execute(
            "INSERT OR REPLACE INTO Tests(testName, targetName, executionTime, result, output) VALUES(?, ?, ?, ?, ?)",
            (test_name, target_name, execution_time, result.value, output),
        )

    def add_test_case_record(
        self,
        test_name: str,
        test_case_name: str,
        test_case_index: int,
        target_name: str,
        result: TestResult,
        output: str,
    ) -> None:
        """
        Add or update a record of a test to the TestCases table.

        Replaces the record if it already exists
        """
        _ = self._database.execute(
            "INSERT OR REPLACE INTO TestCases(testName, testCaseName, testCaseIndex, targetName, result, output) "
            "VALUES(?, ?, ?, ?, ?, ?)",
            (test_name, test_case_name, test_case_index, target_name, result.value, output),
        )

    def get_targets_with_tests(self) -> list[tuple[str, str]]:
        """
        Get a cursor containing the target names for which we have test records available.

        Also returns their target families.
        """
        cursor = self._database.execute("""
SELECT DISTINCT targetName, mcuFamilyTarget
FROM
    Tests
    INNER JOIN Targets ON Tests.targetName == Targets.name
ORDER BY targetName ASC
""")
        targets_with_tests = [(row["targetName"], row["mcuFamilyTarget"]) for row in cursor]
        cursor.close()
        return targets_with_tests

    def get_tests(self) -> list[str]:
        """
        Get a list of all tests that have data for any target.
        """
        cursor = self._database.execute("""
SELECT DISTINCT testName
FROM
    Tests
ORDER BY testName ASC
        """)
        tests = [row["testName"] for row in cursor]
        cursor.close()
        return tests

    def get_targets_with_test(self, test_name: str) -> sqlite3.Cursor:
        """
        Get a cursor containing the target names for which we have test records available for a given test.

        Also returns their target families.
        """
        return self._database.execute(
            """
SELECT DISTINCT targetName, mcuFamilyTarget
FROM
    Tests
    INNER JOIN Targets ON Tests.targetName == Targets.name
WHERE
    testName = ?
ORDER BY targetName ASC
""",
            (test_name,),
        )

    def get_test_results(self) -> dict[str, dict[str, TestResult]]:
        """
        Get the results of all tests for all targets.

        Returns ``{test name: {target name: TestResult}}``.
        """
        cursor = self._database.execute("""
SELECT
    testName,
    group_concat(targetName, ",") AS targets,
    group_concat(result, ",") AS results
FROM Tests
GROUP BY testName
ORDER BY testName ASC
""")

        all_test_results: dict[str, dict[str, TestResult]] = {}

        for row in cursor:
            this_test_results: dict[str, TestResult] = {}

            # Split up the targets and results into individual elements
            target_names = row["targets"].split(",")
            results = row["results"].split(",")

            # Store data in dict
            for target_idx, target in enumerate(target_names):
                this_test_results[target] = TestResult(int(results[target_idx]))

            all_test_results[row["testName"]] = this_test_results

        cursor.close()
        return all_test_results

    def get_test_details(self, test_name: str) -> dict[str, dict[str, TestResult]]:
        """
        Get the results of all cases for the given test for all targets.

        Returns ``{test case name: {target name: TestResult}}``.
        Test cases will be ordered by increasing index.
        """
        # Note: Ordering the test cases is slightly complicated because not every target might run every test case.
        # e.g. if an #ifdef blocks a target from executing a test case on target Y, test cases after that one will
        # be at a lower index for Y than other targets.  We fix this by ordering by the maximum test case index that any
        # target saw from running the test.
        cursor = self._database.execute(
            """
SELECT
    testCaseName,
    group_concat(targetName, ",") AS targets,
    group_concat(result, ",") AS results
FROM TestCases
WHERE testName = ?
GROUP BY testCaseName
ORDER BY max(testCaseIndex) ASC
""",
            (test_name,),
        )

        all_test_case_results: dict[str, dict[str, TestResult]] = {}

        for row in cursor:
            this_test_case_results: dict[str, TestResult] = {}

            # Split up the targets and results into individual elements
            target_names = row["targets"].split(",")
            results = row["results"].split(",")

            # Store data in dict
            for target_idx, target in enumerate(target_names):
                this_test_case_results[target] = TestResult(int(results[target_idx]))

            all_test_case_results[row["testCaseName"]] = this_test_case_results

        cursor.close()
        return all_test_case_results

    def get_test_case_run_output(self, test_name: str, test_case_name: str, target_name: str) -> str:
        """
        Get the output from running a test case on a target.
        """
        cursor = self._database.execute(
            """
SELECT output
FROM TestCases
WHERE
    testName = ?
    AND testCaseName = ?
    AND targetName = ?
""",
            (test_name, test_case_name, target_name),
        )
        output = next(cursor)["output"]
        cursor.close()
        return output
