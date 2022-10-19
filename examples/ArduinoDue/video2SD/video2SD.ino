// Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
// License: MIT License (https://en.wikipedia.org/wiki/MIT_License)
// Web: http://www.ArduCAM.com
// This program is a demo of how to use the enhanced functions
// It can  continue shooting  and store it into the SD card  in AVI format
// The demo sketch will do the following tasks
// 1.Shoot video button, began to shoot video 
// 2. Set the camera to JPEG output mode.
// 3. Capture a JPEG photo and buffer the image to FIFO
// 4.Write AVI Header
// 5.Write the video data to the SD card
// 6.More updates AVI file header
// 7.close the file
// The file header introduction
// 00-03 :RIFF
// 04-07 :The size of the data
// 08-0B :File identifier
// 0C-0F :The first list of identification number
// 10-13 :The size of the first list
// 14-17 :The hdr1 of identification
// 18-1B :Hdr1 contains avih piece of identification 
// 1C-1F :The size of the avih
// 20-23 :Maintain time per frame picture
#include "Arducam_Mega.h"
// #include "Platform.h"
#include <SD.h>

#define  FRAMES_NUM   200
#define  AVIOFFSET    240
#define  rate         0x0a
#define  BUFFER_SIZE  0xff

const int KEY = 2;
const int CS = 7;
const int SD_CS = 9;
Arducam_Mega myCAM( CS );
uint8_t count = 0;
char name[10] = {0};
uint8_t imageData = 0;
uint8_t imageDataNext = 0;
uint8_t headFlag = 0;
uint8_t keyState = 0;
uint8_t isCaptureFlag = 0;
uint8_t remnant = 0;
unsigned long position = 0;
uint16_t frame_cnt = 0;
uint8_t i=0;
unsigned int imageCount = 0;
uint8_t imageBuff[BUFFER_SIZE] = {0};
File outFile;


uint32_t total_time = 0;
unsigned long movi_size = 0;
unsigned long jpeg_size = 0;
const char zero_buf[4] = {0x00, 0x00, 0x00, 0x00};
const int avi_header[AVIOFFSET] PROGMEM ={
  0x52, 0x49, 0x46, 0x46, 0xD8, 0x01, 0x0E, 0x00, 0x41, 0x56, 0x49, 0x20, 0x4C, 0x49, 0x53, 0x54,
  0xD0, 0x00, 0x00, 0x00, 0x68, 0x64, 0x72, 0x6C, 0x61, 0x76, 0x69, 0x68, 0x38, 0x00, 0x00, 0x00,
  0xA0, 0x86, 0x01, 0x00, 0x80, 0x66, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x40, 0x01, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4C, 0x49, 0x53, 0x54, 0x84, 0x00, 0x00, 0x00,
  0x73, 0x74, 0x72, 0x6C, 0x73, 0x74, 0x72, 0x68, 0x30, 0x00, 0x00, 0x00, 0x76, 0x69, 0x64, 0x73,
  0x4D, 0x4A, 0x50, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, rate, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x74, 0x72, 0x66,
  0x28, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x18, 0x00, 0x4D, 0x4A, 0x50, 0x47, 0x00, 0x84, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4C, 0x49, 0x53, 0x54,
  0x10, 0x00, 0x00, 0x00, 0x6F, 0x64, 0x6D, 0x6C, 0x64, 0x6D, 0x6C, 0x68, 0x04, 0x00, 0x00, 0x00,
  0x64, 0x00, 0x00, 0x00, 0x4C, 0x49, 0x53, 0x54, 0x00, 0x01, 0x0E, 0x00, 0x6D, 0x6F, 0x76, 0x69,
};

void print_quartet(unsigned long i,File fd){
  fd.write(i % 0x100);  i = i >> 8;   //i /= 0x100;
  fd.write(i % 0x100);  i = i >> 8;   //i /= 0x100;
  fd.write(i % 0x100);  i = i >> 8;   //i /= 0x100;
  fd.write(i % 0x100);
}


