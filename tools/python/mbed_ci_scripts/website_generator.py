"""
Generates an HTML website with various "dynamic" data about the current Mbed version.

This includes:
- The hierarchy of targets
- All existing drivers/features and what they do
- Matrix of test results for each target.

Copyright (c) 2026 Jamie Smith
SPDX-License-Identifier: Apache-2.0
"""

import binascii
import collections
import html
import pathlib
from typing import TextIO

import prettytable

from .mbed_test_database import NO_MCU_TARGET_FAMILY, DriverType, MbedTestDatabase, TestResult

# Allow usage of `write()` and `write_text()` without caring about the return type
# pyright: reportUnusedCallResult=false


def write_global_stylesheet(gen_path: pathlib.Path) -> None:
    """
    Write out the global stylesheet to a location
    """
    gen_path.write_text("""
body {
    margin-left: 30px;
    margin-right: 30px;
    margin-top: 30px;
    overflow: scroll;
}
/* Styles for test result tables */
div.passed-marker {
    width: 100%;
    height: 100%;
    background-color: lightgreen
}
div.skipped-marker {
    width: 100%;
    height: 100%;
    background-color: lightgray
}
div.failed-marker {
    width: 100%;
    height: 100%;
    background-color: lightpink
}
div.prior-crashed-marker {
    width: 100%;
    height: 100%;
    background-color: burlywood
}
.test_result_table>tbody>tr>td {
    padding: 0 !important;
}
""")


def get_test_case_run_path(test_name: str, test_case_name: str, target_name: str) -> pathlib.Path:
    """
    Get the (relative) path for a test case run's HTML file within the tests dir
    """
    # Convert test case name (which could be any string) into a short filesystem-safe string by CRCing it.
    # Note that this used to encode the case name as base64, but that was actually running into path length
    # limits on some systems.
    test_case_name_crc = binascii.crc32(test_case_name.encode("UTF-8"))
    return pathlib.Path("runs") / target_name / f"{test_name}-case-{test_case_name_crc}.html"


def write_html_header(output_file: TextIO, page_title: str, levels_deep: int = 1) -> None:
    """
    Write the common HTML header to a file.  Includes Semantic CSS and applies the given title.

    :param levels_deep: How many levels deep from the root folder of the site this page is
    """
    up_to_root_path = "../" * levels_deep
    output_file.write(f"""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <title>{page_title}</title>
    <script
      src="https://code.jquery.com/jquery-3.1.1.min.js"
      integrity="sha256-hVVnYaiADRTO2PzUGmuLJr8BLUSjGIZsDYGmIJLv2b8="
      crossorigin="anonymous"></script>
    <script src=" https://cdn.jsdelivr.net/npm/semantic-ui@2.5.0/dist/semantic.min.js "></script>
    <link href=" https://cdn.jsdelivr.net/npm/semantic-ui@2.5.0/dist/semantic.min.css " rel="stylesheet">
    <link rel="stylesheet" href="{up_to_root_path}mbed-results-site.css">
</head>
<body>
    <h1>{page_title}</h1>""")


def generate_drivers_index_page(database: MbedTestDatabase, out_path: pathlib.Path) -> None:
    """
    Generate the driver index page.  This page contains tables with summaries of the features and components.

    :param database: Database to use
    :param out_path: Path to write out page to
    """
    with out_path.open("w", encoding="utf8") as index_page:
        write_html_header(index_page, "Mbed Drivers Index")

        for driver_type in DriverType:
            index_page.write(f"<h2>Drivers of type {driver_type.value}</h2>\n")

            if driver_type == DriverType.PERIPHERAL:
                index_page.write(
                    "<p>This lists peripheral drivers (drivers for internal parts of the MCU) that exist"
                    " in Mbed CE.  Check these pages carefully: If a MCU's datasheet shows that it "
                    " has a peripheral, but "
                    " Mbed does not list a driver for that peripheral, then the peripheral will not"
                    " be accessible via Mbed APIs on that MCU.</p>\n"
                )
            elif driver_type == DriverType.COMPONENT:
                index_page.write(
                    "<p>This lists drivers which support external components (or in some cases "
                    "co-processors) which are present on a target board so that they are supported"
                    " out of the box.</p>\n"
                )
            elif driver_type == DriverType.FEATURE:
                index_page.write("<p>Larger Mbed OS optional features supported for certain targets.</p>\n")

            driver_table = prettytable.PrettyTable()
            driver_table.field_names = ["Name", "Mbed Internal Name", "Description"]

            drivers_of_type = database.get_all_drivers(driver_type)
            for driver in drivers_of_type:
                driver_table.add_row(
                    [
                        f'<a href="{driver.name}.html">{driver.friendly_name}</a>',
                        f"<code>{driver.name}</code>",
                        driver.description,
                    ]
                )

            index_page.write(html.unescape(driver_table.get_html_string(attributes={"class": "ui celled table"})))

        index_page.write("\n</body>")


