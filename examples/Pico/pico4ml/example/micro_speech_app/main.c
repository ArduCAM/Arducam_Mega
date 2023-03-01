#include "LCD_Touch.h"
#include "apps_config.h"
#include "gui_func.h"
#include "micro_speech_apps.h"
#include "micro_speech_main_functions.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"

void core1_entry();

int micro_speech_cmd = -1;

const UI_LOG_MSG_T windows_customize = {
    .logo_data = 0,
    .x_start = 0,
    .y_start = 135,
    .width = 320,
    .height = 240,
    .x_end = 320,
    .y_end = 225,
};

int main() {
    char str_buff[40];
    multicore_launch_core1(core1_entry);
    pico4ml_init(true);
    clear_screen( WHITE);
    display_text(60, 15, "micro speech");
    int32_t latest_timestamp = 0;
    size_t offset = 0;
    int16_t pre_y = 175;
    display_text_color(10, 60, "Please say:", BLACK, 20);
    // display_text_color(30, 95, "[unknown]", GRAY, 16);
    display_text_color(80, 95, "[yes]", GRAY, 16);
    display_text_color(120 + 66 + 20, 95, "[no]", GRAY, 16);
    int cnt = 0;
    for (;;) {
        if (micro_speech_cmd != -1) {
            switch (micro_speech_cmd) {
            case 2:
                // display_text_color(30, 95, "[unknown]", GRAY, 16);
                display_text_color(80, 95, "[yes]", GREEN, 16);
                display_text_color(120 + 66 + 20, 95, "[no]", GRAY, 16);
                cnt = 10;
                break;
            case 3:
                // display_text_color(30, 95, "[unknown]", GRAY, 16);
                display_text_color(80, 95, "[yes]", GRAY, 16);
                display_text_color(206, 95, "[no]", GREEN, 16);
                cnt = 10;
                break;
            default:
                // if(cnt  == 0)
                // display_text_color(30, 95, "[unknown]", RED, 16);
                // break;
                -- cnt;
            }
            if (cnt <= 0) {
                cnt = 0;
                display_text_color(80, 95, "[yes]", GRAY, 16);
                display_text_color(206, 95, "[no]", GRAY, 16);
            }
            micro_speech_cmd = -1;
        }

        if (latest_timestamp != g_latest_audio_timestamp) {
            display_color(windows_customize, WHITE);
            offset = 0;
            for (size_t i = 1; i < 320; i += 1) {
                int16_t y = pdm_g_audio_capture_buffer[offset] / 400 + 175;
                if (y < 135)
                    y = 135;
                else if (y > 224)
                    y = 224;
                draw_line(i - 1, pre_y, i, y, BLACK);
                offset += 10;
                pre_y = y;
            }
        }
    }
}

void core1_entry() {
    pdm_init(on_pdm_samples_ready);
    micro_speech_setup();
    while (1) {
        micro_speech_cmd = micro_speech_loop(pdm_g_audio_capture_buffer,
                                             g_latest_audio_timestamp);
    }
}
