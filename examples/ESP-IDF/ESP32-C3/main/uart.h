#ifndef __UART_H
#define __UART_H


#include <stdio.h>
#include "driver/uart.h"
#include "esp_log.h"
#include "Arducam.h"



#define SET_PICTURE_RESOLUTION   0X01
#define SET_VIDEO_RESOLUTION     0X02
#define SET_BRIGHTNESS			 0X03
#define SET_CONTRAST             0X04
#define SET_SATURATION           0X05
#define SET_EV					 0X06
#define SET_WHITEBALANCE		 0X07
#define SET_SPECIAL_EFFECTS      0X08
#define SET_FOCUS_CONTROL        0X09
#define SET_EXPOSURE_CONTROL     0X0A
#define SET_GAIN_CONTROL         0X0B
#define SET_WHILEBALANCE_CONTROL 0X0C
#define SET_MANUAL_GAIN          0X0D
#define SET_MANUAL_EXPOSURE      0X0E
#define GET_CAMERA_INFO          0X0F
#define TAKE_PICTURE             0X10
#define SET_SHARPNESS            0X11
#define DEBUG_WRITE_REGISTER     0X12
#define READ_IMAGE_LENGTH        255

#define ECHO_TEST_TXD (21)
#define ECHO_TEST_RXD (20)
#define ECHO_TEST_RTS (-1)
#define ECHO_TEST_CTS (-1)

#define ECHO_UART_PORT_NUM 0
#define ECHO_TASK_STACK_SIZE 100
#define BUF_SIZE 100

void uartBegin(uint32_t baudrate);
void uartInit(uint32_t baudrate);
void reportCameraInfo(ArducamCamera*camera);
uint8_t commandProcessing(ArducamCamera*camera,uint8_t* buff,uint8_t length);
uint32_t arducamUartAvailable(void);
uint8_t arducamUartRead(void);
void cameraGetPicture(ArducamCamera*camera);
void arducamUartWrite(uint8_t data);
void uartWriteBuffer(uint8_t* buff,uint32_t length);
void arducamUartFlush(void);

#endif /*__UART_H*/