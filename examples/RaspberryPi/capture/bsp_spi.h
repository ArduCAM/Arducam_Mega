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

#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"


/////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////
// GPIO FSEL Types
#define INPUT  0
#define OUTPUT 1

/////////////////////////////////////////////////////////////////////
// GPIO Functions
/////////////////////////////////////////////////////////////////////
#define HIGH  1
#define LOW   0

/////////////////////////////////////////////////////////////////////
// SPI Setting
/////////////////////////////////////////////////////////////////////
typedef struct spi_setting
{
  int spi_fd;
  char *spi_dev_path;
  uint32_t mode;
  uint8_t bits;
  uint32_t speed;
  uint16_t delayms;  
}SPI_SET;

void spiBegin(void);
unsigned char spiReadWriteByte(unsigned char);
void spiCsHigh(int);
void spiCsLow(int);
void spiCsOutputMode(int);
void delayMs(unsigned int);
void delayUs(unsigned int);

void spiRelease(void);
void pioInit(void);
void digitalWrite(int pin, int val);
void spiDevFileOpen(void);
static void transfer(int fd, uint8_t const *tx, uint8_t const *rx, uint32_t len);
void spiModeSet(int fd, uint32_t spi_mode, uint32_t spi_speed, uint8_t bits_per_word);
void spiBlockRead(uint8_t data, uint8_t *rx_buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /*__BSP_SPI_H*/