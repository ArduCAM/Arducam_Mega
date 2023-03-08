#include "main.h"
const int READ_IMAGE_LENGTH = 2048;
uint8_t frame_buff[153600] = {0};

volatile bool buffer_ready = false;
volatile int read_buffer_lenght = -1;
void camera_get_picture(ArducamCamera* camera)
{
    while (buffer_ready != false)
        tight_loop_contents();
    takePicture(camera, CAM_IMAGE_MODE_96X96, CAM_IMAGE_PIX_FMT_RGB565);
    int rs = readBuff(camera, frame_buff, camera->totalLength);
    buffer_ready = true;
}

void camera_save_picture(ArducamCamera* camera, CAM_IMAGE_MODE save_resolution)
{
    while (buffer_ready != false)
        tight_loop_contents();
    takePicture(camera, save_resolution, CAM_IMAGE_PIX_FMT_JPG);
    while (camera->receivedLength) {
        read_buffer_lenght = readBuff(camera, frame_buff, READ_IMAGE_LENGTH);
    }

    takePicture(camera, save_resolution, CAM_IMAGE_PIX_FMT_JPG);
    // sleep_ms(50);
    while (camera->receivedLength) {
        while (buffer_ready != false)
            tight_loop_contents();
        read_buffer_lenght = readBuff(camera, frame_buff, READ_IMAGE_LENGTH);
        buffer_ready = true;
    }
    while (buffer_ready != false)
        tight_loop_contents();
}
