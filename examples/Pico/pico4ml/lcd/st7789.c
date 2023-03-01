#include "hardware/spi.h"
#include "pico/stdlib.h"
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "st7789.h"

#define TAG          "ST7789"
#define _DEBUG_      0

#define LCD_SPI_PORT spi0

#define LCD_CS_PIN   20
#define LCD_DC_PIN   21
#define LCD_RST_PIN  22

void lcd_cs_high(uint8_t pin)
{
    gpio_put(pin, 1);
}

void lcd_cs_low(uint8_t pin)
{
    gpio_put(pin, 0);
}

void spiMasterInit(TFT_t* dev, int16_t GPIO_MOSI, int16_t GPIO_SCLK, int16_t GPIO_CS, int16_t GPIO_DC,
                   int16_t GPIO_RESET, int16_t GPIO_BL)
{
    if (GPIO_CS >= 0) {
        gpio_init(GPIO_CS);
        gpio_set_dir(GPIO_CS, GPIO_OUT);
        gpio_put(GPIO_CS, 1);
    }

    gpio_init(GPIO_DC);
    gpio_set_dir(GPIO_DC, GPIO_OUT);
    gpio_put(GPIO_DC, 1);

    if (GPIO_RESET >= 0) {
        // gpio_pad_select_gpio( GPIO_RESET );
        gpio_init(GPIO_RESET);
        gpio_set_dir(GPIO_RESET, GPIO_OUT);
        gpio_put(GPIO_RESET, 1);
        delayMS(100);
        gpio_put(GPIO_RESET, 0);
        delayMS(100);
        gpio_put(GPIO_RESET, 1);
        delayMS(100);
    }

    if (GPIO_BL >= 0) {
        // gpio_pad_select_gpio(GPIO_BL);
        gpio_init(GPIO_BL);
        gpio_set_dir(GPIO_BL, GPIO_OUT);
        gpio_put(GPIO_BL, 0);
    }
}

bool spiMasterWriteByte(const uint8_t* Data, size_t DataLength)
{
    lcd_cs_low(LCD_CS_PIN);
    // sleep_us(20);
    int wd = spi_write_blocking(LCD_SPI_PORT, Data, DataLength);
    // sleep_us(20);
    lcd_cs_high(LCD_CS_PIN);

    return wd == DataLength;
}

bool spiMasterWriteCommand(TFT_t* dev, uint8_t cmd)
{
    static uint8_t Byte = 0;

    Byte = cmd;
    gpio_put(LCD_DC_PIN, 0);
    return spiMasterWriteByte(&Byte, 1);
}

bool spiMasterWriteDataByte(TFT_t* dev, uint8_t data)
{
    static uint8_t Byte = 0;

    Byte = data;
    gpio_put(LCD_DC_PIN, 1);
    return spiMasterWriteByte(&Byte, 1);
}

bool spiMasterWriteDataWord(TFT_t* dev, uint16_t data)
{
    static uint8_t Byte[2];
    Byte[0] = (data >> 8) & 0xFF;
    Byte[1] = data & 0xFF;
    gpio_put(LCD_DC_PIN, 1);
    return spiMasterWriteByte(Byte, 2);
}

bool spiMasterWriteAddr(TFT_t* dev, uint16_t addr1, uint16_t addr2)
{
    static uint8_t Byte[4];
    Byte[0] = (addr1 >> 8) & 0xFF;
    Byte[1] = addr1 & 0xFF;
    Byte[2] = (addr2 >> 8) & 0xFF;
    Byte[3] = addr2 & 0xFF;
    gpio_put(LCD_DC_PIN, 1);
    return spiMasterWriteByte(Byte, 4);
}

bool spiMasterWriteColor(TFT_t* dev, uint16_t color, uint16_t size)
{
    static uint8_t Byte[1024];
    int index = 0;
    for (int i = 0; i < size; i++) {
        Byte[index++] = (color >> 8) & 0xFF;
        Byte[index++] = color & 0xFF;
    }
    gpio_put(LCD_DC_PIN, 1);
    return spiMasterWriteByte(Byte, size * 2);
}

