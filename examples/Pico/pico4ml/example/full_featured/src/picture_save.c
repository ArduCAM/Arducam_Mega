#include "main.h"
const int READ_IMAGE_LENGTH = 512;
uint8_t frame_buff[153600] = {0};

volatile bool preview_buffer_ready = false;
volatile bool save_buffer_ready = false;

volatile int read_buffer_lenght = -1;
void camera_get_picture(ArducamCamera* camera)
{
    int rs;
    while (preview_buffer_ready != false)
        tight_loop_contents();
    // writeReg(camera, 0x06, 0x80);
    takePicture(camera, CAM_IMAGE_MODE_96X96, CAM_IMAGE_PIX_FMT_RGB565);
    do {
        rs = readBuff(camera, frame_buff, camera->totalLength);
    } while (rs <= 0);
    preview_buffer_ready = true;
}

void camera_save_picture(ArducamCamera* camera, CAM_IMAGE_MODE save_resolution)
{
    while (save_buffer_ready != false)
        tight_loop_contents();
    // writeReg(camera, 0x06, 0x00);
    if (camera->cameraId == SENSOR_3MP_1) {
        setAutoExposure(camera,0);
        setAbsoluteExposure(camera,5000);
        setAutoWhiteBalance(camera,0);
        setAutoWhiteBalanceMode(camera,CAM_WHITE_BALANCE_MODE_OFFICE);
    }
    takePicture(camera, save_resolution, CAM_IMAGE_PIX_FMT_JPG);
    while (camera->receivedLength) {
        readBuff(camera, frame_buff, READ_IMAGE_LENGTH);
    }

    takePicture(camera, save_resolution, CAM_IMAGE_PIX_FMT_JPG);
    // sleep_ms(50);
    while (camera->receivedLength) {
        while (save_buffer_ready != false)
            tight_loop_contents();
        read_buffer_lenght = readBuff(camera, frame_buff, READ_IMAGE_LENGTH);
        if (read_buffer_lenght <= 0)
            continue;
        save_buffer_ready = true;
    }
    while (save_buffer_ready != false)
        tight_loop_contents();
}
