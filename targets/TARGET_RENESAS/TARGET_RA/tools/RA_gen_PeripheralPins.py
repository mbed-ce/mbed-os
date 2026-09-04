# Copyright (c) 2024 ARM Limited
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Renesas RA PeripheralPins.c generator.
#
# Parses a Renesas pin-configuration XML file (PinCfg*.xml, exported by the
# RA Smart Configurator) and generates the mbed PeripheralPins.c pin maps.
# The peripheral selection macro (IOPORT_PERIPHERAL_*) for each pin function
# is taken from the alt/registerSetting element in the XML when available;
# the hard-coded macros in the rules below are only used as a fallback.

import re
import os
import sys
import xml.etree.ElementTree as ET

# ============================================================
# Peripheral matching rules (compatible with both old and new
# XML capability naming formats)
# ============================================================
PERIPHERAL_RULES = [
    # --- ADC ---
    {
        "array_name": "PinMap_ADC",
        "pattern": r"ADC(\d+): AN(\d+)",
        "pin_mode": "RA_PIN_MODE_ANALOG",
        "pull": "RA_PIN_PULL_NONE",
        "peripheral_sel": 0,
        "channel_prefix": "ADC_CHANNEL_",
        "has_channel": True,
        "speed": None,
        "channel_transform": lambda x: int(x) % 100,
    },
    # --- DAC ---
    {
        "array_name": "PinMap_DAC",
        "pattern": r"DAC(\d+): DA",
        "periph_num_transform": lambda x: x % 120,  # DAC120 -> 0, DAC121 -> 1
        "pin_mode": "RA_PIN_MODE_ANALOG",
        "pull": "RA_PIN_PULL_NONE",
        "peripheral_sel": 0,
        "has_channel": False,
        "speed": None,
    },
    # --- PWM (AGT + GPT) ---
    {
        "array_name": "PinMap_PWM",
        "pattern_list": [
            {
                "pattern": r"AGT(\d+): AGTO([AB]?)\d*",
                "periph_prefix": "PWM_AGT",
                "periph_sel_macro": "IOPORT_PERIPHERAL_AGT",
                "channel_extract": lambda m: 1 if m.group(2) == 'B' else 0,
            },
            {
                "pattern": r"GPT(\d+): GTIOC\d*([AB])",
                "periph_prefix": "PWM_GPT",
                "periph_sel_macro": "IOPORT_PERIPHERAL_GPT1",
                "channel_extract": lambda m: 0 if m.group(2) == 'A' else 1,
            },
        ],
        "pin_mode": "RA_PIN_MODE_PERIPHERAL_PP",
        "pull": "RA_PIN_PULL_NONE",
        "has_channel": True,
        "speed": None,
    },
    # --- IRQ ---
    {
        "array_name": "PinMap_IRQ",
        # Compatible with "IRQ0: IRQ3-DS" (old format) and "IRQ: IRQ3"
        # (new format, no number before the colon)
        "pattern": r"IRQ\d*: IRQ(\d+)(?:-DS)?",
        "pin_mode": "RA_PIN_MODE_IRQ",
        "pull": "RA_PIN_PULL_NONE",
        "peripheral_sel": 0,
        "has_channel": False,
        "speed": None,
    },
    # --- UART RX/TX ---
    {
        "array_name": "PinMap_UART_RX",
        "pattern": r"SCI(\d+): RXD",
        "pin_mode": "RA_PIN_MODE_PERIPHERAL_PP",
        "pull": "RA_PIN_PULL_UP",
        "periph_sel_macro_template": "IOPORT_PERIPHERAL_SCI{}",
        "has_channel": False,
        "speed": None,
    },
    {
        "array_name": "PinMap_UART_TX",
        "pattern": r"SCI(\d+): TXD",
        "pin_mode": "RA_PIN_MODE_PERIPHERAL_PP",
        "pull": "RA_PIN_PULL_UP",
        "periph_sel_macro_template": "IOPORT_PERIPHERAL_SCI{}",
        "has_channel": False,
        "speed": "RA_PIN_SPEED_MID",
    },
    # --- SPI ---
    {
        "array_name": "PinMap_SPI_MOSI",
        "pattern": r"SPI(\d+): MOSI",
        "pin_mode": "RA_PIN_MODE_PERIPHERAL_PP",
        "pull": "RA_PIN_PULL_NONE",
        "periph_sel_macro": "IOPORT_PERIPHERAL_SPI",
        "has_channel": False,
        "speed": "RA_PIN_SPEED_HS_HIGH",
    },
    {
        "array_name": "PinMap_SPI_MISO",
        "pattern": r"SPI(\d+): MISO",
        "pin_mode": "RA_PIN_MODE_PERIPHERAL_PP",
        "pull": "RA_PIN_PULL_NONE",
        "periph_sel_macro": "IOPORT_PERIPHERAL_SPI",
        "has_channel": False,
        "speed": None,
    },
    {
        "array_name": "PinMap_SPI_SCLK",
        "pattern": r"SPI(\d+): RSPCK",
        "pin_mode": "RA_PIN_MODE_PERIPHERAL_PP",
        "pull": "RA_PIN_PULL_NONE",
        "periph_sel_macro": "IOPORT_PERIPHERAL_SPI",
        "has_channel": False,
        "speed": "RA_PIN_SPEED_HS_HIGH",
    },
    {
        "array_name": "PinMap_SPI_SSEL",
        "pattern": r"SPI(\d+): SSL",
        "pin_mode": "RA_PIN_MODE_PERIPHERAL_PP",
        "pull": "RA_PIN_PULL_NONE",
        "periph_sel_macro": "IOPORT_PERIPHERAL_SPI",
        "has_channel": False,
        "speed": "RA_PIN_SPEED_MID",
    },
    # --- I2C ---
    # Compatible with "IIC0: SCL" (old format) and "I3C/IIC: I3C_SCL" /
    # "I3C/IIC: SCL0" (new format; defaults to channel 0 when unnumbered)
    {
        "array_name": "PinMap_I2C_SCL",
        "pattern": r"(?:I3C/)?IIC(\d+)?: (?:I3C_)?SCL\d*",
        "periph_num_default": 0,
        "pin_mode": "RA_PIN_MODE_PERIPHERAL_OD",
        "pull": "RA_PIN_PULL_NONE",
        "periph_sel_macro": "IOPORT_PERIPHERAL_IIC",
        "has_channel": False,
        "speed": "RA_PIN_SPEED_MID_I2C",
    },
    {
        "array_name": "PinMap_I2C_SDA",
        "pattern": r"(?:I3C/)?IIC(\d+)?: (?:I3C_)?SDA\d*",
        "periph_num_default": 0,
        "pin_mode": "RA_PIN_MODE_PERIPHERAL_OD",
        "pull": "RA_PIN_PULL_NONE",
        "periph_sel_macro": "IOPORT_PERIPHERAL_IIC",
        "has_channel": False,
        "speed": "RA_PIN_SPEED_MID_I2C",
    },
    # --- CAN ---
    {
        "array_name": "PinMap_CAN_RD",
        "pattern": r"CAN(?:FD)?(\d+): CRX",
        "pin_mode": "RA_PIN_MODE_PERIPHERAL_PP",
        "pull": "RA_PIN_PULL_NONE",
        "periph_sel_macro": "IOPORT_PERIPHERAL_CAN",
        "has_channel": False,
        "speed": None,
    },
    {
        "array_name": "PinMap_CAN_TD",
        "pattern": r"CAN(?:FD)?(\d+): CTX",
        "pin_mode": "RA_PIN_MODE_PERIPHERAL_PP",
        "pull": "RA_PIN_PULL_NONE",
        "periph_sel_macro": "IOPORT_PERIPHERAL_CAN",
        "has_channel": False,
        "speed": "RA_PIN_SPEED_HS_HIGH",
    },
]

