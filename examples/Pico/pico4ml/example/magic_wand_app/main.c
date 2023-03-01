#include "3d_cube.h"
#include "ICM42622.h"
#include "LCD_Touch.h"
#include "apps_config.h"
#include "gui_func.h"
#include "hardware/watchdog.h"
#include "magic_wand_app.h"
#include "magic_wand_main_functions.h"
#include "pico/multicore.h"
#include "string.h"

int current_app = 1;

void core1_entry() {
    icm42622_init();
    setup();

    while (1) {
        if (current_app == 1)
            on_imu_samples_ready();
        else
            refresh_imu();
    }
}

const UI_LOG_MSG_T magic_loc = {
    .logo_data = 0,
    .x_start = 40,
    .y_start = 50,
    .width = 160,
    .height = 160,
    .x_end = 200,
    .y_end = 210,
};

const UI_LOG_MSG_T text_loc = {
    .logo_data = 0,
    .x_start = 120,
    .y_start = 220,
    .width = 120,
    .height = 40,
    .x_end = 240,
    .y_end = 260,
};

char displaystr[40];

int main() {
    pico4ml_init(2);
    clear_screen(WHITE);
    magic_wand_setup();
    multicore_launch_core1(core1_entry);

    // display_text(30, 20, "magic wand");
    UI_LOG_MSG_T array[] = {{0, 280, 160, 40, 80, 320, 240},
                            {0, 280, 80, 40, 80, 320, 160},
                            {0, 280, 0, 40, 80, 320, 80}};
    UI_LOG_MSG_T win[] = {{0, 0, 280, 80, 40, 79, 320},
                          {0, 80, 280, 80, 40, 159, 320},
                          {0, 160, 280, 80, 40, 240, 320}};
    // UI_LOG_MSG_T array[] = {
    //     {0,40,200,40,15,80,215},
    //     {0,90,200,40,15,130,215}
    // };
    display_text_color(7, 290, "TEST", GRAY, 24);
    display_text_color(87, 290, "WAND", GREEN, 24);
    display_text_color(175, 290, "<--", BLACK, 24);
    for (size_t i = 0; i < 3; i++)
        draw_rect(win[i], BLACK);
    for (;;) {
        int ret = select_touch(array, 3);
        if (ret != -1) {
            current_app = ret;
            if (ret == 0) {
                display_color(magic_loc, WHITE);
                display_color(text_loc, WHITE);
                display_text_color(7, 290, "TEST", GREEN, 24);
                display_text_color(87, 290, "WAND", GRAY, 24);

            } else if (ret == 1) {
                display_color(magic_loc, WHITE);
                display_text_color(7, 290, "TEST", GRAY, 24);
                display_text_color(87, 290, "WAND", GREEN, 24);
            } else {
                watchdog_enable(10, 1);
                watchdog_enable_caused_reboot();
                watchdog_caused_reboot();
            }
        }
        if (current_app == 1) {
            if (done_just_triggered) {
                display_image(magic_loc, (unsigned char *)displayBuf);

                sprintf(displaystr, "%c (%2d%%)", labels[max_index],
                        ((max_score + 128) * 100) >> 8);
                display_string(120, 220, displaystr, BLACK, 16);
                sprintf(displaystr, "%c (%2d%%)", labels[pre_max_index],
                        ((pre_max_score + 128) * 100) >> 8);
                display_string(120, 240, displaystr, BLACK, 16);
            }
        } else {
            // tight_loop_contents();
            loop();
        }
    }
}
