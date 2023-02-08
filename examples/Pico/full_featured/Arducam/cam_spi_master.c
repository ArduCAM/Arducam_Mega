#include "stdlib.h"
#include "string.h"
#include "pico/malloc.h"
#include "pico/binary_info.h"
#include "hardware/irq.h"
#include "hardware/spi.h"
#include "cam_spi_master.h"
// #include "pico/stdlib.h"

uint8_t spi_write_read(uint8_t value)
{
	uint8_t rxDat;
	spi_write_read_blocking(CAM_SPI_PORT, &value, &rxDat, 1);
	return rxDat;
}

int spi_write_block(uint8_t *p_value, size_t len)
{
	return spi_write_blocking(CAM_SPI_PORT, p_value, len);
}

int spi_read_block(uint8_t *ptr_value, size_t len)
{
	return spi_read_blocking(CAM_SPI_PORT,0x00, ptr_value, len);
}

void spi_begin()
{
	spi_init(CAM_SPI_PORT, 8*1000*1000);
    gpio_set_function(CAM_MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(CAM_MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(CAM_SCK_PIN, GPIO_FUNC_SPI);
}

int spi_cs_output_mode(uint8_t pin)
{
	gpio_init(pin);
	gpio_set_dir(pin, GPIO_OUT);
	gpio_put(pin, 1);
	return 0;
}

void spi_cs_high(uint16_t pin)
{
	gpio_put(pin, 1);
}
void spi_cs_low(uint8_t pin)
{
	gpio_put(pin, 0);
}
