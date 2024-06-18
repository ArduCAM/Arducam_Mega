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

ArducamLink::ArducamLink() {}

ArducamLink::~ArducamLink() {}

void ArducamLink::arducamUartBegin(uint32_t baudRate)
{
    SerialBegin(baudRate);
}

void ArducamLink::reportVerInfo(Arducam_Mega* myCamera)
{
    ArducamCamera* cameraInstance = myCamera->getCameraInstance();
    uint8_t headAndtail[]         = {0xff, 0xaa, 0x03, 0xff, 0xbb};

    uint32_t len = 6;
    arducamUartWriteBuff(&headAndtail[0], 3);
    arducamUartWriteBuff((uint8_t*)&len, 4);
    arducamUartWriteBuff(cameraInstance->verDateAndNumber, 4);
    printf("\r\n");
    arducamUartWriteBuff(&headAndtail[3], 2);
}

void ArducamLink::reportSdkVerInfo(Arducam_Mega* myCamera)
{
    ArducamCamera* cameraInstance = myCamera->getCameraInstance();
    uint8_t headAndtail[]         = {0xff, 0xaa, 0x05, 0xff, 0xbb};

    uint32_t len = 6;
    arducamUartWriteBuff(&headAndtail[0], 3);
    arducamUartWriteBuff((uint8_t*)&len, 4);
    arducamUartWriteBuff((uint8_t*)&cameraInstance->currentSDK->sdkVersion, 5);
    printf("\r\n");
    arducamUartWriteBuff(&headAndtail[3], 2);
}

void ArducamLink::reportCameraInfo(Arducam_Mega* myCamera)
{
    ArducamCamera* cameraInstance = myCamera->getCameraInstance();
    uint8_t headAndtail[]         = {0xff, 0xaa, 0x02, 0xff, 0xbb};

    uint32_t len = 0;
    char buff[400];
    arducamUartWriteBuff(&headAndtail[0], 3);
    sprintf(buff,
            "ReportCameraInfo\r\nCamera Type:%s\r\nCamera Support Resolution:%d\r\nCamera Support "
            "specialeffects:%d\r\nCamera Support Focus:%d\r\nCamera Exposure Value Max:%ld\r\nCamera Exposure Value "
            "Min:%d\r\nCamera Gain Value Max:%d\r\nCamera Gain Value Min:%d\r\nCamera Support Sharpness:%d\r\n",
            cameraInstance->myCameraInfo.cameraId, cameraInstance->myCameraInfo.supportResolution,
            cameraInstance->myCameraInfo.supportSpecialEffects, cameraInstance->myCameraInfo.supportFocus,
            cameraInstance->myCameraInfo.exposureValueMax, cameraInstance->myCameraInfo.exposureValueMin,
            cameraInstance->myCameraInfo.gainValueMax, cameraInstance->myCameraInfo.gainValueMin,
            cameraInstance->myCameraInfo.supportSharpness);
    len = strlen(buff);
    arducamUartWriteBuff((uint8_t*)&len, 4);
    printf(buff);
    arducamUartWriteBuff(&headAndtail[3], 2);
}

uint8_t head_date[] = {0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46,
                       0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00};

uint8_t exif_date[] = {
    0xff, 0xe1, 0x00, 0x98, 0x45, 0x78, 0x69, 0x66, 0x00, 0x00, 0x4d, 0x4d, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x08, 0x00,
    0x03, 0x01, 0x0f, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x32, 0x01, 0x10, 0x00, 0x02, 0x00, 0x00,
    0x00, 0x14, 0x00, 0x00, 0x00, 0x3a, 0x87, 0x69, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x4e, 0x00,
    0x00, 0x00, 0x00, 0x41, 0x72, 0x64, 0x75, 0x63, 0x61, 0x6d, 0x00, 0x41, 0x72, 0x64, 0x75, 0x63, 0x61, 0x6d, 0x20,
    0x4d, 0x65, 0x67, 0x61, 0x20, 0x43, 0x61, 0x6d, 0x65, 0x72, 0x61, 0x00, 0x00, 0x02, 0x90, 0x03, 0x00, 0x02, 0x00,
    0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x68, 0x90, 0x04, 0x00, 0x02, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x7c,
};