def generate_driver_page(
    database: MbedTestDatabase, driver_info: MbedTestDatabase.TargetDriverInfo, out_path: pathlib.Path
) -> None:
    """
    Generate a webpage for a feature.

    This includes information about the feature like which targets have it.
    """
    with out_path.open("w", encoding="utf8") as target_page:
        write_html_header(target_page, f"Mbed CE Driver Info: {driver_info.friendly_name}")
        target_page.write('<p><a href="index.html">Back to Drivers Index ^</a></p>\n')
        target_page.write(f"""
<p><strong>Mbed Internal Name:</strong> <code>{driver_info.name}</code></p>
<p><strong>Description:</strong> {driver_info.description}</p>
""")

        target_page.write("<h2>Target Families With this Feature</h2>")
        target_table = prettytable.PrettyTable()
        target_table.field_names = ["Target Family", "MCU Vendor", "Board Targets With Feature"]

        target_families_cursor = database.get_targets_with_driver_by_family(driver_info.name)
        for row in target_families_cursor:
            mcu_family_target = row["mcuFamilyTarget"]
            target_family_link = f'<a href="../targets/{mcu_family_target}.html">{mcu_family_target}</a>'
            all_targets_in_family = row["targetNames"].split(",")
            cpu_vendor_name = row["mcuVendorName"]

            # Special handling for boards with no MCU target family (avoid linking to None.html)
            if mcu_family_target is None:
                target_family_link = "&lt;None&gt;"
                cpu_vendor_name = ""

            target_table.add_row([target_family_link, cpu_vendor_name, ", ".join(sorted(all_targets_in_family))])

        # Write the table to the page.
        # Note: html.unescape() prevents HTML in the cells from being escaped in the page (which prettytable
        # seems to do)
        target_page.write(html.unescape(target_table.get_html_string(attributes={"class": "ui celled table"})))

        target_page.write("\n</body>")


def generate_targets_index_page(
    database: MbedTestDatabase, mcu_family_targets: list[str], out_path: pathlib.Path
) -> None:
    """
    Generates the index page for the targets folder.

    This includes a link to each MCU target family and other information about it.
    """
    with out_path.open("w", encoding="utf8") as targets_index:
        write_html_header(targets_index, "Mbed CE MCU Target Families")
        targets_index.write(
            "<p>Each row on this page represents one microcontroller family that Mbed supports."
            " Microcontroller family, here, refers to a group of Mbed targets that use chips from the"
            " same family and use the same low level drivers, with the differences only being memory "
            " sizes, specific chip part numbers, and the board that the microcontroller is mounted on"
            ".</p>\n"
        )

        target_table = prettytable.PrettyTable()
        target_table.field_names = ["Target Family", "MCU Vendor", "Board Targets", "Features", "Peripherals"]
        for mcu_family_target in mcu_family_targets:
            target_link = f'<a href="{mcu_family_target}.html">{mcu_family_target}</a>'

            mcu_vendor_name = ""
            boards_cursor = database.get_all_boards_in_mcu_family(mcu_family_target)
            boards_list: list[str] = []
            for row in boards_cursor:
                boards_list.append(row["name"])

                # Not every board may have the CPU vendor set (since the JSON doesn't require the 'device_name'
                # property to be set so the board may not get linked to CMSIS) so only store this field if it exists.
                # Also for NO_FAMILY we don't want to set the vendor name as there could be multiple.
                if row["mcuVendorName"] is not None and mcu_family_target != NO_MCU_TARGET_FAMILY:
                    mcu_vendor_name = row["mcuVendorName"]
            boards_cursor.close()

            target_features = database.get_target_drivers(mcu_family_target)
            features_list = []
            peripherals_list = []
            for feature in target_features:
                feature_string = f'<a href="../drivers/{feature.name}.html">{feature.friendly_name}</a>'

                # Features and peripherals each go in separate table entries.  We ignore COMPONENTs for now
                # because those are a property of the board not the MCU.
                if feature.type == DriverType.FEATURE:
                    features_list.append(feature_string)
                elif feature.type == DriverType.PERIPHERAL:
                    peripherals_list.append(feature_string)

            target_table.add_row(
                [
                    target_link,
                    mcu_vendor_name,
                    ", ".join(sorted(boards_list)),
                    ", ".join(sorted(features_list)),
                    ", ".join(sorted(peripherals_list)),
                ]
            )

        # Write the table to the page.
        # Note: html.unescape() prevents HTML in the cells from being escaped in the page (which prettytable
        # seems to do)
        targets_index.write(html.unescape(target_table.get_html_string(attributes={"class": "ui celled table"})))

        targets_index.write("\n</body>")


