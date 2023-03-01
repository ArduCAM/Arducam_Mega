/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "apps_config.h"
#include "bsp/board.h"
#include "ff.h"
#include "tusb.h"
#include "gui_func.h"
// #include "rtc.h"
/*------------- MAIN -------------*/
FATFS microSDFatFs;

void SD_Init(void) {
    // Check the mounted device

    FRESULT f_res = f_mount(&microSDFatFs, (TCHAR const *)"/", 1);

    if (f_res != FR_OK) {
        printf("SD card mount file system failed ,error code :(%d)\r\n", f_res);
    } else {
        printf("SD card mount file system success!! \r\n");
    }
}
int main(void) {
    //   board_init();
    pico4ml_init(true);
    // time_init();
    printf("use device\n");
    // display_string("",24)
    clear_screen(WHITE);
    display_string(20,115,"USB mass storage",BLACK,24);

    SD_Init();

    tusb_init();

    while (1) {
        tud_task(); // tinyusb device task
    }

    return 0;
}

//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+

// Invoked when cdc when line state changed e.g connected/disconnected
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts)
{
  (void) itf;
  (void) rts;

  // TODO set some indicator
  if ( dtr )
  {
    // Terminal connected
  }else
  {
    // Terminal disconnected
  }
}

// Invoked when CDC interface received data from host
void tud_cdc_rx_cb(uint8_t itf)
{
  (void) itf;
}
