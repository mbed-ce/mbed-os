# STM32 targets in MbedCE

This document describes the STM32 target structure and the current workflow
for adding an STM32 family, MCU target, or board to MbedCE. The repository
currently contains both modernized and legacy family layouts; new work should
follow the modern structure described below.

## Status of the existing documentation

The following guidance from the existing README remains valid:

- use current ST-LINK firmware and install ST USB drivers when the operating
  system does not expose all probe interfaces;
- use STM32CubeMX and STM32Cube MCU packages as reference material;
- generate candidate pin maps from STM32 open pin data, then review them
  against the board schematic and MCU datasheet;
- select high- and low-speed clocks according to the board hardware;
- account for conflicts between connector pins and LEDs, buttons, debug,
  oscillators, and other board functions.

The following parts require rework and are replaced in this document:

- STM32 vendor code is not always copied from a complete STM32Cube package.
  Migrated families use pinned CMSIS-device and HAL-driver submodules;
- a custom board should inherit the correct MCU target instead of being made by
  copying the nearest development board;
- startup files, linker scripts, vector-table sizing, memory banks, and the
  helpers in `tools/stm32.cmake` are now part of the documented target model;
- clock-source names and deep-sleep modes are family-specific, not universal;
- STM32CubeProgrammer is an active MbedCE upload method;
- old ARMmbed repository links, obsolete command output, and static tool
  version examples must be replaced with MbedCE and current ST sources;
- Wi-Fi, Ethernet, CAN, and asynchronous SPI details must be maintained with
  their drivers or family implementations instead of as universal STM32 rules.

## Repository structure

The STM32 port is divided into common, family, MCU, and board layers:

