#include "Arducam_Mega.h"
#include <pico/stdlib.h>
#include "ArducamLink.h"
#include <cstdio>

const int CS = 17;
Arducam_Mega myCAM(CS);
uint8_t temp             = 0xff;
uint8_t sendFlag         = TRUE;
uint8_t commandBuff[20]  = {0};
uint8_t commandLength    = 0;
uint32_t readImageLength = 0;
uint8_t jpegHeadFlag     = 0;

ArducamLink myUart;

uint8_t readBuffer(uint8_t* imagebuf, uint8_t length)
{
    if (imagebuf[0] == 0xff && imagebuf[1] == 0xd8) {
        jpegHeadFlag    = 1;
        readImageLength = 0;
        myUart.arducamUartWrite(0xff);
        myUart.arducamUartWrite(0xAA);
        myUart.arducamUartWrite(0x01);

        myUart.arducamUartWrite((uint8_t)(myCAM.getTotalLength() & 0xff));
        myUart.arducamUartWrite((uint8_t)((myCAM.getTotalLength() >> 8) & 0xff));
        myUart.arducamUartWrite((uint8_t)((myCAM.getTotalLength() >> 16) & 0xff));
        myUart.arducamUartWrite((uint8_t)((myCAM.getTotalLength() >> 24) & 0xff));
        myUart.arducamUartWrite(((CAM_IMAGE_PIX_FMT_JPG & 0x0f) << 4) | 0x01);
    }
    if (jpegHeadFlag == 1) {
        readImageLength += length;
        for (uint8_t i = 0; i < length; i++) {
            myUart.arducamUartWrite(imagebuf[i]);
        }
    }
    if (readImageLength == myCAM.getTotalLength()) {
        jpegHeadFlag = 0;
        myUart.arducamUartWrite(0xff);
        myUart.arducamUartWrite(0xBB);
    }
    return sendFlag;
}

void stop_preivew()
{
    readImageLength = 0;
    jpegHeadFlag    = 0;
    uint32_t len    = 9;
    myUart.arducamUartWrite(0xff);
    myUart.arducamUartWrite(0xBB);
    myUart.arducamUartWrite(0xff);
    myUart.arducamUartWrite(0xAA);
    myUart.arducamUartWrite(0x06);
    myUart.arducamUartWriteBuff((uint8_t*)&len, 4);
    myUart.printf(const_cast<char*>("streamoff"));
    myUart.arducamUartWrite(0xff);
    myUart.arducamUartWrite(0xBB);
}

int main()
{
    stdio_init_all();
    myUart.send_data_pack(7,const_cast<char*>("Hello Raspberry Pi Pico!"));
    myCAM.begin();
    myUart.send_data_pack(8,const_cast<char*>("Mega start!"));
    myCAM.registerCallBack(readBuffer, 200, stop_preivew);
    while (true) {
        if (myUart.arducamUartAvailable()) {
        temp = myUart.arducamUartRead();
        // sleep_(5);
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
        myCAM.captureThread();
    }
}