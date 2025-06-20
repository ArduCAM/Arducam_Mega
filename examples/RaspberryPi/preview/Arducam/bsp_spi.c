/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#include "bsp_spi.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include<string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include <gpiod.h>

#ifndef	CONSUMER
#define	CONSUMER	"Consumer"
#endif

SPI_SET spidev_01 = {
  .spi_dev_path = "/dev/spidev0.1",
  .mode = 0x00,
  .bits = 8,
  .speed = 10000000, 
  .delayms = 1,
};    

// GPIO Pin
char *chipname = "gpiochip0";	        
struct timespec ts = { 1, 0 };
struct gpiod_chip *chip;
struct gpiod_line *line;

static void pabort(const char *s)
{
	perror(s);
	abort();
}

void spiBegin(void)
{
    spiDevFileOpen();
    spiModeSet(spidev_01.spi_fd, spidev_01.mode, spidev_01.speed, spidev_01.bits);
    pioInit();
}

unsigned char spiReadWriteByte(unsigned char data)
{
    unsigned char ret;
    transfer(spidev_01.spi_fd, &data, &ret, 1);
    return ret;
}


void spiBlockRead(uint8_t data, uint8_t *rx_buf, uint32_t len)
{
    transfer(spidev_01.spi_fd, &data, rx_buf, len);
}

void spiDevFileOpen(void)
{
	if ((spidev_01.spi_fd = open(spidev_01.spi_dev_path, O_RDWR | O_SYNC) ) < 0)
	      pabort("can't open spi device");
}

void spiModeSet(int fd, uint32_t spi_mode, uint32_t spi_speed, uint8_t bits_per_word)
{
  int ret;
  /* spi mode */
  ret = ioctl(fd, SPI_IOC_WR_MODE, &spi_mode);
	if (ret == -1)
		pabort("can't set spi mode");
  /* bits per word */
  ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word);
	if (ret == -1)
		pabort("can't set bits per word");
  /* max speed hz */
  ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
	if (ret == -1)
		pabort("can't set max speed hz");
}

static void transfer(int fd, uint8_t const *tx, uint8_t const *rx, uint32_t len)
{
  int ret;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = len,
		.delay_usecs = spidev_01.delayms,
		.speed_hz = spidev_01.speed,
		.bits_per_word = spidev_01.bits,
	};

	if (spidev_01.mode & SPI_TX_QUAD)
		tr.tx_nbits = 4;
	else if (spidev_01.mode & SPI_TX_DUAL)
		tr.tx_nbits = 2;
	if (spidev_01.mode & SPI_RX_QUAD)
		tr.rx_nbits = 4;
	else if (spidev_01.mode & SPI_RX_DUAL)
		tr.rx_nbits = 2;
	if (!(spidev_01.mode & SPI_LOOP)) {
		if (spidev_01.mode & (SPI_TX_QUAD | SPI_TX_DUAL))
			tr.rx_buf = 0;
		else if (spidev_01.mode & (SPI_RX_QUAD | SPI_RX_DUAL))
			tr.tx_buf = 0;
	}

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");
}

void pioInit(void) 
{
  chip = gpiod_chip_open_by_name(chipname);
  if (chip == NULL) {
	  perror("Open chip failed\n");
		gpiod_line_release(line);
    gpiod_chip_close(chip);
  }   
}

void digitalWrite(int pin, int val) 
{
  int ret;
  line = gpiod_chip_get_line(chip, pin);
	if (line == NULL) {
		perror("Get line failed\n");
 		gpiod_chip_close(chip);
	}
  ret = gpiod_line_set_value(line, val);
	if (ret < 0) {
		perror("Set line output failed\n");
		gpiod_line_release(line);
	}
}

void spiCsHigh(int pin)
{
  int ret;
  line = gpiod_chip_get_line(chip, pin);
	if (line == NULL) {
		perror("Get line failed\n");
 		gpiod_chip_close(chip);
	}

	ret = gpiod_line_set_value(line, HIGH);
	if (ret < 0) {
		perror("Set line output failed\n");
		gpiod_line_release(line);
	}
}
void spiCsLow(int pin)
{
  int ret;
  line = gpiod_chip_get_line(chip, pin);
	if (line == NULL) {
		perror("Get line failed\n");
 		gpiod_chip_close(chip);
	}

	ret = gpiod_line_set_value(line, LOW);
	if (ret < 0) {
		perror("Set line output failed\n");
		gpiod_line_release(line);
	}
}
void spiCsOutputMode(int pin)
{
  int ret;
  line = gpiod_chip_get_line(chip, pin);
	if (line == NULL) {
		perror("Get line failed\n");
 		gpiod_chip_close(chip);
	}

	ret = gpiod_line_request_output(line, CONSUMER, 0);
	if (ret < 0) {
		perror("Request line as output failed\n");
		gpiod_chip_close(chip);
	} 
}

void spiRelease(void)
{
    close(spidev_01.spi_fd);
}
void delayMs(unsigned int s)
{
    sleep(s/1000.0);
}
void delayUs(unsigned int us)
{
    usleep(us);
}
