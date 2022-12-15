/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#include "delay.h"
#include "intrinsics.h"

void delayUs(unsigned long us)
{
    unsigned long count = 0;
    for(count = 0 ; count < us; count++)
    {
        __delay_cycles(1);
    }

}
void delayMs(unsigned long ms)
{
    unsigned long count = 0;
    for(count = 0 ; count < ms*333; count++)
    {
        __delay_cycles(1);
    }
}
