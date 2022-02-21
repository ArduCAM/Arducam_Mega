/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#include "Arducam.h"


/// @cond

#define ARDUCHIP_FRAMES		    0x01  
#define ARDUCHIP_TEST1       	0x00  //TEST register
#define ARDUCHIP_FIFO      		0x04  //FIFO and I2C control
#define FIFO_CLEAR_MASK    		0x01
#define FIFO_START_MASK    		0x02
#define FIFO_RDPTR_RST_MASK     0x10
#define FIFO_WRPTR_RST_MASK     0x20

#define ARDUCHIP_TRIG      		0x44  //Trigger source
#define VSYNC_MASK         		0x01
#define SHUTTER_MASK       		0x02
#define CAP_DONE_MASK      		0x04


#define FIFO_SIZE1				0x45  //Camera write FIFO size[7:0] for burst to read
#define FIFO_SIZE2				0x46  //Camera write FIFO size[15:8]
#define FIFO_SIZE3				0x47  //Camera write FIFO size[18:16]


#define BURST_FIFO_READ			0x3C  //Burst FIFO read operation
#define SINGLE_FIFO_READ		0x3D  //Single FIFO read operation

#if defined (__MSP430G2553__)
    #define BUF_MAX_LENGTH  50
#else
    #define BUF_MAX_LENGTH  255
#endif

#define CAPRURE_MAX_NUM      0xff

#define CAM_REG_POWER_CONTROL        				0X02
#define CAM_REG_SENSOR_RESET				 		0X07			
#define CAM_REG_FORMAT               				0X20
#define CAM_REG_CAPTURE_RESOLUTION   				0X21
#define CAM_REG_BRIGHTNESS_CONTROL   				0X22
#define CAM_REG_CONTRAST_CONTROL     				0X23
#define CAM_REG_SATURATION_CONTROL   				0X24
#define CAM_REG_EV_CONTROL           				0X25
#define CAM_REG_WHILEBALANCE_MODE_CONTROL   		0X26
#define CAM_REG_COLOR_EFFECT_CONTROL 				0X27
#define CAM_REG_SHARPNESS_CONTROL   				0X28
#define CAM_REG_AUTO_FOCUS_CONTROL   				0X29
#define CAM_REG_EXPOSURE_GAIN_WHILEBALANCE_CONTROL 	0X2A
#define CAM_REG_MANUAL_GAIN_BIT_9_8  				0X2B
#define CAM_REG_MANUAL_GAIN_BIT_7_0  				0X2C
#define CAM_REG_MANUAL_EXPOSURE_BIT_19_16  			0X2D
#define CAM_REG_MANUAL_EXPOSURE_BIT_15_8   			0X2E
#define CAM_REG_MANUAL_EXPOSURE_BIT_7_0   	 		0X2F
#define CAM_REG_SENSOR_ID 					 		0x40
#define CAM_REG_SENSOR_STATE				 		0x44
#define CAM_REG_DEBUG_DEVICE_ADDRESS  				0X0A
#define CAM_REG_DEBUG_REGISTER_HIGH   				0X0B
#define CAM_REG_DEBUG_REGISTER_LOW    				0X0C
#define CAM_REG_DEBUG_REGISTER_VALUE  				0X0D

#define CAM_REG_SENSOR_STATE_IDLE (1<<1)
#define CAM_SENSOR_RESET_ENABLE   (1<<6)
#define CAM_FORMAT_BASICS         (0<<0)
#define CAM_SET_CAPTURE_MODE      (0<<7)
#define CAM_SET_VIDEO_MODE        (1<<7)

#define SET_WHILEBALANCE  0X02
#define SET_EXPOSURE      0X01
#define SET_GAIN		  0X00

#define CAMERA_TYPE_NUMBER    2

#define CAMERA_OV5640     0
#define CAMERA_OV3640     1

#define FORMAT_JPEG    0X01
#define FORMAT_RGB     0X02
#define FORMAT_YUV     0X03




#define RESOLUTION_160X120     (1<<0)
#define RESOLUTION_320X240     (1<<1)
#define RESOLUTION_640X480     (1<<2)
#define RESOLUTION_800X600     (1<<3)
#define RESOLUTION_1280X720    (1<<4)
#define RESOLUTION_1280X960    (1<<5)
#define RESOLUTION_1600X1200   (1<<6)
#define RESOLUTION_1920X1080   (1<<7)
#define RESOLUTION_2048X1536   (1<<8)
#define RESOLUTION_2592X1944   (1<<9)

