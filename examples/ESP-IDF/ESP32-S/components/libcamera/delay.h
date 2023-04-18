#ifndef __DELAY_H
#define __DELAY_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#define delayMs(ms) vTaskDelay((ms) / portTICK_PERIOD_MS)

#define delayUs(us) usleep((us) /portTICK_PERIOD_MS/1000)

#endif