// Add 202001
bool spiMasterWriteColors(TFT_t* dev, uint16_t* colors, uint16_t size)
{
    static uint8_t Byte[1024];
    int index = 0;
    for (int i = 0; i < size; i++) {
        Byte[index++] = (colors[i] >> 8) & 0xFF;
        Byte[index++] = colors[i] & 0xFF;
    }
    gpio_put(LCD_DC_PIN, 1);
    return spiMasterWriteByte(Byte, size * 2);
}

bool spiMasterWriteColorsInvert(TFT_t* dev, uint16_t* colors, uint16_t size)
{
    gpio_put(LCD_DC_PIN, 1);
    return spiMasterWriteByte((const uint8_t*)colors, size * 2);
}

void delayMS(int ms)
{
    sleep_ms(ms);
}

void lcdInitData(TFT_t* dev)
{
    spiMasterWriteCommand(dev, 0xb2);
    spiMasterWriteDataByte(dev, 0x0c);
    spiMasterWriteDataByte(dev, 0x0c);
    spiMasterWriteDataByte(dev, 0x00);
    spiMasterWriteDataByte(dev, 0x33);
    spiMasterWriteDataByte(dev, 0x33);
    spiMasterWriteCommand(dev, 0xb7);
    spiMasterWriteDataByte(dev, 0x35);
    spiMasterWriteCommand(dev, 0xbb);
    spiMasterWriteDataByte(dev, 0x28);
    spiMasterWriteCommand(dev, 0xc0);
    spiMasterWriteDataByte(dev, 0x3c);
    spiMasterWriteCommand(dev, 0xc2);
    spiMasterWriteDataByte(dev, 0x01);
    spiMasterWriteCommand(dev, 0xc3);
    spiMasterWriteDataByte(dev, 0x0b);
    spiMasterWriteCommand(dev, 0xc4);
    spiMasterWriteDataByte(dev, 0x20);
    spiMasterWriteCommand(dev, 0xc6);
    spiMasterWriteDataByte(dev, 0x0f);
    spiMasterWriteCommand(dev, 0xD0);
    spiMasterWriteDataByte(dev, 0xa4);
    spiMasterWriteDataByte(dev, 0xa1);
    spiMasterWriteCommand(dev, 0xe0);
    spiMasterWriteDataByte(dev, 0xd0);
    spiMasterWriteDataByte(dev, 0x01);
    spiMasterWriteDataByte(dev, 0x08);
    spiMasterWriteDataByte(dev, 0x0f);
    spiMasterWriteDataByte(dev, 0x11);
    spiMasterWriteDataByte(dev, 0x2a);
    spiMasterWriteDataByte(dev, 0x36);
    spiMasterWriteDataByte(dev, 0x55);
    spiMasterWriteDataByte(dev, 0x44);
    spiMasterWriteDataByte(dev, 0x3a);
    spiMasterWriteDataByte(dev, 0x0b);
    spiMasterWriteDataByte(dev, 0x06);
    spiMasterWriteDataByte(dev, 0x11);
    spiMasterWriteDataByte(dev, 0x20);
    spiMasterWriteCommand(dev, 0xe1);
    spiMasterWriteDataByte(dev, 0xd0);
    spiMasterWriteDataByte(dev, 0x02);
    spiMasterWriteDataByte(dev, 0x07);
    spiMasterWriteDataByte(dev, 0x0a);
    spiMasterWriteDataByte(dev, 0x0b);
    spiMasterWriteDataByte(dev, 0x18);
    spiMasterWriteDataByte(dev, 0x34);
    spiMasterWriteDataByte(dev, 0x43);
    spiMasterWriteDataByte(dev, 0x4a);
    spiMasterWriteDataByte(dev, 0x2b);
    spiMasterWriteDataByte(dev, 0x1b);
    spiMasterWriteDataByte(dev, 0x1c);
    spiMasterWriteDataByte(dev, 0x22);
    spiMasterWriteDataByte(dev, 0x1f);
    spiMasterWriteCommand(dev, 0x55);
    spiMasterWriteDataByte(dev, 0xB0);
    spiMasterWriteCommand(dev, 0x29);
}