SCI_GROUP_EVEN = {0, 2, 4, 6, 8}
SCI_GROUP_ODD = {1, 3, 5, 7, 9}


def pin_name_to_mbed(pin_id: str) -> str:
    """p014 -> P0_14"""
    match = re.match(r"p(\d)(\d+)", pin_id, re.IGNORECASE)
    if match:
        port = match.group(1)
        pin_num = str(int(match.group(2)))
        return f"P{port}_{pin_num}"
    return pin_id.upper()


def get_sci_periph_macro(sci_num: int) -> str:
    if sci_num in SCI_GROUP_EVEN:
        return "IOPORT_PERIPHERAL_SCI0_2_4_6_8"
    return "IOPORT_PERIPHERAL_SCI1_3_5_7_9"


def _get_xml_namespace(root: ET.Element) -> str:
    if root.tag.startswith("{") and "}" in root.tag:
        return root.tag.split("}")[0].strip("{")
    return ""


def parse_xml_components(xml_content: str) -> dict:
    """Returns ({pin_id: caps}, {pin_id: {"periph.func": psel_macro}})"""
    root = ET.fromstring(xml_content)
    ns = _get_xml_namespace(root)

    def tag(name: str) -> str:
        return f"{{{ns}}}{name}" if ns else name

    result = {}
    alt_psel = {}
    for comp in root.iter(tag("component")):
        if comp.get("type") != "port":
            continue
        pin_id = comp.get("id")
        if not pin_id:
            continue

        caps_value = ""
        for prop in comp.iter(tag("property")):
            if "capabilitylist" in prop.get("id", ""):
                caps_value = prop.get("value", "")
                break
        if not caps_value:
            continue

        caps = [c.strip() for c in caps_value.splitlines() if c.strip()]
        result[pin_id] = caps

        # Collect the peripheral selection macro of every alternate function
        # from its registerSetting element, keyed by "<periph>.<func>".
        pin_alts = {}
        for alt in comp.iter(tag("alt")):
            alt_id = alt.get("id", "")
            if alt_id.startswith(pin_id + "."):
                for reg in alt.iter(tag("registerSetting")):
                    value = reg.get("value", "")
                    if value.startswith("IOPORT_PERIPHERAL_"):
                        pin_alts[alt_id[len(pin_id) + 1:].lower()] = value
                        break
        alt_psel[pin_id] = pin_alts

    return result, alt_psel