```text
targets/TARGET_STM/
|-- CMakeLists.txt                 # selects the active STM32 family
|-- tools/
|   |-- stm32.cmake               # family, clock, and startup helpers
|   `-- STM32_gen_PeripheralPins.py
|-- TARGET_STM32<family>/
|   |-- CMakeLists.txt             # family HAL and common implementation
|   |-- STM32Cube_FW/              # CMSIS device and HAL integration
|   |-- clock_cfg/
|   |-- linker_scripts/
|   `-- TARGET_STM32<device>/
|       |-- CMakeLists.txt         # MCU selection
|       `-- TARGET_<board>/
|           |-- CMakeLists.txt
|           |-- PinNames.h
|           `-- PeripheralPins.c
`-- common STM32 Mbed HAL sources
```

Target metadata is stored separately:

- `targets/targets.json5` defines inheritance, labels, capabilities, clocks,
  device names, and configured memory restrictions;
- `targets/cmsis_mcu_descriptions.json5` stores physical memory descriptions
  obtained from CMSIS packs;
- `targets/upload_method_cfg/` contains board upload and debug configuration.

Only the selected STM32 family is added by the root `TARGET_STM/CMakeLists.txt`.
The family is resolved from the exact family label in `MBED_TARGET_LABELS`.

## Supported STM32 families

The following family directories currently exist in this repository:

| Family | Directory | Notes |
| --- | --- | --- |
| STM32F0 | [`TARGET_STM32F0`](TARGET_STM32F0/) | Legacy copied vendor layout |
| STM32F1 | [`TARGET_STM32F1`](TARGET_STM32F1/) | Legacy copied vendor layout |
| STM32F2 | [`TARGET_STM32F2`](TARGET_STM32F2/) | Legacy copied vendor layout |
| STM32F3 | [`TARGET_STM32F3`](TARGET_STM32F3/) | Legacy copied vendor layout |
| STM32F4 | [`TARGET_STM32F4`](TARGET_STM32F4/) | Pinned CMSIS/HAL submodules and common linker |
| STM32F7 | [`TARGET_STM32F7`](TARGET_STM32F7/) | Pinned CMSIS/HAL submodules and common linker |
| STM32G0 | [`TARGET_STM32G0`](TARGET_STM32G0/) | Legacy copied vendor layout |
| STM32G4 | [`TARGET_STM32G4`](TARGET_STM32G4/) | Legacy copied vendor layout |
| STM32H5 | [`TARGET_STM32H5`](TARGET_STM32H5/) | Legacy copied vendor layout |
| STM32H7 | [`TARGET_STM32H7`](TARGET_STM32H7/README.md) | Family-specific documentation |
| STM32L0 | [`TARGET_STM32L0`](TARGET_STM32L0/) | Legacy copied vendor layout |
| STM32L1 | [`TARGET_STM32L1`](TARGET_STM32L1/) | Legacy copied vendor layout |
| STM32L4 | [`TARGET_STM32L4`](TARGET_STM32L4/) | Legacy copied vendor layout |
| STM32L5 | [`TARGET_STM32L5`](TARGET_STM32L5/) | Legacy copied vendor layout |
| STM32U0 | [`TARGET_STM32U0`](TARGET_STM32U0/) | Pinned CMSIS/HAL submodules |
| STM32U5 | [`TARGET_STM32U5`](TARGET_STM32U5/) | Legacy copied vendor layout |
| STM32WB | [`TARGET_STM32WB`](TARGET_STM32WB/README.md) | Dual-core wireless family |
| STM32WB0 | [`TARGET_STM32WB0`](TARGET_STM32WB0/README.md) | Cortex-M0+ wireless family; pinned CMSIS/HAL and common linker |
| STM32WL | [`TARGET_STM32WL`](TARGET_STM32WL/README.md) | Long-range wireless family |

STM32WB0 and STM32WB are different product families. STM32WB0 uses the
`STM32WB0` family label, `TARGET_STM32WB0` directory, `cmsis-device-wb0`, and
`stm32wb0x-hal-driver`; it must not inherit the STM32WB family target.

## ST tools and vendor sources

### ST-LINK software

- [ST-LINK USB driver](https://www.st.com/en/development-tools/stsw-link009.html)
- [ST-LINK firmware upgrade](https://www.st.com/en/development-tools/stsw-link007.html)

Update the probe firmware when connection, reset, mass-storage, or virtual COM
behavior is unreliable. On Windows, install the ST driver if the debug, VCP,
or bridge interfaces are missing.

### STM32Cube and CMSIS

[STM32Cube MCU packages](https://www.st.com/en/embedded-software/stm32cube-mcu-packages.html)
contain CMSIS device support, HAL and LL drivers, examples, BSPs, and optional
middleware. MbedCE uses the CMSIS and HAL/LL portions required by each target;
it does not automatically include a package's BSP or middleware.

New and migrated families should use ST's separate CMSIS-device and HAL-driver
repositories as pinned Git submodules. Keep vendor submodules unmodified. Put
an unavoidable temporary adaptation in a clearly scoped `Mbed_HAL_overrides`
directory and document how it can be removed.

### STM32CubeMX and pin data

[STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) is not
part of the MbedCE build, but its device database and generated configuration
are useful references. MbedCE's pin generator consumes
[STM32 open pin data](https://github.com/STMicroelectronics/STM32_open_pin_data).

### STM32CubeProgrammer

[STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)
is supported by the MbedCE `STM32CUBE` upload method. See
[STM32 Deploy & Debug](https://github.com/mbed-ce/mbed-os/wiki/STM32-Deploy&Debug)
for setup and troubleshooting. Upload behavior is board-specific; verify the
connection mode, ROM base address, programming, verification, and final reset
on physical hardware.

## Adding or changing an STM32 target

First determine the scope:

1. For a board using an existing MCU target, add only board metadata, pins,
   CMake linkage, clocks, and upload configuration.
2. For a new MCU in an existing family, add the MCU target, exact CMSIS device
   name, memory configuration, startup label, and validated capabilities.
3. For a new family, add its CMSIS/HAL integration, family library, common
   drivers, clocks, startup flow, linker support, at least one MCU, and one
   physical board.

For application-owned boards, start from
[mbed-ce-custom-targets](https://github.com/mbed-ce/mbed-ce-custom-targets)
and set `CUSTOM_TARGETS_JSON_PATH` before including
`mbed_toolchain_setup.cmake`. The example repository shows the supported file
layout, target JSON, and CMake ordering.

### From a development-board target to a custom target

A product can begin with a built-in development-board target and later use a
custom target for the settings that differ. The inheritance choice depends on
the product:

- inherit the generic `MCU_STM32...` target when most board-level settings must
  be defined independently;
- inherit an existing Nucleo, Discovery, or other board target when the custom
  target intentionally keeps most of that board's Mbed configuration and only
  needs a small number of overrides.

Review inherited settings before deciding. Inheriting a board also inherits
its clock choices, target labels, capabilities, components, form factor, and
other metadata. Keep inherited values that are still correct and override or
remove only the differences.

The usual MbedCE changes are:

1. Define inheritance and metadata changes in `custom_targets.json5`, using
   `overrides`, `device_has_add`/`device_has_remove`, components, features, and
   labels as required.
2. Add the custom target directory before `mbed-os` in the application's
   `CMakeLists.txt`.
3. Reuse the inherited pin files or provide replacement `PinNames.h` and
   `PeripheralPins.c` through the custom target CMake interface.
4. When replacing a source already supplied by an Mbed MCU target, call
   `mbed_disable_mcu_target_file()` after Mbed project setup and before adding
   the `mbed-os` subdirectory, then provide the replacement source from the
   custom target.
5. Select or replace clock configuration, linker script, memory-bank
   configuration, and upload/debug configuration only where the product
   differs from its parent target.
6. Build and test both the inherited behavior and every override. Pay special
   attention to console pins, standard LED/button names, clocks, flash base,
   and the generated upload command.

See [mbed-ce-custom-targets](https://github.com/mbed-ce/mbed-ce-custom-targets)
for the maintained complete example rather than copying partial snippets from
this README.

### Board pin maps

Generate a candidate pin map with:

```sh
python targets/TARGET_STM/tools/STM32_gen_PeripheralPins.py --help
python targets/TARGET_STM/tools/STM32_gen_PeripheralPins.py --mcu "<device XML>"
```

The script clones or updates `STM32_open_pin_data` unless `--nopull` is used.
Its output is a starting point, not a validated board definition. Review:

- package availability and alternate-function numbers;
- internal-only and unbonded signals;
- SWD, oscillators, ST-LINK VCP, LEDs, buttons, and board peripherals;
- timer channels reserved by Mbed tickers;
- default pull configuration and electrical constraints;
- aliases exposed through `PinNames.h` and any advertised form factor.

Alternate pin names such as `PC_10_ALT0` select a different peripheral mapping
from the default `PC_10` entry. A pin listed in `PeripheralPins.c` can still be
unusable when another enabled board function owns it.

### Clock configuration

The target's `clock-source` setting selects a family implementation. The valid
values are defined by that family's `clock_cfg` and HAL configuration; there is
no global list that applies to every STM32 family. A board should advertise
only sources supported by its fitted oscillator circuit and may provide an
internal-clock fallback when the family implementation supports it.

Use STM32CubeMX's **Clock Configuration** view when creating or changing a
family clock implementation. Configure the exact MCU, supply voltage, HSE and
LSE sources, PLL path, system clock, bus prescalers, peripheral clocks, flash
latency, and voltage scaling. CubeMX is useful for checking clock limits and
for generating a reference `SystemClock_Config()`, but its generated project is
not copied into MbedCE unchanged. Translate the required RCC and power setup
into the family `clock_cfg`, compare it with the current ST HAL version, and
remove unrelated generated initialization.

Check the real board source carefully: a Nucleo may receive HSE from the
ST-LINK MCO while a production board uses a crystal, oscillator module, or no
external high-speed source. After implementation, verify `SystemCoreClock`,
timer and serial timing, USB or radio clock requirements, oscillator failure
fallback, and wake-up clock restoration on hardware.

Low-speed clock handling is also family-specific. The `lse-available` setting
describes whether the board has a 32.768 kHz LSE source. RTC, low-power ticker,
and deep-sleep behavior must be checked against the selected family's driver;
do not assume every STM32 maps Mbed deep sleep to STOP2.

### Startup files

Modern STM32 families use the upstream GCC startup file from the CMSIS-device
submodule. The family CMake file selects and patches it:

```cmake
get_startup_file(STARTUP_FILE "STM32Cube_FW/cmsis-device-xx/Source/Templates/gcc")
patch_startup_file(STARTUP_FILE_GEN "${STARTUP_FILE}")
target_sources(mbed-stm32xx INTERFACE ${STARTUP_FILE_GEN})
```

The MCU target supplies an exact `STARTUP_<device>` label. Use
`PRESERVE_SYSTEM_INIT_ORDER` only when the vendor startup order is required,
such as STM32WB0 deep-stop context restoration. Confirm the generated reset
flow and ELF entry point instead of assuming all vendor startup files match.

### Linker scripts and vector tables

Prefer one preprocessed family linker script driven by generated memory-bank
macros. Select it on the family or MCU interface target:

```cmake
mbed_set_linker_script(mbed-stm32xx linker_scripts/STM32XX_COMMON.ld)
```

If a custom target owns a different memory layout, select its script on the
custom target's interface library in `custom_targets/CMakeLists.txt`:

```cmake
mbed_set_linker_script(
    mbed-my-stm32-board
    ${CMAKE_CURRENT_SOURCE_DIR}/linker_scripts/my_stm32_board.ld
)
```

Use separate scripts only for a real difference in memory topology, core,
security mode, boot flow, or execution mode. An application can deliberately
override the selected script by passing a second argument to its post-build
setup:

```cmake
mbed_set_post_build(my_application path/to/custom_linker.ld)
```

A common linker script should export the flash vector start and end, vector
table size, and RAM vector destination. The family `cmsis.h` can then derive:

```c
extern uint32_t __vector_ram_start__;
extern uint32_t __vector_table_size__;