void lcdInit(TFT_t* dev, int width, int height, int offsetx, int offsety, int flag)
{
    dev->_width = width;
    dev->_height = height;
    dev->_offsetx = offsetx;
    dev->_offsety = offsety;
    dev->_font_direction = DIRECTION0;
    dev->_font_fill = false;
    dev->_font_underline = false;

    // spiMasterWriteCommand(dev, 0x01); // Software Reset
    // delayMS(150);

    spiMasterWriteCommand(dev, 0x11); // Sleep Out
    // delayMS(255);
    delayMS(100);

    spiMasterWriteCommand(dev, 0x3A); // Interface Pixel Format
    spiMasterWriteDataByte(dev, 0x55);
    lcdInitData(dev);

    delayMS(10);
    spiMasterWriteCommand(dev, 0x36); // Memory Data Access Control
    if (width == 240)
        spiMasterWriteDataByte(dev, 0xc0); /* L2R_D2U */
    else
        spiMasterWriteDataByte(dev, 0xa0);
    spiMasterWriteDataByte(dev, 0x00);

    // spiMasterWriteCommand(dev, 0x2A); // Column Address Set
    // spiMasterWriteDataByte(dev, 0x00);
    // spiMasterWriteDataByte(dev, 0x00);
    // spiMasterWriteDataByte(dev, 0x00);
    // spiMasterWriteDataByte(dev, 0xF0);

    // spiMasterWriteCommand(dev, 0x2B); // Row Address Set
    // spiMasterWriteDataByte(dev, 0x00);
    // spiMasterWriteDataByte(dev, 0x00);
    // spiMasterWriteDataByte(dev, 0x00);
    // spiMasterWriteDataByte(dev, 0xF0);

    spiMasterWriteCommand(dev, 0x21); // Display Inversion On
    delayMS(10);

    spiMasterWriteCommand(dev, 0x13); // Normal Display Mode On
    delayMS(10);
    if (flag == false) {
        lcdFillScreen(dev, WHITE);
    }
    spiMasterWriteCommand(dev, 0x29); // Display ON
    // delayMS(255);

    if (dev->_bl >= 0) {
        gpio_put(dev->_bl, 1);
    }
}

void lcdSetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    spiMasterWriteCommand(0, 0x2A); // set column(x) address
    spiMasterWriteAddr(0, x1, x2);
    spiMasterWriteCommand(0, 0x2B); // set Page(y) address
    spiMasterWriteAddr(0, y1, y2);
    spiMasterWriteCommand(0, 0x2C); //	Memory Write
}

void lcdDrawImage(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint8_t* image)
{
    if ((x2 > x1) && (y2 > y1)) {
        lcdSetWindow(x1, y1, x2, y2);
        gpio_put(LCD_DC_PIN, 1);
        spiMasterWriteByte(image, (x2 - x1) * (y2 - y1));
    }
}
// Draw pixel
// x:X coordinate
// y:Y coordinate
// color:color
void lcdDrawPixel(TFT_t* dev, uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= dev->_width)
        return;
    if (y >= dev->_height)
        return;

    uint16_t _x = x + dev->_offsetx;
    uint16_t _y = y + dev->_offsety;

    spiMasterWriteCommand(dev, 0x2A); // set column(x) address
    spiMasterWriteAddr(dev, _x, _x);
    spiMasterWriteCommand(dev, 0x2B); // set Page(y) address
    spiMasterWriteAddr(dev, _y, _y);
    spiMasterWriteCommand(dev, 0x2C); //	Memory Write
    spiMasterWriteDataWord(dev, color);
}

// Draw multi pixel
// x:X coordinate
// y:Y coordinate
// size:Number of colors
// colors:colors
void lcdDrawMultiPixels(TFT_t* dev, uint16_t x, uint16_t y, uint16_t size, uint16_t* colors, bool little)
{
    if (x + size > dev->_width)
        return;
    if (y >= dev->_height)
        return;

    uint16_t _x1 = x + dev->_offsetx;
    uint16_t _x2 = _x1 + (size - 1);
    uint16_t _y1 = y + dev->_offsety;
    uint16_t _y2 = _y1;

    spiMasterWriteCommand(dev, 0x2A); // set column(x) address
    spiMasterWriteAddr(dev, _x1, _x2);
    spiMasterWriteCommand(dev, 0x2B); // set Page(y) address
    spiMasterWriteAddr(dev, _y1, _y2);
    spiMasterWriteCommand(dev, 0x2C); //	Memory Write
    if (little == true)
        spiMasterWriteColors(dev, colors, size);
    else
        spiMasterWriteColorsInvert(dev, colors, size);
}

