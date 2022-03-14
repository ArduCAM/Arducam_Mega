/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#include "Arducam.h"
#include "Platform.h"
#include <stdio.h>
#include <termio.h>
#include <opencv2/opencv.hpp>
#include "time.h"
#include <unistd.h>

using namespace cv;
using namespace std;

#define JPEG_BUF_SIZE   2*1024*1024 //2M
int csPin = 22;
ArducamCamera myCAM;
unsigned int imageLength = 0;



typedef struct  
{
    unsigned long  dataLength;
    uint8_t        pu8ImageData[JPEG_BUF_SIZE];
    uint8_t cameraID;
    unsigned int errCount = 0;
}imageData;


imageData* ArducamImageData = new imageData;

cv::Mat JPGToMat(uint8_t* bytes,int length)
{
	cv::Mat image = Mat(1,length,CV_8UC1,bytes);
	if(length <= 0)
  {
		image.data = NULL;
	    return image;
	}
	image = imdecode(image,IMREAD_ANYCOLOR);
	return image;
}

cv::Mat ConvertImage(imageData* frameData)
{
	cv::Mat rawImage ;
  if(frameData->dataLength<=0)
  {
    printf("The %d camera is error.\r\n",frameData->cameraID);  
    return rawImage;
  }
	rawImage = JPGToMat(frameData->pu8ImageData,frameData->dataLength);
	return rawImage;
}


imageData* getOneframe(imageData* dataBuf)
{
  unsigned long i  = 0;
  bool is_header=0;
  bool is_end = 0;
  bool errorFlag = 0;
  uint8_t temp_last = 0;
  uint8_t temp = 0;
  dataBuf->dataLength = 0;
  takePicture(&myCAM,CAM_IMAGE_MODE_QVGA,CAM_IMAGE_PIX_FMT_JPG);
   while (myCAM.receivedLength) 
   {
      temp_last = temp;
      temp = readByte(&myCAM);
     //Read JPEG data from FIFO
      if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
      {
        is_end = 1;
        dataBuf->pu8ImageData[i++] = temp;  //save the last  0XD9   
        dataBuf->dataLength = i-1;
       // is_header = false;
        break;
      } 
      if (is_header == true)
      { 
        dataBuf->pu8ImageData[i++] = temp;       
      }
      
      else if ((temp == 0xD8) && (temp_last == 0xFF))
      {
        is_header = true;
        dataBuf->pu8ImageData[i++] = temp_last;
        dataBuf->pu8ImageData[i++] = temp; 
      } 
   	}
   if(is_header == 0)
   {
     printf("The %d camera can't find the header\r\n",dataBuf->cameraID);
       errorFlag = 1;
   }
   else
   {
      is_header = 0;
   }
   if(is_end == 0)
   {
     printf("The %d camera can't find the end\r\n",dataBuf->cameraID);
     errorFlag = 1;
   }
   else 
   {
      is_end = 0;
   }   
   if(errorFlag)
   {
     dataBuf->errCount ++;
     printf("The %d frame is error\r\n",dataBuf->errCount);
   }

  return dataBuf;
}

int main(void)
{
    cv::Mat rawImage ;
    myCAM = createArducamCamera(csPin);
    begin(&myCAM);
    printf("Camera Init Succeed\r\n");
    while (1)
    {
      getOneframe(ArducamImageData); 
      rawImage = ConvertImage(ArducamImageData);
      if(rawImage.empty())
      {
        printf("Image load failed\r\n");
      }
      else
      {
        cv::imshow("Arducam Mega", rawImage);
      }
      if(waitKey(20) >=0);      
    }
	return 0;
}





