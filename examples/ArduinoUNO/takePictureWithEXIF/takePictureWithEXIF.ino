// Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
// License: MIT License (https://en.wikipedia.org/wiki/MIT_License)
// Web: http://www.ArduCAM.com
// This program is a demo of how to use most of the functions
// of the library with ArduCAM Spi camera, and can run on Arduino platform.
// This demo was made for ArduCAM Spi Camera.
// It needs to be used in combination with PC software.
// It can test ArduCAM Spi Camerafunctions
#include "ArducamLink.h"
#include "Arducam_Mega.h"
#include <SPI.h>

const int CAM_CS = 7;
Arducam_Mega myCAM(CAM_CS);
ArducamLink myUart;

uint8_t commandBuff[20]  = {0};
uint8_t temp             = 0xff;
uint8_t commandLength    = 0;

void setup()
{
    SPI.begin();
    myUart.arducamUartBegin(115200);
    myUart.send_data_pack(7, "Hello Arduino UNO!");
    myCAM.begin();
    myUart.send_data_pack(8, "Mega start!");
    myCAM.takePicture((CAM_IMAGE_MODE)0x00, (CAM_IMAGE_PIX_FMT)0x01);
}

void loop()
{
    if (myUart.arducamUartAvailable()) {
        temp = myUart.arducamUartRead();
        delay(5);
        if (temp == 0x55) {
            while (myUart.arducamUartAvailable()) {
                commandBuff[commandLength] = myUart.arducamUartRead();
                if (commandBuff[commandLength] == 0xAA) {
                    break;
                }
                commandLength++;
            }
            myUart.arducamFlush();
            myUart.uartCommandProcessing(&myCAM, commandBuff);
            commandLength = 0;
        }
    }
}