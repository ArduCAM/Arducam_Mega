
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "uart.h"
#include "Arducam.h"

ArducamCamera myCAM;
const int cs = 1;

uint8_t temp = 0xff;
uint8_t commandBuff[20] = {0};
uint8_t commandLength = 0;
uint8_t sendFlag = TRUE;
uint32_t readImageLength = 0;
uint8_t jpegHeadFlag = 0;

uint8_t ReadBuffer(uint8_t* imagebuf,uint8_t length)
{
	
  if(arducamUartAvailable())
  {
    temp = arducamUartRead();
    arducamDelayMs(1);
    if (temp == 0x55)
    {
      while (arducamUartAvailable())
      {
        commandBuff[commandLength] = arducamUartRead();
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
        arducamUartWrite(0x55);
        arducamUartWrite(0xBB);
        return FALSE;
      }
      commandLength=0;
    }
  }
  if (imagebuf[0] == 0xff && imagebuf[1] == 0xd8)
  {
    jpegHeadFlag = 1;
    arducamUartWrite(0x55);
    arducamUartWrite(0xAA);
    arducamUartWrite(myCAM.cameraDataFormat);
    arducamUartWrite((uint8_t)(myCAM.totalLength&0xff));
    arducamUartWrite((uint8_t)((myCAM.totalLength>>8)&0xff));
    arducamUartWrite((uint8_t)((myCAM.totalLength>>16)&0xff));
    arducamUartWrite((uint8_t)((myCAM.receivedLength>>24)&0xff));
  }
  if (jpegHeadFlag == 1)
  {
    readImageLength += length;
    uartWriteBuffer(imagebuf,length);
  }
  if (readImageLength == myCAM.totalLength)
  {
    readImageLength = 0;
    jpegHeadFlag = 0;
    arducamUartWrite(0x55);
    arducamUartWrite(0xBB);
  }
  return sendFlag;

}


void app_main(void)
{
  uartBegin(921600);
  myCAM = createArducamCamera(cs);
  begin(&myCAM);
	registerCallback(&myCAM,ReadBuffer,200);
    while (1)
    {   
      if (arducamUartAvailable())
      {
        temp = arducamUartRead();
        if (temp == 0x55)
        {
          while (arducamUartAvailable())
          {
            commandBuff[commandLength] = arducamUartRead();
            if (commandBuff[commandLength] == 0xAA)
            { 
              break;
            }
            commandLength++;
          }    
          commandProcessing(&myCAM,commandBuff,commandLength);
          commandLength = 0;
        }    
      }
    }

    
}