// Draw rectangle of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color
void lcdDrawFillRect(TFT_t* dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    if (x1 >= dev->_width)
        return;
    if (x2 >= dev->_width)
        x2 = dev->_width - 1;
    if (y1 >= dev->_height)
        return;
    if (y2 >= dev->_height)
        y2 = dev->_height - 1;

    // ESP_LOGD(TAG, "offset(x)=%d offset(y)=%d", dev->_offsetx, dev->_offsety);
    uint16_t _x1 = x1 + dev->_offsetx;
    uint16_t _x2 = x2 + dev->_offsetx;
    uint16_t _y1 = y1 + dev->_offsety;
    uint16_t _y2 = y2 + dev->_offsety;

    spiMasterWriteCommand(dev, 0x2A); // set column(x) address
    spiMasterWriteAddr(dev, _x1, _x2);
    spiMasterWriteCommand(dev, 0x2B); // set Page(y) address
    spiMasterWriteAddr(dev, _y1, _y2);
    spiMasterWriteCommand(dev, 0x2C); //	Memory Write
    for (int i = _x1; i <= _x2; i++) {
        uint16_t size = _y2 - _y1 + 1;
        spiMasterWriteColor(dev, color, size);
#if 0
		for(j=y1;j<=y2;j++){
			////ESP_LOGD(TAG,"i=%d j=%d",i,j);
			spiMasterWriteDataWord(dev, color);
		}
#endif
    }
}

// Display OFF
void lcdDisplayOff(TFT_t* dev)
{
    spiMasterWriteCommand(dev, 0x28); // Display off
}

// Display ON
void lcdDisplayOn(TFT_t* dev)
{
    spiMasterWriteCommand(dev, 0x29); // Display on
}

// Fill screen
// color:color
void lcdFillScreen(TFT_t* dev, uint16_t color)
{
    lcdDrawFillRect(dev, 0, 0, dev->_width - 1, dev->_height - 1, color);
}

// Draw line
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End   X coordinate
// y2:End   Y coordinate
// color:color
void lcdDrawLine(TFT_t* dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    int i;
    int dx, dy;
    int sx, sy;
    int E;

    /* distance between two points */
    dx = (x2 > x1) ? x2 - x1 : x1 - x2;
    dy = (y2 > y1) ? y2 - y1 : y1 - y2;

    /* direction of two point */
    sx = (x2 > x1) ? 1 : -1;
    sy = (y2 > y1) ? 1 : -1;

    /* inclination < 1 */
    if (dx > dy) {
        E = -dx;
        for (i = 0; i <= dx; i++) {
            lcdDrawPixel(dev, x1, y1, color);
            x1 += sx;
            E += 2 * dy;
            if (E >= 0) {
                y1 += sy;
                E -= 2 * dx;
            }
        }

        /* inclination >= 1 */
    } else {
        E = -dy;
        for (i = 0; i <= dy; i++) {
            lcdDrawPixel(dev, x1, y1, color);
            y1 += sy;
            E += 2 * dx;
            if (E >= 0) {
                x1 += sx;
                E -= 2 * dy;
            }
        }
    }
}

// Draw rectangle
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End   X coordinate
// y2:End   Y coordinate
// color:color
void lcdDrawRect(TFT_t* dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    lcdDrawLine(dev, x1, y1, x2, y1, color);
    lcdDrawLine(dev, x2, y1, x2, y2, color);
    lcdDrawLine(dev, x2, y2, x1, y2, color);
    lcdDrawLine(dev, x1, y2, x1, y1, color);
}

// Draw rectangle with angle
// xc:Center X coordinate
// yc:Center Y coordinate
// w:Width of rectangle
// h:Height of rectangle
// angle:Angle of rectangle
// color:color

