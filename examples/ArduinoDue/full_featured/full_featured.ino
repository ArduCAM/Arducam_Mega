// Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
// License: MIT License (https://en.wikipedia.org/wiki/MIT_License)
// Web: http://www.ArduCAM.com
// This program is a demo of how to use most of the functions
// of the library with ArduCAM Spi camera, and can run on Arduino platform.
// This demo was made for ArduCAM Spi Camera.
// It needs to be used in combination with PC software.
// It can test ArduCAM Spi Camerafunctions
#include "Arducam_Mega.h"
// #include "Platform.h"
#include "ArducamLink.h"

const int CS = 7;
Arducam_Mega myCAM(CS);
ArducamLink myUart;
uint8_t temp = 0xff;
uint8_t sendFlag = TRUE;
uint8_t commandBuff[20] = { 0 };
uint8_t commandLength = 0;
uint32_t readImageLength = 0;
uint8_t jpegHeadFlag = 0;
uint8_t readBuffer(uint8_t* imagebuf, uint8_t length) {
  	CamStatus state;
	uint16_t gainValue = 0;
	uint32_t exposureValue = 0;
	uint32_t exposureLen1 = 0;
	uint32_t exposureLen2 = 0;
	uint32_t exposureLen3 = 0;
  if (myUart.arducamUartAvailable()) {
    temp = myUart.arducamUartRead();
    arducamDelayMs(1);
    if (temp == 0x55) {
      while (myUart.arducamUartAvailable()) {
        commandBuff[commandLength] = myUart.arducamUartRead();
        if (commandBuff[commandLength] == 0xAA) {
          break;
        }
        commandLength++;
      }
      if (commandBuff[0] == 0x21) {
        readImageLength = 0;
        jpegHeadFlag = 0;
        myUart.arducamUartWrite(0x55);
        myUart.arducamUartWrite(0xBB);
        return FALSE;
      }
      switch (commandBuff[0]) {
        case SET_BRIGHTNESS:  //Set brightness
          myCAM.setBrightness((CAM_BRIGHTNESS_LEVEL)commandBuff[1]);
          break;
        case SET_CONTRAST:  //Set Contrast
          myCAM.setContrast((CAM_CONTRAST_LEVEL)commandBuff[1]);
          break;
        case SET_SATURATION:  //Set saturation
          myCAM.setSaturation((CAM_STAURATION_LEVEL)commandBuff[1]);
          break;
        case SET_EV:  //Set EV
          myCAM.setEV((CAM_EV_LEVEL)commandBuff[1]);
          break;
        case SET_WHITEBALANCE:  //Set White balance
          myCAM.setAutoWhiteBalanceMode((CAM_WHITE_BALANCE)commandBuff[1]);
          break;
        case SET_SPECIAL_EFFECTS:  //Set Special effects
          myCAM.setColorEffect((CAM_COLOR_FX)commandBuff[1]);
          break;
        case SET_FOCUS_CONTROL:  //Focus Control
          myCAM.setAutoFocus(commandBuff[1]);
          if (commandBuff[1] == 0) {
            myCAM.setAutoFocus(0x02);
          }
          break;
        case SET_EXPOSURE_CONTROL:  //exposure control
          myCAM.setAutoExposure(commandBuff[1] & 0x01);
          break;
        case SET_GAIN_CONTROL:  //Gain control
          myCAM.setAutoISOSensitive(commandBuff[1] & 0x01);
          break;
        case SET_WHILEBALANCE_CONTROL:  //while balance control
          myCAM.setAutoWhiteBalance(commandBuff[1] & 0x01);
          break;
        case SET_SHARPNESS:
          myCAM.setSharpness((CAM_SHARPNESS_LEVEL)commandBuff[1]);
          break;
        case SET_MANUAL_GAIN:  //manual gain control
          gainValue = (commandBuff[1] << 8) | commandBuff[2];
          myCAM.setISOSensitivity(gainValue);
          break;
        case SET_MANUAL_EXPOSURE:  //manual exposure control
          exposureLen1 = commandBuff[1];
          exposureLen2 = commandBuff[2];
          exposureLen3 = commandBuff[3];
          exposureValue = (exposureLen1 << 16) | (exposureLen2 << 8) | exposureLen3;
          myCAM.setAbsoluteExposure(exposureValue);
          break;
      }
      commandLength = 0;
    }
  }
  if (imagebuf[0] == 0xff && imagebuf[1] == 0xd8) {
    jpegHeadFlag = 1;
    myUart.arducamUartWrite(0x55);
    myUart.arducamUartWrite(0xAA);
    myUart.arducamUartWrite(CAM_IMAGE_PIX_FMT_JPG);
    myUart.arducamUartWrite((uint8_t)(myCAM.getTotalLength() & 0xff));
    myUart.arducamUartWrite((uint8_t)((myCAM.getTotalLength() >> 8) & 0xff));
    myUart.arducamUartWrite((uint8_t)((myCAM.getTotalLength() >> 16) & 0xff));
    myUart.arducamUartWrite((uint8_t)((myCAM.getTotalLength() >> 24) & 0xff));
  }
  if (jpegHeadFlag == 1) {
    readImageLength += length;
    for (uint8_t i = 0; i < length; i++) {
      myUart.arducamUartWrite(imagebuf[i]);
    }
  }
  if (readImageLength == myCAM.getTotalLength()) {
    readImageLength = 0;
    jpegHeadFlag = 0;
    myUart.arducamUartWrite(0x55);
    myUart.arducamUartWrite(0xBB);
  }
  return sendFlag;
}

void setup() {
  myUart.arducamUartBegin(115200);
  myCAM.begin();
  myCAM.registerCallBack(readBuffer, 200);
}

void loop() {
  if (myUart.arducamUartAvailable()) {
    temp = myUart.arducamUartRead();
    arducamDelayMs(5);
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