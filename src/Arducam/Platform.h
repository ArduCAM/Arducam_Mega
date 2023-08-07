
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

#if defined(STM32F10X_MD)
#include "Stm32Hal.h"
#elif defined(__AVR__) || defined(__SAM3X8E__) || defined(ESP32) || defined(ESP8266) ||                                \
    defined(ARDUINO_ARCH_NRF52840) || defined(ARDUINO_ARCH_RP2040) || defined(NRF52833_XXAA) || defined(ARDUINO_ARCH_RENESAS)
#include "ArduinoHal.h"
#elif defined(__MSP430G2553__)
#include "Msp430Hal.h"
#elif defined(__XC32)
#include "Same54Hal.h"
#elif defined(PICO_BOARD)
#include "picoHal.h"
#elif defined(__PLATFORM_RASPBERRY)
#include "raspberryHal.h"
#elif defined(ESP_PLATFORM)
#include "esp32Hal.h"
#endif
#endif /*__PLATFORM_H*/
