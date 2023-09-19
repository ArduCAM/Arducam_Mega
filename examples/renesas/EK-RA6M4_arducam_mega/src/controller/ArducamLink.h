/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#ifndef __ARDUCAMLINK_H
#define __ARDUCAMLINK_H
#include "../libcamera/ArducamCamera.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RESET_CAMERA                0XFF
#define SET_PICTURE_RESOLUTION      0X01
#define SET_VIDEO_RESOLUTION        0X02
#define SET_BRIGHTNESS              0X03
#define SET_CONTRAST                0X04
#define SET_SATURATION              0X05
#define SET_EV                      0X06
#define SET_WHITEBALANCE            0X07
#define SET_SPECIAL_EFFECTS         0X08
#define SET_FOCUS_CONTROL           0X09
#define SET_EXPOSUREANDGAIN_CONTROL 0X0A
// #define SET_GAIN_CONTROL         0X0B
#define SET_WHILEBALANCE_CONTROL    0X0C
#define SET_MANUAL_GAIN             0X0D
#define SET_MANUAL_EXPOSURE         0X0E
#define GET_CAMERA_INFO             0X0F
#define TAKE_PICTURE                0X10
#define SET_SHARPNESS               0X11
#define DEBUG_WRITE_REGISTER        0X12
#define STOP_STREAM                 0X21
#define GET_FRM_VER_INFO            0X30
#define GET_SDK_VER_INFO            0X40
#define SET_IMAGE_QUALITY           0X50

#define READ_IMAGE_LENGTH           255

#define MESSAGE_HEADER              (uint8_t []){0xff, 0xaa}
#define MESSAGE_TAIL                (uint8_t []){0xff,0xbb}
#define MESSAGE_IMAGE               0x01
#define MESSAGE_CAMERA_INFO         0x02
#define MESSAGE_VERSION             0x03
#define MESSAGE_DEBUGS              0x04
#define MESSAGE_SDK_INFO            0x05
#define MESSAGE_STREAMOFF           0x06
#define MESSAGE_OTHER               0x07

uint8_t uartCommandProcessing(ArducamCamera*, uint8_t*);

void arducamUartBegin(uint32_t);
void reportCameraInfo(ArducamCamera*);
void reportVerInfo(ArducamCamera* myCamera);
void reportSdkVerInfo(ArducamCamera* myCamera);
void cameraGetPicture(ArducamCamera*);
void arducamFlush(void);
void send_data_pack(char* msg);

void arducamUartPrintf(char* buff);
void arducamUartWrite(uint8_t);
void arducamUartWriteBuff(uint8_t*, uint32_t);
uint32_t arducamUartAvailable(void);
uint8_t arducamUartRead(void);
#ifdef __cplusplus
}
#endif
#endif /*__ARDUCAMLINK_H*/