// When the origin is (0, 0), the point (x1, y1) after rotating the point (x, y)
// by the angle is obtained by the following calculation.
//  x1 = x * cos(angle) - y * sin(angle)
//  y1 = x * sin(angle) + y * cos(angle)
void lcdDrawRectAngle(TFT_t* dev, uint16_t xc, uint16_t yc, uint16_t w, uint16_t h, uint16_t angle, uint16_t color)
{
    double xd, yd, rd;
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int x4, y4;
    rd = -angle * M_PI / 180.0;
    xd = 0.0 - w / 2;
    yd = h / 2;
    x1 = (int)(xd * cos(rd) - yd * sin(rd) + xc);
    y1 = (int)(xd * sin(rd) + yd * cos(rd) + yc);

    yd = 0.0 - yd;
    x2 = (int)(xd * cos(rd) - yd * sin(rd) + xc);
    y2 = (int)(xd * sin(rd) + yd * cos(rd) + yc);

    xd = w / 2;
    yd = h / 2;
    x3 = (int)(xd * cos(rd) - yd * sin(rd) + xc);
    y3 = (int)(xd * sin(rd) + yd * cos(rd) + yc);

    yd = 0.0 - yd;
    x4 = (int)(xd * cos(rd) - yd * sin(rd) + xc);
    y4 = (int)(xd * sin(rd) + yd * cos(rd) + yc);

    lcdDrawLine(dev, x1, y1, x2, y2, color);
    lcdDrawLine(dev, x1, y1, x3, y3, color);
    lcdDrawLine(dev, x2, y2, x4, y4, color);
    lcdDrawLine(dev, x3, y3, x4, y4, color);
}

// Draw triangle
// xc:Center X coordinate
// yc:Center Y coordinate
// w:Width of triangle
// h:Height of triangle
// angle:Angle of triangle
// color:color

// When the origin is (0, 0), the point (x1, y1) after rotating the point (x, y)
// by the angle is obtained by the following calculation.
//  x1 = x * cos(angle) - y * sin(angle)
//  y1 = x * sin(angle) + y * cos(angle)
void lcdDrawTriangle(TFT_t* dev, uint16_t xc, uint16_t yc, uint16_t w, uint16_t h, uint16_t angle, uint16_t color)
{
    double xd, yd, rd;
    int x1, y1;
    int x2, y2;
    int x3, y3;
    rd = -angle * M_PI / 180.0;
    xd = 0.0;
    yd = h / 2;
    x1 = (int)(xd * cos(rd) - yd * sin(rd) + xc);
    y1 = (int)(xd * sin(rd) + yd * cos(rd) + yc);

    xd = w / 2;
    yd = 0.0 - yd;
    x2 = (int)(xd * cos(rd) - yd * sin(rd) + xc);
    y2 = (int)(xd * sin(rd) + yd * cos(rd) + yc);

    xd = 0.0 - w / 2;
    x3 = (int)(xd * cos(rd) - yd * sin(rd) + xc);
    y3 = (int)(xd * sin(rd) + yd * cos(rd) + yc);

    lcdDrawLine(dev, x1, y1, x2, y2, color);
    lcdDrawLine(dev, x1, y1, x3, y3, color);
    lcdDrawLine(dev, x2, y2, x3, y3, color);
}

// Draw circle
// x0:Central X coordinate
// y0:Central Y coordinate
// r:radius
// color:color
void lcdDrawCircle(TFT_t* dev, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    int x;
    int y;
    int err;
    int old_err;

    x = 0;
    y = -r;
    err = 2 - 2 * r;
    do {
        lcdDrawPixel(dev, x0 - x, y0 + y, color);
        lcdDrawPixel(dev, x0 - y, y0 - x, color);
        lcdDrawPixel(dev, x0 + x, y0 - y, color);
        lcdDrawPixel(dev, x0 + y, y0 + x, color);
        if ((old_err = err) <= x)
            err += ++x * 2 + 1;
        if (old_err > y || err > x)
            err += ++y * 2 + 1;
    } while (y < 0);
}

