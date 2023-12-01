// Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
// License: MIT License (https://en.wikipedia.org/wiki/MIT_License)
// Web: http://www.ArduCAM.com
// This program is a demo of how to use most of the functions
// It needs to be used in combination with PC software.
// The demo support low power mode.
// The demo sketch will do the following tasks:
// 1. Set the camera to JPEG output mode.
// 2. Read data from Serial port and deal with it
// 3. If receive 0x10,camera will capture a JPEG photo and buffer the image to FIFO.Then write datas to Serial port.
#include "Arducam_Mega.h"
// #include "Platform.h"
#include "ArducamLink.h"

const int CS = 7;
Arducam_Mega myCAM( CS );
ArducamLink myUart;
uint8_t temp = 0xff;
uint8_t sendFlag = TRUE;
uint8_t commandBuff[20] = {0};
uint8_t commandLength = 0;
uint32_t readImageLength = 0;
uint8_t jpegHeadFlag = 0;

void setup() {
  myUart.arducamUartBegin(115200);
  myCAM.begin();
  myCAM.lowPowerOn();
}

 void loop() {
  if (myUart.arducamUartAvailable())
  {
    temp = myUart.arducamUartRead();
    delay(5);
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
      myCAM.lowPowerOff();
      delay(500);     //If the camera is 3MP, you need to lengthen the delay
      myUart.uartCommandProcessing(&myCAM,commandBuff);
      myCAM.lowPowerOn();
      commandLength = 0;
   }    
  }
}


