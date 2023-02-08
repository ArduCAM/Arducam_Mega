
/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */

#ifndef __ARDUCAMSPI_H
#define __ARDUCAMSPI_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

void arducamSpiBegin(void);
uint8_t arducamSpiTransfer(uint8_t);
void arducamSpiCsOutputMode(int);
void arducamSpiCsHigh(int);
void arducamSpiCsLow(int);
void arducamDelayMs(uint16_t);
void arducamDelayUs(uint16_t);
void arducamSpiTransferBlock(uint8_t*,uint16_t);
#ifdef __cplusplus
}
#endif

#endif /*__ARDUCAMSPI_H*/
