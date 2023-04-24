#ifndef __ARDUCAM_SLOT_H
#define __ARDUCAM_SLOT_H
#include "common_init.h"

//#define SerialBegin(baudRate)     usb_cdc_init()
//#define SerialWrite(ch)           usb_cdc_write(&ch, 1)
//#define SerialWriteBuff(buf, len) usb_cdc_write(buf, len)
//#define SerialPrintf(str)         print_to_console(str)
#define delayUs(val)              R_BSP_SoftwareDelay(val, BSP_DELAY_UNITS_MICROSECONDS) // Delay Us


#define SerialBegin(baudRate)     uart_init()
#define SerialWrite(ch)           uart0_write(&ch, 1)
#define SerialWriteBuff(buf, len) uart0_write(buf, len)
#define SerialPrintf(str)         print_to_uart(str)
#endif
