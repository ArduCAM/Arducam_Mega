#ifndef _LCD_SPI_INTERFACE_H_
#define _LCD_SPI_INTERFACE_H_
#include "hardware/spi.h"
#ifdef __cplusplus
extern "C" {
#endif
#define LCD_SPI_PORT spi0

// #define LCD_RST_PIN  10
// #define LCD_DC_PIN   11
// #define LCD_CS_PIN 12

// #define LCD_CS_PIN   20
#define LCD_DC_PIN   21
#define LCD_RST_PIN  22

inline unsigned char lcd_spi_transport_byte(unsigned char wdata) {
  unsigned char rdata;
  spi_write_read_blocking(LCD_SPI_PORT, &wdata, &rdata, 1);
  return rdata;
}
inline unsigned char lcd_spi_write_block(const unsigned char *wdata,
                                         size_t count) {
  unsigned char rdata;
  spi_write_blocking(LCD_SPI_PORT, wdata, count);
}
#ifdef __cplusplus
}
#endif
#endif