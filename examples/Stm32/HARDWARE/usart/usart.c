/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#include "usart.h"
#include "string.h"
uint8_t UartCommBuff[20] = {0};
uint8_t UartCommLength = 0;
uint8_t ReadBuffLength = 0;

#if 1
#pragma import(__use_no_semihosting)
struct __FILE {
    int handle;
};

FILE __stdout;
void _sys_exit(int x)
{
    x = x;
}
int fputc(int ch, FILE* f)
{
    while ((USART1->SR & 0X40) == 0)
        ;
    USART1->DR = (u8)ch;
    return ch;
}
#endif

void uartInit(uint32_t BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

void arducamUartWrite(uint8_t data)
{
    USART_SendData(USART1, data);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;
}
void uartWriteBuffer(uint8_t* buff, uint32_t length)
{
    uint32_t num;
    for (num = 0; num < length; num++) {
        USART_SendData(USART1, buff[num]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
            ;
    }
}

uint8_t arducamUartRead(void)
{
    uint8_t rt = 0;
    rt = UartCommBuff[ReadBuffLength];
    ReadBuffLength++;
    if (ReadBuffLength == UartCommLength) {
        ReadBuffLength = 0;
        UartCommLength = 0;
    }
    return rt;
}
uint32_t arducamUartAvailable(void)
{
    return UartCommLength;
}

uint8_t commandProcessing(ArducamCamera* camera, uint8_t* buff, uint8_t length)
{
    CamStatus state;
    uint16_t GainValue = 0;
    uint32_t ExposureValue = 0;
    uint32_t ExposureLen1 = 0;
    uint32_t ExposureLen2 = 0;
    uint32_t ExposureLen3 = 0;
    uint8_t CameraResolution = camera->currentPictureMode;
    uint8_t CameraFarmat = camera->currentPixelFormat;
    switch (buff[0]) {
    case SET_PICTURE_RESOLUTION: // Set Camera Resolution
        CameraResolution = buff[1] & 0x0f;
        CameraFarmat = (buff[1] & 0x70) >> 4;
        takePicture(camera, (CAM_IMAGE_MODE)CameraResolution, (CAM_IMAGE_PIX_FMT)CameraFarmat);
        break;
    case SET_VIDEO_RESOLUTION: // Set Video Resolution
        camera->previewMode = TRUE;
        CameraResolution = buff[1] & 0x0f;
        state = startPreview(camera, (CAM_VIDEO_MODE)CameraResolution);
        if (state == CAM_ERR_NO_CALLBACK) {
            printf("callback function is not registered");
        }
        break;
    case SET_BRIGHTNESS: // Set brightness
        setBrightness(camera, (CAM_BRIGHTNESS_LEVEL)buff[1]);
        break;
    case SET_CONTRAST: // Set Contrast
        setContrast(camera, (CAM_CONTRAST_LEVEL)buff[1]);
        break;
    case SET_SATURATION: // Set saturation
        setSaturation(camera, (CAM_STAURATION_LEVEL)buff[1]);
        break;
    case SET_EV: // Set EV
        setEV(camera, (CAM_EV_LEVEL)buff[1]);
        break;
    case SET_WHITEBALANCE: // Set White balance
        setAutoWhiteBalanceMode(camera, (CAM_WHITE_BALANCE)buff[1]);
        break;
    case SET_SPECIAL_EFFECTS: // Set Special effects
        setColorEffect(camera, (CAM_COLOR_FX)buff[1]);
        break;
    case SET_FOCUS_CONTROL: // Focus Control
        setAutoFocus(camera, buff[1]);
        if (buff[1] == 0) {
            setAutoFocus(camera, 0x02);
        }
        break;
    case SET_EXPOSUREANDGAIN_CONTROL: // exposure and  Gain control
        setAutoExposure(camera, buff[1] & 0x01);
        setAutoISOSensitive(camera, buff[1] & 0x01);
    case SET_WHILEBALANCE_CONTROL: // while balance control
        setAutoWhiteBalance(camera, buff[1] & 0x01);
        break;
    case SET_SHARPNESS:
        setSharpness(camera, (CAM_SHARPNESS_LEVEL)buff[1]);
        break;
    case SET_MANUAL_GAIN: // manual gain control
        GainValue = (buff[1] << 8) | buff[2];
        setISOSensitivity(camera, GainValue);
        break;
    case SET_MANUAL_EXPOSURE: // manual exposure control
        ExposureLen1 = buff[1];
        ExposureLen2 = buff[2];
        ExposureLen3 = buff[3];
        ExposureValue = (ExposureLen1 << 16) | (ExposureLen2 << 8) | ExposureLen3;
        setAbsoluteExposure(camera, ExposureValue);
        break;
    case GET_CAMERA_INFO: // Get Camera info
        reportCameraInfo(camera);
        break;
    case TAKE_PICTURE:
        takePicture(camera, (CAM_IMAGE_MODE)CameraResolution, (CAM_IMAGE_PIX_FMT)CameraFarmat);
        cameraGetPicture(camera);
        break;
    case DEBUG_WRITE_REGISTER:
        debugWriteRegister(camera, buff + 1);
        break;
    case STOP_STREAM:
        stopPreview(camera);
        break;
    case GET_FRM_VER_INFO: // Get Firmware version info
        reportVerInfo(camera);
        break;
    case GET_SDK_VER_INFO: // Get sdk version info
        reportSdkVerInfo(camera);
        break;
    case RESET_CAMERA:
        reset(camera);
    case SET_IMAGE_QUALITY:
        setImageQuality(camera,(IMAGE_QUALITY)buff[1]);
    default:
        break;
    }

    return buff[0];
}

void cameraGetPicture(ArducamCamera* camera)
{
    camera->burstFirstFlag = 0;
    uint8_t buff[READ_IMAGE_LENGTH] = {0};
    uint8_t headAndtail[] = {0xff, 0xaa, 0x01, 0xff, 0xbb};
    uint8_t rt_length = 0;

    uartWriteBuffer(&headAndtail[0], 3);
    uartWriteBuffer((uint8_t*)(&camera->totalLength), 4);

    arducamUartWrite(((camera->currentPictureMode & 0x0f) << 4) | 0x01);
    while (camera->receivedLength) {
        rt_length = readBuff(camera, buff, READ_IMAGE_LENGTH);
        uartWriteBuffer(buff, rt_length);
    }
    uartWriteBuffer(&headAndtail[3], 2);
}

void reportCameraInfo(ArducamCamera* camera)
{
    uint8_t headAndtail[] = {0xff, 0xaa, 0x02, 0xff, 0xbb};
    uint32_t len = 0;
    char buff[400];
    uartWriteBuffer(&headAndtail[0], 3);
    sprintf(buff,
            "ReportCameraInfo\r\nCamera Type:%s\r\nCamera Support Resolution:%d\r\nCamera Support "
            "specialeffects:%d\r\nCamera Support Focus:%d\r\nCamera Exposure Value Max:%ld\r\nCamera Exposure Value "
            "Min:%d\r\nCamera Gain Value Max:%d\r\nCamera Gain Value Min:%d\r\nCamera Support Sharpness:%d\r\n",
            camera->myCameraInfo.cameraId, camera->myCameraInfo.supportResolution,
            camera->myCameraInfo.supportSpecialEffects, camera->myCameraInfo.supportFocus,
            camera->myCameraInfo.exposureValueMax, camera->myCameraInfo.exposureValueMin,
            camera->myCameraInfo.gainValueMax, camera->myCameraInfo.gainValueMin,
            camera->myCameraInfo.supportSharpness);
    len = strlen(buff);
    uartWriteBuffer((uint8_t*)&len, 4);
    printf(buff);
    uartWriteBuffer(&headAndtail[3], 2);
}
void reportVerInfo(ArducamCamera* myCamera)
{
    uint8_t headAndtail[] = {0xff, 0xaa, 0x03, 0xff, 0xbb};
    uint32_t len = 6;
    uartWriteBuffer(&headAndtail[0], 3);

    uartWriteBuffer((uint8_t*)&len, 4);
    uartWriteBuffer(myCamera->verDateAndNumber, 4);
    printf("\r\n");
    uartWriteBuffer(&headAndtail[3], 2);
}

void reportSdkVerInfo(ArducamCamera* myCamera)
{
    uint8_t headAndtail[] = {0xff, 0xaa, 0x05, 0xff, 0xbb};

    uint32_t len = 6;
    uartWriteBuffer(&headAndtail[0], 3);
    uartWriteBuffer((uint8_t*)&len, 4);
    uartWriteBuffer((uint8_t*)(&myCamera->currentSDK->sdkVersion), 4);
    printf("\r\n");
    uartWriteBuffer(&headAndtail[3], 2);
}

void send_data_pack(char cmd_type, char* msg)
{
    uint8_t headAndtail[] = {0xff, 0xaa, 0x07, 0xff, 0xbb};
    headAndtail[2] = cmd_type;
    uint32_t len = strlen(msg) + 2;
    uartWriteBuffer(&headAndtail[0], 3);
    uartWriteBuffer((uint8_t*)&len, 4);
    printf(msg);
    printf("\r\n");
    uartWriteBuffer(&headAndtail[3], 2);
}

volatile uint8_t uart_state = 0;
volatile uint8_t uart1_rx_cnt = 0;
volatile uint8_t uart1_rx_head = 0;
volatile uint8_t uart1_rx_len = 0;

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        UartCommBuff[uart1_rx_cnt] = USART_ReceiveData(USART1);
        uart1_rx_cnt++;
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);

        if (UartCommBuff[uart1_rx_cnt - 1] == 0x55)
            uart1_rx_head = uart1_rx_cnt - 1;
        if ((UartCommBuff[uart1_rx_head] == 0x55) && (UartCommBuff[uart1_rx_cnt - 1] == 0xAA)) {
            uart1_rx_len = uart1_rx_cnt - 1 - uart1_rx_head;
            uart_state = 1;
        }
    }
}
