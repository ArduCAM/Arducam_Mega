/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#include "ArducamLink.h"
#include <cstdio>
#include <cstring>

uint8_t ArducamLink::uart_state = 0;
uint8_t ArducamLink::uart1_rx_cnt = 0;
uint8_t ArducamLink::uart1_rx_head = 0;
uint8_t ArducamLink::uart1_rx_len = 0;
uint8_t ArducamLink::UartCommBuff[20] = {0};

ArducamLink::ArducamLink() {}

ArducamLink::~ArducamLink() {}

void ArducamLink::uart_rx_handler()
{
    UartCommBuff[uart1_rx_cnt++] = uart_getc(uart0);
    if (UartCommBuff[uart1_rx_cnt - 1] == 0x55)
        uart1_rx_head = uart1_rx_cnt - 1;
    if ((UartCommBuff[uart1_rx_head] == 0x55) && (UartCommBuff[uart1_rx_cnt - 1] == 0xAA)) {
        uart1_rx_len = uart1_rx_cnt - 1 - uart1_rx_head;
        uart_state   = 1;
    }
}

void ArducamLink::arducamUartBegin(uint32_t baudRate)
{
    // Initialise UART 0
    uart_init(uart0, baudRate);

    // Set the GPIO pin mux to the UART - 0 is TX, 1 is RX
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);

    irq_set_exclusive_handler(UART0_IRQ, uart_rx_handler);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(uart0, true, false);
}

void ArducamLink::reportVerInfo(Arducam_Mega* myCamera)
{
    ArducamCamera* cameraInstance = myCamera->getCameraInstance();

    uint32_t len = 5;
    uart_putc_raw(uart0, 0xff);
    uart_putc_raw(uart0, 0xAA);
    uart_putc_raw(uart0, 0x03);
    uart_write_blocking(uart0, (uint8_t*)&len, 4);
    uart_write_blocking(uart0, cameraInstance->verDate, 3);
    uart_puts(uart0, "\n");
    uart_putc_raw(uart0, 0xff);
    uart_putc_raw(uart0, 0xBB);
}

void ArducamLink::reportSdkVerInfo(Arducam_Mega* myCamera)
{
    ArducamCamera* cameraInstance = myCamera->getCameraInstance();

    uint32_t len = 6;
    uart_putc_raw(uart0, 0xff);
    uart_putc_raw(uart0, 0xAA);
    uart_putc_raw(uart0, 0x05);
    uart_write_blocking(uart0, (uint8_t*)&len, 4);
    uart_putc_raw(uart0, (cameraInstance->currentSDK->sdkVersion >> 24) & 0xFF);
    uart_putc_raw(uart0, (cameraInstance->currentSDK->sdkVersion >> 16) & 0xFF);
    uart_putc_raw(uart0, (cameraInstance->currentSDK->sdkVersion >> 8) & 0xFF);
    uart_putc_raw(uart0, (cameraInstance->currentSDK->sdkVersion) & 0xFF);
    uart_puts(uart0, "\r\n");
    uart_putc_raw(uart0, 0xff);
    uart_putc_raw(uart0, 0xBB);
}

void ArducamLink::reportCameraInfo(Arducam_Mega* myCamera)
{
    ArducamCamera* cameraInstance = myCamera->getCameraInstance();

    uint32_t len = 0;
    char buff[400];
    uart_putc_raw(uart0, 0xff);
    uart_putc_raw(uart0, 0xAA);
    uart_putc_raw(uart0, 0x02);
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
    uart_write_blocking(uart0, (uint8_t*)&len, 4);
    uart_puts(uart0, buff);
    uart_putc_raw(uart0, 0xff);
    uart_putc_raw(uart0, 0xBB);
}
static uint8_t frame_buff[READ_IMAGE_LENGTH] = {0};

