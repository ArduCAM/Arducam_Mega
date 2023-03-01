#ifndef __ARDUCAM_SLOT_H
#define __ARDUCAM_SLOT_H
#include <pico/stdlib.h>
#define USE_UART_IRQ

#define SerialBegin(baudRate)                                                                                          \
    do {                                                                                                               \
        uart_init(uart0, baudRate);                                                                                    \
        gpio_set_function(0, GPIO_FUNC_UART);                                                                          \
        gpio_set_function(1, GPIO_FUNC_UART);                                                                          \
        irq_set_exclusive_handler(UART0_IRQ, uart_rx_handler);                                                         \
        irq_set_enabled(UART0_IRQ, true);                                                                              \
        uart_set_irq_enables(uart0, true, false);                                                                      \
    } while (false)

#define SerialWrite(ch)           uart_putc_raw(uart0, ch)
#define SerialWriteBuff(buf, len) uart_write_blocking(uart0, buf, len)
#define SerialPrintf(str)         uart_puts(uart0, str)
#define SerialAvailable()         uart_is_readable(uart0)
#define SerialRead()              uart_getc(uart0)

void ArducamLink::uart_rx_handler()
{
    UartCommBuff[uart1_rx_cnt++] = uart_getc(uart0);
    if (UartCommBuff[uart1_rx_cnt - 1] == 0x55)
        uart1_rx_head = uart1_rx_cnt - 1;
    if ((UartCommBuff[uart1_rx_head] == 0x55) && (UartCommBuff[uart1_rx_cnt - 1] == 0xAA)) {
        uart1_rx_len = uart1_rx_cnt - 1 - uart1_rx_head;
        uart_state = 1;
    }
}
#endif