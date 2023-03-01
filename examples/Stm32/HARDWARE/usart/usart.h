/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#ifndef __USART_H
#define __USART_H

#include "ArducamCamera.h"
#include "stdio.h"
#include "stm32f10x.h"

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

volatile extern uint8_t uart_state;
volatile extern uint8_t uart1_rx_cnt;
volatile extern uint8_t uart1_rx_head;
volatile extern uint8_t uart1_rx_len;
extern uint8_t UartCommBuff[20];

uint8_t commandProcessing(ArducamCamera*, uint8_t*, uint8_t);
void reportCameraInfo(ArducamCamera*);
void reportVerInfo(ArducamCamera* myCamera);
void reportSdkVerInfo(ArducamCamera* myCamera);
void cameraGetPicture(ArducamCamera*);
void send_data_pack(char cmd_type, char* msg);

void uartInit(uint32_t BaudRate);
void arducamUartWrite(uint8_t data);
void uartWriteBuffer(uint8_t* buff, uint32_t length);
uint32_t arducamUartAvailable(void);
uint8_t arducamUartRead(void);

#endif
