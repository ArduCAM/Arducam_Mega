
/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for
 * details.
 *
 */

#include "ArducamSpi.h"
#include <Arduino.h>
#include <SPI.h>

void arducamSpiBegin(void)
{
    return SPI.begin();
}

uint8_t arducamSpiTransfer(uint8_t data)
{
    return SPI.transfer(data);
}

void arducamSpiTransferBlock(uint8_t *buff,uint16_t len){
    return SPI.transfer(buff,len);
}

void arducamSpiCsHigh(int pin)
{
    digitalWrite(pin, HIGH);
}

void arducamSpiCsLow(int pin)
{
    digitalWrite(pin, LOW);
}

void arducamSpiCsOutputMode(int pin)
{
    pinMode(pin, OUTPUT);
}

void arducamDelayMs(uint16_t num)
{
    delay(num);
}
void arducamDelayUs(uint16_t num)
{
    delayMicroseconds(num);
}