// Draw circle of filling
// x0:Central X coordinate
// y0:Central Y coordinate
// r:radius
// color:color
void lcdDrawFillCircle(TFT_t* dev, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    int x;
    int y;
    int err;
    int old_err;
    int ChangeX;

    x = 0;
    y = -r;
    err = 2 - 2 * r;
    ChangeX = 1;
    do {
        if (ChangeX) {
            lcdDrawLine(dev, x0 - x, y0 - y, x0 - x, y0 + y, color);
            lcdDrawLine(dev, x0 + x, y0 - y, x0 + x, y0 + y, color);
        } // endif
        ChangeX = (old_err = err) <= x;
        if (ChangeX)
            err += ++x * 2 + 1;
        if (old_err > y || err > x)
            err += ++y * 2 + 1;
    } while (y <= 0);
}

// Draw rectangle with round corner
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End   X coordinate
// y2:End   Y coordinate
// r:radius
// color:color
void lcdDrawRoundRect(TFT_t* dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint16_t color)
{
    int x;
    int y;
    int err;
    int old_err;
    unsigned char temp;

    if (x1 > x2) {
        temp = x1;
        x1 = x2;
        x2 = temp;
    } // endif

    if (y1 > y2) {
        temp = y1;
        y1 = y2;
        y2 = temp;
    } // endif

    // ESP_LOGD(TAG, "x1=%d x2=%d delta=%d r=%d", x1, x2, x2 - x1, r);
    // ESP_LOGD(TAG, "y1=%d y2=%d delta=%d r=%d", y1, y2, y2 - y1, r);
    if (x2 - x1 < r)
        return; // Add 20190517
    if (y2 - y1 < r)
        return; // Add 20190517

    x = 0;
    y = -r;
    err = 2 - 2 * r;

    do {
        if (x) {
            lcdDrawPixel(dev, x1 + r - x, y1 + r + y, color);
            lcdDrawPixel(dev, x2 - r + x, y1 + r + y, color);
            lcdDrawPixel(dev, x1 + r - x, y2 - r - y, color);
            lcdDrawPixel(dev, x2 - r + x, y2 - r - y, color);
        } // endif
        if ((old_err = err) <= x)
            err += ++x * 2 + 1;
        if (old_err > y || err > x)
            err += ++y * 2 + 1;
    } while (y < 0);

    // ESP_LOGD(TAG, "x1+r=%d x2-r=%d", x1 + r, x2 - r);
    lcdDrawLine(dev, x1 + r, y1, x2 - r, y1, color);
    lcdDrawLine(dev, x1 + r, y2, x2 - r, y2, color);
    // ESP_LOGD(TAG, "y1+r=%d y2-r=%d", y1 + r, y2 - r);
    lcdDrawLine(dev, x1, y1 + r, x1, y2 - r, color);
    lcdDrawLine(dev, x2, y1 + r, x2, y2 - r, color);
}

// Draw arrow
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End   X coordinate
// y2:End   Y coordinate
// w:Width of the botom
// color:color
// Thanks http://k-hiura.cocolog-nifty.com/blog/2010/11/post-2a62.html
void lcdDrawArrow(TFT_t* dev, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t w, uint16_t color)
{
    double Vx = x1 - x0;
    double Vy = y1 - y0;
    double v = sqrt(Vx * Vx + Vy * Vy);
    //	 printf("v=%f\n",v);
    double Ux = Vx / v;
    double Uy = Vy / v;

    uint16_t L[2], R[2];
    L[0] = x1 - Uy * w - Ux * v;
    L[1] = y1 + Ux * w - Uy * v;
    R[0] = x1 + Uy * w - Ux * v;
    R[1] = y1 - Ux * w - Uy * v;
    // printf("L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);

    // lcdDrawLine(x0,y0,x1,y1,color);
    lcdDrawLine(dev, x1, y1, L[0], L[1], color);
    lcdDrawLine(dev, x1, y1, R[0], R[1], color);
    lcdDrawLine(dev, L[0], L[1], R[0], R[1], color);
}