void setup() {
    pinMode(KEY, INPUT);
    pinMode(SD_CS, OUTPUT);
    Serial.begin(115200);
    myCAM.begin();
    while(!SD.begin(SD_CS))
    {
        Serial.println(F("SD Card Error!")); 
        arducamDelayMs(1000);
    }
    Serial.println(F("SD Card detected."));
}


void loop() {
    keyState = digitalRead(KEY);
    if (!keyState)
    {
        isCaptureFlag = 1;
        while(!digitalRead(KEY));  
    }
    if (isCaptureFlag)
    {
        isCaptureFlag = 0;

        sprintf(name,"%d.avi",count);
        count++;
        outFile = SD.open(name, O_WRITE | O_CREAT | O_TRUNC);
        if (!outFile)
        {
            Serial.println(F("File open failed"));
            while (1);
        }
        Serial.println(F("File open succeed"));
        //Write AVI Header
        for ( i = 0; i < AVIOFFSET; i++)
        {
            outFile.write(pgm_read_byte(&avi_header[i]));
        }
        Serial.println("start Capture");
        myCAM.takeMultiPictures(CAM_IMAGE_MODE_QVGA,CAM_IMAGE_PIX_FMT_JPG,FRAMES_NUM);
        Serial.println("stop Capture");
        movi_size = 0;
        frame_cnt = 0;
        while (myCAM.getReceivedLength())
        {
            
            imageData = imageDataNext;
            imageDataNext = myCAM.readByte();

            if (headFlag == 1)
            {
                imageBuff[imageCount++]=imageDataNext;  
                jpeg_size++;
                if (imageCount >= BUFFER_SIZE)
                {
                    outFile.write(imageBuff, imageCount);
                    imageCount = 0;
                }
            }

            if (imageData == 0xff && imageDataNext ==0xd8)
            {
                headFlag = 1;
                jpeg_size = 0;
                outFile.write("00dc");
                outFile.write(zero_buf, 4);
                outFile.write(imageData);
                outFile.write(imageDataNext);
                jpeg_size += 2;       
            }

            if (imageData == 0xff && imageDataNext ==0xd9)
            {
                outFile.write(imageBuff, imageCount);
                imageCount = 0;
                headFlag = 0;
                remnant = (4 - (jpeg_size & 0x00000003)) & 0x00000003;
                jpeg_size = jpeg_size + remnant;
                movi_size = movi_size + jpeg_size;
                if (remnant > 0)
                {
                    outFile.write(zero_buf, remnant);
                }
                position = outFile.position();
                outFile.seek(position - 4 - jpeg_size);
                print_quartet(jpeg_size, outFile);
                position = outFile.position();
                outFile.seek(position + 6);
                outFile.write("AVI1", 4);
                position = outFile.position();
                outFile.seek(position + jpeg_size - 10);
                frame_cnt++;
            }
        }   
        //Modify the MJPEG header from the beginning of the file
        outFile.seek(4);
        print_quartet(movi_size +12*frame_cnt+4, outFile);//riff file size
        //overwrite hdrl
        unsigned long us_per_frame = 1000000 / rate; //(per_usec); //hdrl.avih.us_per_frame
        outFile.seek(0x20);
        print_quartet(us_per_frame, outFile);
        unsigned long max_bytes_per_sec = movi_size * rate/ frame_cnt; //hdrl.avih.max_bytes_per_sec
        outFile.seek(0x24);
        print_quartet(max_bytes_per_sec, outFile);
        unsigned long tot_frames = frame_cnt;    //hdrl.avih.tot_frames
        outFile.seek(0x30);
        print_quartet(tot_frames, outFile);
        unsigned long frames =frame_cnt;// (TOTALFRAMES); //hdrl.strl.list_odml.frames
        outFile.seek(0xe0);
        print_quartet(frames, outFile);
        outFile.seek(0xe8);
        print_quartet(movi_size, outFile);// size again
        //Close the file
        outFile.close();
        Serial.println(F("Movie save OK"));
    }
    
}