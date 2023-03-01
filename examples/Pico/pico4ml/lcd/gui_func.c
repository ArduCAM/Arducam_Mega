#include "gui_func.h"
#include "font/fonts.h"
#include "st7789.h"
static TFT_t dev;

void lcd_init_(int flage) {
    spiMasterInit(&dev, 0, 0, 20, 21, 22, -1);
    lcdInit(&dev, 320, 240, 0, 0, flage);
}
void lcd_init1_(int flage) {
    spiMasterInit(&dev, 0, 0, 20, 21, 22, -1);
    lcdInit(&dev, 240, 320, 0, 0, flage);
}

void display_text(uint16_t x, uint16_t y, unsigned char *string) {
    lcdDrawString(&dev, &Font24, x, y, string, BLACK, WHITE, true);
}

void display_text_color(uint16_t x, uint16_t y, unsigned char *string,
                        uint16_t color, uint8_t font_size) {
    sFONT *font_ = &Font24;
    switch (font_size) {
    case 24:
        font_ = &Font24;
        break;
    case 20:
        font_ = &Font20;
        break;
    case 16:
        font_ = &Font16;
        break;
    case 12:
        font_ = &Font12;
        break;
    case 8:
        font_ = &Font8;
        break;
    }
    lcdDrawString(&dev, font_, x, y, string, color, WHITE, true);
}

void test_spi(uint8_t data){
    spiMasterWriteDataByte(&dev,data);
}

void display_string(uint16_t x, uint16_t y, unsigned char *string,
                    uint16_t color, uint8_t font_size) {
    sFONT *font_ = &Font24;
    switch (font_size) {
    case 24:
        font_ = &Font24;
        break;
    case 20:
        font_ = &Font20;
        break;
    case 16:
        font_ = &Font16;
        break;
    case 12:
        font_ = &Font12;
        break;
    case 8:
        font_ = &Font8;
        break;
    }
    lcdDrawString(&dev, font_, x, y, string, color, WHITE, false);
}

void display_char(uint16_t x, uint16_t y, unsigned char Acsii_Char) {
    lcdDrawChar(&dev, &Font24, x, y, Acsii_Char, BLACK, WHITE, false);
}

void display_color(UI_LOG_MSG_T loc_data, uint16_t color) {
    lcdDrawFillRect(&dev, loc_data.x_start, loc_data.y_start, loc_data.x_end,
                    loc_data.y_end, color);
}

void clear_screen(uint16_t color) { lcdFillScreen(&dev, color); }

void display_image_(UI_LOG_MSG_T loc_data, const uint8_t *dis_date,
                    int invert) {
    for (size_t i = 0; i < loc_data.height; i++) {
        lcdDrawMultiPixels(&dev, loc_data.x_start, loc_data.y_start + i,
                           loc_data.width,
                           (uint16_t *)(dis_date + ((loc_data.height - i - 1) *
                                                    2 * loc_data.width)),
                           invert);
    }
}

void display_image(UI_LOG_MSG_T loc_data, const uint8_t *dis_date) {
    for (size_t i = loc_data.y_start; i < loc_data.y_end; i++) {
        lcdDrawMultiPixels(&dev, loc_data.x_start, i, loc_data.width,
                           (uint16_t *)(dis_date + ((i - loc_data.y_start) * 2 *
                                                    loc_data.width)),
                           false);
    }
    // lcdDrawImage(loc_data.x_start,loc_data.y_start,loc_data.x_end,loc_data.y_end,dis_date);
}

void display_image_customize(UI_LOG_MSG_T loc_data, const uint8_t *dis_date) {
    size_t end = loc_data.y_end - loc_data.y_start;
    for (size_t i = 0; i < end; i++) {
        lcdDrawMultiPixels(&dev, 0, loc_data.y_start + i, loc_data.width,
                           (uint16_t *)(dis_date + ((loc_data.height -
                                                     loc_data.y_start - i - 1) *
                                                    2 * loc_data.width)),
                           false);
    }
}

void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
               uint16_t color) {
    lcdDrawLine(&dev, x1, y1, x2, y2, color);
}

void draw_fill_arrow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                     uint16_t w, uint16_t color) {
    lcdDrawFillArrow(&dev, x1, y1, x2, y2, w, color);
}
void draw_rect(UI_LOG_MSG_T loc_data, uint16_t color) {
    lcdDrawRect(&dev, loc_data.x_start, loc_data.y_start, loc_data.x_end,
                loc_data.y_end, color);
}