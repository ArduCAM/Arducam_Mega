#include "uart.h"
#include"string.h"
#include "stdio.h"
#include <stdlib.h>
uint8_t UartCommBuff[20] = {0};
uint8_t UartCommLength = 0;
uint8_t ReadBuffLength = 0;
uint8_t uart_temp = 0;

void arducamUartWrite(uint8_t data)
{
    while(SERCOM5_USART_WriteIsBusy()){
     ;
    }
    SERCOM5_USART_Write(&data, 1);
   
}

void uartWriteBuffer(uint8_t* buff,uint32_t length)
{
     while(SERCOM5_USART_WriteIsBusy()){
    ;
    }
	SERCOM5_USART_Write(buff, length);
   
}

void write_string(char* buff)
{   
    uint index = 0;
    while(buff[index] != '\0'){
     while(SERCOM5_USART_WriteIsBusy()){
        ;
     }
     arducamUartWrite(buff[index]);
     index++;
    }
    
   
}

void uart_read_callback (){
    UartCommBuff[UartCommLength] = uart_temp;
    UartCommLength++;
    SERCOM5_USART_Read(&uart_temp, 1);
    
    
}
void uart_init(){
    uintptr_t context = 0;
    SERCOM5_USART_ReadCallbackRegister( uart_read_callback, context );
    SERCOM5_USART_Read(&uart_temp, 1);
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


void SendIint(int n) 
{
	unsigned char s;
	while(n!=0)
	{
		s=(unsigned char)n%10+48;
		arducamUartWrite(s);
		s=s/10;
	}
}


char* itoa(int num,char* str,int radix)
{
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//???
    unsigned unum;//????????????,??????????
    int i=0,j,k;//i?????????????????i????????????????????????????k?????????????;j?????????????
 
    //????????????
    if(radix==10&&num<0)//?????????????
    {
        unum=(unsigned)-num;//?num??????unum
        str[i++]='-';//??????????'-'???????1
    }
    else unum=(unsigned)num;//??num????????unum
 
    //??????????????
    do
    {
        str[i++]=index[unum%(unsigned)radix];//?unum??????????str?????????1
        unum/=radix;//unum??????
 
    }while(unum);//??unum?0????
 
    str[i]='\0';//????????'\0'???c??????'\0'???
 
    //???????
    if(str[0]=='-') k=1;//??????????????????????
    else k=0;//???????????
 
    char temp;//?????????????
    for(j=k;j<=(i-1)/2;j++)//?????????i????????????????????1
    {
        temp=str[j];//?????????
        str[j]=str[i-1+k-j];//???????
        str[i-1+k-j]=temp;//???????(??????????)????
    }
 
    return str;//?????????
 
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
					write_string("callback function is not registered");
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
    char string[32] = {0};
	write_string("ReportCameraInfo\n");
            
	write_string("Camera Type:");
    sprintf(string, "%s",camera->myCameraInfo.cameraId);
    write_string(string);
	write_string("\r\n"); 
    
	write_string("Camera Support Resolution:");
    itoa(camera->myCameraInfo.supportResolution,string,10);
    write_string(string);
    write_string("\r\n");
    
	write_string("Camera Support specialeffects:");
    itoa(camera->myCameraInfo.supportSpecialEffects,string,10);
    write_string(string);
	write_string("\r\n")
            ; 
	write_string("Camera Support Focus:");
   // uartWriteBuffer(&camera->myCameraInfo.supportFocus,1 );
    itoa(camera->myCameraInfo.supportFocus,string,10);
    write_string(string);
	write_string("\r\n");
	
	write_string("Camera Exposure Value Max:");
   // uartWriteBuffer((uint8_t*)camera->myCameraInfo.exposureValueMax,sizeof(camera->myCameraInfo.exposureValueMax) );
	itoa(camera->myCameraInfo.exposureValueMax,string,10);
    write_string(string);
    write_string("\r\n");
	
	write_string("Camera Exposure Value Min:");
    itoa(camera->myCameraInfo.exposureValueMin,string,10);
    write_string(string);
    //uartWriteBuffer((uint8_t*)camera->myCameraInfo.exposureValueMin,sizeof(camera->myCameraInfo.exposureValueMin) );
	write_string("\r\n");
 
	write_string("Camera Gain Value Max:");
    itoa(camera->myCameraInfo.gainValueMax,string,10);
    write_string(string);
   // uartWriteBuffer((uint8_t*)camera->myCameraInfo.gainValueMax,sizeof(camera->myCameraInfo.gainValueMax) ); 
	write_string("\r\n");
    
	write_string("Camera Gain Value Min:");
    itoa(camera->myCameraInfo.gainValueMin,string,10);
    write_string(string);
    //uartWriteBuffer((uint8_t*)camera->myCameraInfo.gainValueMin,sizeof(camera->myCameraInfo.gainValueMin) );
	write_string("\r\n");

	write_string("Camera Support Sharpness:");
     itoa(camera->myCameraInfo.supportSharpness,string,10);
    write_string(string);
    //uartWriteBuffer(&camera->myCameraInfo.supportSharpness,1 );
	write_string("\r\n");
}
