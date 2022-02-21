/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#include "ArducamLink.h"
#include "Arduino.h"

ArducamLink::ArducamLink()
{
}

ArducamLink::~ArducamLink()
{
}


void ArducamLink::arducamUartBegin(uint32_t baudRate)
{
	Serial.begin(baudRate);
}


void ArducamLink::reportCameraInfo(Arducam* myCamera)
{
	ArducamCamera* cameraInstance = myCamera->getCameraInstance();
	Serial.println("ReportCameraInfo");
	Serial.print("Camera Type:");
	Serial.println(cameraInstance->myCameraInfo.cameraId); 
	Serial.print("Camera Support Resolution:");
	Serial.println(cameraInstance->myCameraInfo.supportResolution);
	Serial.print("Camera Support specialeffects:");
	Serial.println(cameraInstance->myCameraInfo.supportSpecialEffects);
	Serial.print("Camera Support Focus:");
	Serial.println(cameraInstance->myCameraInfo.supportFocus);
	Serial.print("Camera Exposure Value Max:");
	Serial.println(cameraInstance->myCameraInfo.exposureValueMax); 
	Serial.print("Camera Exposure Value Min:");
	Serial.println(cameraInstance->myCameraInfo.exposureValueMin); 
	Serial.print("Camera Gain Value Max:");
	Serial.println(cameraInstance->myCameraInfo.gainValueMax);
	Serial.print("Camera Gain Value Min:");
	Serial.println(cameraInstance->myCameraInfo.gainValueMin);
	Serial.print("Camera Support Sharpness:");
	Serial.println(cameraInstance->myCameraInfo.supportSharpness);
}

void ArducamLink::cameraGetPicture(Arducam* myCamera)
{
	ArducamCamera* cameraInstance = myCamera->getCameraInstance();
	uint8_t buff[READ_IMAGE_LENGTH] = {0};
	uint8_t rtLength = 0;
	arducamUartWrite(0x55);
	arducamUartWrite(0xAA);
	arducamUartWrite(cameraInstance->cameraDataFormat);
	arducamUartWrite((uint8_t)(myCamera->getTotalLength()&0xff));
	arducamUartWrite((uint8_t)((myCamera->getTotalLength()>>8)&0xff));
	arducamUartWrite((uint8_t)((myCamera->getTotalLength()>>16)&0xff));
	arducamUartWrite((uint8_t)((myCamera->getTotalLength()>>24)&0xff));
	while (myCamera->getReceivedLength())		
	{
		rtLength=readBuff(cameraInstance,buff,READ_IMAGE_LENGTH);
		arducamUartWriteBuff(buff,rtLength);
	}
	arducamUartWrite(0x55);
	arducamUartWrite(0xBB);
}



uint8_t ArducamLink::uartCommandProcessing(Arducam* myCAM,uint8_t* commandBuff)
{
	ArducamCamera* cameraInstance = myCAM->getCameraInstance();
	CamStatus state;
	uint16_t gainValue = 0;
	uint32_t exposureValue = 0;
	uint32_t exposureLen1 = 0;
	uint32_t exposureLen2 = 0;
	uint32_t exposureLen3 = 0;
	uint8_t cameraResolution = cameraInstance->currentPictureMode;
	uint8_t cameraFarmat = cameraInstance->currentPixelFormat; 
	switch (commandBuff[0])
	{
	
	case SET_PICTURE_RESOLUTION:																//Set Camera Resolution
		cameraResolution = commandBuff[1] & 0x0f;
		cameraFarmat = (commandBuff[1] & 0x70) >> 4;
		myCAM->takePicture((CAM_IMAGE_MODE)cameraResolution,(CAM_IMAGE_PIX_FMT)cameraFarmat);
		break;
	case SET_VIDEO_RESOLUTION:																//Set Video Resolution
		cameraResolution = commandBuff[1] & 0x0f;
		state = myCAM->startPreview((CAM_VIDEO_MODE)cameraResolution);
		if (state == CAM_ERR_NO_CALLBACK)
		{
			Serial.println("callback function is not registered");
		}    
		break;
	case SET_BRIGHTNESS:																//Set brightness
		myCAM->setBrightness((CAM_BRIGHTNESS_LEVEL)commandBuff[1]);
		break;
	case SET_CONTRAST:																//Set Contrast
		myCAM->setContrast((CAM_CONTRAST_LEVEL)commandBuff[1]);
		break;
	case SET_SATURATION:																//Set saturation
		myCAM->setSaturation((CAM_STAURATION_LEVEL)commandBuff[1]);
		break;
	case SET_EV:																//Set EV 
		myCAM->setEV((CAM_EV_LEVEL)commandBuff[1]);
		break;
	case SET_WHITEBALANCE:																//Set White balance
		myCAM->setAutoWhiteBalanceMode((CAM_WHITE_BALANCE)commandBuff[1]);
		break;
	case SET_SPECIAL_EFFECTS:																//Set Special effects
		myCAM->setColorEffect((CAM_COLOR_FX)commandBuff[1]);
		break;
	case SET_FOCUS_CONTROL:																//Focus Control
		myCAM->setAutoFocus(commandBuff[1]);
		if (commandBuff[1] == 0)
		{
			myCAM->setAutoFocus(0x02);
		}
		break;
	case SET_EXPOSURE_CONTROL:																//exposure control
		myCAM->setAutoExposure(commandBuff[1]&0x01);
		break;
	case SET_GAIN_CONTROL:																//Gain control
		myCAM->setAutoISOSensitive(commandBuff[1]&0x01);
		break;
	case SET_WHILEBALANCE_CONTROL:																//while balance control
		myCAM->setAutoWhiteBalance(commandBuff[1]&0x01);
		break;
	case SET_SHARPNESS:
		myCAM->setSharpness((CAM_SHARPNESS_LEVEL)commandBuff[1]);
	break;
	case SET_MANUAL_GAIN:															//manual gain control
		gainValue = (commandBuff[1]<<8) | commandBuff[2];
		myCAM->setISOSensitivity(gainValue);
		break;
	case SET_MANUAL_EXPOSURE:															//manual exposure control
		exposureLen1 = commandBuff[1];
		exposureLen2 = commandBuff[2];
		exposureLen3 = commandBuff[3];
		exposureValue = (exposureLen1<<16) | (exposureLen2<<8) | exposureLen3;
			myCAM->setAbsoluteExposure(exposureValue);
		break;
	case GET_CAMERA_INFO:															//Get Camera info
		reportCameraInfo(myCAM);
		break;
	case TAKE_PICTURE:
		myCAM->takePicture((CAM_IMAGE_MODE)cameraResolution,(CAM_IMAGE_PIX_FMT)cameraFarmat);
		cameraGetPicture(myCAM);
		break;	
	case DEBUG_WRITE_REGISTER:
		myCAM->debugWriteRegister(commandBuff+1);
		break;
	default:
		break;
	}
	return CAM_ERR_SUCCESS;
}

void ArducamLink::arducamUartWriteBuff(uint8_t* buff,uint8_t length)
{
	Serial.write(buff,length);
}

void ArducamLink::arducamUartWrite(uint8_t data)
{
    Serial.write(data);
    delayMicroseconds(10);
}

uint32_t ArducamLink::arducamUartAvailable(void)
{
	return Serial.available();
}

uint8_t ArducamLink::arducamUartRead(void)
{
	return Serial.read();
}

void ArducamLink::arducamFlush(void)
{
  while(arducamUartAvailable())
  {
    arducamUartRead();
  }
}