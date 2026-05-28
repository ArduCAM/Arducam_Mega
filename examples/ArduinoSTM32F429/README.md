# Arducam Mega — STM32F429 Extension

This folder contains examples that extend the **Arducam_Mega** library for use on the **STM32F429** (tested on NUCLEO-F429ZI). The standard library targets generic Arduino boards; these examples add the pin configuration and helper classes needed to run correctly on STM32F429.

---

## Why an extension is needed

The Arducam_Mega library selects its hardware abstraction layer (HAL) at compile time via `src/Arducam/Platform.h`. When compiling for STM32F4xx under the Arduino core (`STM32F4xx && ARDUINO` is defined), it automatically picks `ArduinoHal.h`, which maps SPI and delay calls to the Arduino API. That part works without changes.

What the generic library does **not** configure is:

| Problem | Solution in these examples |
|---|---|
| STM32F429 NUCLEO default SPI1 pins conflict with other on-board peripherals | Explicitly remap SPI1 to alternate pins PB3/PB4/PB5 before calling `myCAM.begin()` |
| ST-Link virtual COM port uses USART3 (PD8/PD9), not the default `Serial` pins | `ArducamLink` calls `Serial.setTx(PD_8)` / `Serial.setRx(PD_9)` before `Serial.begin()` |
| Arduino-style integer pin numbers (e.g. `7`) do not map to the correct STM32 ports | All CS pins use STM32 port notation (`PE_7`, `PB_12`, etc.) |

---

## Shared SPI setup

Both examples configure SPI1 the same way in `setup()`:

```cpp
SPI.setMISO(PB_4);
SPI.setMOSI(PB_5);
SPI.setSCLK(PB_3);
SPI.begin();
```

This must be done **before** `myCAM.begin()`, because `begin()` internally calls `SPI.begin()` and will use whichever pins are currently configured.

---

## Examples

### `full_featured`

Streams camera data over UART to the [Arducam Android/PC host app](http://www.arducam.com). Supports all camera controls: resolution, format, brightness, contrast, white balance, exposure, gain, focus, special effects, and streaming.

**Extra files in this example:**

- `ArducamUart.h` — thin macros that map `SerialBegin`, `SerialWrite`, etc. to the Arduino `Serial` object.
- `ArducamLink.h` / `ArducamLink.cpp` — command parser and UART framing layer. Receives binary command packets from the host app, calls the corresponding `Arducam_Mega` API, and sends framed responses back.

**Pin assignment:**

| Signal | STM32 Pin |
|---|---|
| SPI1 SCK | PB3 |
| SPI1 MISO | PB4 |
| SPI1 MOSI | PB5 |
| Camera CS | PE7 |
| UART TX (USART3) | PD8 |
| UART RX (USART3) | PD9 |

---

### `capture2SD`

Captures a JPEG photo every 5 seconds and saves it as a numbered file (`0.jpg`, `1.jpg`, …) on a FAT-formatted SD card.

The SD card module shares **SPI1** with the camera — this is normal SPI bus behaviour; each device is deselected (CS high) when not in use. The standard Arduino **SD** library is used, which only supports the default `SPI` object.

**Pin assignment:**

| Signal | STM32 Pin |
|---|---|
| SPI1 SCK | PB3 |
| SPI1 MISO | PB4 |
| SPI1 MOSI | PB5 |
| Camera CS | PE7 |
| SD card CS | PB12 *(change `SD_CS` to match your wiring)* |
| UART TX (USART3) | PD8 |
| UART RX (USART3) | PD9 |

> **SD card on a separate SPI bus?** The Arduino SD library does not support custom SPI instances. Install the **SdFat** library (Arduino Library Manager) and replace `SD.begin(SD_CS)` with:
> ```cpp
> SPIClass SPI_SD(PB_15, PB_14, PB_13); // MOSI, MISO, SCK (SPI2)
> SdFat SD;
> SD.begin(SdSpiConfig(SD_CS, DEDICATED_SPI, SD_SCK_MHZ(25), &SPI_SD));
> ```

---

## Known issues and fixes

### Linker error: `undefined reference to arducamSpiCsPinLow` / `arducamSpiCsPinHigh` / `arducamCsOutputMode`

**Symptom** — compilation fails at the link stage with errors like:

```
ArducamCamera.c:(.text.cameraCsLow+0x2): undefined reference to `arducamSpiCsPinLow'
ArducamCamera.c:(.text.cameraCsHigh+0x2): undefined reference to `arducamSpiCsPinHigh'
ArducamCamera.c:(.text.cameraInit+0xa): undefined reference to `arducamCsOutputMode'
```

**Root cause** — `arducamSpiCsPinLow`, `arducamSpiCsPinHigh`, and `arducamCsOutputMode` are not real functions. They are macros defined in `src/Arducam/ArduinoHal.h` that alias them to the actual SPI helpers:

```c
#define arducamSpiCsPinHigh(pin)  arducamSpiCsHigh(pin)
#define arducamSpiCsPinLow(pin)   arducamSpiCsLow(pin)
#define arducamCsOutputMode(pin)  arducamSpiCsOutputMode(pin)
```

`src/Arducam/Platform.h` selects which HAL header to include based on preprocessor macros. The STMicroelectronics Arduino core defines `ARDUINO_ARCH_STM32`, but the upstream library only checked for `STM32F7xx && ARDUINO`, so no HAL was included and the macros were never defined.

**Fix** — add `ARDUINO_ARCH_STM32` to the condition in `src/Arducam/Platform.h`:

```c
// Before
#elif defined(__AVR__) || ... || (defined(STM32F7xx) && defined(ARDUINO))

// After
#elif defined(__AVR__) || ... || (defined(STM32F7xx) && defined(ARDUINO)) || defined(ARDUINO_ARCH_STM32)
```

This change has been applied to this copy of the library. If you update the library via the Arduino Library Manager, re-apply this one-line change to `src/Arducam/Platform.h`.

---

## Dependencies

| Library | Where to get |
|---|---|
| Arducam_Mega | [github.com/ArduCAM/Arducam_Mega](https://github.com/ArduCAM/Arducam_Mega) |
| SD (built-in) | Arduino Library Manager — included with Arduino IDE |
| STM32 Arduino core | [github.com/stm32duino/Arduino_Core_STM32](https://github.com/stm32duino/Arduino_Core_STM32) |

---

## Board selection in Arduino IDE

1. Install the STM32 core via **Boards Manager** (search *STM32*).
2. Select **STM32 boards (selected from submenu)** → **Nucleo-144** → **Nucleo F429ZI**.
3. Under **Tools → USB support**, select *CDC (generic Serial supersede U(S)ART)* if you want USB serial; otherwise use the ST-Link virtual COM port (PD8/PD9).
