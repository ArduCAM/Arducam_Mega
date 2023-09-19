#include "common_init.h"

FSP_CPP_HEADER
fsp_err_t spi_master_init(void);
uint8_t spi_transfer(uint8_t tx_data);
void spi_transfer_block(uint8_t *rx_data,uint32_t len);
FSP_CPP_FOOTER

volatile bool g_spi_transfer_complete = false;

fsp_err_t spi_master_init(void)
{
    return R_SPI_Open(&g_spi0_ctrl, &g_spi0_cfg);
}

void spi_master_handle(spi_callback_args_t *p_args)
{
    if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event)
    {
        g_spi_transfer_complete = true;
    }
}


uint8_t spi_transfer(uint8_t tx_data)
{
    uint8_t rxdata;
    g_spi_transfer_complete = false;
    R_SPI_WriteRead(&g_spi0_ctrl,&tx_data,&rxdata,1, SPI_BIT_WIDTH_8_BITS);
    while (false == g_spi_transfer_complete)
    {
        ;
    }
    return rxdata;
}

void spi_transfer_block(uint8_t *rx_data,uint32_t len)
{
    g_spi_transfer_complete = false;
    R_SPI_Read(&g_spi0_ctrl,rx_data,len, SPI_BIT_WIDTH_8_BITS);
    while (false == g_spi_transfer_complete)
    {
        ;
    }
}
