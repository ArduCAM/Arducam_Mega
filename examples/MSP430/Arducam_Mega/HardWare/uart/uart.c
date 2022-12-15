/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#include "uart.h"
#include <msp430.h>

uint8_t uartCommBuff[20] = {0};
uint8_t uartCommLength = 0;
uint8_t readBuffLength = 0;

unsigned char rxData = 0;
int fputc(int ch,FILE *f)
{
    UCA0TXBUF = ch&0xff;
    while(!(IFG2 & UCA0TXIFG));
     return ch;
}
int fputs(const char *_ptr, register FILE *_fp)
{
  unsigned int i, len;

  len = strlen(_ptr);

  for(i=0 ; i<len ; i++){
      UCA0TXBUF = _ptr[i]&0xff;
    while(!(IFG2 & UCA0TXIFG));

  }

  return len;
}


void uartInit(void)
{
    P1SEL |= BIT1 + BIT2;
    P1SEL2 |= BIT1 + BIT2;
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 0x11;                            // 16MHz 921600
    UCA0BR1 = 0;                              // 16MHz 921600
    UCA0MCTL =UCBRS_0;//UCBRS0;               // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;
    _enable_interrupts();
}

void arducamUartWrite(uint8_t data)
{

    while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
    UCA0TXBUF = data;
}

void uartWriteBuffer(uint8_t* buff,uint32_t length)
{
    uint32_t num = 0;
    for(num = 0; num < length; num++)
    {
        arducamUartWrite(buff[num]);
    }
}

uint8_t commandProcessing(ArducamCamera*camera,uint8_t* buff,uint8_t length)
{
    CamStatus state;
    uint16_t GainValue = 0;
    uint32_t ExposureValue = 0;
    uint32_t ExposureLen1 = 0;
    uint32_t ExposureLen2 = 0;
    uint32_t ExposureLen3 = 0;
    uint8_t CameraResolution = camera->currentPictureMode;
    uint8_t CameraFarmat = camera->currentPixelFormat;
    switch (buff[0])
    {
        case SET_PICTURE_RESOLUTION:                                                                //Set Camera Resolution
                CameraResolution = buff[1] & 0x0f;
                CameraFarmat = (buff[1] & 0x70) >> 4;
                takePicture(camera,(CAM_IMAGE_MODE)CameraResolution,(CAM_IMAGE_PIX_FMT)CameraFarmat);
            break;
        case SET_VIDEO_RESOLUTION:                                                              //Set Video Resolution
                camera->previewMode=TRUE;
                CameraResolution = buff[1] & 0x0f;
                state = startPreview(camera,(CAM_VIDEO_MODE)CameraResolution);
                if (state == CAM_ERR_NO_CALLBACK)
                {
//                  printf("callback function is not registered");
                }
            break;
        case SET_BRIGHTNESS:                                                                //Set brightness
                setBrightness(camera,(CAM_BRIGHTNESS_LEVEL)buff[1]);
            break;
        case SET_CONTRAST:                                                              //Set Contrast
                setContrast(camera,(CAM_CONTRAST_LEVEL)buff[1]);
            break;
        case SET_SATURATION:                                                                //Set saturation
                setSaturation(camera,(CAM_STAURATION_LEVEL)buff[1]);
            break;
        case SET_EV:                                                                //Set EV
                setEV(camera,(CAM_EV_LEVEL)buff[1]);
            break;
        case SET_WHITEBALANCE:                                                              //Set White balance
                setAutoWhiteBalanceMode(camera,(CAM_WHITE_BALANCE)buff[1]);
            break;
        case SET_SPECIAL_EFFECTS:                                                               //Set Special effects
                setColorEffect(camera,(CAM_COLOR_FX)buff[1]);
            break;
        case SET_FOCUS_CONTROL:                                                             //Focus Control
                setAutoFocus(camera,buff[1]);
                if (buff[1] == 0)
                {
                    setAutoFocus(camera,0x02);
                }
            break;
        case SET_EXPOSURE_CONTROL:                                                              //exposure control
                setAutoExposure(camera,buff[1]&0x01);
            break;
        case SET_GAIN_CONTROL:                                                              //Gain control
                setAutoISOSensitive(camera,buff[1]&0x01);
            break;
        case SET_WHILEBALANCE_CONTROL:                                                              //while balance control
                setAutoWhiteBalance(camera,buff[1]&0x01);
            break;
        case SET_SHARPNESS:
                setSharpness(camera,(CAM_SHARPNESS_LEVEL)buff[1]);
        break;
        case SET_MANUAL_GAIN:                                                           //manual gain control
                GainValue = (buff[1]<<8) | buff[2];
                setISOSensitivity(camera,GainValue);
            break;
        case SET_MANUAL_EXPOSURE:                                                           //manual exposure control
                ExposureLen1 = buff[1];
                ExposureLen2 = buff[2];
                ExposureLen3 = buff[3];
                ExposureValue = (ExposureLen1<<16) | (ExposureLen2<<8) | ExposureLen3;
                setAbsoluteExposure(camera,ExposureValue);
            break;
        case GET_CAMERA_INFO:                                                           //Get Camera info
                reportCameraInfo(camera);
            break;
        case TAKE_PICTURE:
                takePicture(camera,(CAM_IMAGE_MODE)CameraResolution,(CAM_IMAGE_PIX_FMT)CameraFarmat);
                //printf("length = %d\r\n",camera->totalLength);
                cameraGetPicture(camera);
            break;
        case DEBUG_WRITE_REGISTER:
                debugWriteRegister(camera,buff+1);
            break;
        default:
            break;
    }
    return buff[0];
}