def capability_alt_key(cap: str) -> str:
    """'GPT5: GTIOC5B' -> 'gpt5.gtioc5b', 'I3C/IIC: I3C_SCL' -> 'i3c_fslash_iic.i3c_scl'"""
    parts = cap.split(":", 1)
    if len(parts) != 2:
        return ""
    periph = parts[0].strip().lower().replace("/", "_fslash_")
    func = parts[1].strip().lower()
    return f"{periph}.{func}"


def generate_pin_entry(pin_name: str, periph_name: str, rule: dict, extra: dict = None) -> str:
    extra = extra or {}
    pin_mode = rule["pin_mode"]
    pull = rule["pull"]
    periph_sel = extra.get("periph_sel", rule.get("peripheral_sel", 0))

    has_speed = rule.get("speed") is not None
    has_channel = rule.get("has_channel", False) and "channel" in extra

    if has_speed and has_channel:
        macro = "RA_PIN_DATA_EXT_SPEED"
        args = f"{pin_mode}, {pull}, {periph_sel}, {extra['channel']}, {rule['speed']}"
    elif has_speed:
        macro = "RA_PIN_DATA_SPEED"
        args = f"{pin_mode}, {pull}, {periph_sel}, {rule['speed']}"
    elif has_channel:
        macro = "RA_PIN_DATA_EXT"
        args = f"{pin_mode}, {pull}, {periph_sel}, {extra['channel']}"
    else:
        macro = "RA_PIN_DATA"
        args = f"{pin_mode}, {pull}, {periph_sel}"

    return f"    {{{pin_name}, {periph_name}, {macro}({args})}},"


