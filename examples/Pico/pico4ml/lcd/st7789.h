#ifndef MAIN_ST7789_H_
#define MAIN_ST7789_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "font/fonts.h"

#define RED          0xf800
#define GREEN        0x07e0
#define BLUE         0x001f
#define BLACK        0x0000
#define WHITE        0xffff
#define GRAY         0x8c51
#define YELLOW       0xFFE0
#define CYAN         0x07FF
#define PURPLE       0xF81F

#define DIRECTION0   0
#define DIRECTION90  1
#define DIRECTION180 2
#define DIRECTION270 3

typedef struct {
    uint16_t _width;
    uint16_t _height;
    uint16_t _offsetx;
    uint16_t _offsety;
    uint16_t _font_direction;
    uint16_t _font_fill;
    uint16_t _font_fill_color;
    uint16_t _font_underline;
    uint16_t _font_underline_color;
    int16_t _dc;
    int16_t _bl;
} TFT_t;

void spiMasterInit(TFT_t *dev, int16_t GPIO_MOSI, int16_t GPIO_SCLK,
                     int16_t GPIO_CS, int16_t GPIO_DC, int16_t GPIO_RESET,
                     int16_t GPIO_BL);
bool spiMasterWriteByte(const uint8_t *Data, size_t DataLength);
bool spiMasterWriteCommand(TFT_t *dev, uint8_t cmd);
bool spiMasterWriteDataByte(TFT_t *dev, uint8_t data);
bool spiMasterWriteDataWord(TFT_t *dev, uint16_t data);
bool spiMasterWriteAddr(TFT_t *dev, uint16_t addr1, uint16_t addr2);
bool spiMasterWriteColor(TFT_t *dev, uint16_t color, uint16_t size);
bool spiMasterWriteColors(TFT_t *dev, uint16_t *colors, uint16_t size);

void delayMS(int ms);
void lcdInit(TFT_t *dev, int width, int height, int offsetx, int offsety, int );
void lcdDrawPixel(TFT_t *dev, uint16_t x, uint16_t y, uint16_t color);
void lcdDrawMultiPixels(TFT_t *dev, uint16_t x, uint16_t y, uint16_t size,
                        uint16_t *colors, bool);
void lcdDrawFillRect(TFT_t *dev, uint16_t x1, uint16_t y1, uint16_t x2,
                     uint16_t y2, uint16_t color);
void lcdDisplayOff(TFT_t *dev);
void lcdDisplayOn(TFT_t *dev);
void lcdFillScreen(TFT_t *dev, uint16_t color);
void lcdDrawLine(TFT_t *dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                 uint16_t color);
void lcdDrawRect(TFT_t *dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                 uint16_t color);
void lcdDrawRectAngle(TFT_t *dev, uint16_t xc, uint16_t yc, uint16_t w,
                      uint16_t h, uint16_t angle, uint16_t color);
void lcdDrawTriangle(TFT_t *dev, uint16_t xc, uint16_t yc, uint16_t w,
                     uint16_t h, uint16_t angle, uint16_t color);
void lcdDrawCircle(TFT_t *dev, uint16_t x0, uint16_t y0, uint16_t r,
                   uint16_t color);
void lcdDrawFillCircle(TFT_t *dev, uint16_t x0, uint16_t y0, uint16_t r,
                       uint16_t color);
void lcdDrawRoundRect(TFT_t *dev, uint16_t x1, uint16_t y1, uint16_t x2,
                      uint16_t y2, uint16_t r, uint16_t color);
void lcdDrawArrow(TFT_t *dev, uint16_t x0, uint16_t y0, uint16_t x1,
                  uint16_t y1, uint16_t w, uint16_t color);
void lcdDrawFillArrow(TFT_t *dev, uint16_t x0, uint16_t y0, uint16_t x1,
                      uint16_t y1, uint16_t w, uint16_t color);
uint16_t rgb565_conv(uint16_t r, uint16_t g, uint16_t b);
int lcdDrawChar(TFT_t *dev, sFONT *fx, uint16_t x, uint16_t y, uint8_t ascii,
                uint16_t font_color, uint16_t backgroud_color,
                uint8_t transparent);
int lcdDrawString(TFT_t *dev, sFONT *fx, uint16_t x, uint16_t y, uint8_t *ascii,
                  uint16_t font_color, uint16_t backgroud_color,
                  uint8_t transparent);
// int lcdDrawCode(TFT_t *dev, FontxFile *fx, uint16_t x, uint16_t y, uint8_t
// code,
//                 uint16_t color);
// int lcdDrawUTF8Char(TFT_t * dev, FontxFile *fx, uint16_t x, uint16_t y,
// uint8_t *utf8, uint16_t color); int lcdDrawUTF8String(TFT_t * dev, FontxFile
// *fx, uint16_t x, uint16_t y, unsigned char *utfs, uint16_t color);
void lcdSetFontDirection(TFT_t *dev, uint16_t);
void lcdSetFontFill(TFT_t *dev, uint16_t color);
void lcdUnsetFontFill(TFT_t *dev);
void lcdSetFontUnderLine(TFT_t *dev, uint16_t color);
void lcdUnsetFontUnderLine(TFT_t *dev);
void lcdBacklightOff(TFT_t *dev);
void lcdBacklightOn(TFT_t *dev);
void lcdInversionOff(TFT_t *dev);
void lcdInversionOn(TFT_t *dev);

void lcdDrawImage(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint8_t* image);
#endif /* MAIN_ST7789_H_ */
