/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f10x.h"

void delayInit(void);
void delayMs(uint16_t nms);
void delayUs(uint32_t nus);

#endif
