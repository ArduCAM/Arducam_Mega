#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "ArducamCamera.h"
#include "LCD_Touch.h"
#include "apps_config.h"
#include "ff.h"
#include "gui_func.h"

#include <pico/multicore.h>
#include <pico/stdlib.h>

extern CAM_IMAGE_MODE camera_resoultion[];
extern const unsigned char cam_logo[];
extern volatile int read_buffer_lenght;
extern uint8_t frame_buff[];
extern char* array_label[];
extern UI_LOG_MSG_T array_btn[];
extern UI_LOG_MSG_T win_loc;
extern UI_LOG_MSG_T camera_loc;
extern UI_LOG_MSG_T preview_loc;
extern volatile bool preview_buffer_ready;
extern volatile bool save_buffer_ready;
extern volatile bool wifi_buffer_ready;
extern CAM_IMAGE_MODE picture_resolution;
extern int label_loc[][2];
extern struct ClassCamera CAM;
extern char* htmls[];
enum save_picture_state {
    save_picture_init = 0,
    save_picture_start,
    save_picture_runnig,
    save_picture_complete,
    save_picture_idle,
    save_picture_error,
    wifi_server_run
};

extern void camera_get_picture(ArducamCamera* camera);
extern void camera_save_picture(ArducamCamera *camera, CAM_IMAGE_MODE save_resolution);

extern void uart_rx_handler();
extern void wifi_server_process();
extern void server_get_picture(ArducamCamera* camera);

extern void wifi_server_start();
extern uint8_t get_wifi_server_stats();
#ifdef __cplusplus
}
#endif