uint8_t timestamp[] = {'2', '0', '2', '4', ':', '0', '1', ':', '2', '5',
                       ' ', '1', '0', ':', '4', '2', ':', '0', '0', 0x00};

const uint8_t _head_date_len = sizeof(head_date);
const uint8_t _exif_date_len = sizeof(exif_date);
const uint8_t _timestamp_len = sizeof(timestamp);

int update_timestamp(uint32_t date, uint32_t time)
{
    uint8_t timestamp_len = _timestamp_len - 1;

    for (uint8_t i = 0; i < 6; i++) {
        if (i % 2 == 0) {
            timestamp_len--;
        }
        timestamp[timestamp_len] = (time & 0x0f) + '0';
        time >>= 4;
        timestamp_len--;
    }

    for (uint8_t i = 0; i < 8; i++) {
        if (i % 2 == 0 && i < 6) {
            timestamp_len--;
        }
        timestamp[timestamp_len] = (date & 0x0f) + '0';
        date >>= 4;
        timestamp_len--;
    }
    return 0;
}

void ArducamLink::cameraGetPicture(Arducam_Mega* myCamera, uint32_t date, uint32_t time)
{
    ArducamCamera* cameraInstance   = myCamera->getCameraInstance();
    uint8_t headAndtail[]           = {0xff, 0xaa, 0x01, 0xff, 0xbb};
    uint8_t buff[READ_IMAGE_LENGTH] = {0};

    uint8_t rtLength = 0;
    uint32_t len     = myCamera->getTotalLength() + _exif_date_len + _timestamp_len + _timestamp_len;
    arducamUartWriteBuff(&headAndtail[0], 3);
    arducamUartWriteBuff((uint8_t*)(&len), 4);
    arducamUartWrite(((cameraInstance->currentPictureMode & 0x0f) << 4) | 0x01);

    update_timestamp(date, time);

    arducamUartWriteBuff(head_date, _head_date_len);
    arducamUartWriteBuff(exif_date, _exif_date_len);
    arducamUartWriteBuff(timestamp, _timestamp_len);
    arducamUartWriteBuff(timestamp, _timestamp_len);

    rtLength = readBuff(cameraInstance, buff, READ_IMAGE_LENGTH);
    arducamUartWriteBuff(buff + _head_date_len, rtLength - _head_date_len);

    while (myCamera->getReceivedLength()) {
        rtLength = readBuff(cameraInstance, buff, READ_IMAGE_LENGTH);
        arducamUartWriteBuff(buff, rtLength);
    }
    arducamUartWriteBuff(&headAndtail[3], 2);
}

void ArducamLink::send_data_pack(char cmd_type, char* msg)
{
    uint8_t headAndtail[] = {0xff, 0xaa, 0x07, 0xff, 0xbb};
    headAndtail[2]        = cmd_type;
    uint32_t len          = strlen(msg) + 2;
    arducamUartWriteBuff(&headAndtail[0], 3);
    arducamUartWriteBuff((uint8_t*)&len, 4);
    printf(msg);
    printf("\r\n");
    arducamUartWriteBuff(&headAndtail[3], 2);
}

