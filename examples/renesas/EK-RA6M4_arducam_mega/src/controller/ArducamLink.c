/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#include "ArducamLink.h"
#include "ArducamUart.h"
#include <stdio.h>

static uint8_t _buff[512] = {0};

void arducamUartBegin(__attribute__((unused)) uint32_t baudRate)
{
    SerialBegin(baudRate);
}

void reportVerInfo(ArducamCamera* myCamera)
{
    uint32_t len = 6;
    arducamUartWriteBuff(MESSAGE_HEADER, 2);
    arducamUartWrite(MESSAGE_VERSION);
    arducamUartWriteBuff((uint8_t*)&len, 4);
    arducamUartWriteBuff(myCamera->verDateAndNumber, 4);
    SerialPrintf("\r\n");
    arducamUartWriteBuff(MESSAGE_TAIL, 2);
}

void reportSdkVerInfo(ArducamCamera* myCamera)
{
    uint32_t len = 6;
    arducamUartWriteBuff(MESSAGE_HEADER, 2);
    arducamUartWrite(MESSAGE_SDK_INFO);
    arducamUartWriteBuff((uint8_t*)&len, 4);
    arducamUartWriteBuff((uint8_t*)&myCamera->currentSDK->sdkVersion, 4);
    SerialPrintf("\r\n");
    arducamUartWriteBuff(MESSAGE_TAIL, 2);
}

void reportCameraInfo(ArducamCamera* myCamera)
{
    uint32_t len = 0;
    arducamUartWriteBuff(MESSAGE_HEADER, 2);
    arducamUartWrite(MESSAGE_CAMERA_INFO);
    sprintf((char*)_buff,
            "ReportCameraInfo\r\nCamera Type:%s\r\nCamera Support Resolution:%d\r\nCamera Support "
            "specialeffects:%d\r\nCamera Support Focus:%d\r\nCamera Exposure Value Max:%ld\r\nCamera Exposure Value "
            "Min:%d\r\nCamera Gain Value Max:%d\r\nCamera Gain Value Min:%d\r\nCamera Support Sharpness:%d\r\n",
            myCamera->myCameraInfo.cameraId, myCamera->myCameraInfo.supportResolution,
            myCamera->myCameraInfo.supportSpecialEffects, myCamera->myCameraInfo.supportFocus,
            myCamera->myCameraInfo.exposureValueMax, myCamera->myCameraInfo.exposureValueMin,
            myCamera->myCameraInfo.gainValueMax, myCamera->myCameraInfo.gainValueMin,
            myCamera->myCameraInfo.supportSharpness);
    len = strlen((char*)_buff);
    arducamUartWriteBuff((uint8_t*)&len, 4);
    SerialPrintf((char*)_buff);
    arducamUartWriteBuff(MESSAGE_TAIL, 2);
}

void cameraGetPicture(ArducamCamera* myCamera)
{
    uint32_t rtLength = 0;
    uint32_t len = myCamera->totalLength;
    arducamUartWriteBuff(MESSAGE_HEADER, 2);
    arducamUartWrite(MESSAGE_IMAGE);
    arducamUartWriteBuff((uint8_t*)(&len), 4);
    arducamUartWrite((uint8_t)(((myCamera->currentPictureMode & 0x0f) << 4) | 0x01));

    while (myCamera->receivedLength) {
        rtLength = readBuff(myCamera, _buff, READ_IMAGE_LENGTH);
        arducamUartWriteBuff(_buff, rtLength);
    }
    arducamUartWriteBuff(MESSAGE_TAIL, 2);
}

void send_data_pack(char* msg)
{
    uint32_t len = strlen(msg) + 2;
    arducamUartWriteBuff(MESSAGE_HEADER, 2);
    arducamUartWrite(MESSAGE_OTHER);
    arducamUartWriteBuff((uint8_t*)&len, 4);
    SerialPrintf(msg);
    SerialPrintf("\r\n");
    arducamUartWriteBuff(MESSAGE_TAIL, 2);
}

