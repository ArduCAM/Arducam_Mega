
/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for
 * details.
 *
 */

#include "Arducam_Mega.h"

Arducam_Mega::Arducam_Mega(int CS)
{
    cameraInfo = createArducamCamera(CS);
}
CamStatus Arducam_Mega::reset(void)
{
    return ::reset(&cameraInfo);
}

CamStatus Arducam_Mega::begin(void)
{
    return ::begin(&cameraInfo);
}

CamStatus Arducam_Mega::takePicture(CAM_IMAGE_MODE mode, CAM_IMAGE_PIX_FMT pixel_format)
{
    return ::takePicture(&cameraInfo, mode, pixel_format);
}

CamStatus Arducam_Mega::takeMultiPictures(CAM_IMAGE_MODE mode, CAM_IMAGE_PIX_FMT pixel_format, uint8_t num)
{
    return ::takeMultiPictures(&cameraInfo, mode, pixel_format, num);
}

CamStatus Arducam_Mega::startPreview(CAM_VIDEO_MODE mode)
{
    return ::startPreview(&cameraInfo, mode);
}
void Arducam_Mega::captureThread(void)
{
    return ::captureThread(&cameraInfo);
}

CamStatus Arducam_Mega::stopPreview(void)
{
    return ::stopPreview(&cameraInfo);
}

CamStatus Arducam_Mega::setAutoExposure(uint8_t val)
{
    return ::setAutoExposure(&cameraInfo, val);
}

CamStatus Arducam_Mega::setAbsoluteExposure(uint32_t val)
{
    return ::setAbsoluteExposure(&cameraInfo, val);
}

CamStatus Arducam_Mega::setAutoISOSensitive(uint8_t val)
{
    return ::setAutoISOSensitive(&cameraInfo, val);
}

CamStatus Arducam_Mega::setISOSensitivity(int val)
{
    return ::setISOSensitivity(&cameraInfo, val);
}

CamStatus Arducam_Mega::setAutoWhiteBalance(uint8_t val)
{
    return ::setAutoWhiteBalance(&cameraInfo, val);
}
CamStatus Arducam_Mega::setAutoWhiteBalanceMode(CAM_WHITE_BALANCE mode)
{
    return ::setAutoWhiteBalanceMode(&cameraInfo, mode);
}

CamStatus Arducam_Mega::setColorEffect(CAM_COLOR_FX effect)
{
    return ::setColorEffect(&cameraInfo, effect);
}

CamStatus Arducam_Mega::setAutoFocus(uint8_t val)
{
    return ::setAutoFocus(&cameraInfo, val);
}

CamStatus Arducam_Mega::setSaturation(CAM_STAURATION_LEVEL level)
{
    return ::setSaturation(&cameraInfo, level);
}

CamStatus Arducam_Mega::setEV(CAM_EV_LEVEL level)
{
    return ::setEV(&cameraInfo, level);
}
CamStatus Arducam_Mega::setContrast(CAM_CONTRAST_LEVEL level)
{
    return ::setContrast(&cameraInfo, level);
}

CamStatus Arducam_Mega::setBrightness(CAM_BRIGHTNESS_LEVEL level)
{
    return ::setBrightness(&cameraInfo, level);
}

CamStatus Arducam_Mega::setSharpness(CAM_SHARPNESS_LEVEL level)
{
    return ::setSharpness(&cameraInfo, level);
}
CamStatus Arducam_Mega::setImageQuality(IMAGE_QUALITY quality)
{
    return ::setImageQuality(&cameraInfo,quality);
}
uint8_t Arducam_Mega::readBuff(uint8_t* buff, uint8_t length)
{
    return ::readBuff(&cameraInfo, buff, length);
}
uint8_t Arducam_Mega::readByte(void)
{
    return ::readByte(&cameraInfo);
}

void Arducam_Mega::debugWriteRegister(uint8_t* buff)
{
    ::debugWriteRegister(&cameraInfo, buff);
}
void Arducam_Mega::registerCallBack(BUFFER_CALLBACK function, uint8_t blockSize, STOP_HANDLE handle)
{
    ::registerCallback(&cameraInfo, function, blockSize, handle);
}

void Arducam_Mega::lowPowerOn(void)
{
    ::lowPowerOn(&cameraInfo);
}

void Arducam_Mega::lowPowerOff(void)
{
    ::lowPowerOff(&cameraInfo);
}

uint32_t Arducam_Mega::getTotalLength(void)
{
    return cameraInfo.totalLength;
}

uint32_t Arducam_Mega::getReceivedLength(void)
{
    return cameraInfo.receivedLength;
}

ArducamCamera* Arducam_Mega::getCameraInstance(void)
{
    return &cameraInfo;
}

