#include "uart.h"
#include <string.h>

uint8_t uartCommBuff[BUF_SIZE] = {0};
uint8_t uartCommLength = 0;
uint8_t readBuffLength = 0;

void uartReceiveTask(void *arg)
{   
	int len = 0;
    while (1)
    {
        len = uart_read_bytes(ECHO_UART_PORT_NUM, uartCommBuff, (BUF_SIZE - 1), 20 / portTICK_RATE_MS);
        if (len > 0)
        {
			uartCommLength = len;
        }
    } 
}


void uartBegin(uint32_t baudrate)
{
    uartInit(baudrate);
    xTaskCreate(uartReceiveTask, "uartReceiveTaskName", 4096, NULL, 10, NULL);
}

void uartInit(uint32_t baudrate)
{
    uart_config_t uart_config = {
        .baud_rate = baudrate,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    int intr_alloc_flags = 0;
    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, -1, -1, ECHO_TEST_RTS, ECHO_TEST_CTS));   
}

uint32_t arducamUartAvailable(void)
{
	return uartCommLength;
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
void arducamUartFlush(void)
{
	readBuffLength = 0;
	uartCommLength = 0;	
	memset(uartCommBuff,0,BUF_SIZE);
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
		uartWriteBuffer(buff,rt_length);
	}
	arducamUartWrite(0x55);
	arducamUartWrite(0xBB);
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

void reportCameraInfo(ArducamCamera*camera)
{
	printf("ReportCameraInfo\n");
	printf("Camera Type:");
	printf("%s\n",camera->myCameraInfo.cameraId); 
	printf("Camera Support Resolution:");
	printf("%d\n",camera->myCameraInfo.supportResolution);
	printf("Camera Support specialeffects:");
	printf("%d\n",camera->myCameraInfo.supportSpecialEffects);
	printf("Camera Support Focus:");
	printf("%d\n",camera->myCameraInfo.supportFocus);
	printf("Camera Exposure Value Max:");
	printf("%ld\n",camera->myCameraInfo.exposureValueMax); 
	printf("Camera Exposure Value Min:");
	printf("%d\n",camera->myCameraInfo.exposureValueMin); 
	printf("Camera Gain Value Max:");
	printf("%d\n",camera->myCameraInfo.gainValueMax);
	printf("Camera Gain Value Min:");
	printf("%d\n",camera->myCameraInfo.gainValueMin);
	printf("Camera Support Sharpness:");
	printf("%d\n",camera->myCameraInfo.supportSharpness);
}


void arducamUartWrite(uint8_t data)
{
	uint8_t buff[1]={0};
	buff[0] = data;
	uart_write_bytes(ECHO_UART_PORT_NUM, (const char *) buff, 1);
}

void uartWriteBuffer(uint8_t* buff,uint32_t length)
{
	uart_write_bytes(ECHO_UART_PORT_NUM, (const char *) buff, length);
}