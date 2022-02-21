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

int fb;
const char* spiDevice = "/dev/spidev0.0";
static unsigned char bits = 8;
static unsigned int  speed = 20000000;


void spiBegin(void)
{
    unsigned int mode = SPI_MODE_0;
    int ret = 0;
    if ((fb = open(spiDevice,O_RDWR)) < 0)
    {
        printf("spiDevice open failed\r\n");
        return;
    }
	/*
	 * spi mode
	 */
	ret = ioctl(fb, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		printf("can't set spi mode");

	ret = ioctl(fb, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		printf("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fb, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		printf("can't set bits per word");

	ret = ioctl(fb, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		printf("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fb, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		printf("can't set max speed hz");

	ret = ioctl(fb, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		printf("can't get max speed hz");
}
unsigned char spiReadWriteByte(unsigned char data)
{
    unsigned char tx_data[1] = {0};
    unsigned char rx_data[1] = {0};
    int i = 0;
    struct spi_ioc_transfer spiDev ;
    tx_data[0] = data;
    memset (&spiDev,0, sizeof(spiDev)) ;
    spiDev.tx_buf        = (unsigned long)tx_data ;
    spiDev.rx_buf        = (unsigned long)rx_data ;
    spiDev.len           = 1 ;
    spiDev.delay_usecs   = 0 ;
    spiDev.speed_hz      = speed ;
    spiDev.bits_per_word = bits ;
    ioctl(fb, SPI_IOC_MESSAGE(1), &spiDev);
    return rx_data[0];
}

void spiCsHigh(int pin)
{
    FILE * fp;
    char command[30] = {0};
    sprintf(command,"raspi-gpio set %d dh",pin);
    fp=popen(command,"r");
    pclose(fp);
}
void spiCsLow(int pin)
{
    FILE * fp;
    char command[30] = {0};
    sprintf(command,"raspi-gpio set %d dl",pin);
    fp=popen(command,"r");
    pclose(fp);
}
void spiCsOutputMode(int pin)
{
    FILE * fp;
    char command[30] = {0};
    sprintf(command,"raspi-gpio set %d op",pin);
    fp=popen(command,"r");
    pclose(fp);
}

void spiRelease(void)
{
    close(fb);
}
void delayMs(unsigned int s)
{
    sleep(s/1000.0);
}
void delayUs(unsigned int us)
{
    usleep(us);
}
