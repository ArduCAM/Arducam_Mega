/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */


#ifndef  __RASPBERRYHAL_H
#define  __RASPBERRYHAL_H

#include "bsp_spi.h"


#define arducamSpiBegin()                    spiBegin()
#define arducamSpiTransfer(val)              spiReadWriteByte(val)    //  SPI communication sends a byte
#define arducamSpiCsPinHigh(pin)             spiCsHigh(pin)    //Set the CS pin of SPI to high level
#define arducamSpiCsPinLow(pin)              spiCsLow(pin)  //Set the CS pin of SPI to low level
#define arducamCsOutputMode(pin)             spiCsOutputMode(pin)
#define arducamDelayMs(val)                  delayMs(val)           //  Delay Ms
#define arducamDelayUs(val)                  delayUs(val) //Delay Us

#endif /*__RASPBERRYHAL_H*/

