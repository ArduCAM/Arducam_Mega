#include "main.h"

const int cam_x = 24, cam_y = 11;
UI_LOG_MSG_T win_loc = {.logo_data = 0,
                        .x_start   = cam_x,
                        .y_start   = cam_y,
                        .width     = 130,
                        .height    = 106,
                        .x_end     = cam_x + 130,
                        .y_end     = cam_y + 106};

UI_LOG_MSG_T camera_loc = {.logo_data = cam_logo,
                           .x_start   = cam_x + 6,
                           .y_start   = cam_y + 5,
                           .width     = 16,
                           .height    = 16,
                           .x_end     = cam_x + 32,
                           .y_end     = cam_y + 21};

UI_LOG_MSG_T preview_loc = {.logo_data = 0,
                            .x_start   = 29 + cam_x,
                            .y_start   = cam_y + 5,
                            .width     = 96,
                            .height    = 96,
                            .x_end     = cam_x + 125,
                            .y_end     = cam_y + 101};

UI_LOG_MSG_T array_btn[] = {{0, 24, 125, 130, 48, 154, 173}, //[WIFI]
                            {0, 24, 182, 272, 48, 296, 230}, //[return]
                            {0, 164, 11, 130, 48, 294, 59},
                            {0, 164, 68, 130, 48, 294, 116},
                            {0, 164, 125, 130, 48, 294, 173}};

int label_loc[][2] = {55, 137, 126, 194, 180, 27, 180, 84, 180, 141};

const unsigned char cam_logo[512] = {
    /* 0X10,0X10,0X00,0X10,0X00,0X10,0X01,0X1B, */
    0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04,
    0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1,
    0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04,
    0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
    0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04,
    0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1,
    0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00,
    0X00, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00,
    0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04,
    0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1,
    0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X00,
    0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1,
    0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04,
    0XF1, 0X04, 0XF1, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1,
    0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X04,
    0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00,
    0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04,
    0XF1, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1,
    0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X00,
    0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
    0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X04,
    0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1,
    0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
    0X00, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1,
    0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04,
    0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1,
    0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X04, 0XF1, 0X00, 0X00, 0X00, 0X00,
};