def generate_target_family_page(
    database: MbedTestDatabase,
    mcu_family_target: str,
    out_path: pathlib.Path,
) -> None:
    """
    Generate a webpage for a target family.

    This includes info like the target's features and the targets in the family.
    """
    with out_path.open("w", encoding="utf8") as target_page:
        write_html_header(target_page, mcu_family_target + " Target Family Info")
        target_page.write('<p><a href="index.html">Back to Target Families Index ^</a></p>')

        # Lookup features for each target.  Builds a dict from a target name to its features,
        # and a set containing the intersections of all targets' features
        targets_cursor = database.get_all_boards_in_mcu_family(mcu_family_target)
        targets_in_family_info = [(row["name"], row["imageURL"], row["mcuPartNumber"]) for row in targets_cursor]
        targets_cursor.close()

        target_features: dict[str, dict[DriverType, set[MbedTestDatabase.TargetDriverInfo]]] = {}
        for target_name, _, _ in targets_in_family_info:
            this_target_features = database.get_target_drivers(target_name)
            this_target_features_by_type = collections.defaultdict(set)
            for feature in this_target_features:
                this_target_features_by_type[feature.type].add(feature)
            target_features[target_name] = this_target_features_by_type

        common_features_by_type: dict[DriverType, set[MbedTestDatabase.TargetDriverInfo]] = {}
        for feature_type in DriverType:
            common_features_by_type[feature_type] = set.intersection(
                *[feature_dict[feature_type] for feature_dict in target_features.values()]
            )

        # Generate features list (with the features and peripherals)
        target_page.write("<h2>Features Supported</h2>\n<ul>")
        for feature in common_features_by_type[DriverType.FEATURE]:
            target_page.write(
                f'<li><a href="../drivers/{feature.name}.html">{feature.friendly_name}</a>: {feature.description}</li>\n'
            )
        target_page.write("</ul>\n<h2>Peripheral Drivers Supported</h2>\n<ul>")
        target_page.writelines(
            f'<li><a href="../drivers/{peripheral.name}.html">{peripheral.friendly_name}</a>: {peripheral.description}</li>\n'
            for peripheral in common_features_by_type[DriverType.PERIPHERAL]
        )
        target_page.write("</ul>\n")

        # Generate board table
        target_page.write("<h2>Boards in this Target Family</h2>")
        target_table = prettytable.PrettyTable()
        target_table.field_names = [
            "Board",
            "MCU Part Number",
            "Extra Features",
            "Extra Peripheral Drivers",
            "Components",
            "RAM Banks",
            "Flash Banks",
        ]
        for target_name, image_url, mcu_part_number in targets_in_family_info:
            # Format board image
            if image_url is None:
                board_image_html = ""
            else:
                board_image_html = (
                    f'<img src="{image_url}" alt="{target_name} Image" width="200px" style="display: block;">'
                )

            # Figure out any extra features that this target contains which the MCU family doesn't
            target_unique_features = (
                target_features[target_name][DriverType.FEATURE] - common_features_by_type[DriverType.FEATURE]
            )
            target_unique_peripherals = (
                target_features[target_name][DriverType.PERIPHERAL] - common_features_by_type[DriverType.PERIPHERAL]
            )

            # Make lists for each of the features
            target_unique_feature_strings = [
                f'<a href="../drivers/{feature.name}.html">{feature.friendly_name}</a>'
                for feature in target_unique_features
            ]
            target_unique_periph_strings = [
                f'<a href="../drivers/{feature.name}.html">{feature.friendly_name}</a>'
                for feature in target_unique_peripherals
            ]
            target_component_strings = [
                f'<a href="../drivers/{feature.name}.html">{feature.friendly_name}</a>'
                for feature in target_features[target_name][DriverType.COMPONENT]
            ]

            # Make lists for each of the memory banks
            ram_bank_string = "<ul>"
            rom_bank_string = "<ul>"

            memory_bank_cursor = database.get_target_memories(target_name)
            for row in memory_bank_cursor:
                bank_str = f"<li>{row['bankName']}: {row['size'] // 1024} kiB</li>"
                if row["isFlash"] == 1:
                    rom_bank_string += bank_str
                else:
                    ram_bank_string += bank_str

            ram_bank_string += "</ul>"
            rom_bank_string += "</ul>"

            mcu_part_number_str = "" if mcu_part_number is None else mcu_part_number

            target_table.add_row(
                [
                    target_name + board_image_html,
                    mcu_part_number_str,
                    ", ".join(target_unique_feature_strings),
                    ", ".join(target_unique_periph_strings),
                    ", ".join(target_component_strings),
                    ram_bank_string,
                    rom_bank_string,
                ]
            )
            memory_bank_cursor.close()

        # Write the table to the page.
        # Note: html.unescape() prevents HTML in the cells from being escaped in the page (which prettytable
        # seems to do)
        target_page.write(html.unescape(target_table.get_html_string(attributes={"class": "ui celled table"})))

        if mcu_family_target != NO_MCU_TARGET_FAMILY:
            # Generate inheritance graph
            target_page.write("<h2>Inheritance Graph</h2>")

            inheritance_graph = database.get_inheritance_graph(mcu_family_target)
            inheritance_graph_basename = out_path.parent / "assets" / f"{mcu_family_target}.dot"
            inheritance_graph_svg_file = pathlib.Path(
                inheritance_graph.render(inheritance_graph_basename, format="svg")
            )
            target_page.write(f'<img src="assets/{inheritance_graph_svg_file.name}" alt="Inheritance Graph"/>\n')

            target_page.write("\n</body>")