uint8_t ArducamLink::uartCommandProcessing(Arducam_Mega* myCAM, uint8_t* commandBuff)
{
    ArducamCamera* cameraInstance = myCAM->getCameraInstance();
    CamStatus state;
    uint16_t gainValue     = 0;
    uint32_t exposureValue = 0;
    uint32_t exposureLen1  = 0;
    uint32_t exposureLen2  = 0;
    uint32_t exposureLen3  = 0;

    switch (commandBuff[0]) {
    case SET_PICTURE_RESOLUTION: // Set Camera Resolution
        myCAM->takePicture((CAM_IMAGE_MODE)(commandBuff[1] & 0x0f), (CAM_IMAGE_PIX_FMT)0x01);
        break;
    case SET_BRIGHTNESS: // Set brightness
        myCAM->setBrightness((CAM_BRIGHTNESS_LEVEL)commandBuff[1]);
        break;
    case SET_CONTRAST: // Set Contrast
        myCAM->setContrast((CAM_CONTRAST_LEVEL)commandBuff[1]);
        break;
    case SET_SATURATION: // Set saturation
        myCAM->setSaturation((CAM_STAURATION_LEVEL)commandBuff[1]);
        break;
    case SET_EV: // Set EV
        myCAM->setEV((CAM_EV_LEVEL)commandBuff[1]);
        break;
    case SET_WHITEBALANCE: // Set White balance
        myCAM->setAutoWhiteBalanceMode((CAM_WHITE_BALANCE)commandBuff[1]);
        break;
    case SET_SPECIAL_EFFECTS: // Set Special effects
        myCAM->setColorEffect((CAM_COLOR_FX)commandBuff[1]);
        break;
    case SET_FOCUS_CONTROL: // Focus Control
        myCAM->setAutoFocus(commandBuff[1]);
        if (commandBuff[1] == 0) {
            myCAM->setAutoFocus(0x02);
        }
        break;
    case SET_EXPOSUREANDGAIN_CONTROL: // exposure and  Gain control
        myCAM->setAutoExposure(commandBuff[1] & 0x01);
        myCAM->setAutoISOSensitive(commandBuff[1] & 0x01);
        break;
    case SET_WHILEBALANCE_CONTROL: // while balance control
        myCAM->setAutoWhiteBalance(commandBuff[1] & 0x01);
        break;
    case SET_SHARPNESS:
        myCAM->setSharpness((CAM_SHARPNESS_LEVEL)commandBuff[1]);
        break;
    case SET_MANUAL_GAIN: // manual gain control
        gainValue = (commandBuff[1] << 8) | commandBuff[2];
        myCAM->setISOSensitivity(gainValue);
        break;
    case SET_MANUAL_EXPOSURE: // manual exposure control
        exposureLen1  = commandBuff[1];
        exposureLen2  = commandBuff[2];
        exposureLen3  = commandBuff[3];
        exposureValue = (exposureLen1 << 16) | (exposureLen2 << 8) | exposureLen3;
        myCAM->setAbsoluteExposure(exposureValue);
        break;
    case GET_CAMERA_INFO: // Get Camera info
        reportCameraInfo(myCAM);
        break;
    case TAKE_PICTURE:
        exposureLen1 = (commandBuff[1] << 24) | (commandBuff[2] << 16) | (commandBuff[3] << 8) | commandBuff[4];
        exposureLen2 = (commandBuff[5] << 24) | (commandBuff[6] << 16) | (commandBuff[7] << 8) | commandBuff[8];
        myCAM->takePicture((CAM_IMAGE_MODE)cameraInstance->currentPictureMode, (CAM_IMAGE_PIX_FMT)cameraInstance->currentPixelFormat);
        cameraGetPicture(myCAM, exposureLen1, exposureLen2);
        break;
    case DEBUG_WRITE_REGISTER:
        myCAM->debugWriteRegister(commandBuff + 1);
        break;
    case GET_FRM_VER_INFO: // Get Firmware version info
        reportVerInfo(myCAM);
        break;
    case GET_SDK_VER_INFO: // Get sdk version info
        reportSdkVerInfo(myCAM);
        break;
    case RESET_CAMERA:
        myCAM->reset();
    case SET_IMAGE_QUALITY:
        myCAM->setImageQuality((IMAGE_QUALITY)commandBuff[1]);
    default:
        break;
    }
    return CAM_ERR_SUCCESS;
}

void ArducamLink::arducamUartWrite(uint8_t data)
{
    SerialWrite(data);
    delayUs(12);
}

void ArducamLink::arducamUartWriteBuff(uint8_t* buff, uint16_t length)
{
    // SerialWriteBuff(buff, length);
    // delayUs(12);
    for (uint16_t i = 0; i < length; i++)
        arducamUartWrite(buff[i]);
}

void ArducamLink::printf(char* buff)
{
    uint16_t len = strlen(buff);
    // SerialPrintf(buff);
    // delayUs(12);
    arducamUartWriteBuff((uint8_t*)buff, len);
}

uint32_t ArducamLink::arducamUartAvailable(void)
{
    return SerialAvailable();
}

uint8_t ArducamLink::arducamUartRead(void)
{
    return SerialRead();
}

void ArducamLink::arducamFlush(void)
{
    while (arducamUartAvailable()) {
        arducamUartRead();
    }
}
