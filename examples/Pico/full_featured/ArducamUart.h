#ifndef __ARDUCAM_SLOT_H
#define __ARDUCAM_SLOT_H
#include "bsp/board.h"
#include "tusb.h"
#include <pico/stdlib.h>

#define SerialBegin(baudRate)     dummy(baudRate)

#define SerialWrite(ch)           serialWriteBuff(&ch, 1)
#define SerialWriteBuff(buf, len) serialWriteBuff(buf, len)
#define SerialPrintf(str)         ::printf(str)
#define SerialAvailable()         tud_cdc_available()
#define SerialRead()              SerialUsbRead()
#define delayUs(us)               sleep_us(us)

void dummy(uint32_t) {}

void serialWriteBuff(uint8_t* buf, uint32_t length)
{
    static uint64_t last_avail_time;
    int i = 0;
    if (tud_cdc_connected()) {
        for (int i = 0; i < length;) {
            int n = length - i;
            int avail = tud_cdc_write_available();
            if (n > avail)
                n = avail;
            if (n) {
                int n2 = tud_cdc_write(buf + i, n);
                tud_task();
                tud_cdc_write_flush();
                i += n2;
                last_avail_time = time_us_64();
            } else {
                tud_task();
                tud_cdc_write_flush();
                if (!tud_cdc_connected() ||
                    (!tud_cdc_write_available() && time_us_64() > last_avail_time + 1000000 /* 1 second */)) {
                    break;
                }
            }
        }
    } else {
        // reset our timeout
        last_avail_time = 0;
    }
}

int32_t SerialUsbRead(void)
{
    if (tud_cdc_connected() && tud_cdc_available()) {
        return tud_cdc_read_char();
    }
    return -1;
}
#endif