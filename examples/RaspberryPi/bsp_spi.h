/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#ifndef  __BSP_SPI_H
#define  __BSP_SPI_H

#include "stdint.h"

void spiBegin(void);
void spiRelease(void);
unsigned char spiReadWriteByte(unsigned char);
void spiCsHigh(int);
void spiCsLow(int);
void spiCsOutputMode(int);
void delayMs(unsigned int);
void delayMs(unsigned int);


#endif /*__BSP_SPI_H*/