#include "multitude_camera_app.h"
#include "ArducamCamera.h"
#include "LCD_Touch.h"
#include "apps_config.h"
#include "gui_func.h"
#include "static_data.h"
uint8_t frame_buff[23477];

// uint8_t buff[96 * 96 * 2];

UI_LOG_MSG_T list_cam_loc[] = {{0, 52, 14, 96, 96, 52 + 96, 14 + 96},
                               {0, 194, 14, 96, 96, 194 + 96, 14 + 96},
                               {0, 52, 128, 96, 96, 52 + 96, 128 + 96},
                               {0, 194, 128, 96, 96, 194 + 96, 129 + 96}};

UI_LOG_MSG_T list_win_loc[] = {{window, 23, 9, 130, 106, 23 + 130, 9 + 106},
                               {window, 165, 9, 130, 106, 165 + 130, 9 + 106},
                               {window, 23, 123, 130, 106, 23 + 130, 123 + 106},
                               {window, 165, 123, 130, 106, 165 + 130, 123 + 106}};

UI_LOG_MSG_T list_cam_log[] = {{cam_log, 29, 14, 16, 16, 29 + 16, 14 + 16},
                               {cam_log, 171, 14, 16, 16, 171 + 16, 14 + 16},
                               {cam_log, 29, 128, 16, 16, 29 + 16, 128 + 16},
                               {cam_log, 171, 128, 16, 16, 171 + 16, 128 + 16}};

UI_LOG_MSG_T list_cam_num[] = {{gImage_num_1, 29, 99, 16, 12, 29 + 16, 99 + 12},
                               {gImage_num_2, 29 + 142, 99, 16, 12, 29 + 142 + 16, 99 + 12},
                               {gImage_num_3, 29, 85 + 129, 16, 12, 29 + 16, 85 + 129 + 12},
                               {gImage_num_4, 29 + 142, 85 + 129, 16, 12, 29 + 142 + 16, 85 + 129 + 12}};

void camera_get_picture(ArducamCamera* camera)
{
    takePicture(camera, CAM_IMAGE_MODE_96X96,
                CAM_IMAGE_PIX_FMT_RGB565); // CAM_IMAGE_PIX_FMT_JPG
    int rs = readBuff(camera, frame_buff, camera->totalLength);
}

void draw_multitude_camera(void)
{
    clear_screen(WHITE);
    for (size_t i = 0; i < 4; i++) {
        display_image_(list_win_loc[i], list_win_loc[i].logo_data, false);
        display_image_(list_cam_log[i], list_cam_log[i].logo_data, false);
        display_image(list_cam_num[i], list_cam_num[i].logo_data);
    }
}
static volatile bool buffer_ready  = false;
static volatile uint8_t camera_idx = 0;

void capture_loop(void)
{
    pico4ml_camera_spi_init(SPI1_BAUDRATE);
    sleep_ms(300);
    struct ClassCamera cameras[4] = {0};
    // 4,3,1,2
    int cameras_cs[] = {13, 28, 17, 25};

    for (size_t i = 0; i < 4; i++) {
        cameras[i].cam    = createArducamCamera(cameras_cs[i]);
        cameras[i].status = Camera_init;
    }

    for (uint8_t i = 0;; i++) {
        if (i >= 4)
            i = 0;
        if (cameraHeartBeat(&cameras[i].cam)) {
            switch (cameras[i].status) {
            case Camera_close:
            case Camera_init:
                begin(&cameras[i].cam);
                cameras[i].status = Camera_open;
                printf("%d-start\n", i);
                break;
            case Camera_open:
                while (buffer_ready != false)
                    tight_loop_contents();
                camera_get_picture(&cameras[i].cam);
                buffer_ready = true;
                camera_idx   = i;
                break;
            }
        } else {
            cameras[i].status = Camera_close;
        }
    }
}

void display_loop(void)
{
    printf("multitude_camera_app\n");
    draw_multitude_camera();
    while (true) {
        if (buffer_ready == true) {
            display_image(list_cam_loc[camera_idx], frame_buff);
            buffer_ready = false;
        }
        // test_spi(0xaa);
        // test_spi(0x55);
    }
}