def generate_tests_index_page(database: MbedTestDatabase, out_path: pathlib.Path) -> None:
    """
    Generate the page with the index of all the tests and their status on each target
    """
    with out_path.open("w", encoding="utf8") as targets_index:
        write_html_header(targets_index, "All Test Results by Target")

        test_table = prettytable.PrettyTable()

        # Figure out list of targets that we have test data for
        target_table_header_text = ["Test Name"]

        targets_and_families_with_tests = database.get_targets_with_tests()
        for target_name, mcu_family_target in targets_and_families_with_tests:
            target_table_header_text.append(
                f'<div style="writing-mode: vertical-lr;"><a href="../targets/{mcu_family_target}.html">{target_name}</a></div>'
            )

        # Now fill in test results
        for test_name, target_test_results in database.get_test_results().items():
            row_content = [f'<a href="{test_name}.html">{test_name}</a>']

            for target, _ in targets_and_families_with_tests:
                if target in target_test_results:
                    if target_test_results[target] == TestResult.PASSED:
                        row_content.append('<div class="passed-marker">Passed</div>')
                    elif target_test_results[target] == TestResult.FAILED:
                        row_content.append('<div class="failed-marker">Failed</div>')
                    else:  # skipped
                        row_content.append('<div class="skipped-marker">Skipped</div>')
                else:
                    # Not run for this target, e.g. due to the test folder being
                    # excluded for this target by the build system
                    row_content.append('<div class="skipped-marker">Skipped</div>')
            test_table.add_row(row_content)

        # Write the table to the page.
        # Note: html.unescape() prevents HTML in the cells from being escaped in the page (which prettytable
        # seems to do)
        test_table.field_names = target_table_header_text
        targets_index.write(
            html.unescape(test_table.get_html_string(attributes={"class": "ui celled table test_result_table"}))
        )

        targets_index.write("\n</body>")


