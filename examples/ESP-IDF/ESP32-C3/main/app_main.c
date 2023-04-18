
#include "ArducamCamera.h"
#include "ArducamLink.h"
#include "delay.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ArducamCamera myCAM;
const int cs = 1;

uint8_t temp             = 0xff;
uint8_t commandBuff[20]  = {0};
uint8_t commandLength    = 0;
uint8_t sendFlag         = TRUE;
uint32_t readImageLength = 0;
uint8_t jpegHeadFlag     = 0;

uint8_t ReadBuffer(uint8_t* imagebuf, uint8_t length)
{
    if (imagebuf[0] == 0xff && imagebuf[1] == 0xd8) {
        jpegHeadFlag    = 1;
        readImageLength = 0;
        arducamUartWrite(0xff);
        arducamUartWrite(0xAA);
        arducamUartWrite(0x01);
        arducamUartWrite((uint8_t)(myCAM.totalLength & 0xff));
        arducamUartWrite((uint8_t)((myCAM.totalLength >> 8) & 0xff));
        arducamUartWrite((uint8_t)((myCAM.totalLength >> 16) & 0xff));
        arducamUartWrite((uint8_t)((myCAM.receivedLength >> 24) & 0xff));
        arducamUartWrite(myCAM.currentPixelFormat);
    }
    if (jpegHeadFlag == 1) {
        readImageLength += length;
        uartWriteBuffer(imagebuf, length);
    }
    if (readImageLength == myCAM.totalLength) {
        jpegHeadFlag = 0;
        arducamUartWrite(0xff);
        arducamUartWrite(0xBB);
    }
    return sendFlag;
}

void stop_preivew()
{
    readImageLength = 0;
    jpegHeadFlag    = 0;
    uint32_t len    = 9;

    arducamUartWrite(0xff);
    arducamUartWrite(0xBB);
    arducamUartWrite(0xff);
    arducamUartWrite(0xAA);
    arducamUartWrite(0x06);
    arducamUartWriteBuff((uint8_t*)&len, 4);
    printf("streamoff");
    arducamUartWrite(0xff);
    arducamUartWrite(0xBB);
}

void app_main(void)
{
    uartBegin(921600);
    send_data_pack(7,"Hello esp32s!");
    myCAM = createArducamCamera(cs);
    begin(&myCAM);
    send_data_pack(8,"Mega start!");

    registerCallback(&myCAM, ReadBuffer, 200, stop_preivew);
    while (1) {
        if (arducamUartAvailable()) {
            temp = arducamUartRead();
            if (temp == 0x55) {
                while (arducamUartAvailable()) {
                    commandBuff[commandLength] = arducamUartRead();
                    if (commandBuff[commandLength] == 0xAA) {
                        break;
                    }
                    commandLength++;
                }

                uartCommandProcessing(&myCAM, commandBuff);
                commandLength = 0;
            }
        }
        captureThread(&myCAM);
        vTaskDelay(20/portTICK_PERIOD_MS);
    }
}
