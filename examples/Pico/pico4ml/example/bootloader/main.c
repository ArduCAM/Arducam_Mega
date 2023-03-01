#include "LCD_Touch.h"
#include "apps_config.h"
#include "bootloader.h"
#include "gui_func.h"
#include "static_data.h"
#include "ff.h"
const int func_width = 80;
const int x_start    = 25;
const int y_start    = 32;
const int func_gap   = 15;
UI_LOG_MSG_T list_func_loc[6];
FATFS microSDFatFs;


void SD_Init(void)
{
    // Check the mounted device
    FRESULT f_res = f_mount(&microSDFatFs, (TCHAR const*)"/", 1);
    if (f_res != FR_OK) {
        printf("SD card mount file system failed ,error code :(%d)\r\n", f_res);
    } else {
        printf("SD card mount file system success!! \r\n");
    }
}

int main()
{
    pico4ml_init(false);
    printf("bootload\n");
    SD_Init();
    clear_screen(WHITE);
    display_image(start_loc, start_loc.logo_data);

    sleep_ms(1000);
    clear_screen(WHITE);
    display_image(func_loc, func_loc.logo_data);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            list_func_loc[i * 3 + j].x_start = x_start + func_width * j + func_gap * j;
            list_func_loc[i * 3 + j].y_start = y_start + func_width * i + func_gap * i;
            list_func_loc[i * 3 + j].width   = func_width;
            list_func_loc[i * 3 + j].height  = func_width;
            list_func_loc[i * 3 + j].x_end   = x_start + func_width * (j + 1) + func_gap * (j);
            list_func_loc[i * 3 + j].y_end   = y_start + func_width * (i + 1) + func_gap * (i);
            // draw_rect(list_func_loc[i*2+j],BLACK);
        }
    }
    for (;;) {
        int start_app_idx = select_touch(list_func_loc, 6);
        if (start_app_idx >= 0) {
            switch (start_app_idx) {
            case 0:
                printf("startMainApplication(0x37000); ");
                startMainApplication(0x37000);
                break;
            case 1:
                printf("startMainApplication(0x46000);");
                startMainApplication(0x46000);
                break;
            case 2:
                printf("startMainApplication(0x716800);");
                startMainApplication(0xb1800);
                break;
            case 3:
                printf("startMainApplication(0x180000);");
                startMainApplication(0xc5800);
                break;
            case 4:
                printf("startMainApplication(0x180000);");
                startMainApplication(0xEd800);
                break;
            case 5:
                printf("startMainApplication(0x118000);");
                startMainApplication(0x118000);
                break;
            }
        }
    }
    return 0;
}
