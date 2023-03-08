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

#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

#include "Arducam/ArducamCamera.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"

void video_task(void);
static unsigned frame_num = 0;
static unsigned tx_busy = 0;
static unsigned interval_ms = 1000 / FRAME_RATE;
static uint8_t frame_buffer[FRAME_WIDTH * FRAME_HEIGHT * 16 / 8];

volatile bool buffer_ready = true;
static void fill_color_bar(uint8_t* buffer, unsigned start_position);
void capture_loop(void)
{
    sleep_ms(3000);
    struct ClassCamera cameras;
    cameras.cam = createArducamCamera(17);
    cameras.status = Camera_init;
    int p[] = {173, 42, 173, 26};
    while (1) {
        tud_task(); // tinyusb device task

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
                readBuff(&cameras.cam, frame_buffer, 96 * 96 * 2);
                // tud_video_n_frame_xfer(0, 0, (void*)frame_buffer, FRAME_WIDTH * FRAME_HEIGHT * 16 / 8);

                // buffer_ready = true;
                break;
            }
        } else {
            cameras.status = Camera_close;
        }
    }
}

void loop()
{
    // tud_init(BOARD_TUD_RHPORT);

    while (1) {
        tud_task(); // tinyusb device task
        video_task();
    }
}

/*------------- MAIN -------------*/
int main(void)
{
    stdio_init_all();
    tud_init(BOARD_TUD_RHPORT);

    multicore_launch_core1(loop);

    // init device stack on configured roothub port

    capture_loop();

    return 0;
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
    // blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
    // blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
    (void)remote_wakeup_en;
    // blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
    // blink_interval_ms = BLINK_MOUNTED;
}

//--------------------------------------------------------------------+
// USB Video
//--------------------------------------------------------------------+
void video_task(void)
{
    // static unsigned start_ms = 0;
    // static unsigned already_sent = 0;

    // if (!tud_video_n_streaming(0, 0)) {
    //     already_sent = 0;
    //     frame_num = 0;
    //     return;
    // }

    // if (!already_sent) {
    //     already_sent = 1;
    //     fill_color_bar(frame_buffer, frame_num);
    //     buffer_ready = false;
    //     while (buffer_ready != true)
    //         tight_loop_contents();
    //     start_ms = board_millis();
    //     tud_video_n_frame_xfer(0, 0, (void*)frame_buffer, FRAME_WIDTH * FRAME_HEIGHT * 16 / 8);
    // }
    // buffer_ready = false;
    // while (buffer_ready != true);
    //     tight_loop_contents();
    unsigned cur = board_millis();
    // if (cur - start_ms < interval_ms)
    //     return; // not enough time
    // if (tx_busy)
    //     return;
    // start_ms += interval_ms;
    if (buffer_ready == true)
        tud_video_n_frame_xfer(0, 0, (void*)frame_buffer, FRAME_WIDTH * FRAME_HEIGHT * 16 / 8);
}

void tud_video_frame_xfer_complete_cb(uint_fast8_t ctl_idx, uint_fast8_t stm_idx)
{
    (void)ctl_idx;
    (void)stm_idx;
    tx_busy = 0;
    /* flip buffer */
    ++frame_num;
}

int tud_video_commit_cb(uint_fast8_t ctl_idx, uint_fast8_t stm_idx, video_probe_and_commit_control_t const* parameters)
{
    (void)ctl_idx;
    (void)stm_idx;
    /* convert unit to ms from 100 ns */
    interval_ms = parameters->dwFrameInterval / 10000;
    return VIDEO_ERROR_NONE;
}