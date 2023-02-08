
/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */

#ifndef __ARDUINOHAL_H
#define __ARDUINOHAL_H

#include "ArducamSpi.h"

#define arducamSpiBegin()                 arducamSpiBegin()
#define arducamSpiTransfer(val)           arducamSpiTransfer(val) //  SPI communication sends a byte
#define arducamSpiReadBlock(p_val, count) arducamSpiTransferBlock(p_val, count)
#define arducamSpiCsPinHigh(pin)          arducamSpiCsHigh(pin) // Set the CS pin of SPI to high level
#define arducamSpiCsPinLow(pin)           arducamSpiCsLow(pin)  // Set the CS pin of SPI to low level
#define arducamCsOutputMode(pin)          arducamSpiCsOutputMode(pin)
#define arducamDelayMs(val)               arducamDelayMs(val) //  Delay Ms
#define arducamDelayUs(val)               arducamDelayUs(val) // Delay Us

#endif /*__ARDUINOHAL_H*/