#ifndef __SPI_H
#define __SPI_H

#include <stdio.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"


#define CAMERA_HOST  SPI2_HOST
#define PIN_NUM_MISO 5
#define PIN_NUM_MOSI 6
#define PIN_NUM_CLK  4
#define CAMERA_SPI_CLK_FREQ         (8*1000*1000)   


void spiBegin(void);
void spiCsOutputMode(int cs);
void spiCsHigh(int cs);
void spiCsLow(int cs);
uint8_t spiReadWriteByte(uint8_t val);

#endif /*__SPI_H*/