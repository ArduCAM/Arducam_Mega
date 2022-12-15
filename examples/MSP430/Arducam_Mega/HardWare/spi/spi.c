/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#include "spi.h"


void spiBegin(void)
{
    //SPI Pins
    P1SEL |= BIT5 + BIT6 + BIT7;
    P1SEL2 |= BIT5 + BIT6 + BIT7;
    P1DIR |= BIT5 + BIT6 + BIT7;; // SPICLK, SOMI, and SIMO as outputs
    P1DIR &= ~BIT6; // SOMI as input

    //Clock Polarity: The inactive state is high
    //MSB First, 8-bit, Master, 3-pin mode, Synchronous
    UCB0CTL0 |= UCMSB + UCMST + UCSYNC + UCCKPH ;
    UCB0CTL1 |= UCSSEL_2 + UCSWRST ;                     // SMCLK
    UCB0BR0 |= 0x20;                          // /2
    UCB0BR1 = 0;                              //
    UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}


void spiCsOutputMode(unsigned int pin)
{
  //CS Pins
  CS_DIR |= pin;
  P1REN  |= pin;
  CS_OUT |= pin;

}

void spiCsHigh(unsigned int pin)
{
    CS_OUT |= pin;
}
void spiCsLow(unsigned int pin)
{
    CS_OUT &= ~pin;
}

unsigned char spiWriteRead(unsigned char val)
{
  unsigned char rt = 0;
//  spiCsLow(BIT0);
//  __delay_cycles(10);
  UCB0TXBUF = val;
  while ( (UCB0STAT & UCBUSY) );
  rt = UCB0RXBUF;
  while ( (UCB0STAT & UCBUSY) );
//  spiCsHigh(BIT0);
//  __delay_cycles(10);
  return rt;
}

