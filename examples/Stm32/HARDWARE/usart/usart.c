/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#include "usart.h"	  

uint8_t UartCommBuff[20] = {0};
uint8_t UartCommLength = 0;
uint8_t ReadBuffLength = 0;

#if 1
#pragma import(__use_no_semihosting)                           
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;        
void _sys_exit(int x) 
{ 
	x = x; 
} 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);
    USART1->DR = (u8) ch;      
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
	USART_SendData(USART1,data);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
}
void uartWriteBuffer(uint8_t* buff,uint32_t length)
{
	uint32_t num = 0;
	for(num = 0; num < length; num++)
	{
		USART_SendData(USART1,buff[num]);
	}
}

uint8_t arducamUartRead(void)
{
	uint8_t rt = 0;
	rt = UartCommBuff[ReadBuffLength];
	ReadBuffLength++;
	if(ReadBuffLength == UartCommLength)
	{
		ReadBuffLength = 0;
		UartCommLength = 0;
	}
	return rt;
}
uint32_t arducamUartAvailable(void)
{
	return UartCommLength;
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
		case SET_PICTURE_RESOLUTION:																//Set Camera Resolution
				CameraResolution = buff[1] & 0x0f;
				CameraFarmat = (buff[1] & 0x70) >> 4;
				takePicture(camera,(CAM_IMAGE_MODE)CameraResolution,(CAM_IMAGE_PIX_FMT)CameraFarmat);
			break;
		case SET_VIDEO_RESOLUTION:																//Set Video Resolution
                camera->previewMode=TRUE;
				CameraResolution = buff[1] & 0x0f;
				state = startPreview(camera,(CAM_VIDEO_MODE)CameraResolution);
				if (state == CAM_ERR_NO_CALLBACK)
				{
//					printf("callback function is not registered");
				}
			break;
		case SET_BRIGHTNESS:																//Set brightness
				setBrightness(camera,(CAM_BRIGHTNESS_LEVEL)buff[1]);
			break;
		case SET_CONTRAST:																//Set Contrast
				setContrast(camera,(CAM_CONTRAST_LEVEL)buff[1]);
			break;
		case SET_SATURATION:																//Set saturation
				setSaturation(camera,(CAM_STAURATION_LEVEL)buff[1]);
			break;
		case SET_EV:																//Set EV 
				setEV(camera,(CAM_EV_LEVEL)buff[1]);
			break;
		case SET_WHITEBALANCE:																//Set White balance
				setAutoWhiteBalanceMode(camera,(CAM_WHITE_BALANCE)buff[1]);
			break;
		case SET_SPECIAL_EFFECTS:																//Set Special effects
				setColorEffect(camera,(CAM_COLOR_FX)buff[1]);
			break;
		case SET_FOCUS_CONTROL:																//Focus Control
				setAutoFocus(camera,buff[1]);
				if (buff[1] == 0)
				{
					setAutoFocus(camera,0x02);
				}
			break;
		case SET_EXPOSURE_CONTROL:																//exposure control
				setAutoExposure(camera,buff[1]&0x01);
			break;
		case SET_GAIN_CONTROL:																//Gain control
				setAutoISOSensitive(camera,buff[1]&0x01);
			break;
		case SET_WHILEBALANCE_CONTROL:																//while balance control
				setAutoWhiteBalance(camera,buff[1]&0x01);
			break;
		case SET_SHARPNESS:
				setSharpness(camera,(CAM_SHARPNESS_LEVEL)buff[1]);
		break;
		case SET_MANUAL_GAIN:															//manual gain control
				GainValue = (buff[1]<<8) | buff[2];
				setISOSensitivity(camera,GainValue);
			break;
		case SET_MANUAL_EXPOSURE:															//manual exposure control
				ExposureLen1 = buff[1];
				ExposureLen2 = buff[2];
				ExposureLen3 = buff[3];
				ExposureValue = (ExposureLen1<<16) | (ExposureLen2<<8) | ExposureLen3;
		    	setAbsoluteExposure(camera,ExposureValue);
			break;
		case GET_CAMERA_INFO:															//Get Camera info
				reportCameraInfo(camera);
			break;
		case TAKE_PICTURE:
				takePicture(camera,(CAM_IMAGE_MODE)CameraResolution,(CAM_IMAGE_PIX_FMT)CameraFarmat);
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



void USART1_IRQHandler(void)                	
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		UartCommBuff[UartCommLength] = USART_ReceiveData(USART1);
		UartCommLength++;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
} 


