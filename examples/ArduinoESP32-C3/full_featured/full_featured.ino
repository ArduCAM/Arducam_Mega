// Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
// License: MIT License (https://en.wikipedia.org/wiki/MIT_License)
// Web: http://www.ArduCAM.com
// This program is a demo of how to use most of the functions
// of the library with ArduCAM Spi camera, and can run on ESP32 Node platform.
// This demo was made for ArduCAM Spi Camera.
// It needs to be used in combination with PC software.
// It can test ArduCAM Spi Camerafunctions
#include "Arducam.hpp"
#include "Platform.h"
#include "ArducamLink.h"

const int CS = 1;
Arducam myCAM( CS );
ArducamLink myUart;
uint8_t temp = 0xff;
uint8_t sendFlag = TRUE;
uint8_t commandBuff[20] = {0};
uint8_t commandLength = 0;
uint32_t readImageLength = 0;
uint8_t jpegHeadFlag = 0;
uint8_t readBuffer(uint8_t* imagebuf,uint8_t length)
{
  if(myUart.arducamUartAvailable())
  {
    temp = myUart.arducamUartRead();
    arducamDelayMs(1);
    if (temp == 0x55)
    {
      while (myUart.arducamUartAvailable())
      {
        commandBuff[commandLength] = myUart.arducamUartRead();
        if (commandBuff[commandLength] == 0xAA)
        {
          break;
        }
        commandLength++;
      }
      if(commandBuff[0]==0x21)
      {
        readImageLength = 0;
        jpegHeadFlag = 0;
        myUart.arducamUartWrite(0x55);
        myUart.arducamUartWrite(0xBB);
        return FALSE;
      }
      commandLength=0;
    }
  }
  if (imagebuf[0] == 0xff && imagebuf[1] == 0xd8)
  {
    jpegHeadFlag = 1;
    myUart.arducamUartWrite(0x55);
    myUart.arducamUartWrite(0xAA);
    myUart.arducamUartWrite(CAM_IMAGE_PIX_FMT_JPG);
    myUart.arducamUartWrite((uint8_t)(myCAM.getTotalLength()&0xff));
    myUart.arducamUartWrite((uint8_t)((myCAM.getTotalLength()>>8)&0xff));
    myUart.arducamUartWrite((uint8_t)((myCAM.getTotalLength()>>16)&0xff));
    myUart.arducamUartWrite((uint8_t)((myCAM.getTotalLength()>>24)&0xff));
  }
  if (jpegHeadFlag == 1)
  {
    readImageLength += length;
    myUart.arducamUartWriteBuff(imagebuf,length);
  }
  if (readImageLength == myCAM.getTotalLength())
  {
    readImageLength = 0;
    jpegHeadFlag = 0;
    myUart.arducamUartWrite(0x55);
    myUart.arducamUartWrite(0xBB);
  }
  return sendFlag;
}

void setup() {
  myUart.arducamUartBegin(921600);
  myCAM.begin();
  myCAM.registerCallBack(readBuffer,200);
}

 void loop() {
  if (myUart.arducamUartAvailable())
  {
    temp = myUart.arducamUartRead();
    arducamDelayMs(5);
    if (temp == 0x55)
    {
      while (myUart.arducamUartAvailable())
      {
        commandBuff[commandLength] = myUart.arducamUartRead();
        if (commandBuff[commandLength] == 0xAA)
        { 
          break;
        }
        commandLength++;
      }    
      myUart.arducamFlush();
      myUart.uartCommandProcessing(&myCAM,commandBuff);
      commandLength = 0;
   }    
  }
}