#define SPECIAL_NORMAL       (0<<0)
#define SPECIAL_BLUEISH      (1<<0)
#define SPECIAL_REDISH	 	 (1<<1)
#define SPECIAL_BW			 (1<<2)
#define SPECIAL_SEPIA		 (1<<3)
#define SPECIAL_NEGATIVE	 (1<<4)
#define SPECIAL_GREENISH	 (1<<5)
#define SPECIAL_OVEREXPOSURE (1<<6)
#define SPECIAL_SOLARIZE	 (1<<7)
#define SPECIAL_YELLOWISH    (1<<8)


struct cameraDefaultState
{
	uint8_t cameraDefaultFormat;
	uint8_t cameraDefaultResolution;
};

void setCapture(ArducamCamera* MyCamera);
void csHigh(ArducamCamera*MyCamera);
void csLow(ArducamCamera*MyCamera);
void writeReg(ArducamCamera*MyCamera,uint8_t addr,uint8_t val);
uint8_t readReg(ArducamCamera*MyCamera,uint8_t addr);
uint8_t busRead(ArducamCamera*MyCamera,int address);
uint8_t busWrite(ArducamCamera*MyCamera,int address,int value);
void clearFifoFlag(ArducamCamera*MyCamera);
uint32_t readFifoLength(ArducamCamera*MyCamera);
uint8_t getBit(ArducamCamera*MyCamera,uint8_t addr, uint8_t bit);
void setFifoBurst(ArducamCamera*MyCamera);
void setCapture(ArducamCamera* MyCamera);
void waitI2cIdle(ArducamCamera*MyCamera);
uint32_t imageAvailable(ArducamCamera*MyCamera);
void flushFifo(ArducamCamera*MyCamera);
void startCapture(ArducamCamera*MyCamera);


struct CameraInfo OV5640_CameraInfo = {
	.cameraId = "5MP",
	.supportResolution = RESOLUTION_320X240|RESOLUTION_640X480|RESOLUTION_1280X720|RESOLUTION_1600X1200|RESOLUTION_1920X1080|RESOLUTION_2592X1944,				
	.supportSpecialEffects = SPECIAL_BLUEISH|SPECIAL_REDISH|SPECIAL_BW|SPECIAL_SEPIA|SPECIAL_NEGATIVE|SPECIAL_GREENISH|SPECIAL_OVEREXPOSURE|SPECIAL_SOLARIZE,
	.exposureValueMax = 30000,	
	.exposureValueMin = 1000,
	.gainValueMax = 1023,
	.gainValueMin = 10,
	.supportFocus = TRUE,
	.supportSharpness = FALSE,
	.deviceAddress = 0x78,
};

struct CameraInfo OV3640_CameraInfo = {
	.cameraId = "3MP",
	.supportResolution = RESOLUTION_320X240|RESOLUTION_640X480|RESOLUTION_1280X720|RESOLUTION_1600X1200|RESOLUTION_1920X1080|RESOLUTION_2048X1536,
	.supportSpecialEffects=SPECIAL_BLUEISH|SPECIAL_REDISH|SPECIAL_BW|SPECIAL_SEPIA|SPECIAL_NEGATIVE|SPECIAL_GREENISH|SPECIAL_YELLOWISH,
	.exposureValueMax = 1400,	
	.exposureValueMin = 100,
	.gainValueMax = 31,
	.gainValueMin = 1,
	.supportFocus = FALSE,
	.supportSharpness = TRUE,
	.deviceAddress = 0x78,
};				


struct CameraInfo CameraType[CAMERA_TYPE_NUMBER];

struct cameraDefaultState ov5640DefaultState = {
	.cameraDefaultFormat = CAM_IMAGE_PIX_FMT_JPG,
	.cameraDefaultResolution = CAM_IMAGE_MODE_WQXGA2,
};

struct cameraDefaultState ov3640efaultState = {
	.cameraDefaultFormat = CAM_IMAGE_PIX_FMT_JPG,
	.cameraDefaultResolution = CAM_IMAGE_MODE_QXGA,
};

struct cameraDefaultState cameraDefaultInfo[CAMERA_TYPE_NUMBER];

														
uint8_t ov3640GainValue[]={0x00,0x10,0x18,0x30,0x34,0x38,0x3b,0x3f,0x72,0x74,0x76,0x78,0x7a,0x7c,0x7e,0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff};



void cameraInit(ArducamCamera* MyCamera)
{
	arducamSpiBegin();
	arducamCsOutputMode(MyCamera->csPin);
	arducamSpiCsPinLow(MyCamera->csPin);
}




