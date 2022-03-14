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

int key = 0;
int csPin = 22;
ArducamCamera myCAM;
int count = 0;
char name[10] = {0};
unsigned char imageBuff[300] = {0};
unsigned int imageLength = 0;
int scanKeyboard()
{
    int in;
    struct termios new_settings;
    struct termios stored_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0,&stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&new_settings);
    
    in = getchar();
    
    tcsetattr(0,TCSANOW,&stored_settings);
    return in;
}

void saveImage(ArducamCamera myCAM)
{
    unsigned long value = 0;
    unsigned char data = 0;
    FILE* fl = NULL;
    sprintf(name,"%d.jpg",count);
    fl = fopen(name,"w+");
    if (fl == NULL)
    {
        printf("Create file failed\r\n");
        return;
    }
    while (myCAM.receivedLength)
    {
        imageLength = readBuff(&myCAM,imageBuff,200);
        fwrite(imageBuff,1,imageLength,fl);
    }
    
    fclose(fl);
    count++;
    
}

int main(void)
{
	unsigned char sendBuff[10] = {0};
	unsigned char readBuff[10] = {0};
	myCAM = createArducamCamera(csPin);
	begin(&myCAM);
	printf("Camera Init Succeed\r\n");
    printf("Click the 's' button on the keyboard to save the image\r\n");
    printf("Press the keyboard 'q' key to exit\r\n");
    while (1)
    {
        key = scanKeyboard();
        if(key == 's')  //'s'
        {
            printf("\r\n");
            takePicture(&myCAM,CAM_IMAGE_MODE_QVGA,CAM_IMAGE_PIX_FMT_JPG);
            saveImage(myCAM);
            printf("Image save succeed\r\n");
        }
        else if(key == 'q')
        {
            printf("\r\n");
            printf("quit picture mode\r\n");
            break;
        }
    }
	spiRelease();
	return 0;
}