void cameraGetPicture(ArducamCamera*camera)
{
    camera->burstFirstFlag = 0;
    uint8_t buff[READ_IMAGE_LENGTH] = {0};
    uint8_t rt_length = 0;
    arducamUartWrite(0x55);
    arducamUartWrite(0xAA);
    arducamUartWrite(camera->cameraDataFormat);
    arducamUartWrite((uint8_t)(camera->totalLength&0xff));
    arducamUartWrite((uint8_t)((camera->totalLength>>8)&0xff));
    arducamUartWrite((uint8_t)((camera->totalLength>>16)&0xff));
    arducamUartWrite((uint8_t)((camera->totalLength>>24)&0xff));
    while (camera->receivedLength)
    {
        rt_length=readBuff(camera,buff,READ_IMAGE_LENGTH);
        for (uint8_t i = 0; i < rt_length; i++)
        {
            arducamUartWrite(buff[i]);

        }
    }
    arducamUartWrite(0x55);
    arducamUartWrite(0xBB);
}


void reportCameraInfo(ArducamCamera*camera)
{
    printf("ReportCameraInfo\r\n");
    printf("Camera Type:");
    printf("%s\r\n",camera->myCameraInfo.cameraId);
    printf("Camera Support Resolution:");
    printf("%d\r\n",camera->myCameraInfo.supportResolution);
    printf("Camera Support specialeffects:");
    printf("%d\r\n",camera->myCameraInfo.supportSpecialEffects);
    printf("Camera Support Focus:");
    printf("%d\r\n",camera->myCameraInfo.supportFocus);
    printf("Camera Exposure Value Max:");
    printf("%d\r\n",camera->myCameraInfo.exposureValueMax);
    printf("Camera Exposure Value Min:");
    printf("%d\r\n",camera->myCameraInfo.exposureValueMin);
    printf("Camera Gain Value Max:");
    printf("%d\r\n",camera->myCameraInfo.gainValueMax);
    printf("Camera Gain Value Min:");
    printf("%d\r\n",camera->myCameraInfo.gainValueMin);
    printf("Camera Support Sharpness:");
    printf("%d\r\n",camera->myCameraInfo.supportSharpness);
}


uint8_t arducamUartRead(void)
{
    uint8_t rt = 0;
    rt = uartCommBuff[readBuffLength];
    readBuffLength++;
    if(readBuffLength == uartCommLength)
    {
        readBuffLength = 0;
        uartCommLength = 0;
    }
    return rt;
}


uint32_t arducamUartAvailable(void)
{
    return uartCommLength;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    uartCommBuff[uartCommLength] = UCA0RXBUF;
    uartCommLength++;

}

