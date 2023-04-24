#ifndef __COMMON_INIT_H
#define __COMMON_INIT_H
#include "hal_data.h"

#define NUM_STRING_DESCRIPTOR               (7U)
#define READ_BUF_SIZE                       (8U)
#define LINE_CODING_LENGTH                  (0x07U)

#define APP_ERR_TRAP(a)             if(a) {__asm("BKPT #0\n");} /* trap the error location */

fsp_err_t usb_cdc_init(void);
bool usb_cdc_stask(uint8_t **);
fsp_err_t print_to_console(char *p_data);
fsp_err_t usb_cdc_write(uint8_t *p_data, uint32_t len);

fsp_err_t spi_master_init(void);
uint8_t spi_transfer(uint8_t tx_data);
void spi_transfer_block(uint8_t *rx_data,uint32_t len);

fsp_err_t uart_init(void);
fsp_err_t print_to_uart(char *p_data);
fsp_err_t uart0_write(uint8_t *p_data, uint32_t len);

extern uint8_t g_uart0_buf[READ_BUF_SIZE];
extern volatile bool recv_flag;

#endif
