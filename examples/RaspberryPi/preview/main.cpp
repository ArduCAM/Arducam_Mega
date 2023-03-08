/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#include "ArducamCamera.h"
#include "time.h"

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <termio.h>
#include <unistd.h>

using namespace cv;
using namespace std;

#define JPEG_BUF_SIZE 10 * 1024 * 1024 // 2M
int csPin = 22;
ArducamCamera myCAM;
unsigned int imageLength = 0;

void display_fps(void)
{
    static int count = 0;
    ++count;
    static std::chrono::high_resolution_clock::time_point time_beg = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point time_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::ratio<1, 1>> duration_s = time_end - time_beg;
    if (duration_s.count() >= 1) {
        std::cout << "fps:" << count << std::endl;
        count = 0;
        time_beg = time_end;
    }
}

typedef struct {
    unsigned long dataLength;
    uint8_t pu8ImageData[JPEG_BUF_SIZE];
    uint8_t cameraID;
    unsigned int errCount = 0;
} imageData;

imageData* frameData = new imageData;

cv::Mat JPGToMat(uint8_t* bytes, int length)
{
    cv::Mat image = Mat(1, length, CV_8UC1, bytes);
    if (length <= 0) {
        image.data = NULL;
        return image;
    }
    image = imdecode(image, IMREAD_ANYCOLOR);
    return image;
}

cv::Mat ConvertImage(imageData* frameData)
{
    cv::Mat rawImage;
    if (frameData->dataLength <= 0) {
        printf("The %d camera is error.\r\n", frameData->cameraID);
        return rawImage;
    }
    rawImage = JPGToMat(frameData->pu8ImageData, frameData->dataLength);
    return rawImage;
}

uint8_t getOneframe(imageData* dataBuf)
{
    unsigned long retLen;
    bool getheader = false;
    bool getend = false;
    bool errorFlag = 0;
    uint8_t temp_pre = 0;
    uint8_t buff[128] = {0};
    dataBuf->dataLength = 0;
    takePicture(&myCAM, CAM_IMAGE_MODE_VGA, CAM_IMAGE_PIX_FMT_JPG);
#if 1
    do {
        retLen = readBuff(&myCAM, buff, 128);
        for (uint16_t i = 0; i < retLen; i++) {

            if (getheader == true) {
                dataBuf->pu8ImageData[++dataBuf->dataLength] = buff[i];
                // If find the end ,break while,
                if ((dataBuf->pu8ImageData[dataBuf->dataLength] == 0xD9) &&
                    (dataBuf->pu8ImageData[dataBuf->dataLength - 1] == 0xFF)) {
                    getend = true;
                    break;
                }
            } else if ((buff[i] == 0xD8) && (dataBuf->pu8ImageData[dataBuf->dataLength] == 0xFF)) {
                getheader = true;
                dataBuf->pu8ImageData[++dataBuf->dataLength] = buff[i];
            } else {
                dataBuf->pu8ImageData[dataBuf->dataLength] = buff[i];
            }
        }
        if (getheader == true && getend == true)
            return true;
    } while (myCAM.receivedLength);
    return false;
#elif 0
    dataBuf->dataLength = readBuff(&myCAM, dataBuf->pu8ImageData, myCAM.totalLength);
    return true;
#endif
}

int main(void)
{
    cv::Mat captureImage;
    myCAM = createArducamCamera(csPin);
    begin(&myCAM);
    printf("Camera Init Succeed\r\n");
    while (1) {
        if (getOneframe(frameData)) {
            captureImage = JPGToMat(frameData->pu8ImageData, frameData->dataLength);
            if (captureImage.empty()) {
                printf("Image load failed\r\n");
            } else {
                cv::imshow("Arducam Mega", captureImage);
            }
            display_fps();
            if (waitKey(1) == 'q')
                break;
        } else {
            frameData->errCount++;
            printf("The %d frame is error\r\n", frameData->errCount);
        }
    }
    return 0;
}
