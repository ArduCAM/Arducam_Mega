#ifndef __CAM_SPI_MASTER_H
#define __CAM_SPI_MASTER_H
#include "stdio.h"
#include "pico/stdlib.h"
#include "stdint.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define CAM_SPI_PORT spi1

void spi_begin();
int spi_cs_output_mode(uint8_t pin);
uint8_t spi_write_read(uint8_t value);
void spi_cs_high(uint16_t pin);
void spi_cs_low(uint8_t pin);
int spi_write_block(uint8_t *p_value, size_t len);
int spi_read_block(uint8_t *p_value, size_t len);

#ifdef __cplusplus
}
#endif

#endif