#define NVIC_NUM_VECTORS \
    ((uint32_t)(uintptr_t)&__vector_table_size__) / sizeof(uint32_t)
#define NVIC_RAM_VECTOR_ADDRESS (uint32_t *)&__vector_ram_start__
```

This replaces copied per-MCU `cmsis_nvic.h` constants. Keep a per-MCU file only
when the vector layout truly differs and cannot be expressed by linker symbols.

### Memory banks

For a built-in MCU target, set `device_name` to the exact CMSIS pack key and
store its physical banks in `targets/cmsis_mcu_descriptions.json5`. Use the
repository tools from `tools/python` to refresh and check the data:

```sh
python -m mbed_tools.cli.main cmsis-mcu-descr reload-cache
python -m mbed_tools.cli.main cmsis-mcu-descr fetch-missing
python -m mbed_tools.cli.main cmsis-mcu-descr check-missing
python -m mbed_tools.cli.main cmsis-mcu-descr find-unused
```

`fetch-missing` prints a candidate entry for review; it does not update the
description file automatically. Compare addresses and capacities with the
datasheet, reference manual, and vendor linker file.

Custom targets that cannot use a built-in `device_name` must define physical
`memory_banks` inline. Use `memory_bank_config` only to restrict an existing
bank for an application offset, bootloader, reserved NVM, or similar purpose:

```json5
{
    "MY_STM32_BOARD": {
        "inherits": ["MCU_STM32xxxx"],
        "memory_bank_config": {
            "IROM1": {
                "start": 0x08008000,
                "size": 0x00078000
            }
        }
    }
}
```

Only `start` and `size` may be changed by `memory_bank_config`. Inspect the
configuration summary and generated `memory_banks.json`; confirm that physical
and configured values, linker macros, and upload base address agree.

### `tools/stm32.cmake`

The current helper file provides:

- `get_stm32_family()` to resolve the complete family label, including the
  three-character `STM32WB0` suffix;
- `get_system_clock_file()` to select a family clock file from target labels;
- `get_startup_file()` to map an exact startup label to an upstream file;
- `patch_startup_file()` to generate an Mbed-compatible startup file without
  modifying the CMSIS submodule.

Keep family-name parsing bounded so MCU-level labels cannot be mistaken for a
family. When adding a new naming pattern, test existing two-character families
as well as the new family.

### Connectivity drivers

Not every STM32-specific driver belongs below `targets/TARGET_STM`. The target
tree owns MCU HAL implementations, startup, clocks, memory, pins, and other
code required to expose Mbed's hardware APIs. Drivers that implement a Mbed
connectivity subsystem should normally live under
[`connectivity/drivers`](../../connectivity/drivers/), alongside the subsystem
interfaces and other vendor implementations.

Existing STM32 examples include:

- [Ethernet MAC](../../connectivity/drivers/emac/TARGET_STM/);
- [Wi-Fi modules](../../connectivity/drivers/wifi/TARGET_STM/);
- [STM32WB Bluetooth integration](../../connectivity/drivers/ble/FEATURE_BLE/TARGET_STM32WB/);
- [STM32WL LoRa radio](../../connectivity/drivers/lora/TARGET_STM32WL/);
- [STM32 cryptographic acceleration for Mbed TLS](../../connectivity/drivers/mbedtls/TARGET_STM/).

Small target-side glue may still be needed for interrupts, clocks, transport,
or low power, but the protocol or network driver should remain with the
subsystem that consumes it. Use target labels, features, and components to
select the implementation without pulling connectivity middleware into
applications that do not use it.

## Family-level implementation notes

Pin, clock, sleep, DMA, and peripheral behavior varies between STM32 families.
Do not copy a family-specific workaround into common `TARGET_STM` code without
checking all callers. Advertise a capability in `device_has` only after its
Mbed HAL is implemented and validated on hardware.

For networking, storage, USB, wireless, and DMA-backed asynchronous APIs,
verify the required middleware, external hardware, interrupt behavior, cache
maintenance, and low-power interaction separately. Family or board READMEs
should document only persistent product behavior; build logs and Greentea
results belong in the pull request or test report.

## Validation

For a new or changed target:

1. configure and build a minimal application;
2. inspect ELF sections, reset vector, vector-table size, stack, and memory
   boundaries;
3. program and verify the board through each advertised upload method;
4. test clock accuracy, GPIO, serial, tickers, interrupts, and sleep/wake;
5. run focused tests for every newly enabled API;
6. run the local Greentea suite and classify failures separately from missing
   peripherals or external fixtures;
7. record exact target, toolchain, configuration, dependency revisions, board
   revision, probe firmware, and test results in the pull request.

Compilation alone is not sufficient evidence that an STM32 target is
supported.

## Related documentation

- [MbedCE documentation](https://mbed-ce.dev/)
- [MbedCE wiki](https://github.com/mbed-ce/mbed-os/wiki)
- [Mbed memory-bank information](https://github.com/mbed-ce/mbed-os/wiki/Mbed-Memory-Bank-Information)
- [Running Greentea tests locally](https://github.com/mbed-ce/mbed-os/wiki/Running-the-Mbed-Greentea-Tests-Locally)
