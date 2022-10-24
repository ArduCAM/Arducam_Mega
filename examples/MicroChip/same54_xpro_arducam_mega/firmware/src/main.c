/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "delay.h"
#include "uart.h"
#include "spi.h"
#include "stdio.h"
#include "../../../../../src/Arducam/ArducamCamera.h"
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
int CS = -1;
uint8_t temp = 0xff;
uint8_t commandBuff[20] = {0};
uint8_t commandLength = 0;
uint8_t sendFlag = TRUE;
uint32_t readImageLength = 0;
uint8_t jpegHeadFlag = 0;
ArducamCamera myCAM;

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
    for (uint8_t i = 0; i < length; i++)
    {
        arducamUartWrite(imagebuf[i]);
    }
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
int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    delay_init();
    uart_init();
    spiBegin();
    myCAM = createArducamCamera(CS);
    begin(&myCAM);
    registerCallback(&myCAM,ReadBuffer,200);
    while(true)
	{
		if (arducamUartAvailable())
		{
			arducamDelayMs(10);
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

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

