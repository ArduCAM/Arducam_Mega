#ifndef __ARDUCAM_SLOT_H
#define __ARDUCAM_SLOT_H
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "uart.h"

#define SerialBegin(baudRate)     uartBegin(baudRate)
#define SerialWrite(ch)           uartWrite(ch)
#define SerialWriteBuff(buf, len) uartWriteBuffer(buf, len)
#define SerialPrintf(str)         print(str)
#define SerialAvailable()         uartAvailable()
#define SerialRead()              uartRead()
#define delayUs(us)               vTaskDelay((us) / portTICK_PERIOD_MS / 1000)
#endif