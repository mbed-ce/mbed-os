# Mbed CE Changelog

All notable changes to this project will be documented in this file.

The format is based on [CHANGELOG.md][CHANGELOG.md]
and this project adheres to [Semantic Versioning][Semantic Versioning].

<!-- 
UNRELEASED CHANGES - add all changes committed to Mbed here pending the next release

## [major.minor.patch] - yyyy-mm-dd

A message that notes the main changes in the update.

### Added

### Changed

### Deprecated

### Fixed

### Removed

### Security

_______________________________________________________________________________
 
 -->

<!--
EXAMPLE

## [7.x.y] - 2021-06-02

Lorem Ipsum dolor sit amet.

### Added

- Cat pictures hidden in the library
- Added beeswax to the gears

### Changed

- Updated localisation files

-->

_______________________________________________________________________________

## [7.0.0] - 2025-x-x

### Added
- New upload method system allows flashing boards as part of a CMake build, using one of a number of available debug tools
   - This system currently has config files for around half of all Mbed OS targets, and we are adding more as we test them.
- New memory bank system, so the build system can know about what memory banks exist on a target and make this available to the OS and linker script
- Mbed CE is now available as a PlatformIO framework, via a new adapter layer (inspired by the ESP-IDF implementation) that reads build system data from CMake and converts the equivalent structures in SCons.
- Memory mapper (script that runs after a program is built and prints info to the console) now shows a fill percentage for each memory bank on your chip. This should help you understand which banks you still have space in to use!
- All STM32 devices now support DMA for SPI
- New `RP2040` MCU family added, containing `RASPBERRY_PI_PICO` target
- New MIMXRT family targets added: Teensy 4.0, Teensy 4.1, MIMXRT1060_EVK
- New `STM32U0` MCU family added, containing `NUCLEO_U083RC` target
- New `STM32H5` MCU family added, containing `NUCLEO_H503RB` and `NUCLEO_H563ZI` targets
- Generic Ethernet MAC driver framework (`CompositeEMAC`) and generic Ethernet PHY driver (`GenericEthPHY`) added
  (though only some targets currently use the new system)