// Draw arrow of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End   X coordinate
// y2:End   Y coordinate
// w:Width of the botom
// color:color
void lcdDrawFillArrow(TFT_t* dev, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t w, uint16_t color)
{
    double Vx = x1 - x0;
    double Vy = y1 - y0;
    double v = sqrt(Vx * Vx + Vy * Vy);
    // printf("v=%f\n",v);
    double Ux = Vx / v;
    double Uy = Vy / v;

    uint16_t L[2], R[2];
    L[0] = x1 - Uy * w - Ux * v;
    L[1] = y1 + Ux * w - Uy * v;
    R[0] = x1 + Uy * w - Ux * v;
    R[1] = y1 - Ux * w - Uy * v;
    // printf("L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);

    lcdDrawLine(dev, x0, y0, x1, y1, color);
    lcdDrawLine(dev, x1, y1, L[0], L[1], color);
    lcdDrawLine(dev, x1, y1, R[0], R[1], color);
    lcdDrawLine(dev, L[0], L[1], R[0], R[1], color);

    int ww;
    for (ww = w - 1; ww > 0; ww--) {
        L[0] = x1 - Uy * ww - Ux * v;
        L[1] = y1 + Ux * ww - Uy * v;
        R[0] = x1 + Uy * ww - Ux * v;
        R[1] = y1 - Ux * ww - Uy * v;
        // printf("Fill>L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);
        lcdDrawLine(dev, x1, y1, L[0], L[1], color);
        lcdDrawLine(dev, x1, y1, R[0], R[1], color);
    }
}

