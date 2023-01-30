/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"

void spiBegin(void);
uint8_t spiReadWriteByte(uint8_t TxData);
void spiCsLow(int pin);
void spiCsHigh(int pin);
void spiCsOutputMode(int pin);
#endif
