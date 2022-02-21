#include "delay.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

void delayMs(uint32_t ms)
{
    vTaskDelay(ms);
}
void delayUs(uint32_t us)
{
    vTaskDelay(us);
}