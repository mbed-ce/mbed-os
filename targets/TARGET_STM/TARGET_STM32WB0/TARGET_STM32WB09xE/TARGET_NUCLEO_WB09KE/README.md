# NUCLEO_WB09KE target

`NUCLEO_WB09KE` supports the ST NUCLEO-WB09KE development board, composed of
the MB1801 Nucleo-64 mezzanine and MB2032 STM32WB09 RF board. It uses an
STM32WB09KEV6 MCU and the on-board STLINK-V3EC debugger/programmer.

- [NUCLEO-WB09KE product page](https://www.st.com/en/evaluation-tools/nucleo-wb09ke.html)
- [NUCLEO-WB09KE user manual (UM3345)](https://www.st.com/resource/en/user_manual/um3345-stm32wb09-nucleo64-board-mb1801-and-mb2032-stmicroelectronics.pdf)
- [STM32WB09xE datasheet](https://www.st.com/resource/en/datasheet/stm32wb09ke.pdf)

## Board configuration

| Property | Configuration |
| --- | --- |
| Target name | `NUCLEO_WB09KE` |
| MCU target | [`MCU_STM32WB09xE`](../README.md) |
| System clock | 64 MHz RC64MPLL, with 64 MHz HSI fallback |
| HSE | 32 MHz crystal |
| LSE | 32.768 kHz crystal |
| Default supply | 3.3 V |
| Debug probe | STLINK-V3EC over SWD |
| Console | USART1 through the ST-LINK virtual COM port, 115200 baud |

The board physically provides Arduino Uno V3 and ST morpho connectors.
However, the Mbed target does not advertise an Arduino form factor because the
small-pin-count MCU and board functions do not provide a conflict-free,
complete default Arduino peripheral set with the APIs supported by the initial
port.

## Standard pin names

| Mbed name | MCU pin | Board function |
| --- | --- | --- |
| `CONSOLE_TX` | `PA_1` | ST-LINK VCP receive |
| `CONSOLE_RX` | `PB_0` | ST-LINK VCP transmit |
| `LED1` | `PB_1` | User LED LD1 |
| `LED2` | `PB_4` | User LED LD2 |
| `LED3` | `PB_2` | User LED LD3 |
| `BUTTON1` | `PA_0` | User button B1 |
| `BUTTON2` | `PB_5` | User button B2 |
| `BUTTON3` | `PB_14` | User button B3 |
| `DEBUG_SWDIO` | `PA_2` | SWD data |
| `DEBUG_SWCLK` | `PA_3` | SWD clock |
| `RCC_OSC32_IN` | `PB_13` | LSE input |
| `RCC_OSC32_OUT` | `PB_12` | LSE output |

Some connector pins share board LEDs, buttons, debug signals or oscillator
functions. Check UM3345 and the solder-bridge configuration before reusing
those pins.

## Programming

See the MbedCE [STM32 Deploy & Debug](https://github.com/mbed-ce/mbed-os/wiki/STM32-Deploy&Debug)
guide for the general STM32 programming and debugging workflow.

The default upload method for this target is `STM32CUBE`. The board is
connected through SWD in hot-plug mode because normal reset-mode connection is
not reliable:

```text
-c port=SWD mode=HOTPLUG -hardRst -halt
```

Raw binary files are programmed at the application flash base
`0x10040000`. The generated MbedCE flash target supplies this address
automatically.