CamStatus cameraBegin(ArducamCamera*MyCamera)
{
	cameraInit(MyCamera);
	writeReg(MyCamera,CAM_REG_SENSOR_RESET,CAM_SENSOR_RESET_ENABLE);  //reset cpld and camera
	waitI2cIdle(MyCamera);																					//Wait I2c Idle
	MyCamera->cameraId=readReg(MyCamera,CAM_REG_SENSOR_ID);
	waitI2cIdle(MyCamera);
	MyCamera->cameraId&=0X0F;
	MyCamera->cameraId-=1;
	MyCamera->myCameraInfo = CameraType[MyCamera->cameraId];
	MyCamera->currentPixelFormat = cameraDefaultInfo[MyCamera->cameraId].cameraDefaultFormat;
	MyCamera->currentPictureMode = cameraDefaultInfo[MyCamera->cameraId].cameraDefaultResolution;
	writeReg(MyCamera,CAM_REG_DEBUG_DEVICE_ADDRESS,CameraType[MyCamera->cameraId].deviceAddress);
	waitI2cIdle(MyCamera);
	return CAM_ERR_SUCCESS;
}

void cameraSetCapture(ArducamCamera*MyCamera)
{
	flushFifo(MyCamera);
	clearFifoFlag(MyCamera);
	startCapture(MyCamera);
	while(getBit(MyCamera,ARDUCHIP_TRIG, CAP_DONE_MASK)==0);
	MyCamera->receivedLength=readFifoLength(MyCamera);	
	MyCamera->totalLength = MyCamera->receivedLength;
	MyCamera->burstFirstFlag = 0;
}

uint32_t cameraImageAvailable(ArducamCamera*MyCamera)
{
	return MyCamera->receivedLength;
}


CamStatus cameraSetAutoFocus(ArducamCamera*MyCamera,uint8_t val)
{
	writeReg(MyCamera,CAM_REG_AUTO_FOCUS_CONTROL,val);  //auto focus control
	waitI2cIdle(MyCamera);															//Wait I2c Idle
	return 	CAM_ERR_SUCCESS;
}


CamStatus cameraTakePicture(ArducamCamera*MyCamera,CAM_IMAGE_MODE mode, CAM_IMAGE_PIX_FMT pixel_format)
{
	if ((MyCamera->currentPixelFormat != pixel_format) || (MyCamera->currentPictureMode != mode))
	{
		MyCamera->currentPixelFormat = pixel_format;
		MyCamera->cameraDataFormat = pixel_format;
		MyCamera->currentPictureMode = mode;
		writeReg(MyCamera,CAM_REG_FORMAT,pixel_format);  //set the data format
		waitI2cIdle(MyCamera);					//Wait I2c Idle	
		writeReg(MyCamera,CAM_REG_CAPTURE_RESOLUTION,CAM_SET_CAPTURE_MODE|mode);  //set resolution
		waitI2cIdle(MyCamera);					//Wait I2c Idle
	}
	setCapture(MyCamera);
	return CAM_ERR_SUCCESS;
}


CamStatus cameratakeMultiPictures(ArducamCamera*MyCamera,CAM_IMAGE_MODE mode, CAM_IMAGE_PIX_FMT pixel_format, uint8_t num)
{
	if ((MyCamera->currentPixelFormat != pixel_format) || (MyCamera->currentPictureMode != mode))
	{
		MyCamera->currentPixelFormat = pixel_format;
		MyCamera->cameraDataFormat = pixel_format;
		MyCamera->currentPictureMode = mode;
		writeReg(MyCamera,CAM_REG_FORMAT,pixel_format);  //set the data format
		waitI2cIdle(MyCamera);					//Wait I2c Idle	
		writeReg(MyCamera,CAM_REG_CAPTURE_RESOLUTION,CAM_SET_CAPTURE_MODE|mode);  //set resolution
		waitI2cIdle(MyCamera);					//Wait I2c Idle
	}
	if (num > CAPRURE_MAX_NUM)
	{
		num = CAPRURE_MAX_NUM;
	}
	writeReg(MyCamera,ARDUCHIP_FRAMES,num);
	setCapture(MyCamera);
	return CAM_ERR_SUCCESS;
}

void cameraRegisterCallback(ArducamCamera* MyCamera,BUFFER_CALLBACK function,uint8_t size)
{
	
	MyCamera->callBackFunction=function;
	MyCamera->blockSize=size;
}