def generate_test_page(database: MbedTestDatabase, test_name: str, out_path: pathlib.Path) -> None:
    """
    Generate the page that shows each test case of a test and its results on each target
    """
    with out_path.open("w", encoding="utf8") as test_page:
        write_html_header(test_page, f"Results of {test_name}")

        test_page.write('<p><a href="index.html">Back to All Test Results ^</a></p>')

        test_table = prettytable.PrettyTable()

        # Figure out list of targets that we have test data for
        targets_with_test_data = []
        target_table_header_text = ["Test Case"]
        targets_cursor = database.get_targets_with_test(test_name)
        for row in targets_cursor:
            targets_with_test_data.append(row["targetName"])
            target_table_header_text.append(
                f'<div style="writing-mode: vertical-lr;"><a href="../targets/{row["mcuFamilyTarget"]}.html">{row["targetName"]}</a></div>'
            )
        targets_cursor.close()

        # Now fill in test results
        for test_case_name, target_test_results in database.get_test_details(test_name).items():
            row_content = [test_case_name]

            for target in targets_with_test_data:
                if target in target_test_results:
                    if target_test_results[target] == TestResult.PASSED:
                        row_content.append(
                            f'<div class="passed-marker"><a href="{get_test_case_run_path(test_name, test_case_name, target)!s}">Passed</a></div>'
                        )
                    elif target_test_results[target] == TestResult.FAILED:
                        row_content.append(
                            f'<div class="failed-marker"><a href="{get_test_case_run_path(test_name, test_case_name, target)!s}">Failed</a></div>'
                        )
                    elif target_test_results[target] == TestResult.PRIOR_TEST_CASE_CRASHED:
                        row_content.append('<div class="prior-crashed-marker">Prior Case Crashed</div>')
                    else:  # skipped
                        row_content.append(
                            f'<div class="skipped-marker"><a href="{get_test_case_run_path(test_name, test_case_name, target)!s}">Skipped</a></div>'
                        )
                else:
                    # Test case does not exist for this target, e.g. due to an ifdef
                    row_content.append('<div class="skipped-marker">Not Run</div>')
            test_table.add_row(row_content)

        # Write the table to the page.
        # Note: html.unescape() prevents HTML in the cells from being escaped in the page (which prettytable
        # seems to do)
        test_table.field_names = target_table_header_text
        test_page.write(
            html.unescape(test_table.get_html_string(attributes={"class": "ui celled table test_result_table"}))
        )

        test_page.write("\n</body>")


def generate_test_case_run_page(
    database: MbedTestDatabase, test_name: str, test_case_name: str, target_name: str, out_path: pathlib.Path
) -> None:
    """
    Generate a page that shows the result of running a test case on a target.
    """
    with out_path.open("w", encoding="utf8") as test_page:
        write_html_header(test_page, "Test Case Output", levels_deep=3)
        test_page.write(f'<p><a href="../../{test_name}.html">Back to {test_name} Results ^</a></p>')

        test_page.write(f"""
<p class="ui">
<b>Target:</b> {target_name}<br>
<b>Test:</b> {test_name}<br>
<b>Test Case:</b> {test_case_name}
</p>
""")

        test_page.write(
            f'<div class="ui raised segment"><pre><code class="code">{database.get_test_case_run_output(test_name, test_case_name, target_name)}</code></pre></div>'
        )

        test_page.write("\n</body>")


def generate_tests_and_targets_website(database: MbedTestDatabase, gen_path: pathlib.Path) -> None:
    """
    Generate a static website containing info about all the Mbed tests and targets.

    :param database: Database object to use
    :param gen_path: Path to generate the site at
    """
    gen_path.mkdir(exist_ok=True)

    # Generate CSS
    write_global_stylesheet(gen_path / "mbed-results-site.css")

    # Generate drivers subdirectory
    drivers_dir = gen_path / "drivers"
    drivers_dir.mkdir(exist_ok=True)
    generate_drivers_index_page(database, drivers_dir / "index.html")

    all_drivers = database.get_all_drivers()
    for driver in all_drivers:
        generate_driver_page(database, driver, drivers_dir / f"{driver.name}.html")

    # Generate targets subdirectory
    targets_dir = gen_path / "targets"
    targets_dir.mkdir(exist_ok=True)
    targets_assets_dir = targets_dir / "assets"
    targets_assets_dir.mkdir(exist_ok=True)

    mcu_family_targets = database.get_mcu_family_targets()
    mcu_family_targets.append(NO_MCU_TARGET_FAMILY)  # Also generate a page for the NO_FAMILY target

    generate_targets_index_page(database, mcu_family_targets, targets_dir / "index.html")
    for mcu_family_target in mcu_family_targets:
        generate_target_family_page(database, mcu_family_target, targets_dir / f"{mcu_family_target}.html")

    # Generate tests subdirectory
    tests_dir = gen_path / "tests"
    tests_dir.mkdir(exist_ok=True)
    generate_tests_index_page(database, tests_dir / "index.html")

    for test_name in database.get_tests():
        generate_test_page(database, test_name, tests_dir / f"{test_name}.html")
        test_details = database.get_test_details(test_name)

        for test_case_name in test_details:
            for target_name, result in test_details[test_case_name].items():
                if result != TestResult.PRIOR_TEST_CASE_CRASHED:
                    run_path = tests_dir / get_test_case_run_path(test_name, test_case_name, target_name)
                    run_path.parent.mkdir(exist_ok=True, parents=True)
                    generate_test_case_run_page(database, test_name, test_case_name, target_name, run_path)