uint8_t uartCommandProcessing(ArducamCamera* myCAM, uint8_t* commandBuff)
{
    CamStatus state;
    uint16_t gainValue     = 0;
    uint32_t exposureValue = 0;
    uint32_t exposureLen1  = 0;
    uint32_t exposureLen2  = 0;
    uint32_t exposureLen3  = 0;

    uint8_t cameraResolution = myCAM->currentPictureMode;
    uint8_t cameraFarmat     = myCAM->currentPixelFormat;
    switch (commandBuff[0]) {
    case SET_PICTURE_RESOLUTION: // Set Camera Resolution
        cameraResolution = commandBuff[1] & 0x0f;
        cameraFarmat     = (commandBuff[1] & 0x70) >> 4;
        takePicture(myCAM, (CAM_IMAGE_MODE)cameraResolution, (CAM_IMAGE_PIX_FMT)cameraFarmat);
        break;
    case SET_VIDEO_RESOLUTION: // Set Video Resolution
        cameraResolution = commandBuff[1] & 0x0f;
        state            = startPreview(myCAM, (CAM_VIDEO_MODE)cameraResolution);
        if (state == CAM_ERR_NO_CALLBACK) {
            SerialPrintf("callback function is not registered\n");
        }
        break;
    case SET_BRIGHTNESS: // Set brightness
        setBrightness(myCAM, (CAM_BRIGHTNESS_LEVEL)commandBuff[1]);
        break;
    case SET_CONTRAST: // Set Contrast
        setContrast(myCAM, (CAM_CONTRAST_LEVEL)commandBuff[1]);
        break;
    case SET_SATURATION: // Set saturation
        setSaturation(myCAM, (CAM_STAURATION_LEVEL)commandBuff[1]);
        break;
    case SET_EV: // Set EV
        setEV(myCAM, (CAM_EV_LEVEL)commandBuff[1]);
        break;
    case SET_WHITEBALANCE: // Set White balance
        setAutoWhiteBalanceMode(myCAM, (CAM_WHITE_BALANCE)commandBuff[1]);
        break;
    case SET_SPECIAL_EFFECTS: // Set Special effects
        setColorEffect(myCAM, (CAM_COLOR_FX)commandBuff[1]);
        break;
    case SET_FOCUS_CONTROL: // Focus Control
        setAutoFocus(myCAM, commandBuff[1]);
        if (commandBuff[1] == 0) {
            setAutoFocus(myCAM, 0x02);
        }
        break;
    case SET_EXPOSUREANDGAIN_CONTROL: // exposure and  Gain control
        setAutoExposure(myCAM, commandBuff[1] & 0x01);
        setAutoISOSensitive(myCAM, commandBuff[1] & 0x01);
        break;
    case SET_WHILEBALANCE_CONTROL: // while balance control
        setAutoWhiteBalance(myCAM, commandBuff[1] & 0x01);
        break;
    case SET_SHARPNESS:
        setSharpness(myCAM, (CAM_SHARPNESS_LEVEL)commandBuff[1]);
        break;
    case SET_MANUAL_GAIN: // manual gain control
        gainValue = (uint16_t)((commandBuff[1] << 8) | commandBuff[2]);
        setISOSensitivity(myCAM, gainValue);
        break;
    case SET_MANUAL_EXPOSURE: // manual exposure control
        exposureLen1  = commandBuff[1];
        exposureLen2  = commandBuff[2];
        exposureLen3  = commandBuff[3];
        exposureValue = (exposureLen1 << 16) | (exposureLen2 << 8) | exposureLen3;
        setAbsoluteExposure(myCAM, exposureValue);
        break;
    case GET_CAMERA_INFO: // Get Camera info
        reportCameraInfo(myCAM);
        break;
    case TAKE_PICTURE:
        takePicture(myCAM, (CAM_IMAGE_MODE)cameraResolution, (CAM_IMAGE_PIX_FMT)cameraFarmat);
        cameraGetPicture(myCAM);
        break;
    case DEBUG_WRITE_REGISTER:
        debugWriteRegister(myCAM, commandBuff + 1);
        break;
    case STOP_STREAM:
        stopPreview(myCAM);
        break;
    case GET_FRM_VER_INFO: // Get Firmware version info
        reportVerInfo(myCAM);
        break;
    case GET_SDK_VER_INFO: // Get sdk version info
        reportSdkVerInfo(myCAM);
        break;
    case RESET_CAMERA:
        reset(myCAM);
        break;
    case SET_IMAGE_QUALITY:
        setImageQuality(myCAM, (IMAGE_QUALITY)commandBuff[1]);
        break;
    }
    return CAM_ERR_SUCCESS;
}

void arducamUartWrite(uint8_t data)
{
    SerialWrite(data);
    // delayUs(12);
}

void arducamUartWriteBuff(uint8_t* buff, uint32_t length)
{
    //    uint16_t offset = 0;
    //    while(length !=0){
    //        if(length < 64)
    //        {
    //            SerialWriteBuff(&buff[offset], length);
    //            length = 0;
    //        }else{
    //            SerialWriteBuff(&buff[offset], 64);
    //            length -= 64;
    //            offset += 64;
    //        }
    //            delayUs(12);
    //    }
    SerialWriteBuff(buff, length);
    // delayUs(12);
    //    for(uint32_t i = 0;i < length;i++)
    //        arducamUartWrite(buff[i]);
}

void arducamUartPrintf(char* buff)
{
    SerialPrintf(buff);
    // delayUs(12);
}
