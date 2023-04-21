/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */

#ifndef __PLATFORM_H
#define __PLATFORM_H
#include "common_init.h"

#define arducamSpiBegin()                 spi_master_init()
#define arducamSpiTransfer(val)           spi_transfer(val) //  SPI communication sends a byte
#define arducamSpiReadBlock(p_val, count) spi_transfer_block(p_val, count)
#define arducamSpiCsPinHigh(pin)          R_IOPORT_PinWrite(&g_ioport_ctrl,pin,BSP_IO_LEVEL_HIGH)
#define arducamSpiCsPinLow(pin)           R_IOPORT_PinWrite(&g_ioport_ctrl,pin,BSP_IO_LEVEL_LOW) // Set the CS pin of SPI to low level
#define arducamCsOutputMode(pin)          R_IOPORT_PinCfg(&g_ioport_ctrl, pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
#define arducamDelayMs(val)               R_BSP_SoftwareDelay(val, BSP_DELAY_UNITS_MILLISECONDS)
#define arducamDelayUs(val)               R_BSP_SoftwareDelay(val, BSP_DELAY_UNITS_MICROSECONDS) // Delay Us

#endif /*__PLATFORM_H*/