CamStatus cameraStartPreview(ArducamCamera*MyCamera,CAM_VIDEO_MODE mode)
{
	MyCamera->cameraDataFormat = CAM_IMAGE_PIX_FMT_JPG;
	MyCamera->previewMode=TRUE;
	uint8_t symbol=TRUE;
	uint8_t callBackLength=0;
	uint8_t callBackBuff[BUF_MAX_LENGTH];
	writeReg(MyCamera,CAM_REG_FORMAT,CAM_IMAGE_PIX_FMT_JPG);  //set  jpeg format
	waitI2cIdle(MyCamera);					//Wait I2c Idle
	writeReg(MyCamera,CAM_REG_CAPTURE_RESOLUTION,CAM_SET_VIDEO_MODE|mode);  //set  video mode
	waitI2cIdle(MyCamera);					//Wait I2c Idle
	setCapture(MyCamera);
	while (MyCamera->previewMode)
	{
		callBackLength=readBuff(MyCamera,callBackBuff,MyCamera->blockSize);
		if(callBackLength != FALSE)
		{
			symbol=MyCamera->callBackFunction(callBackBuff,callBackLength);
			if (symbol == FALSE)
			{
				stopPreview(MyCamera);
				break;				
			}
		}
		else 
		{
			setCapture(MyCamera);
		}
	}
	return CAM_ERR_SUCCESS;
}

CamStatus cameraStopPreview(ArducamCamera* MyCamera)
{
	MyCamera->currentPixelFormat = CAM_IMAGE_PIX_FMT_JPG;
	MyCamera->currentPictureMode = CAM_IMAGE_MODE_QVGA;
	MyCamera->previewMode = FALSE;
	MyCamera->receivedLength = 0;
	MyCamera->totalLength = 0;
	writeReg(MyCamera,CAM_REG_FORMAT,CAM_IMAGE_PIX_FMT_JPG);  //set  jpeg format
	waitI2cIdle(MyCamera);					//Wait I2c Idle
	return CAM_ERR_SUCCESS;
}

CamStatus cameraSetAutoWhiteBalanceMode(ArducamCamera* MyCamera,CAM_WHITE_BALANCE mode)
{
	writeReg(MyCamera,CAM_REG_WHILEBALANCE_MODE_CONTROL,mode);  	//set Light Mode
	waitI2cIdle(MyCamera);					//Wait I2c Idle
	return CAM_ERR_SUCCESS;
}

CamStatus cameraSetAutoWhiteBalance(ArducamCamera*MyCamera,uint8_t val)
{
	unsigned char symbol = 0;
	if (val == 0)
	{
		symbol |= 0x80;
	}
	symbol |= SET_WHILEBALANCE;
	writeReg(MyCamera,CAM_REG_EXPOSURE_GAIN_WHILEBALANCE_CONTROL,symbol);			//while balance control
	waitI2cIdle(MyCamera);					//Wait I2c Idle
	return CAM_ERR_SUCCESS;
}

CamStatus cameraSetAutoISOSensitive(ArducamCamera*MyCamera,uint8_t val)
{
	unsigned char symbol = 0;
	if (val == 0)
	{
		symbol |= 0x80;
	}
	symbol |= SET_GAIN;
	writeReg(MyCamera,CAM_REG_EXPOSURE_GAIN_WHILEBALANCE_CONTROL,symbol);			// auto gain control
	waitI2cIdle(MyCamera);					//Wait I2c Idle
	return CAM_ERR_SUCCESS;
}

CamStatus cameraSetISOSensitivity(ArducamCamera*MyCamera,int iso_sense)
{
	if (MyCamera->cameraId == CAMERA_OV3640)
	{
		iso_sense = ov3640GainValue[iso_sense-1];
	}
	writeReg(MyCamera,CAM_REG_MANUAL_GAIN_BIT_9_8, iso_sense>>8);  //set AGC VALUE
	waitI2cIdle(MyCamera);
	writeReg(MyCamera,CAM_REG_MANUAL_GAIN_BIT_7_0, iso_sense&0xff); 
	waitI2cIdle(MyCamera);
	return CAM_ERR_SUCCESS;	
}

CamStatus cameraSetAutoExposure(ArducamCamera*MyCamera,uint8_t val)
{
	unsigned char symbol=0;
	if (val == 0)
	{
		symbol |= 0x80;
	}
	symbol |= SET_EXPOSURE;
	writeReg(MyCamera,CAM_REG_EXPOSURE_GAIN_WHILEBALANCE_CONTROL,symbol);			// auto EXPOSURE control
	waitI2cIdle(MyCamera);					//Wait I2c Idle
	return CAM_ERR_SUCCESS;
}

