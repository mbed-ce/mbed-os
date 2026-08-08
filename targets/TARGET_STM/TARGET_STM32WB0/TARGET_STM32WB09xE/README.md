# STM32WB09xE MCU target

`MCU_STM32WB09xE` is the MbedCE MCU target for the 512 KiB STM32WB09xE
devices. The initial physical board uses the VFQFPN32 `STM32WB09KEV6`; the
CMSIS device key used by MbedCE is `STM32WB09KEVx`.

- [STM32WB09KE product page](https://www.st.com/en/microcontrollers-microprocessors/stm32wb09ke.html)
- [STM32WB09xE datasheet (DS14210)](https://www.st.com/resource/en/datasheet/stm32wb09ke.pdf)
- [STM32WB0 documentation](https://www.st.com/en/microcontrollers-microprocessors/stm32wb0-series/documentation.html)

## Supported boards

- [`NUCLEO_WB09KE`](TARGET_NUCLEO_WB09KE/README.md)

## Device summary

| Property | STM32WB09xE |
| --- | --- |
| CPU | Arm Cortex-M0+, up to 64 MHz |
| FPU | None |
| MPU | 8 regions |
| Flash | 512 KiB, 2 KiB pages |
| SRAM | 64 KiB, four contiguous 16 KiB banks |
| Package represented by this target | VFQFPN32 |
| GPIO | Up to 20 |
| Serial interfaces | USART1 and LPUART1 |
| Other hardware | ADC, I2C1, SPI3/I2S3, DMA, RTC, timers, IWDG, TRNG, CRC, PKA |
| Radio | Bluetooth LE 5.4 and proprietary 2.4 GHz |

The radio timing-critical work is assisted by a DMA-based radio coprocessor.
This is not the Cortex-M4 plus Cortex-M0+ dual-core architecture used by the
separate STM32WB family.

## MbedCE memory map

| Region | Start | Physical size | Application size |
| --- | ---: | ---: | ---: |
| Flash | `0x10040000` | 512 KiB | 508 KiB |
| SRAM | `0x20000000` | 64 KiB | 64 KiB |

The final 4 KiB of flash is reserved for STM32WB0 stack nonvolatile data, so
the Mbed application ROM size is `0x7F000` bytes.

STM32WB0 ROM ABI data occupies the first `0x100` bytes of SRAM. The common
linker script preserves this area and places the runtime vector table after it.
The physical SRAM banks are contiguous and are therefore exposed to MbedCE as
one 64 KiB `SRAM` region.
