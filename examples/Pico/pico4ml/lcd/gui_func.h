#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
#define RED    0xf800
#define GREEN  0x07e0
#define BLUE   0x001f
#define BLACK  0x0000
#define WHITE  0xffff
#define GRAY   0x8c51
#define YELLOW 0xFFE0
#define CYAN   0x07FF
#define PURPLE 0xF81F

typedef struct ui_logo_msg {
    const uint8_t *logo_data;
    uint16_t x_start;
    uint16_t y_start;
    uint16_t width;
    uint16_t height;
    uint16_t x_end;
    uint16_t y_end;
} UI_LOG_MSG_T;

void lcd_init_(int);

void lcd_init1_(int);

void display_color(UI_LOG_MSG_T loc_data, uint16_t color);

void display_number(uint16_t x, uint16_t y, uint32_t num);

void clear_screen(uint16_t color);

void display_text(uint16_t x, uint16_t y, unsigned char *string);

void display_char(uint16_t x, uint16_t y, unsigned char Acsii_Char);

void display_image(UI_LOG_MSG_T loc_data, const uint8_t *dis_date);

void display_image_customize(UI_LOG_MSG_T loc_data, const uint8_t *dis_date);

void display_image_(UI_LOG_MSG_T loc_data, const uint8_t *dis_date, int invert);

void display_text_color(uint16_t x, uint16_t y, unsigned char *string,
                        uint16_t color, uint8_t font_size);

void display_string(uint16_t x, uint16_t y, unsigned char *string,
                    uint16_t color, uint8_t font_size);

void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
               uint16_t color);

void draw_fill_arrow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                     uint16_t w, uint16_t color);

void draw_rect(UI_LOG_MSG_T loc_data, uint16_t color);

void test_spi(uint8_t data);
#ifdef __cplusplus
}
#endif