def process_peripheral(pin_caps: dict, rule: dict, alt_psel: dict = None) -> list:
    entries = []
    alt_psel = alt_psel or {}

    if "pattern_list" in rule:
        patterns = rule["pattern_list"]
    else:
        patterns = [{"pattern": rule["pattern"]}]

    for pin_id, caps in sorted(pin_caps.items()):
        mbed_pin = pin_name_to_mbed(pin_id)
        for cap in caps:
            for sub_rule in patterns:
                match = re.match(sub_rule["pattern"], cap)
                if not match:
                    continue

                # Extract and convert the peripheral number (the number group
                # is optional, e.g. unnumbered I3C/IIC falls back to a default)
                periph_num_group = rule.get("periph_num_group", 1)
                group_value = match.group(periph_num_group)
                if group_value is None:
                    periph_num = rule.get("periph_num_default", 0)
                else:
                    periph_num = int(group_value)
                if "periph_num_transform" in rule:
                    periph_num = rule["periph_num_transform"](periph_num)

                # Build the peripheral name
                if "periph_prefix" in sub_rule:
                    periph_name = f"{sub_rule['periph_prefix']}{periph_num}"
                elif rule["array_name"].startswith("PinMap_UART"):
                    periph_name = f"UART_{periph_num}"
                elif rule["array_name"].startswith("PinMap_SPI"):
                    periph_name = f"SPI_{periph_num}"
                elif rule["array_name"].startswith("PinMap_I2C"):
                    periph_name = f"I2C_{periph_num}"
                elif rule["array_name"].startswith("PinMap_CAN"):
                    periph_name = f"CAN_{periph_num}"
                elif rule["array_name"].startswith("PinMap_ADC"):
                    periph_name = f"ADC_{periph_num}"
                elif rule["array_name"].startswith("PinMap_DAC"):
                    periph_name = f"DAC_{periph_num}"
                elif rule["array_name"].startswith("PinMap_IRQ"):
                    periph_name = f"IRQ_{periph_num}"
                else:
                    periph_name = str(periph_num)

                # Peripheral selection macro: prefer the real PSEL of this pin
                # function from the XML alt/registerSetting element
                extra = {}
                alt_key = capability_alt_key(cap)
                xml_psel = alt_psel.get(pin_id, {}).get(alt_key, None)
                if xml_psel is not None:
                    extra["periph_sel"] = xml_psel
                elif "periph_sel_macro" in sub_rule:
                    extra["periph_sel"] = sub_rule["periph_sel_macro"]
                elif "periph_sel_macro" in rule:
                    extra["periph_sel"] = rule["periph_sel_macro"]
                elif "periph_sel_macro_template" in rule:
                    if "SCI" in rule["periph_sel_macro_template"]:
                        extra["periph_sel"] = get_sci_periph_macro(periph_num)
                    else:
                        extra["periph_sel"] = rule["periph_sel_macro_template"].format(periph_num)

                # Channel number
                if rule.get("has_channel", False):
                    if "channel_extract" in sub_rule:
                        extra["channel"] = sub_rule["channel_extract"](match)
                    elif "channel_prefix" in rule:
                        ch_num = match.group(2)
                        if "channel_transform" in rule:
                            ch_num = rule["channel_transform"](ch_num)
                        extra["channel"] = f"{rule['channel_prefix']}{ch_num}"
                    else:
                        ch_num = match.group(2)
                        if "channel_transform" in rule:
                            ch_num = rule["channel_transform"](ch_num)
                        extra["channel"] = ch_num

                entries.append(generate_pin_entry(mbed_pin, periph_name, rule, extra))
                break

    return entries


