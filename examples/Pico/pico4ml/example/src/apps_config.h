#pragma once

#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "stdio.h"
#ifdef __cplusplus
extern "C" {
#endif

#define UART_ID         uart0
#define UART_IRQ        UART0_IRQ
#define DATA_BITS       8
#define STOP_BITS       1
#define PARITY          UART_PARITY_NONE

// #define PICO4ML_SPI_PORT spi0
// #define PICO4ML_MISO_PIN 16
// #define PICO4ML_CLK_PIN  18
// #define PICO4ML_MOSI_PIN 19

#define CAMERA_SPI_PORT spi1
#define CAMERA_MISO_PIN 12
#define CAMERA_MOSI_PIN 11
#define CAMERA_SCK_PIN  10

#define LCD_SPI_PORT    spi0
#define LCD_MISO_PIN    16
#define LCD_MOSI_PIN    19
#define LCD_SCK_PIN     18

extern void pico4ml_init(int irq_flag);
extern void pico4ml_camera_spi_init(uint32_t spi_baudrate);
#ifdef __cplusplus
}
#endif
