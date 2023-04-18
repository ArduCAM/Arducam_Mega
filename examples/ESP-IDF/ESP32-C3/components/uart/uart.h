#ifndef __UART_H
#define __UART_H

#include "ArducamCamera.h"
#include "driver/uart.h"
#include "esp_log.h"
#include <stdio.h>

#define ECHO_TEST_TXD        (21)
#define ECHO_TEST_RXD        (20)
#define ECHO_TEST_RTS        (-1)
#define ECHO_TEST_CTS        (-1)

#define ECHO_UART_PORT_NUM   0
#define ECHO_TASK_STACK_SIZE 100
#define BUF_SIZE             100

void uartBegin(uint32_t baudrate);
uint32_t uartAvailable(void);
uint8_t uartRead(void);
void uartWrite(uint8_t data);
void uartWriteBuffer(uint8_t* buff, uint32_t length);
void uartFlush(void);

#endif /*__UART_H*/