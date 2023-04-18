#include "spi.h"
#include <string.h>

spi_device_handle_t spi; 


void spiBegin(void)
{
    spi_bus_config_t buscfg={
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32,
    };

    spi_device_interface_config_t devcfg={
        .mode=0,                                //SPI mode 0
        .spics_io_num=-1,               //CS pin
        .queue_size=256,                          //We want to be able to queue 7 transactions at a time
        .clock_speed_hz = CAMERA_SPI_CLK_FREQ,
    };

    spi_bus_initialize(CAMERA_HOST, &buscfg, SPI_DMA_DISABLED);
    spi_bus_add_device(CAMERA_HOST, &devcfg, &spi);

}

void spiCsOutputMode(int cs)
{
    gpio_config_t io_conf = {};
    uint64_t pinSel = 1ULL << cs;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = pinSel;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
}

void spiCsHigh(int cs)
{
    gpio_set_level(cs, 1);
}

void spiCsLow(int cs)
{
    gpio_set_level(cs, 0);
}

uint8_t spiReadWriteByte(uint8_t val)
{
    uint8_t rt = 0;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length=8;                     //Command is 8 bits
    t.tx_buffer=&val;               //The data is the cmd itself
    t.user=(void*)0;                //D/C needs to be set to 0
    t.rxlength = 8;
    t.flags = SPI_TRANS_USE_RXDATA;
    spi_device_polling_transmit(spi, &t);  //Transmit!
    rt = t.rx_data[0];
    return rt;
}