CamStatus cameraSetAbsoluteExposure(ArducamCamera*MyCamera,uint32_t exposure_time)
{
	writeReg(MyCamera,CAM_REG_MANUAL_EXPOSURE_BIT_19_16, (uint8_t)((exposure_time>>16)&0xff));  	//set exposure output [19:16]  
	waitI2cIdle(MyCamera);
	writeReg(MyCamera,CAM_REG_MANUAL_EXPOSURE_BIT_15_8, (uint8_t)((exposure_time>>8)&0xff));  		//set exposure output [15:8]   
	waitI2cIdle(MyCamera);
	writeReg(MyCamera,CAM_REG_MANUAL_EXPOSURE_BIT_7_0, (uint8_t)(exposure_time&0xff));			//set exposure output [7:0]    
	waitI2cIdle(MyCamera);
	return CAM_ERR_SUCCESS;
}


CamStatus cameraSetColorEffect(ArducamCamera* MyCamera,CAM_COLOR_FX effect)
{
	writeReg(MyCamera,CAM_REG_COLOR_EFFECT_CONTROL,effect);  	//set effect
	waitI2cIdle(MyCamera);					//Wait I2c Idle
	return CAM_ERR_SUCCESS;
}
CamStatus cameraSetSaturation(ArducamCamera*MyCamera,CAM_STAURATION_LEVEL level)
{
	writeReg(MyCamera,CAM_REG_SATURATION_CONTROL,level);  	//set Saturation Level 
	waitI2cIdle(MyCamera);					//Wait I2c Idle
	return CAM_ERR_SUCCESS;
}
CamStatus cameraSetEV(ArducamCamera*MyCamera,CAM_EV_LEVEL level)
{
	writeReg(MyCamera,CAM_REG_EV_CONTROL,level);  	//set Exposure  Compensation Level
	waitI2cIdle(MyCamera);					//Wait I2c Idle
	return CAM_ERR_SUCCESS;
}
CamStatus cameraSetContrast(ArducamCamera*MyCamera,CAM_CONTRAST_LEVEL level)
{
	writeReg(MyCamera,CAM_REG_CONTRAST_CONTROL,level);  	//set Contrast Level
	waitI2cIdle(MyCamera);					//Wait I2c Idle
	return CAM_ERR_SUCCESS;
}
CamStatus cameraSetSharpness(ArducamCamera*MyCamera,CAM_SHARPNESS_LEVEL level)
{
	writeReg(MyCamera,CAM_REG_SHARPNESS_CONTROL,level);  	//set Brightness Level
	waitI2cIdle(MyCamera);					//Wait I2c Idle	
	return CAM_ERR_SUCCESS;
}

