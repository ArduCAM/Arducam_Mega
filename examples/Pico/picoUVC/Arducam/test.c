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

#include "Arducam/ArducamCamera.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "tusb.h"

static uint8_t frame_buffer[96 * 96 * 16 / 8];

volatile bool buffer_ready = true;
void capture_loop(void)
{
    sleep_ms(3000);
    struct ClassCamera cameras;
    cameras.cam = createArducamCamera(17);
    cameras.status = Camera_init;
    while (1) {
        if (cameraHeartBeat(&cameras.cam)) {
            switch (cameras.status) {
            case Camera_close:
            case Camera_init:
                begin(&cameras.cam);
                cameras.status = Camera_open;
                break;
            case Camera_open:
                // while (buffer_ready != false)
                //     tight_loop_contents();
                takePicture(&cameras.cam, CAM_IMAGE_MODE_96X96, CAM_IMAGE_PIX_FMT_YUV);
                readBuff(&cameras.cam, frame_buffer, cameras.cam.totalLength);

                // fill_color_bar(frame_buffer, frame_num);
                printf("after\r\n");
                for (size_t i = 0; i < 18432 /*76800*/; i++) {
                    if (i % 24 == 0) {
                        printf("\n");
                    }
                    printf("0x%02x ", frame_buffer[i]);
                }
                // tud_cdc_write(frame_buffer,96 * 96 * 16 / 8);
                // buffer_ready = true;
                break;
            }
        } else {
            cameras.status = Camera_close;
        }
    }
}

/*------------- MAIN -------------*/
int main(void)
{
    stdio_init_all();
    // init device stack on configured roothub port

    capture_loop();

    return 0;
}