- `ARDUINO_GIGA_CM7` and `ARDUINO_GIGA_CM4` targets added to `STM32H7` MCU family
- `ARDUINO_NICLA_SENSE_ME` target added to `NRF52832` MCU family
- `ARDUINO_NANO33BLE` target and Portenta targets (which previously existed in Mbed but weren't really usable) updated with the correct linker script configuration and upload methods, so they can be used to develop for these boards without any additional Arduino software.
- Support for the LPC1769 CPU and the `LPCXPRESSO_LPC1769` target that uses it
- All JSON files can now be written as JSON5 instead, allowing comments and hex values to be used (yesssss)
- Add an `nsapi_strerror()` function to convert an nsapi_error_t to a string
- `Apollo3` MCU family: Add support for analog inputs and PWM outputs
- `LPC546XX` MCU family: Add support in linker script for crash capture RAM and use of SRAMX bank as additional heap
- `STM32H7`: 
    - Added `target.enable-overdrive-mode` option to control whether the CPU core runs at the highest clock speed or not. The default depends on the specific target. Previously this was always enabled, violating the datasheet specifications on targets where the power scheme for the CPU prevents using overdrive mode.
    - External oscillator frequency range expanded to support any frequency between 4MHz and 50MHz that is divisible by 2MHz or 5MHz
- `STM32U0`:
    - External oscillator frequency range expanded to support any frequency between 4MHz and 50MHz that is divisible by 4MHz or 5MHz
- Add support for using an ESP32 chip as an external wifi-fi interface via [ESP AT firmware](https://github.com/espressif/esp-at). This functionality can be accessed by adding the `ESPRESSIF_ESP32` component to your application.
- `SDBlockDevice` can now use async SPI, allowing other threads to execute while large SD card transfers are ongoing
- `CacheAlignedBuffer` class added, providing a way to manage DMA buffers on cores that have a CPU cache

### Changed

- CMake is now the only way to build Mbed. CMake build system has been substantially rewritten and extended.
- All JSON config options are required to be in `skewer-case`. If an option is found that is not in skewer-case, it is converted (for compatibility) and a warning is printed
- All JSON files are now validated against a Pydantic schema when being loaded to ensure they contain valid data. Currently, invalid mbed_app.json's are still allowed even if they don't pass the schema for compatibility.
- `I2C` class updated to use enums for return codes
- Sleep and external RAM disabled (for now) on MIMRT105x/6x targets to improve reliability
- `PORTENTA_H7_CM[7/4]` targets renamed to `ARDUINO_PORTENTA_H7_CM[7/4]` for consistency
- On many Arduino target boards with wi-fi support, a large (300kB+) firmware blob needs to be available to load into the wi-fi chip. Previously, this was always read from an external QSPI flash, with no way built-in to Mbed to actually load this image. Now, unless the `target.wifi-driver-in-qspi-flash` option is set to true, the binary will be included with the main application so that things work out of the box (at the cost of some program flash). 
- `Apollo3` MCU family: PinMode open-drain values now use standard names (`OpenDrain`, `OpenDrainPullUp`, etc) and are properly documented
- NXP `MIMXRT105x` MCU family: Fix always using the slow (528MHz) clock speed. This is now controlled by a new `target.enable-overdrive-mode` which can be changed to 0 to run at the slower clock speed (needs to be done for certain chip part numbers) 
- Add destructor to `SPISlave` which calls `spi_free()`. This makes sure that resources used by the SPI slave can actually be cleaned up when it is destroyed.
- Implement missing reference counting code in `SPI` destructor that would actually free the SPI bus and its associated resources. This fixes problems across multiple targets where the SPI peripheral and/or its DMA channels were not released, so destroying and re-creating an SPI class would fail.
- If you called `SPI::select()` to manually select an SPI device, then did an async transfer, previously the device would always be deselected at the end of the async transfer. Now it is not, allowing an asynchronous operation to be used as only _part_ of a larger transfer, which opens up some new ways to use SPI.
- Enabled SHA-1 format CA certificates by default. This consumes a bit more space but prevents cryptic errors in the default configuration. See also https://github.com/ARMmbed/mbed-os/issues/8638

### Fixed

- All STM32 MCU families:
  - QSPI and OSPI drivers updated to use DMA internally. While they still don't support asynchronous transfers, the transfer rate has been substantially improved.
  - SDIO HAL layer for STM32 chips to interact with Infineon wi-fi modules has been significantly rewritten. While some weirdness remains, it is substantially less janky.
  - Ethernet drivers for all STM32 devices completely rewritten from scratch to avoid the troubled HAL drivers, perform better, properly support multicast, and be compatible with the CPU cache.
  - PWM driver updated to use [smarter](https://github.com/mbed-ce/mbed-os/pull/283) prescaler selection logic. This provides much, much higher duty cycle resolution when frequencies of 100+ kHz are used.
  - CAN FD support implemented for MCUs with HW support for it
  - Fix bug where calling I2CSlave::frequency() after constructing the object would wipe out the I2C address in the registers, causing the I2C slave to not respond to any messages on the bus
  - Fix I2C slave read operation erroneously returning an error (0 bytes) if the master transferred less bytes than expected by the slave, instead of returning the actual number of bytes transferred.
- `STM32U5`: Fix very slow wake-up from deep sleep due to incorrect MSIRANGE constant used
- `STM32H7`:
  - Fix some MCUs having an out-of-spec HSI frequency when using internal HSI oscillator, causing e.g. UART to not work. Cause was clock code incorrectly setting the HSI calibration value.
  - Fix AnalogIn failing to init when using the HSI oscillator
  - Fix inability to use the LPUART peripherals above 9600 baud due to buggy clock code
- `STM32L4`: Fix unusual linker script on many STM32L4 MCUs that only allocated 1k of stack space to the application in baremetal mode, regardless of total RAM or heap usage. This caused the application to crash HARD (can't even run the fault handler) if it used more than 1k of stack.
- `Apollo3` MCU family:
  - Fixed an issue where if a pin was configured as an open-drain output, reading it would always return the written state of the pin instead of its actual electrical state.
  - Fix infinite hang in UART interrupt handler caused by noise / invalid data on UART Rx line 
  - Fix UART Rx FIFO threshold being set incorrectly, meaning if you send only a few bytes, it may never show up on the Mbed side
  - Fix setting interrupts for both rise and fall on the same pin causing no interrupts to be delivered
  - Fix bug where calling InterruptIn::read() after constructing the InterruptIn but before calling rise() or fall() would always return 0 instead of the actual logic level on the pin
- `LPC546xx` MCU family: Fix default MPU configuration that erroneously write-protected the SRAMX memory bank
- `LPC17xx` MCU family:
  - Support for open drain pin modes
  - Fix the blue LED not blinking on fatal error in Mbed 6 due to missing LED #define's
  - Split-heap support, so that the 2nd memory bank can be used as additional heap if not used for Ethernet
  - 16-bit SPI transfers fixed
  - Fix destroying and recreating an I2C object causing the next transfer to fail
  - Rewrite I2C slave HAL to behave as documented (e.g. I2CSlave::read() would return one more byte than actually read)
  - Fix destroying a PwmOut not stopping the PWM signal
  - Fix reading the pulse width and period from a PwmOut just returning raw register values rather than correctly scaled numbers
- Nuvoton `M480` MCU family:
  - Rewrite Ethernet driver to use new driver framework
  - Fix I2C slave NACKing the last byte from the master when receiving
  - Fix asynchronous I2C doing a repeated start instead of a stop at the end of the transaction
  - Fix SPI not handling a word size != 1 byte
  - Fix aborting interrupt-based async SPI transfers (though aborting DMA transfers [remains broken](https://github.com/mbed-ce/mbed-os/issues/466) and will likely require help from Nuvoton to fix)
  - Fix issues where DMA channels would be leaked when a DMA-using SPI instance was reinitialized
  - Work around errata where, if a watchdog reset was encountered while in power-down mode, the chip would not reboot cleanly
- NXP/Freescale `K64F` MCU family:
  - Fix SPI always sending 0 as the fill data for the upper word in 16-bit mode, regardless of `set_default_write_value()` setting.
  - Fix inability to do async SPI transfers with word sizes greater than 8 bits
  - Fix infinite hang if you used a single-byte SPI transfer after an async one
  - Fix all DMA SPI transactions being done twice due to incorrect ISR logic
- NXP `MIMXRT105x` MCU family:
  - Fix setting the serial format putting the UART peripheral in an invalid configuration due to trying to store an 32-bit register value in a 8-bit integer
  - Fix using a higher core voltage than needed when running at 528MHz, easting energy
  - Fix CPU clock always changing to 600MHz when exiting deep sleep, even if configured for 528MHz
- `K64F` and `MIMXRT105x` MCU families: Fix SPI frequency being reset back to default if you called `SPI::format()` after `SPI::frequency()`
- Infineon WHD wi-fi driver now uses the Mbed memory manager instead of directly depending on LwIP. This means it can now be used with Nanostack ([kind of](https://github.com/mbed-ce/mbed-os/issues/505)) and the EMAC unit tests
- tdbstore library: fix off-by-one error causing false error when `kv_set` is called with data exactly equal to the available storage capacity
- Fix QSPIF quirks logic to handle `MX25L12833F` SPI flash (as used on Arduino Portenta)
- Fix heap size as reported by `mbed_stats_heap_get()` not including the size of the second heap on split-heap targets
- Fix undefined behavior when `PwmOut::resume()` is called on a PwmOut that was created from a static pinmap
- Fixed aborting an SPI transfer still leaving the chip selected if GPIO CS is used
- Fixed init order issue causing `QSPIFBlockDevice` and `OSPIFBlockDevice` to potentially behave incorrectly if declared as global objects.

### Removed
- Removed support for ARM Compiler, as we could not get a version to use for testing, and having to maintain linker scripts in two different formats was not a good use of time.
- Removed support for all GigaDevices MCUs as we could not find a way to buy the dev boards for testing here in the
    US, and there was some evidence that the target boards that Mbed supported were no longer being manufactured at all
- Similarly, removed support for all Samsung MCUs as we could not find a way to buy the dev boards for testing.
- Removed a number of target boards which appear to have been EOLed (or never actually released!):
  - `SFE_EDGE`
  - `SFE_EDGE2`
  - `CYBSYSKIT_01`
  - `MTS_DRAGONFLY_F413RH`
  - `WIO_3G`
  - `WIO_BG96`
  - `RHOMBIO_L476DMW1K`
  - `ADV_WISE_1570`
  - `MTS_DRAGONFLY_L496VG`
- The Mbed CLI 1 python-based build system has been removed
- The `PlatformMutex` class has been removed. This class was useless because `rtos::Mutex` is available as a stub in baremetal builds, so there is no need for another layer of abstraction over it.
- Removed useless `SPISlave::frequency()` function. This function does not actually work because the SPI master sets the clock frequency; the slave has no control over it.
- SPINANDBlockDevice was moved out of the Mbed CE source tree and into a library: https://github.com/mbed-ce-libraries-examples/MacronixFlash
- Quectel cellular modems were moved out of the Mbed CE source tree and into their own library: https://github.com/mbed-ce-libraries-examples/Quectel-Cellular-Driver
  - Note that as of this writing, not all the drivers have been updated for compatibility with the Mbed CE build system yet.

### Security
- Fix several CVEs that could cause memory corruption / code execution by a malicious Bluetooth chip connected to the HCI interface. See the PRs ([1](https://github.com/mbed-ce/mbed-os/pull/384), [2](https://github.com/mbed-ce/mbed-os/pull/385), [3](https://github.com/mbed-ce/mbed-os/pull/386), [4](https://github.com/mbed-ce/mbed-os/pull/387), [5](https://github.com/mbed-ce/mbed-os/pull/388), [6](https://github.com/mbed-ce/mbed-os/pull/374)) for details.

_______________________________________________________________________________

[CHANGELOG.md]: https://keepachangelog.com/en/1.1.0/
[Semantic Versioning]: http://semver.org/

<!-- markdownlint-configure-file {
    "MD022": false,
    "MD024": false,
    "MD030": false,
    "MD032": false
} -->
<!--
    MD022: Blanks around headings
    MD024: No duplicate headings
    MD030: Spaces after list markers
    MD032: Blanks around lists
-->