def debug_unmatched_caps(pin_caps: dict):
    all_caps = set()
    for caps in pin_caps.values():
        all_caps.update(caps)

    if not all_caps:
        print("\n[Warning] No pin capabilities parsed, check the XML structure and namespace")
        return

    matched_caps = set()
    for rule in PERIPHERAL_RULES:
        if "pattern_list" in rule:
            patterns = rule["pattern_list"]
        else:
            patterns = [{"pattern": rule["pattern"]}]

        for sub_rule in patterns:
            pat = re.compile(sub_rule["pattern"])
            for cap in all_caps:
                if pat.match(cap):
                    matched_caps.add(cap)

    unmatched = sorted(all_caps - matched_caps)
    if unmatched:
        print("\n=== Unmatched capability list (functions other than mbed-standard peripherals) ===")
        for cap in unmatched[:20]:
            print(f"  {cap}")
        if len(unmatched) > 20:
            print(f"  ... {len(unmatched)} unmatched in total (mostly non-mbed peripherals, safe to ignore)")
        else:
            print(f"{len(unmatched)} unmatched in total")
    else:
        print("\nAll capabilities matched")


def generate_peripheral_pins(xml_content: str, output_path: str, debug: bool = False):
    pin_caps, alt_psel = parse_xml_components(xml_content)

    if debug:
        debug_unmatched_caps(pin_caps)

    output_order = [
        ("ADC", "PinMap_ADC"),
        ("DAC", "PinMap_DAC"),
        ("PWM", "PinMap_PWM"),
        ("IRQ", "PinMap_IRQ"),
        ("SERIAL", "PinMap_UART_RX"),
        ("", "PinMap_UART_TX"),
        ("SPI", "PinMap_SPI_MOSI"),
        ("", "PinMap_SPI_MISO"),
        ("", "PinMap_SPI_SCLK"),
        ("", "PinMap_SPI_SSEL"),
        ("I2C", "PinMap_I2C_SCL"),
        ("", "PinMap_I2C_SDA"),
        ("CAN", "PinMap_CAN_RD"),
        ("", "PinMap_CAN_TD"),
    ]

    periph_entries = {}
    for rule in PERIPHERAL_RULES:
        periph_entries[rule["array_name"]] = process_peripheral(pin_caps, rule, alt_psel)

    # Generate the output file
    lines = [
        "/* mbed Microcontroller Library",
        " * Copyright (c) 2024 ARM Limited",
        " * SPDX-License-Identifier: Apache-2.0",
        " */",
        "",
        '#include "PeripheralPins.h"',
        '#include "mbed_toolchain.h"',
        '#include "r_adc_api.h"',
        '#include "r_dac_api.h"',
        '#include "r_ioport.h"',
        "",
    ]

    last_section = ""
    for section_comment, array_name in output_order:
        entries = periph_entries.get(array_name, [])
        if section_comment:
            if last_section:
                lines.append("")
            lines.append(f"//*** {section_comment} ***")
            lines.append("")
            last_section = section_comment

        lines.append(f"const PinMap {array_name}[] = {{")
        for entry in entries:
            lines.append(entry)
        lines.append("    {NC, NC, 0}")
        lines.append("};")

    with open(output_path, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")

    print(f"\nGeneration done: {output_path}")
    print(f"Parsed {len(pin_caps)} pins")
    for name, entries in periph_entries.items():
        print(f"  {name}: {len(entries)} entries")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(f"Usage: {os.path.basename(sys.argv[0])} <PinCfg*.xml> [output.c] [--debug]")
        print("")
        print("  <PinCfg*.xml>   Renesas RA Smart Configurator pin-configuration XML file")
        print("  [output.c]      output file, defaults to PeripheralPins.c in the current directory")
        print("  [--debug]       print unmatched pin capabilities")
        sys.exit(1)

    args = [a for a in sys.argv[2:] if not a.startswith("--")]
    debug = "--debug" in sys.argv
    xml_path = sys.argv[1]
    output_path = args[0] if args else "PeripheralPins.c"

    if os.path.exists(xml_path):
        with open(xml_path, "r", encoding="utf-8") as f:
            xml_content = f.read()
        generate_peripheral_pins(xml_content, output_path, debug=debug)
    else:
        print(f"XML file not found: {xml_path}")
        sys.exit(1)