void ArducamLink::cameraGetPicture(Arducam_Mega* myCamera)
{
    ArducamCamera* cameraInstance = myCamera->getCameraInstance();

    uint32_t rtLength = 0;
    uart_putc_raw(uart0, 0xff);
    uart_putc_raw(uart0, 0xAA);
    uart_putc_raw(uart0, 0x01);
    uart_putc_raw(uart0, (uint8_t)(myCamera->getTotalLength() & 0xff));
    uart_putc_raw(uart0, (uint8_t)((myCamera->getTotalLength() >> 8) & 0xff));
    uart_putc_raw(uart0, (uint8_t)((myCamera->getTotalLength() >> 16) & 0xff));
    uart_putc_raw(uart0, (uint8_t)((myCamera->getTotalLength() >> 24) & 0xff));
    uart_putc_raw(uart0, ((cameraInstance->cameraDataFormat & 0x0f) << 4) | 0x01);
    while (myCamera->getReceivedLength()) {
        rtLength = readBuff(cameraInstance, frame_buff, READ_IMAGE_LENGTH);
        uart_write_blocking(uart0, frame_buff, rtLength);
    }
    uart_putc_raw(uart0, 0xff);
    uart_putc_raw(uart0, 0xBB);
}

uint8_t ArducamLink::uartCommandProcessing(Arducam_Mega* myCAM)
{
    if (uart_state != 1)
        return 0;
    uart_state           = 0;
    uint8_t* commandBuff = &UartCommBuff[uart1_rx_head + 1];
    uart1_rx_len         = 0;
    uart1_rx_cnt         = 0;

    ArducamCamera* cameraInstance = myCAM->getCameraInstance();
    CamStatus state;
    uint16_t gainValue     = 0;
    uint32_t exposureValue = 0;
    uint32_t exposureLen1  = 0;
    uint32_t exposureLen2  = 0;
    uint32_t exposureLen3  = 0;

    uint8_t cameraResolution = cameraInstance->currentPictureMode;
    uint8_t cameraFarmat     = cameraInstance->currentPixelFormat;
    switch (commandBuff[0]) {

    case SET_PICTURE_RESOLUTION: // Set Camera Resolution
        cameraResolution = commandBuff[1] & 0x0f;
        cameraFarmat     = (commandBuff[1] & 0x70) >> 4;
        myCAM->takePicture((CAM_IMAGE_MODE)cameraResolution, (CAM_IMAGE_PIX_FMT)cameraFarmat);
        break;
    case SET_VIDEO_RESOLUTION: // Set Video Resolution
        cameraResolution = commandBuff[1] & 0x0f;
        state            = myCAM->startPreview((CAM_VIDEO_MODE)cameraResolution);
        if (state == CAM_ERR_NO_CALLBACK) {
            uart_puts(uart0, "callback function is not registered");
        }
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
    case SET_EXPOSURE_CONTROL: // exposure control
        myCAM->setAutoExposure(commandBuff[1] & 0x01);
        break;
    case SET_GAIN_CONTROL: // Gain control
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
        myCAM->takePicture((CAM_IMAGE_MODE)cameraResolution, (CAM_IMAGE_PIX_FMT)cameraFarmat);
        cameraGetPicture(myCAM);
        break;
    case DEBUG_WRITE_REGISTER:
        myCAM->debugWriteRegister(commandBuff + 1);
        break;
    case STOP_STREAM:
        myCAM->stopPreview();
        break;
    case GET_FRM_VER_INFO: // Get Firmware version info
        reportVerInfo(myCAM);
        break;
    case GET_SDK_VER_INFO: // Get sdk version info
        reportSdkVerInfo(myCAM);
        break;
    default:
        break;
    }
    return CAM_ERR_SUCCESS;
}

void ArducamLink::arducamUartWriteBuff(uint8_t* buff, uint8_t length)
{
    uart_write_blocking(uart0, buff, length);
}

void ArducamLink::arducamUartWrite(uint8_t data)
{
    uart_putc_raw(uart0, data);
    sleep_us(12);
}

void ArducamLink::printf(char* buff)
{
    uart_puts(uart0, buff);
    sleep_us(12);
}

uint32_t ArducamLink::arducamUartAvailable(void)
{
    return uart_is_readable(uart0);
}

uint8_t ArducamLink::arducamUartRead(void)
{
    return uart_getc(uart0);
}

void ArducamLink::arducamFlush(void)
{
    while (arducamUartAvailable()) {
        arducamUartRead();
    }
}