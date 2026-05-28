# capture2SD — Arducam Mega on STM32F429 (NUCLEO-F429ZI)

Captures JPEG photos with an Arducam Mega camera and saves them to a FAT-formatted SD card. A new numbered file (`0.jpg`, `1.jpg`, …) is written every 5 seconds.

## Requirements

- Board: STM32F429 (e.g. **NUCLEO-F429ZI**)
- [Arducam_Mega](https://github.com/ArduCAM/Arducam_Mega) library
- Arduino built-in **SD** library

## Wiring

Both the camera and the SD card module share **SPI1** on its alternate pins. Each device has its own CS line.

| Signal      | STM32 Pin | Notes                        |
|-------------|-----------|------------------------------|
| SPI1 SCK    | PB3       | Shared by camera and SD card |
| SPI1 MISO   | PB4       | Shared by camera and SD card |
| SPI1 MOSI   | PB5       | Shared by camera and SD card |
| Camera CS   | PE7       | `CS` in sketch               |
| SD card CS  | PB12      | `SD_CS` in sketch — **change to match your wiring** |
| UART TX     | PD8       | ST-Link virtual COM (USART3) |
| UART RX     | PD9       | ST-Link virtual COM (USART3) |

> **Using a different SD CS pin?** Change the `SD_CS` constant at the top of `capture2SD.ino`.

## Using a separate SPI bus for the SD card

The standard Arduino SD library only supports the default `SPI` object. If you need the SD card on a physically separate SPI bus (e.g. SPI2), install the **SdFat** library (Arduino Library Manager → search *SdFat* by Bill Greiman) and replace the SD setup with:

```cpp
#include <SdFat.h>

SPIClass SPI_SD(PB_15, PB_14, PB_13);  // MOSI, MISO, SCK (SPI2)
const int SD_CS = PB_12;

SdFat SD;

// In setup():
SD.begin(SdSpiConfig(SD_CS, DEDICATED_SPI, SD_SCK_MHZ(25), &SPI_SD));
```

## Serial output

Open the Serial Monitor at **115200 baud** to see status messages:

```
SD Card detected.
Image save succeed
Image save succeed
...
```

## How it works

1. `setup()` configures SPI1 alternate pins, initializes the camera, then mounts the SD card.
2. `loop()` calls `takePicture()` every 5 seconds, streams the JPEG byte-by-byte, detects the `FF D8` start-of-image and `FF D9` end-of-image markers, and writes the data in 255-byte chunks to a new file on the SD card.
