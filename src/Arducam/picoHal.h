
/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for
 * details.
 *
 */

#ifndef __PICO_HAL_H
#define __PICO_HAL_H

#include "cam_spi_master.h"

#define arducamSpiBegin()                   spi_begin()
#define arducamSpiTransfer(val)             spi_write_read(val)
#define arducamSpiReadBlock(ptr_val, count) spi_read_block(ptr_val, count)
#define arducamSpiCsPinHigh(pin)            spi_cs_high(pin)
#define arducamSpiCsPinLow(pin)             spi_cs_low(pin)
#define arducamCsOutputMode(pin)            spi_cs_output_mode(pin)
#define arducamDelayMs(val)                 sleep_ms(val)
#define arducamDelayUs(val)                 sleep_us(val)

#endif /*__PICO_HAL_H*/