CamStatus cameraSetBrightness(ArducamCamera*MyCamera,CAM_BRIGHTNESS_LEVEL level)
{
	writeReg(MyCamera,CAM_REG_BRIGHTNESS_CONTROL,level);  	//set Brightness Level
	waitI2cIdle(MyCamera);					//Wait I2c Idle
	return CAM_ERR_SUCCESS;
}
void cameraFlushFifo(ArducamCamera*MyCamera)
{
	writeReg(MyCamera,ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}

void cameraStartCapture(ArducamCamera*MyCamera)
{
	writeReg(MyCamera,ARDUCHIP_FIFO, FIFO_START_MASK);
}

void cameraClearFifoFlag(ArducamCamera*MyCamera)
{
	writeReg(MyCamera,ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}

uint32_t cameraReadFifoLength(ArducamCamera*MyCamera)
{
	uint32_t len1,len2,len3,length=0;
	len1 = readReg(MyCamera,FIFO_SIZE1);
	len2 = readReg(MyCamera,FIFO_SIZE2);
	len3 = readReg(MyCamera,FIFO_SIZE3) & 0x7f;
	length = ((len3 << 16) | (len2 << 8) | len1) & 0x07fffff;
	return length;	
}

uint8_t cameraGetBit(ArducamCamera*MyCamera,uint8_t addr, uint8_t bit)
{
  uint8_t temp;
  temp = readReg(MyCamera,addr);
  temp = temp & bit;
  return temp;
}

void cameraSetFifoBurst(ArducamCamera*MyCamera)
{
    arducamSpiTransfer(BURST_FIFO_READ);	
}

uint8_t cameraReadByte(ArducamCamera*MyCamera)
{
	uint8_t data = 0;
	arducamSpiCsPinLow(MyCamera->csPin);	
	arducamSpiTransfer(SINGLE_FIFO_READ);
	arducamSpiTransfer(0x00);
	data = arducamSpiTransfer(0x00);
	arducamSpiCsPinHigh(MyCamera->csPin);	
	MyCamera->receivedLength-=1;
	return data;
}


uint8_t cameraReadBuff(ArducamCamera*MyCamera,uint8_t* buff,uint8_t length)
{
	if ((length > BUF_MAX_LENGTH)||(length==0)) 
	{
		return 0;
	}
	if(imageAvailable(MyCamera) == 0)
	{
		return 0;
	}
	if(MyCamera->receivedLength<length)
	{
		length=MyCamera->receivedLength;
	}
	arducamSpiCsPinLow(MyCamera->csPin);	
	setFifoBurst(MyCamera);
	if (MyCamera->burstFirstFlag == 0)
	{
		MyCamera->burstFirstFlag = 1;
		arducamSpiTransfer(0x00);
	}
	for (uint8_t count = 0; count < length; count++)
	{
		buff[count]=arducamSpiTransfer(0x00);
	}
	arducamSpiCsPinHigh(MyCamera->csPin);	
	MyCamera->receivedLength-=length;
  return length;
}

void cameraWriteReg(ArducamCamera*MyCamera,uint8_t addr,uint8_t val)
{
	busWrite(MyCamera,addr | 0x80, val); 
}

uint8_t cameraReadReg(ArducamCamera*MyCamera,uint8_t addr)
{
	uint8_t data;
	data = busRead(MyCamera,addr & 0x7F);
	return data;
}

uint8_t cameraBusWrite(ArducamCamera*MyCamera,int address,int value)
{	
	arducamSpiCsPinLow(MyCamera->csPin);
	arducamSpiTransfer(address);
	arducamSpiTransfer(value);
	arducamSpiCsPinHigh(MyCamera->csPin);
	arducamDelayMs(1);
	return 1;
}


void cameraCsHigh(ArducamCamera*MyCamera)
{
	 arducamSpiCsPinHigh(MyCamera->csPin);	
}
void cameraCsLow(ArducamCamera*MyCamera)
{
	 arducamSpiCsPinLow(MyCamera->csPin);	
}


uint8_t cameraBusRead(ArducamCamera*MyCamera,int address)
{
	uint8_t value;
	arducamSpiCsPinLow(MyCamera->csPin);
	arducamSpiTransfer(address);
	value = arducamSpiTransfer(0x00);
	value = arducamSpiTransfer(0x00);
	arducamSpiCsPinHigh(MyCamera->csPin);
	return value;

}




void cameraWaitI2cIdle(ArducamCamera*MyCamera)
{
	while ((readReg(MyCamera,CAM_REG_SENSOR_STATE)&0X03) != CAM_REG_SENSOR_STATE_IDLE)
	{
		arducamDelayMs(2);
	}
}



void cameraDebugWriteRegister(ArducamCamera* MyCamera,uint8_t* buff)
{
	uint8_t register_high = buff[0];
	uint8_t	register_low = buff[1];
	uint8_t value = buff[2];
	writeReg(MyCamera,CAM_REG_DEBUG_REGISTER_HIGH,register_high);
	writeReg(MyCamera,CAM_REG_DEBUG_REGISTER_LOW,register_low);	
	writeReg(MyCamera,CAM_REG_DEBUG_REGISTER_VALUE,value);	

}

void cameraLowPowerOn(ArducamCamera*MyCamera)
{
	writeReg(MyCamera,CAM_REG_POWER_CONTROL,0X07);
}

void cameraLowPowerOff(ArducamCamera*MyCamera)
{
	writeReg(MyCamera,CAM_REG_POWER_CONTROL,0X05);
}


CamStatus begin(ArducamCamera* MyCamera)
{
	return MyCamera->arducamCameraOp->begin(MyCamera);
}

CamStatus takePicture(ArducamCamera*MyCamera,CAM_IMAGE_MODE mode, CAM_IMAGE_PIX_FMT pixel_format)
{
	return MyCamera->arducamCameraOp->takePicture(MyCamera,mode,pixel_format);
}

CamStatus takeMultiPictures(ArducamCamera*MyCamera,CAM_IMAGE_MODE mode, CAM_IMAGE_PIX_FMT pixel_format, uint8_t num)
{
	return MyCamera->arducamCameraOp->takeMultiPictures(MyCamera,mode,pixel_format,num);
}

CamStatus startPreview(ArducamCamera*MyCamera,CAM_VIDEO_MODE mode)
{
	return MyCamera->arducamCameraOp->startPreview(MyCamera,mode);
}

CamStatus stopPreview(ArducamCamera*MyCamera)
{
	return MyCamera->arducamCameraOp->stopPreview(MyCamera);
}

CamStatus setAutoExposure(ArducamCamera*MyCamera,uint8_t val)
{
	return MyCamera->arducamCameraOp->setAutoExposure(MyCamera,val);
}
CamStatus setAbsoluteExposure(ArducamCamera*MyCamera,uint32_t val)
{
	return MyCamera->arducamCameraOp->setAbsoluteExposure(MyCamera,val);
}
CamStatus setAutoISOSensitive(ArducamCamera*MyCamera,uint8_t val)
{
	return MyCamera->arducamCameraOp->setAutoISOSensitive(MyCamera,val);
}
CamStatus setISOSensitivity(ArducamCamera*MyCamera,int iso_sense)
{
	return MyCamera->arducamCameraOp->setISOSensitivity(MyCamera,iso_sense);
}

CamStatus setAutoWhiteBalance(ArducamCamera*MyCamera,uint8_t val)
{
	return MyCamera->arducamCameraOp->setAutoWhiteBalance(MyCamera,val);
}

CamStatus setAutoWhiteBalanceMode(ArducamCamera* MyCamera,CAM_WHITE_BALANCE mode)
{
	return MyCamera->arducamCameraOp->setAutoWhiteBalanceMode(MyCamera,mode);
}
CamStatus setColorEffect(ArducamCamera* MyCamera,CAM_COLOR_FX effect)
{
	return MyCamera->arducamCameraOp->setColorEffect(MyCamera,effect);
}

CamStatus setAutoFocus(ArducamCamera*MyCamera,uint8_t val)
{
	return MyCamera->arducamCameraOp->setAutoFocus(MyCamera,val);
}

CamStatus setSaturation(ArducamCamera*MyCamera,CAM_STAURATION_LEVEL level)
{
	return MyCamera->arducamCameraOp->setSaturation(MyCamera,level);
}

CamStatus setEV(ArducamCamera*MyCamera,CAM_EV_LEVEL level)
{
	return MyCamera->arducamCameraOp->setEV(MyCamera,level);
}

CamStatus setContrast(ArducamCamera*MyCamera,CAM_CONTRAST_LEVEL level)
{
	return MyCamera->arducamCameraOp->setContrast(MyCamera,level);
}

CamStatus setBrightness(ArducamCamera*MyCamera,CAM_BRIGHTNESS_LEVEL level)
{
	return MyCamera->arducamCameraOp->setBrightness(MyCamera,level);
}

CamStatus setSharpness(ArducamCamera*MyCamera,CAM_SHARPNESS_LEVEL level)
{
	return MyCamera->arducamCameraOp->setSharpness(MyCamera,level);
}

uint8_t readBuff(ArducamCamera*MyCamera,uint8_t*buff,uint8_t length)
{
	return MyCamera->arducamCameraOp->readBuff(MyCamera,buff,length);
}

uint8_t readByte(ArducamCamera*MyCamera)
{
	return MyCamera->arducamCameraOp->readByte(MyCamera);
}

void debugWriteRegister(ArducamCamera* MyCamera,uint8_t* buff)
{
	MyCamera->arducamCameraOp->debugWriteRegister(MyCamera,buff);
}

void registerCallback(ArducamCamera* MyCamera,BUFFER_CALLBACK function,uint8_t blockSize)
{
	MyCamera->arducamCameraOp->registerCallback(MyCamera,function,blockSize);
}


void lowPowerOn(ArducamCamera*MyCamera)
{
	MyCamera->arducamCameraOp->lowPowerOn(MyCamera);
}
void lowPowerOff(ArducamCamera*MyCamera)
{
	MyCamera->arducamCameraOp->lowPowerOff(MyCamera);	
}




void csHigh(ArducamCamera*MyCamera)
{
	MyCamera->arducamCameraOp->csHigh(MyCamera);
}
void csLow(ArducamCamera*MyCamera)
{
	MyCamera->arducamCameraOp->csLow(MyCamera);
}


void writeReg(ArducamCamera*MyCamera,uint8_t addr,uint8_t val)
{
	MyCamera->arducamCameraOp->writeReg(MyCamera,addr,val);
}

uint8_t readReg(ArducamCamera*MyCamera,uint8_t addr)
{
	return MyCamera->arducamCameraOp->readReg(MyCamera,addr);
}

uint8_t busRead(ArducamCamera*MyCamera,int address)
{
	return MyCamera->arducamCameraOp->busRead(MyCamera,address);
}

uint8_t busWrite(ArducamCamera*MyCamera,int address,int value)
{
	return MyCamera->arducamCameraOp->busWrite(MyCamera,address,value);
}

void startCapture(ArducamCamera*MyCamera)
{
	MyCamera->arducamCameraOp->startCapture(MyCamera);
}
void clearFifoFlag(ArducamCamera*MyCamera)
{
	MyCamera->arducamCameraOp->clearFifoFlag(MyCamera);
}
uint32_t readFifoLength(ArducamCamera*MyCamera)
{
	return MyCamera->arducamCameraOp->readFifoLength(MyCamera);
}
uint8_t getBit(ArducamCamera*MyCamera,uint8_t addr, uint8_t bit)
{
	return MyCamera->arducamCameraOp->getBit(MyCamera,addr,bit);
}
void setFifoBurst(ArducamCamera*MyCamera)
{
	MyCamera->arducamCameraOp->setFifoBurst(MyCamera);
}
void setCapture(ArducamCamera* MyCamera)
{
	MyCamera->arducamCameraOp->setCapture(MyCamera);
}
void waitI2cIdle(ArducamCamera*MyCamera)
{
	MyCamera->arducamCameraOp->waitI2cIdle(MyCamera);
}

uint32_t imageAvailable(ArducamCamera*MyCamera)
{
	return MyCamera->arducamCameraOp->imageAvailable(MyCamera);
}

void flushFifo(ArducamCamera*MyCamera)
{
	MyCamera->arducamCameraOp->flushFifo(MyCamera);
}


const struct CameraOperations ArducamcameraOperations = {
	.begin = cameraBegin,
	.takePicture = cameraTakePicture,
	.takeMultiPictures = cameratakeMultiPictures,
	.startPreview = cameraStartPreview,
	.stopPreview = cameraStopPreview,
	.setAutoExposure = cameraSetAutoExposure,
	.setAbsoluteExposure = cameraSetAbsoluteExposure,
	.setAutoISOSensitive = cameraSetAutoISOSensitive,
	.setISOSensitivity = cameraSetISOSensitivity,
	.setAutoWhiteBalance = cameraSetAutoWhiteBalance,
	.setAutoWhiteBalanceMode = cameraSetAutoWhiteBalanceMode,
	.setColorEffect = cameraSetColorEffect,
	.setAutoFocus = cameraSetAutoFocus,
	.setSaturation = cameraSetSaturation,
	.setEV = cameraSetEV,
	.setContrast = cameraSetContrast,
	.setBrightness = cameraSetBrightness,
	.setSharpness = cameraSetSharpness,
	.registerCallback = cameraRegisterCallback,
	.imageAvailable = cameraImageAvailable,
	.csHigh = cameraCsHigh,
	.csLow = cameraCsLow,
	.readBuff = cameraReadBuff,
	.readByte = cameraReadByte,
	.debugWriteRegister = cameraDebugWriteRegister,
	.writeReg = cameraWriteReg,
	.readReg = cameraReadReg,
	.busRead = cameraBusRead,
	.busWrite = cameraBusWrite,
	.flushFifo = cameraFlushFifo,
	.startCapture = cameraStartCapture,
	.clearFifoFlag = cameraClearFifoFlag,
	.readFifoLength = cameraReadFifoLength,
	.getBit = cameraGetBit,
	.setFifoBurst = cameraSetFifoBurst,
	.setCapture = cameraSetCapture,
	.waitI2cIdle = cameraWaitI2cIdle,
	.lowPowerOn = cameraLowPowerOn,
	.lowPowerOff = cameraLowPowerOff,
};

ArducamCamera createArducamCamera(int CS)
{
	ArducamCamera MyCamera;
	CameraType[0] = OV5640_CameraInfo;
	CameraType[1] = OV3640_CameraInfo;
	cameraDefaultInfo[0] = ov5640DefaultState;
	cameraDefaultInfo[1] = ov3640efaultState;
	MyCamera.cameraId = FALSE;
	MyCamera.cameraDataFormat = FORMAT_JPEG;
	MyCamera.burstFirstFlag = FALSE;
	MyCamera.previewMode=FALSE;
	MyCamera.csPin = CS;
	MyCamera.arducamCameraOp = &ArducamcameraOperations;
	return MyCamera;
}

/// @endcond

