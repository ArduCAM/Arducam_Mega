#include "apps_config.h"
#include "arducam_hm01b0.h"
#include "gui_func.h"

#define Gray2RGB565(gray)                                                      \
    (((gray & 0xF8) << 8) | ((gray & 0xFC) << 3) |                             \
     ((gray & 0xF8) >> 3) & 0x00ffff);

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
    UI_LOG_MSG_T person_loc = {0, 112, 75, 96, 96, 208, 171};

uint8_t image_data[96*96];
uint16_t frame_buff_16[96*96];

int main()
{
    pico4ml_init(false);
    printf("hm01b0\n");
    
    arducam_init(&config);
    while (1)
    {
        arducam_capture_frame(&config, image_data);
        // int16_t *frame_buff_16 = (uint16_t *)(frame_buff);
        for (size_t i = 0; i < 96 * 96; i++) {
            uint8_t tmp = (uint8_t)(image_data[i] & 0x00ff);
            frame_buff_16[i] = Gray2RGB565(tmp);
        }
        display_image_(person_loc, (uint8_t *)frame_buff_16,true);
    }
    
}