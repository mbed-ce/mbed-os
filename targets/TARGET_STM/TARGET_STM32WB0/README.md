# STM32WB0 family

The STM32WB0 family contains low-power wireless microcontrollers based on an
Arm Cortex-M0+ application core and a 2.4 GHz radio subsystem. The core runs at
up to 64 MHz and the family provides Bluetooth Low Energy and proprietary
2.4 GHz radio hardware.

STM32WB0 is a separate product family from STM32WB. STM32WB0 devices use their
own CMSIS device package, HAL driver and startup flow; they must not be placed
under `TARGET_STM32WB`.

- [STM32WB0 family page](https://www.st.com/en/microcontrollers-microprocessors/stm32wb0-series.html)
- [STM32CubeWB0](https://github.com/STMicroelectronics/STM32CubeWB0)
- [STM32WB0 CMSIS device package](https://github.com/STMicroelectronics/cmsis-device-wb0)
- [STM32WB0 HAL driver](https://github.com/STMicroelectronics/stm32wb0x-hal-driver)

## Vendor dependencies

| Component | Version | Pinned revision |
| --- | --- | --- |
| CMSIS device package | `v1.4.0` | `f893ef4a3886f53f8042daca733cfcde9b708ed2` |
| STM32WB0 HAL driver | `v1.5.0` | `9469b33cff37a3c01cccdd9935a0fa7676aacc08` |

## MbedCE target structure

The family target is `MCU_STM32WB0`, implemented by the
`mbed-stm32wb0` CMake interface library. The family layer owns the common
startup selection, linker script, clock configuration and WB0-specific HAL
implementations.

Startup code is taken from the upstream CMSIS device submodule and patched at
configure time. The vendor `SystemInit` ordering is preserved because it is
part of the STM32WB0 deep-stop context restoration path. Vector-table size and
RAM placement are derived from symbols exported by the common linker script.

## Supported MCUs

- [`MCU_STM32WB09xE`](TARGET_STM32WB09xE/README.md)

## Implemented Mbed APIs

The initial family port provides:

- digital GPIO and port I/O;
- `InterruptIn`;
- USART and LPUART serial operation;
- microsecond ticker and `wait_ns`;
- RTC with LSE and LSI support;
- RTC-based low-power ticker;
- sleep and deep sleep;
- Arm MPU support.

The following hardware blocks are not yet advertised as Mbed devices because
their Mbed HAL implementations have not been completed and validated for WB0:

- ADC;
- I2C;
- SPI;
- PWM;
- asynchronous DMA-backed peripheral APIs;
- FlashIAP;
- watchdog and reset-reason APIs;
- TRNG;
- Bluetooth Low Energy and proprietary radio support.

The MCU contains several of these peripherals in hardware. Their absence from
`device_has` describes the current MbedCE port, not the silicon capability.

## Clocks and low power

The common clock configuration supports these system clock sources:

| Configuration | System clock | External component |
| --- | ---: | --- |
| `USE_RC64MPLL` | 64 MHz | 32 MHz HSE crystal |
| `USE_DIRECT_HSE` | 32 MHz | 32 MHz HSE crystal |
| `USE_HSI` | 64 MHz | None |

RTC clocking can use an external 32.768 kHz LSE or the internal LSI. The
low-power ticker uses the RTC wake-up timer rather than an LPTIM peripheral.
