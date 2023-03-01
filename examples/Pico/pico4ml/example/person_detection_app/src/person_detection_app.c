#include "person_detection_app.h"
#include "arducam_hm01b0.h"
#include "gui_func.h"
#include "person_detection_main_functions.h"
#include <stdio.h>
#define Gray2RGB565(gray) (((gray & 0xF8) << 8) | ((gray & 0xFC) << 3) | ((gray & 0xF8) >> 3) & 0x00ffff);

uint8_t frame_buff[96*96];
uint16_t frame_buff_16[96*96];
struct arducam_config config= {
    .sccb  = i2c0,
    .sccb_mode = I2C_MODE_16_8,
    .sensor_address = 0x24,
    .pin_sioc = 5,
    .pin_siod = 4,
    .pin_resetb = -1,
    .pin_xclk = 3,
    .pin_vsync = 2,
    .pin_y2_pio_base = 6,
    .pio = pio0,
    .pio_sm = 0,
    .dma_channel = 0
};
                                
void app_loop(void)
{
    char str_buff[80];
    clear_screen(WHITE);
    arducam_init(&config);
    display_text(30, 15, "Person Detection");
        display_string(80, 181, "Score:", BLACK, 24);

    person_detection_setup();
    UI_LOG_MSG_T person_loc = {0, 112, 75, 96, 96, 208, 171};
    while (true) {
        arducam_capture_frame(&config, frame_buff);
        double detect_person_score = person_detection_loop((uint16_t*)(frame_buff));

        sprintf(str_buff, "%02.0lf", detect_person_score);
        if (detect_person_score > 50)
            display_string(180, 181, str_buff, GREEN, 24);
        else
            display_string(180, 181, str_buff, RED, 24);

        for (size_t i = 0; i < 96 * 96; i++) {
            frame_buff_16[i] = Gray2RGB565(frame_buff[i]);
        }
        display_image_(person_loc, (uint8_t*)frame_buff_16, true);
    }
}
