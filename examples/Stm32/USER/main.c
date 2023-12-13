/*
 * This file is part of the Arducam SPI Camera project.
 *
 * Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 */
#include "ArducamCamera.h"
#include "Platform.h"
#include "usart.h"

int CS                   = 4;
uint8_t temp             = 0xff;
uint8_t commandBuff[20]  = {0};
uint8_t commandLength    = 0;
uint8_t sendFlag         = TRUE;
uint32_t readImageLength = 0;
uint8_t jpegHeadFlag     = 0;
ArducamCamera myCAM;

uint8_t ReadBuffer(uint8_t* imagebuf, uint8_t length)
{
    if (imagebuf[0] == 0xff && imagebuf[1] == 0xd8) {
        jpegHeadFlag = 1;
        arducamUartWrite(0xff);
        arducamUartWrite(0xAA);
        arducamUartWrite(0x01);
        uartWriteBuffer((uint8_t*)&myCAM.totalLength,4);
        arducamUartWrite(((myCAM.currentPictureMode & 0x0f) << 4) | 0x01);
    }
    if (jpegHeadFlag == 1) {
        readImageLength += length;
        for (uint8_t i = 0; i < length; i++) {
            arducamUartWrite(imagebuf[i]);
        }
    }
    if (readImageLength == myCAM.totalLength) {
        readImageLength = 0;
        jpegHeadFlag    = 0;
        arducamUartWrite(0xff);
        arducamUartWrite(0xBB);
    }
    return sendFlag;
}

void stop_handle()
{
    readImageLength = 0;
    jpegHeadFlag    = 0;
    uint32_t len = 9;
    arducamUartWrite(0xff);
    arducamUartWrite(0xBB);

    arducamUartWrite(0xff);
    arducamUartWrite(0xAA);
    arducamUartWrite(0x06);
    uartWriteBuffer((uint8_t*)&len,4);
    printf("streamoff");
    arducamUartWrite(0xff);
    arducamUartWrite(0xBB);
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delayInit();
    uartInit(921600);
    send_data_pack(7,"Hello stm32");
    myCAM = createArducamCamera(CS);
    begin(&myCAM);
    send_data_pack(8,"mega start");

    registerCallback(&myCAM, ReadBuffer, 200, stop_handle);
    while (1) {

        if (uart_state) {
            // arducamDelayMs(5);
            // arducamUartWrite(UartCommBuff[uart1_rx_head + 1]);

            uart_state = 0;
            commandProcessing(&myCAM, &UartCommBuff[uart1_rx_head + 1], uart1_rx_len);
            uart1_rx_len = 0;
            uart1_rx_cnt = 0;
        }
        captureThread(&myCAM);
    }
}
