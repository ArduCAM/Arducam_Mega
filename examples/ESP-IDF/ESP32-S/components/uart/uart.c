#include "uart.h"
#include <string.h>

uint8_t uartCommBuff[BUF_SIZE] = {0};
uint8_t uartCommLength         = 0;
uint8_t readBuffLength         = 0;

void uartReceiveTask(void* arg)
{
    int len = 0;
    while (1) {
        len = uart_read_bytes(ECHO_UART_PORT_NUM, uartCommBuff, (BUF_SIZE - 1), 20 / portTICK_RATE_MS);
        if (len > 0) {
            uartCommLength = len;
        }
        vTaskDelay(20/portTICK_RATE_MS);
    }
}

uint32_t uartAvailable(void)
{
    return uartCommLength;
}

uint8_t uartRead(void)
{
    uint8_t rt = 0;
    rt         = uartCommBuff[readBuffLength];
    readBuffLength++;
    if (readBuffLength == uartCommLength) {
        readBuffLength = 0;
        uartCommLength = 0;
    }
    return rt;
}

void arducamUartFlush(void)
{
    readBuffLength = 0;
    uartCommLength = 0;
    memset(uartCommBuff, 0, BUF_SIZE);
}

void uartInit(uint32_t baudrate)
{
    uart_config_t uart_config = {
        .baud_rate  = baudrate,
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    int intr_alloc_flags = 0;
    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));
}


void uartBegin(uint32_t baudrate)
{
    uartInit(baudrate);
    xTaskCreate(uartReceiveTask, "uartReceiveTaskName", 3072, NULL, 10, NULL);
}


void uartWrite(uint8_t data)
{
    uint8_t buff[1] = {0};
    buff[0]         = data;
    uart_write_bytes(ECHO_UART_PORT_NUM, (const char*)buff, 1);
}

void uartWriteBuffer(uint8_t* buff, uint32_t length)
{
    uart_write_bytes(ECHO_UART_PORT_NUM, (const char*)buff, length);
}