// RGB565 conversion
// RGB565 is R(5)+G(6)+B(5)=16bit color format.
// Bit image "RRRRRGGGGGGBBBBB"
uint16_t rgb565_conv(uint16_t r, uint16_t g, uint16_t b)
{
    return (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
}

// Draw ASCII character
// x:X coordinate
// y:Y coordinate
// ascii: ascii code
// color:color
int lcdDrawChar(TFT_t* dev, sFONT* fxs, uint16_t x, uint16_t y, uint8_t ascii, uint16_t font_color,
                uint16_t backgroud_color, uint8_t transparent)
{
    if (x > dev->_width || y > dev->_height) {
        return -1;
    }

    uint32_t offset_ = (ascii - ' ') * fxs->Height * (fxs->Width / 8 + (fxs->Width % 8 ? 1 : 0));
    const unsigned char* fonts = &fxs->table[offset_];

    for (uint16_t Page = 0; Page < fxs->Height; Page++) {
        for (uint16_t Column = 0; Column < fxs->Width; Column++) {
            // To determine whether the font background color and screen
            // background color is consistent

            if (*fonts & (0x80 >> (Column % 8)))
                lcdDrawPixel(dev, x + Column, y + Page, font_color);
            else if (transparent == false)
                lcdDrawPixel(dev, x + Column, y + Page, backgroud_color);

            // One pixel is 8 bits
            if (Column % 8 == 7)
                fonts++;
        } /* Write a line */
        if (fxs->Width % 8 != 0)
            fonts++;
    }
    return 0;
}

int lcdDrawString(TFT_t* dev, sFONT* fx, uint16_t x, uint16_t y, uint8_t* ascii, uint16_t font_color,
                  uint16_t backgroud_color, uint8_t transparent)
{
    int length = strlen((char*)ascii);
    uint16_t Xpoint = x;
    uint16_t Ypoint = y;
    if (_DEBUG_)
        printf("lcdDrawString length=%d\n", length);
    for (int i = 0; i < length; i++) {
        if (_DEBUG_)
            printf("ascii[%d]=%x x=%d y=%d\n", i, ascii[i], x, y);
        if ((x + fx->Width) > dev->_width) {
            Xpoint = x;
            Ypoint += fx->Height;
        }

        // If the Y direction is full, reposition to(Xstart, Ystart)
        if ((Ypoint + fx->Height) > dev->_height) {
            Xpoint = x;
            Ypoint = y;
        }
        lcdDrawChar(dev, fx, Xpoint, Ypoint, ascii[i], font_color, backgroud_color, transparent);

        // The next word of the abscissa increases the font of the broadband
        Xpoint += fx->Width;
    }

    return 0;
}

// // Draw Non-Alphanumeric character
// // x:X coordinate
// // y:Y coordinate
// // code:character code
// // color:color
// int lcdDrawCode(TFT_t *dev, FontxFile *fx, uint16_t x, uint16_t y, uint8_t
// code,
//                 uint16_t color) {
//     if (_DEBUG_)
//         printf("code=%x x=%d y=%d\n", code, x, y);
//     if (dev->_font_direction == 0)
//         x = lcdDrawChar(dev, fx, x, y, code, color);
//     if (dev->_font_direction == 1)
//         y = lcdDrawChar(dev, fx, x, y, code, color);
//     if (dev->_font_direction == 2)
//         x = lcdDrawChar(dev, fx, x, y, code, color);
//     if (dev->_font_direction == 3)
//         y = lcdDrawChar(dev, fx, x, y, code, color);
//     if (dev->_font_direction == 0)
//         return x;
//     if (dev->_font_direction == 2)
//         return x;
//     if (dev->_font_direction == 1)
//         return y;
//     if (dev->_font_direction == 3)
//         return y;
//     return 0;
// }

#if 0
// Draw UTF8 character
// x:X coordinate
// y:Y coordinate
// utf8:UTF8 code
// color:color
int lcdDrawUTF8Char(TFT_t * dev, FontxFile *fx, uint16_t x,uint16_t y,uint8_t *utf8,uint16_t color) {
	uint16_t sjis[1];

	sjis[0] = UTF2SJIS(utf8);
	if(_DEBUG_)printf("sjis=%04x\n",sjis[0]);
	return lcdDrawSJISChar(dev, fx, x, y, sjis[0], color);
}

// Draw UTF8 string
// x:X coordinate
// y:Y coordinate
// utfs:UTF8 string
// color:color
int lcdDrawUTF8String(TFT_t * dev, FontxFile *fx, uint16_t x, uint16_t y, unsigned char *utfs, uint16_t color) {

	int i;
	int spos;
	uint16_t sjis[64];
	spos = String2SJIS(utfs, strlen((char *)utfs), sjis, 64);
	if(_DEBUG_)printf("spos=%d\n",spos);
	for(i=0;i<spos;i++) {
		if(_DEBUG_)printf("sjis[%d]=%x y=%d\n",i,sjis[i],y);
		if (dev->_font_direction == 0)
			x = lcdDrawSJISChar(dev, fx, x, y, sjis[i], color);
		if (dev->_font_direction == 1)
			y = lcdDrawSJISChar(dev, fx, x, y, sjis[i], color);
		if (dev->_font_direction == 2)
			x = lcdDrawSJISChar(dev, fx, x, y, sjis[i], color);
		if (dev->_font_direction == 3)
			y = lcdDrawSJISChar(dev, fx, x, y, sjis[i], color);
	}
	if (dev->_font_direction == 0) return x;
	if (dev->_font_direction == 2) return x;
	if (dev->_font_direction == 1) return y;
	if (dev->_font_direction == 3) return y;
	return 0;
}
#endif

// Set font direction
// dir:Direction
void lcdSetFontDirection(TFT_t* dev, uint16_t dir)
{
    dev->_font_direction = dir;
}

// Set font filling
// color:fill color
void lcdSetFontFill(TFT_t* dev, uint16_t color)
{
    dev->_font_fill = true;
    dev->_font_fill_color = color;
}

// UnSet font filling
void lcdUnsetFontFill(TFT_t* dev)
{
    dev->_font_fill = false;
}

// Set font underline
// color:frame color
void lcdSetFontUnderLine(TFT_t* dev, uint16_t color)
{
    dev->_font_underline = true;
    dev->_font_underline_color = color;
}

// UnSet font underline
void lcdUnsetFontUnderLine(TFT_t* dev)
{
    dev->_font_underline = false;
}

// Backlight OFF
void lcdBacklightOff(TFT_t* dev)
{
    if (dev->_bl >= 0) {
        gpio_put(dev->_bl, 0);
    }
}

// Backlight ON
void lcdBacklightOn(TFT_t* dev)
{
    if (dev->_bl >= 0) {
        gpio_put(dev->_bl, 1);
    }
}

// Display Inversion Off
void lcdInversionOff(TFT_t* dev)
{
    spiMasterWriteCommand(dev, 0x20); // Display Inversion Off
}

// Display Inversion On
void lcdInversionOn(TFT_t* dev)
{
    spiMasterWriteCommand(dev, 0x21); // Display Inversion On
}
