
/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */


#include "Arducam.hpp"
#include "Platform.h"

Arducam::Arducam(int CS)
{
    cameraInfo = createArducamCamera(CS);
}



CamStatus Arducam::begin(void)
{
   return ::begin(&cameraInfo);
}

CamStatus Arducam::takePicture(CAM_IMAGE_MODE mode, CAM_IMAGE_PIX_FMT pixel_format)
{
    return ::takePicture(&cameraInfo,mode,pixel_format);
}

CamStatus Arducam::takeMultiPictures(CAM_IMAGE_MODE mode, CAM_IMAGE_PIX_FMT pixel_format, uint8_t num)
{
    return ::takeMultiPictures(&cameraInfo,mode,pixel_format,num);
}


CamStatus Arducam::startPreview(CAM_VIDEO_MODE mode)
{
    return ::startPreview(&cameraInfo,mode);
}

CamStatus Arducam::stopPreview(void)
{
    return ::stopPreview(&cameraInfo);
}

CamStatus Arducam::setAutoExposure(uint8_t val)
{
    return ::setAutoExposure(&cameraInfo,val);
}

CamStatus Arducam::setAbsoluteExposure(uint32_t val)
{
    return ::setAbsoluteExposure(&cameraInfo,val);
}

CamStatus Arducam::setAutoISOSensitive(uint8_t val)
{
    return ::setAutoISOSensitive(&cameraInfo,val);
}

CamStatus Arducam::setISOSensitivity(int val)
{
    return ::setISOSensitivity(&cameraInfo,val);
}

CamStatus Arducam::setAutoWhiteBalance(uint8_t val)
{
    return ::setAutoWhiteBalance(&cameraInfo,val);
}
CamStatus Arducam::setAutoWhiteBalanceMode(CAM_WHITE_BALANCE mode)
{
    return ::setAutoWhiteBalanceMode(&cameraInfo,mode);
}

CamStatus Arducam::setColorEffect(CAM_COLOR_FX effect)
{
    return ::setColorEffect(&cameraInfo,effect);
}

CamStatus Arducam::setAutoFocus(uint8_t val)
{
    return ::setAutoFocus(&cameraInfo,val);
}

CamStatus Arducam::setSaturation(CAM_STAURATION_LEVEL level)
{
    return ::setSaturation(&cameraInfo,level);
}

CamStatus Arducam::setEV(CAM_EV_LEVEL level)
{
    return ::setEV(&cameraInfo,level);
}
CamStatus Arducam::setContrast(CAM_CONTRAST_LEVEL level)
{
    return ::setContrast(&cameraInfo,level);
}

CamStatus Arducam::setBrightness(CAM_BRIGHTNESS_LEVEL level)
{
    return ::setBrightness(&cameraInfo,level);
}

CamStatus Arducam::setSharpness(CAM_SHARPNESS_LEVEL level)
{
    return ::setSharpness(&cameraInfo,level);
}

uint8_t Arducam::readBuff(uint8_t*buff,uint8_t length)
{
    return ::readBuff(&cameraInfo,buff,length);
}
uint8_t Arducam::readByte(void)
{
    return ::readByte(&cameraInfo);
}
void Arducam::debugWriteRegister(uint8_t* buff)
{
    ::debugWriteRegister(&cameraInfo,buff);
}
void Arducam::registerCallBack(BUFFER_CALLBACK function,uint8_t blockSize)
{
    ::registerCallback(&cameraInfo,function,blockSize);
}

void Arducam::lowPowerOn(void)
{
    ::lowPowerOn(&cameraInfo);
}


void Arducam::lowPowerOff(void)
{
    ::lowPowerOff(&cameraInfo);
}

uint32_t Arducam::getTotalLength(void)
{
    return cameraInfo.totalLength;
}

uint32_t Arducam::getReceivedLength(void)
{
    return cameraInfo.receivedLength;
}

ArducamCamera* Arducam::getCameraInstance(void)
{
    return &cameraInfo;
}