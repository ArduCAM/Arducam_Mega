// Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
// License: MIT License (https://en.wikipedia.org/wiki/MIT_License)
// Web: http://www.ArduCAM.com
// This program is a demo of how to use the enhanced functions
// It can  continue shooting and store it into the SD card  in JPEG format
// The demo sketch will do the following tasks
// 1. Set the camera to JPEG output mode.
// 2. Capture a JPEG photo and buffer the image to FIFO
// 3.Write the picture data to the SD card
// 5.close the file
#include "Arducam_Mega.h"
// #include "Platform.h"
#include <SD.h>

#define  BUFFER_SIZE  0xff

const int CS = 7;
const int SD_CS = 9;
uint8_t count = 0;
char name[10] = {0};
uint8_t rtLength = 0;
File outFile;
uint8_t imageData = 0;
uint8_t imageDataNext = 0;
uint8_t headFlag = 0;
unsigned int i = 0;
uint8_t imageBuff[BUFFER_SIZE] = {0};

Arducam_Mega myCAM( CS );
uint8_t keyState = 0;
uint8_t isCaptureFlag = 0;
void setup() {
    pinMode(SD_CS, OUTPUT);
    Serial.begin(115200);
    myCAM.begin();
    myCAM.takePicture(CAM_IMAGE_MODE_QVGA,CAM_IMAGE_PIX_FMT_JPG);
    while(!SD.begin(SD_CS))
    {
        Serial.println(F("SD Card Error!")); 
        arducamDelayMs(1000);
    }
    Serial.println(F("SD Card detected."));
}


void loop() {
    myCAM.takePicture(CAM_IMAGE_MODE_QVGA,CAM_IMAGE_PIX_FMT_JPG);
    while (myCAM.getReceivedLength())
    {
        imageData = imageDataNext;
        imageDataNext = myCAM.readByte();
        if (headFlag == 1)
        {
            imageBuff[i++]=imageDataNext;  
            if (i >= BUFFER_SIZE)
            {
                outFile.write(imageBuff, i);
                i = 0;
            }
        }
        if (imageData == 0xff && imageDataNext ==0xd8)
        {
            headFlag = 1;
            sprintf(name,"%d.jpg",count);
            count++;
            outFile = SD.open(name,FILE_WRITE|FILE_READ);
            if (! outFile)
            {
                Serial.println(F("File open failed"));
                while (1);
            }
            imageBuff[i++]=imageData;
            imageBuff[i++]=imageDataNext;  
        }
        if (imageData == 0xff && imageDataNext ==0xd9)
        {
            headFlag = 0;
            outFile.write(imageBuff, i);
            i = 0;
            outFile.close();
            Serial.println(F("Image save succeed"));
            break;
        }
    }
    arducamDelayMs(5000);
    
}