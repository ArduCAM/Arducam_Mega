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




int fb;
static unsigned int  speed = 4000000;
// Pointers that will be memory mapped when pioInit() is called
volatile unsigned int *gpio; //pointer to base of gpio
volatile unsigned int *spi;  //pointer to base of spi registers

void spiBegin(void)
{
    pioInit();
    //set GPIO 8 (CE), 9 (MISO), 10 (MOSI), 11 (SCLK) alt fxn 0 (SPI0)
    //pinMode(8, ALT0);
    SPI0CSbits.TA = 0;          // turn SPI on with the "transfer active" bit
    pinMode(9, ALT0);
    pinMode(10, ALT0);
    pinMode(11, ALT0);

    //Note: clock divisor will be rounded to the nearest power of 2
    SPI0CLK = 250000000/speed;   // set SPI clock to 250MHz / freq
    SPI0CS = 0;  
	SPI0CSbits.CLEAR = 3;   // this is very important 
    SPI0CSbits.TA = 1;          // turn SPI on with the "transfer active" bit
}
unsigned char spiReadWriteByte(unsigned char data)
{
	SPI0FIFO = data;            // send data to slave
	while(!SPI0CSbits.DONE);	// wait until SPI transmission complete
    return SPI0FIFO;            // return received data
}

void pioInit(void) 
{
	int  mem_fd;
	void *reg_map;

	// /dev/mem is a psuedo-driver for accessing memory in the Linux filesystem
	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
	      printf("can't open /dev/mem \n");
	      exit(-1);
	}

	reg_map = mmap(
	  NULL,             //Address at which to start local mapping (null means don't-care)
      BLOCK_SIZE,       //Size of mapped memory block
      PROT_READ|PROT_WRITE,// Enable both reading and writing to the mapped memory
      MAP_SHARED,       // This program does not have exclusive access to this memory
      mem_fd,           // Map to /dev/mem
      GPIO_BASE);       // Offset to GPIO peripheral

	if (reg_map == MAP_FAILED) {
      printf("gpio mmap error %d\n", (int)reg_map);
      close(mem_fd);
      exit(-1);
    }

	gpio = (volatile unsigned *)reg_map;

    reg_map = mmap(
	  NULL,             //Address at which to start local mapping (null means don't-care)
      BLOCK_SIZE,       //Size of mapped memory block
      PROT_READ|PROT_WRITE,// Enable both reading and writing to the mapped memory
      MAP_SHARED,       // This program does not have exclusive access to this memory
      mem_fd,           // Map to /dev/mem
      SPI0_BASE);       // Offset to SPI peripheral

    if (reg_map == MAP_FAILED) {
      printf("spi mmap error %d\n", (int)reg_map);
      close(mem_fd);
      exit(-1);
    }

    spi = (volatile unsigned *)reg_map;

	close(mem_fd);
}

void pinMode(int pin, int function) 
{
    int reg      =  pin/10;
    int offset   = (pin%10)*3;
    GPFSEL[reg] &= ~((0b111 & ~function) << offset);
    GPFSEL[reg] |=  ((0b111 &  function) << offset);
}

void digitalWrite(int pin, int val) 
{
    int reg = pin / 32;
    int offset = pin % 32;

    if (val) GPSET[reg] = 1 << offset;
    else     GPCLR[reg] = 1 << offset;
}


void spiCsHigh(int pin)
{
    digitalWrite(pin,HIGH);
}
void spiCsLow(int pin)
{
    digitalWrite(pin,LOW);
}
void spiCsOutputMode(int pin)
{
    pinMode(pin, OUTPUT);   
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
