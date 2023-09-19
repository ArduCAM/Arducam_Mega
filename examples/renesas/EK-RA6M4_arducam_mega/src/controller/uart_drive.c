#include "common_init.h"
FSP_CPP_HEADER
fsp_err_t uart_init(void);
fsp_err_t print_to_uart(char *p_data);
fsp_err_t uart0_write(uint8_t *p_data, uint32_t len);
FSP_CPP_FOOTER

volatile bool g_uart_transfer_complete = false;
volatile bool g_receive_complete  = false;
uint8_t g_uart0_buf[READ_BUF_SIZE] = {0};
volatile bool recv_flag = false;
uint8_t g_uart_buf_idx =0;

fsp_err_t uart_init(void){
    return R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
}

fsp_err_t print_to_uart(char *p_data)
{
    fsp_err_t err = FSP_SUCCESS;
    uint32_t len = (uint32_t)strlen(p_data);
    g_uart_transfer_complete = false;
    err = R_SCI_UART_Write(&g_uart0_ctrl, (uint8_t *)p_data, len);

    while (false == g_uart_transfer_complete)
    {
       ;
    }
    return err;
}

fsp_err_t uart0_write(uint8_t *p_data, uint32_t len)
{
    fsp_err_t err = FSP_SUCCESS;
    g_uart_transfer_complete = false;
    err = R_SCI_UART_Write(&g_uart0_ctrl, p_data, len);
    while (false == g_uart_transfer_complete)
    {
       ;
    }
    return err;
}

void uart0_drive_callback(uart_callback_args_t * p_args)
{
    /* Handle the UART event */
    switch (p_args->event)
    {
        /* Received a character */
        case UART_EVENT_RX_CHAR:
        {
            if (UART_DATA_BITS_8 >= g_uart0_cfg.data_bits)
            {
                uint8_t f_buf = (uint8_t) p_args->data;
                if(f_buf == 0x55)
                {
                    g_uart_buf_idx = 0;
                } else if(f_buf == 0xaa && g_uart_buf_idx < READ_BUF_SIZE) {
                    recv_flag = true;
                }
                if(g_uart_buf_idx < READ_BUF_SIZE)
                {
                    g_uart0_buf[g_uart_buf_idx] = f_buf;
                }
                g_uart_buf_idx++;
            }
            break;
        }
        /* Receive complete */
        case UART_EVENT_RX_COMPLETE:
        {
            g_receive_complete = true;
            break;
        }
        /* Transmit complete */
        case UART_EVENT_TX_COMPLETE:
        {
            g_uart_transfer_complete = true;
            break;
        }
        default:
        {
        }
    }
}
