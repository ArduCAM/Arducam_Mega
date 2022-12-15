/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#ifndef  __SPI_H
#define  __SPI_H

#include <msp430.h>

#define CS_OUT    P2OUT
#define CS_DIR    P2DIR
#define CS_PIN    BIT0

void spiBegin(void);
void spiCsOutputMode(unsigned int pin);
void spiCsHigh(unsigned int pin);
void spiCsLow(unsigned int pin);
unsigned char spiWriteRead(unsigned char val);
#endif /*__SPI_H*/
