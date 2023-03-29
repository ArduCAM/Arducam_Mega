#include "apps_config.h"
#include "LCD_Touch.h"
#include "gui_func.h"
#include "hardware/uart.h"
#include "lcd_driver/LCD_SPI_interface.h"

void pico4ml_uart_init(uint32_t baudrate)
{
    stdio_init_all();
    uart_init(UART_ID, 2400);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);
    // Actually, we want a different speed
    // The call will return the actual baud rate selected, which will be as
    // close as possible to that requested
    int actual = uart_set_baudrate(UART_ID, baudrate);
    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);
    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);
}

/*Init spi bus and cs */
void pico4ml_lcd_spi_init(uint32_t spi_baudrate)
{
    spi_init(LCD_SPI_PORT, spi_baudrate);
    gpio_set_function(LCD_MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(LCD_MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(LCD_SCK_PIN, GPIO_FUNC_SPI);
}

/*Init spi bus and cs */
void pico4ml_camera_spi_init(uint32_t spi_baudrate)
{
    spi_init(CAMERA_SPI_PORT, spi_baudrate);
    gpio_set_function(CAMERA_MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(CAMERA_MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(CAMERA_SCK_PIN, GPIO_FUNC_SPI);
}

void pico4ml_lcd_init(int irq_flag)
{
    // lcd_cs_init(LCD_CS_PI    N);
    // lcd_dc_init(LCD_DC_PIN);
    // LCD_Init(SCAN_DIR_DFT);
    if (irq_flag != true && irq_flag != false)
        lcd_init1_(irq_flag);
    else
        lcd_init_(irq_flag);
    touch_cs_init(TP_CS_PIN);
    TP_Init(7);
    // LCD_Clear(GRAY);
    touch_irq_init(TP_IRQ_PIN, irq_flag);
}

void pico4ml_init(int irq_flag)
{
    pico4ml_uart_init(115200);
    pico4ml_lcd_spi_init(SPI0_BAUDRATE);
    pico4ml_lcd_init(irq_flag);
}
